#define __STDC_WANT_LIB_EXT1__ // For strcpy_s
#include <string.h>
#include "Network/NetGameJoin.h"
#include "Pomme.h"

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

#define LOG POMME_GENLOG(POMME_DEBUG_NET, "NET_JOIN")
using namespace Pomme::Network;

NetGameJoin::NetGameJoin(const NSpPlayerInfo &ourPlayerInfo)
	: mOurPlayerInfo(ourPlayerInfo)
{
	// Do nothing
}

// Returns valid socket, or -1 on failure
int NetGameJoin::getConnectionSocket(NSpAddressReference addr)
{

	int status = 0;
	addrinfo hints{};
	addrinfo *tmp;
	std::unique_ptr<addrinfo, decltype(&freeaddrinfo)> servInfoRes(nullptr, &freeaddrinfo);

	// Support IPv4 and IPv6
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_family = AF_UNSPEC;

	if((status = getaddrinfo(addr->address, std::to_string(addr->port).c_str(), &hints, &tmp) != 0))
	{
		std::wcerr << "getaddrinfo error: " << gai_strerror(status) << std::endl;
		return false;
	}
	servInfoRes.reset(tmp); // Let smart-pointer manage deletion

	// Loop through all address results and connect to the first one we can
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

		// Only warn if these options fail
		if(setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char *)&yes, sizeof(yes)) == -1)
			std::cout << "Warning: can't set SO_REUSEADDR: " << getSockErrorStr() << std::endl;

		if(setsockopt(sock, IPPROTO_TCP, TCP_NODELAY, (char *)&yes, sizeof(yes)) == -1)
			std::cout << "Warning: can't set TCP_NODELAY: " << getSockErrorStr() << std::endl;

		if((sock = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1)
		{
			LOG << "socket failed: " << getSockErrorStr() << std::endl;
			continue;
		}

		if(connect(sock, p->ai_addr, static_cast<int>(p->ai_addrlen)) == -1)
		{
			LOG << "socket connect failed: " << getSockErrorStr() << std::endl;
			continue;
		}

		// We've got a valid connection!
		getsockname(sock, &boundAddr, &boundAddrLen);
		std::cout << "Connected to " << *p << " from " << boundAddr << std::endl;
		return sock;
	}

	return -1;
}

// Returns false on failure
bool NetGameJoin::joinGame(NSpAddressReference addr, const std::string &password,
	unsigned userDataLen, void *userData)
{
	NSpJoinRequestMessage requestMsg {};
	if((mSock = getConnectionSocket(addr)) == -1)
		return false;

	// Populate JoinRequestMessage and send
	requestMsg.header.what   = kNSpJoinRequest;
	requestMsg.type          = mOurPlayerInfo.type;
	requestMsg.customDataLen = userDataLen;
	strcpy_s(requestMsg.name,       sizeof(Str31), mOurPlayerInfo.name);
	strcpy_s(requestMsg.password,   sizeof(Str31), password.c_str());
	memcpy_s(requestMsg.customData, sizeof(NSpJoinRequestMessage::customData), userData, userDataLen);

	sendMsg(mSock, requestMsg);
	return true;
}