// Inspired by: https://beej.us/guide/bgnet/html

#include <string>
#include <iostream>
#include <memory>
#include "Pomme.h"
#include "Network/NetGameHost.h"

#ifdef _WIN32
	#include <Windows.h>
	#include <winsock2.h>
	#include <ws2tcpip.h>
#else
	#include <sys/types.h>
	#include <sys/socket.h>
	#include <netdb.h>
	#include <fcntl.h>
#endif

#define LOG POMME_GENLOG(POMME_DEBUG_NET, "NET_HOST")
using namespace Pomme::Network;

NetGameHost::~NetGameHost()
{
	// Make sure we stop listening
	stopListening();
}

int NetGameHost::createListeningSocket(int port, bool supportIPv6)
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
	for(p = servInfoRes.get(); p != NULL; p = p->ai_next)
	{
		int sock = 0;
		sockaddr boundAddr{};
		socklen_t boundAddrLen = sizeof(boundAddr);
		int yes = 1;

		if((sock = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1)
		{
			LOG << "socket attempt failed: " << getSockErrorStr() << std::endl;
			continue;
		}

		// Only warn if this fails
		if(setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char *)&yes, sizeof(yes)) == -1)
		{
			std::cout << "Warning: can't set SO_REUSEADDR: " << getSockErrorStr() << std::endl;
		}

		if(bind(sock, p->ai_addr, static_cast<int>(p->ai_addrlen)) == -1)
		{
			LOG << "bind attempt failed: " << getSockErrorStr() << std::endl;
			continue;
		}

		// Set non-blocking
#ifdef _WIN32
		u_long iMode = 1;
		int iResult = ioctlsocket(sock, FIONBIO, &iMode);
		if(iResult != NO_ERROR)
		{
			std::cerr << "Can't set socket as non-blocking, error: " << iResult << std::endl;
			continue;
		}
#else
		if(fcntl(sock, F_SETFL, fcntl(sock, F_GETFL, 0) | O_NONBLOCK) == -1)
		{
			std::cerr << "Can't set socket as non-blocking, error: " << getSockErrorStr() << std::endl;
			continue;
		}
#endif

		if(listen(sock, ACCEPT_BACKLOG) == -1)
		{
			std::cerr << "Can't listen on socket error: " << getSockErrorStr() << std::endl;
			continue;
		}

		// We've got a valid socket!
		getsockname(sock, &boundAddr, &boundAddrLen);
		std::cout << "Listening on port: " << getPortFromAddr(p->ai_family, boundAddr) << std::endl;
		return sock;
	}

	return -1;
}

// Returns false on failure
bool NetGameHost::startListening(const std::string &password, unsigned maxPlayers, int port, bool supportIPv6)
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

	mListeningThread = std::thread([this, listenSock]()
		{
			while(mListening)
			{
				sockaddr_storage addr{};
				socklen_t addrLen = sizeof(addr);

				int socket = accept(listenSock, reinterpret_cast<sockaddr *>(&addr), &addrLen);
				if(socket != -1)
				{
					std::cerr << "Accepting connection" << std::endl;
				}
#ifdef _WIN32
				else if(socket == -1 && !(WSAGetLastError() == WSAEWOULDBLOCK))
#else
				else if(socket == -1 && !(errno == EWOULDBLOCK || errno == EAGAIN))
#endif
				{
					std::cerr << "Socket accept error: " << getSockErrorStr() << std::endl;
				}
			}
		});

	return true;
}

bool NetGameHost::stopListening()
{
	mListening = false;
	if(mListeningThread.joinable())
		mListeningThread.join();

	return true;
}