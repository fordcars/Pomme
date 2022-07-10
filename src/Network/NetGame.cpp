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
