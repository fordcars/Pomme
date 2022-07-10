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

// IP-agnostic print operator
std::ostream &Pomme::Network::operator<<(std::ostream &lhs, const addrinfo &rhs)
{
	char ipStr[INET6_ADDRSTRLEN];
	const void *addr = nullptr;

	// Get addr acccording to IP version
	if(rhs.ai_family == AF_INET)
	{
		// IPv4
		const sockaddr_in *ipv4 = reinterpret_cast<const sockaddr_in *>(rhs.ai_addr);
		addr = &(ipv4->sin_addr);
	}
	else
	{
		// IPv6
		const sockaddr_in6 *ipv6 = reinterpret_cast<const sockaddr_in6 *>(rhs.ai_addr);
		addr = &(ipv6->sin6_addr);
	}

	// Convert to string
	inet_ntop(rhs.ai_family, addr, ipStr, sizeof(ipStr));
	lhs << ipStr;
	return lhs;
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

unsigned NetGame::getPortFromAddr(int addrFamily, const sockaddr &addr)
{
	unsigned port = 0;

	if(addrFamily == AF_INET)
	{
		// IPv4
		const sockaddr_in *ipv4 = reinterpret_cast<const sockaddr_in *>(&addr);
		port = ntohs(ipv4->sin_port);
	}
	else
	{
		// IPv6
		const sockaddr_in6 *ipv6 = reinterpret_cast<const sockaddr_in6 *>(&addr);
		port = ntohs(ipv6->sin6_port);
	}

	return port;
}
