#pragma once

#include <string>
#include <atomic>
#include <thread>
#include <vector>
#include "PommeTypes.h"

namespace Pomme::Network
{
	class NetGame
	{
	private:
		static const unsigned ACCEPT_BACKLOG = 10;

		std::string mGameName;
		std::atomic<bool> mListening = false;
		std::thread mListeningThread;
		std::vector<NSpPlayerInfo> mPlayers;

		static int createListeningSocket(int port, bool supportIPv6);

	public:
		NetGame(const std::string& gameName);
		~NetGame();
		bool startListening(const std::string& password, unsigned maxPlayers, int port, bool supportIPv6);
		bool stopListening();
	};
}
