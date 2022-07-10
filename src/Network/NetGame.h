#pragma once

#include <string>
#include <iostream>

struct addrinfo;
struct sockaddr;
namespace Pomme::Network
{
	std::ostream &operator<<(std::ostream &lhs, const addrinfo &rhs);

	class NetGame
	{
	private:
		std::string mGameName;

	protected:
		static std::string getSockErrorStr();
		static unsigned getPortFromAddr(int addrFamily, const sockaddr &addr);

	public:
		NetGame();
		virtual ~NetGame();
	};
}
