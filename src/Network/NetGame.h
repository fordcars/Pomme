#pragma once

#include <string>
#include <iostream>

struct sockaddr;
struct sockaddr_storage;
struct addrinfo;
namespace Pomme::Network
{
	std::ostream &operator<<(std::ostream &lhs, const sockaddr &rhs);
	std::ostream &operator<<(std::ostream &lhs, const sockaddr_storage &rhs);
	std::ostream &operator<<(std::ostream &lhs, const addrinfo &rhs);

	class NetGame
	{
	private:
		std::string mGameName;

	protected:
		static std::string getSockErrorStr();
		void setGameName(const std::string &gameName);
		std::string getGameName();

	public:
		NetGame();
		virtual ~NetGame();
	};
}
