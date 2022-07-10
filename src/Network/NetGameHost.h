#pragma once

#include <string>
#include <atomic>
#include <thread>
#include <vector>
#include "Network/NetGame.h"
#include "PommeTypes.h"

namespace Pomme::Network
{
	class NetGameHost : public NetGame
	{
	private:
		static const unsigned ACCEPT_BACKLOG = 10;

		std::atomic<bool> mListening = false;
		std::thread mListeningThread;
		std::vector<NSpPlayerInfo> mPlayers;

		static int createListeningSocket(int port, bool supportIPv6);

	public:
		~NetGameHost();
		bool startListening(const std::string &gameName, const std::string &password,
			unsigned maxPlayers, int port, bool supportIPv6 = false);
		bool stopListening();
	};
}
