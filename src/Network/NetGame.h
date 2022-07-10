#pragma once

#include <cstddef>
#include <string>
#include <iostream>

struct sockaddr;
struct sockaddr_storage;
struct addrinfo;
struct NSpMessageHeader;
namespace Pomme::Network
{
	std::ostream &operator<<(std::ostream &lhs, const sockaddr &rhs);
	std::ostream &operator<<(std::ostream &lhs, const sockaddr_storage &rhs);
	std::ostream &operator<<(std::ostream &lhs, const addrinfo &rhs);

	class NetGame
	{
	private:
		static const unsigned NSPROCKET_PROTOCOL_VERSION;

		std::string mGameName;
		static int sendBytes(int sock, const void *data, size_t length);
		static int readBytes(int sock, void *buf, size_t length);

	protected:
		static int sendMsgRaw(int sock, NSpMessageHeader *inMsg);
		static int readMsgRaw(int sock, NSpMessageHeader *outMsgBuf, size_t bufSize);
		static bool msgReceived(int sock);
		static std::string getSockErrorStr();

		void setGameName(const std::string &gameName);
		std::string getGameName();

		// Helper function for sending NetSprocket messages from C++
		// Modifies msg header with relevant fields
		template<typename NSprocketMsgT>
		static int sendMsg(int sock, NSprocketMsgT& msg)
		{
			static_assert(sizeof(NSprocketMsgT) >= sizeof(NSpMessageHeader), "sendMsg requires a NSpMessage type");
			msg.header.messageLen = sizeof(NSprocketMsgT);
			return sendMsgRaw(sock, &msg.header);
		}

		// Helper function for reading NetSprocket messages from C++
		template<typename NSprocketMsgT>
		static NSprocketMsgT readMsg(int sock)
		{
			NSprocketMsgT msg;
			readMsgRaw(sock, &msg.header, sizeof(NSprocketMsgT));
			return msg;
		}

	public:
		NetGame();
		virtual ~NetGame();
	};
}
