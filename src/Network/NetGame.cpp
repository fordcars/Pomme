// Inspired by: https://beej.us/guide/bgnet/html

#define __STDC_WANT_LIB_EXT1__ 1 // For strerror_s
#include <mutex>
#include <string>
#include "Pomme.h"
#include "Network/NetGame.h"

#ifdef _WIN32
	#include <Windows.h>
	#include <winsock2.h>
	#include <ws2tcpip.h>
	static std::once_flag gWinSockOnceFlag;
#else
	#include <sys/types.h>
	#include <sys/socket.h>
	#include <netdb.h>
	#include <fcntl.h>
#endif

#define LOG POMME_GENLOG(POMME_DEBUG_NET, "NET_BASE")
using namespace Pomme::Network;

const unsigned NetGame::NSPROCKET_PROTOCOL_VERSION = 1;

// Print operators
std::ostream &Pomme::Network::operator<<(std::ostream &lhs, const sockaddr &rhs)
{
	char ipStr[INET6_ADDRSTRLEN];
	const void *inAddr = nullptr;
	unsigned port = 0;

	// Get in_addr and port acccording to IP version
	if(rhs.sa_family == AF_INET)
	{
		// IPv4
		const sockaddr_in &ipv4 = reinterpret_cast<const sockaddr_in &>(rhs);
		inAddr = &(ipv4.sin_addr);
		port = ntohs(ipv4.sin_port);
	}
	else
	{
		// IPv6
		const sockaddr_in6 &ipv6 = reinterpret_cast<const sockaddr_in6 &>(rhs);
		inAddr = &(ipv6.sin6_addr);
		port = ntohs(ipv6.sin6_port);

	}

	// Convert to string
	inet_ntop(rhs.sa_family, inAddr, ipStr, sizeof(ipStr));
	lhs << ipStr;
	if(port > 0)
		lhs << ':' << port;

	return lhs;
}

std::ostream &Pomme::Network::operator<<(std::ostream &lhs, const sockaddr_storage &rhs)
{
	return lhs << reinterpret_cast<const sockaddr &>(rhs);
}

std::ostream &Pomme::Network::operator<<(std::ostream &lhs, const addrinfo &rhs)
{
	return lhs << *rhs.ai_addr;
}

NetGame::NetGame()
{
#ifdef _WIN32
	// Windows-specific socket setup
	std::call_once(gWinSockOnceFlag, []()
	{
		WSADATA wsaData;
		int res = WSAStartup(MAKEWORD(2, 2), &wsaData);
		if(res != 0)
			std::cerr << "WSAStartup failed: " << res << std::endl;
	});
#endif
}

NetGame::~NetGame()
{
#ifdef _WIN32
	WSACleanup();
#endif
}

// Send bytes over network (blocking)
// Guaranteed to send specified number of bytes
// Returns bytes sent, or -1
int NetGame::sendBytes(int sock, const void *data, size_t length)
{
	unsigned sent = 0;
	while(sent < length)
	{
		int size = send(sock, reinterpret_cast<const char *>(data) + sent, static_cast<int>(length) - sent, 0);
		if(size == -1)
			return -1; // Error

		sent += size;
	}

	return sent;
}

// Read bytes from the socket (blocking)
// Guaranteed to read specified number of bytes
// Returns bytes read, or -1
int NetGame::readBytes(int sock, void *buf, size_t length)
{
	unsigned read = 0;
	while(read < length)
	{
		int size = recv(sock, reinterpret_cast<char *>(buf) + read, static_cast<int>(length) - read, 0);
		if(size == -1)
			return -1; // Error

		read += size;
	}

	return read;
}

// Send a well-formed NetSprocket message
// Message must have messageLen field set
// Modifies inMsg header with relevant read-only fields
// Returns bytes sent, or -1
int NetGame::sendMsgRaw(int sock, NSpMessageHeader *inMsg)
{
	inMsg->version = NSPROCKET_PROTOCOL_VERSION;
	//msg->from;
	//msg->id;
	//msg->when;
	// TODO: Serialize properly
	return sendBytes(sock, inMsg, inMsg->messageLen);
}

// Read a NetSprocket msg from the socket into msgBuf (blocking)
// Returns bytes read, or -1
int NetGame::readMsgRaw(int sock, NSpMessageHeader *outMsgBuf, size_t bufSize)
{
	UInt32 totalMsgLength;
	if(sizeof(NSpMessageHeader) > bufSize)
	{
		std::cerr
			<< "Error reading NetSprocket msg header: outMsgBuf not large enough (required "
			<< sizeof(NSpMessageHeader) << " bytes, got " << bufSize << " bytes)"
			<< std::endl;
		return -1;
	}

	// Read message header
	// TODO: Deserialize properly
	if(readBytes(sock, outMsgBuf, sizeof(NSpMessageHeader)) == -1)
	{
		std::cerr << "Error reading NetSprocket msg header: " << getSockErrorStr() << std::endl;
		return -1;
	}

	// Check protocol version
	if(outMsgBuf->version != NSPROCKET_PROTOCOL_VERSION)
	{
		std::cerr
			<< "Error reading NetSprocket msg: protocol version unsupported (expected v"
			<< NSPROCKET_PROTOCOL_VERSION << ", got v" << outMsgBuf->version << ")"
			<< std::endl;
		return -1;
	}

	// Read remaining message
	totalMsgLength = outMsgBuf->messageLen;
	if(totalMsgLength > bufSize)
	{
		std::cerr
			<< "Error reading NetSprocket msg header: outMsgBuf not large enough (required "
			<< totalMsgLength << " bytes, got " << bufSize << " bytes)"
			<< std::endl;
		return -1;
	}

	if(readBytes(sock, outMsgBuf + 1, totalMsgLength - sizeof(NSpMessageHeader)) == -1)
	{
		std::cerr << "Error reading NetSprocket msg: " << getSockErrorStr() << std::endl;
		return -1;
	}
	return totalMsgLength;
}

// Returns true if a msg was received and is ready to be read, false otherwise
bool NetGame::msgReceived(int sock)
{
#ifdef _WIN32
	u_long iCount = 0;
	int iResult = ioctlsocket(sock, FIONREAD, &iCount);
	if(iResult != NO_ERROR)
		std::cerr << "msgReceived ioctlsocket error: " << iResult << std::endl;
	
	return iCount >= sizeof(NSpMessageHeader); // Minimum msg length
#else
	int count = 0;
	if(ioctl(sock, FIONREAD, &count) == -1)
		std::cerr << "msgReceived ioctl error: " << getSockErrorStr() << std::endl;

	return count >= sizeof(NSpMessageHeader); // Minimum msg length
#endif
}

std::string NetGame::getSockErrorStr()
{
#ifdef _WIN32
	return std::to_string(WSAGetLastError());
#else
	char errorStr[255];
	strerror_s(errorStr, sizeof(errorStr), errno);
	return errorStr;
#endif
}

void NetGame::setGameName(const std::string &gameName)
{
	mGameName = gameName;
}

std::string NetGame::getGameName()
{
	return mGameName;
}
