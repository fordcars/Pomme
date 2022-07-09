// Inspired by: https://beej.us/guide/bgnet/html

#define __STDC_WANT_LIB_EXT1__ 1 // For strerror_s
#include <mutex>
#include <string>
#include <iostream>
#include <memory>
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
#endif

#define LOG POMME_GENLOG(POMME_DEBUG_NET, "NET")
using namespace Pomme::Network;

unsigned getPort(int addrFamily, const sockaddr &addr)
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

// IP-agnostic print operator
std::ostream &operator<<(std::ostream &lhs, const addrinfo& rhs)
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
	// Make sure we stop listening
	stopListening();

#ifdef _WIN32
	WSACleanup();
#endif
}

int NetGame::createListeningSocket(int port, bool supportIPv6)
{
	int status = 0;
	addrinfo hints{};
	addrinfo *tmp;
	std::unique_ptr<addrinfo, decltype(&freeaddrinfo)> servInfoRes(nullptr, &freeaddrinfo);

	// TCP, get address of all valid interfaces
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	if(supportIPv6)
		hints.ai_family = AF_UNSPEC;
	else
		hints.ai_family = AF_INET;

	// Get addresses we can bind to
	if((status = getaddrinfo(NULL, std::to_string(port).c_str(), &hints, &tmp) != 0))
	{
		std::wcerr << "getaddrinfo error: " << gai_strerror(status) << std::endl;
		return false;
	}
	servInfoRes.reset(tmp); // Let smart-pointer manage deletion

	// Loop through all address results and bind to the first one we can
	addrinfo *p = nullptr;
	char errorStr[255];
	for(p = servInfoRes.get(); p != NULL; p = p->ai_next)
	{
		int sock = 0;
		sockaddr boundAddr{};
		socklen_t boundAddrLen = sizeof(boundAddr);
		int yes = 1;

		if((sock = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1)
		{
			strerror_s(errorStr, sizeof(errorStr), errno);
			LOG << "socket attempt failed: " << errorStr << std::endl;
			continue;
		}

		// Only warn if this fails
		if(setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char *)&yes, sizeof(yes)) == -1)
		{
			strerror_s(errorStr, sizeof(errorStr), errno);
			std::cout << "Warning: can't set SO_REUSEADDR: " << errorStr << std::endl;
		}

		if(bind(sock, p->ai_addr, static_cast<int>(p->ai_addrlen)) == -1)
		{
			strerror_s(errorStr, sizeof(errorStr), errno);
			LOG << "bind attempt failed: " << errorStr << std::endl;
			continue;
		}

		if(listen(sock, ACCEPT_BACKLOG) == -1)
		{
			strerror_s(errorStr, sizeof(errorStr), errno);
			LOG << "listen attempt failed: " << errorStr << std::endl;
			continue;
		}

		// We've got a valid socket!
		getsockname(sock, &boundAddr, &boundAddrLen);
		std::cout << "Listening on port: " << getPort(p->ai_family, boundAddr) << std::endl;
		return sock;
	}

	return -1;
}

// Returns false on failure
bool NetGame::startListening(int port, bool supportIPv6)
{
	if(mListening)
		return false;
	mListening = true;

	int listenSock = createListeningSocket(port, supportIPv6);
	if(listenSock == -1)
	{
		std::cerr << "Could not create listening socket!" << std::endl;
		return false;
	}


	return true;
}

bool NetGame::stopListening()
{
	mListening = false;
	if(mListeningThread.joinable())
		mListeningThread.join();

	return true;
}