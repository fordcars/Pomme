#pragma once

#include <atomic>
#include <thread>

namespace Pomme::Network
{
	class NetGame
	{
	private:
		static const unsigned ACCEPT_BACKLOG = 10;

		std::atomic<bool> mListening = false;
		std::thread mListeningThread;

		static int createListeningSocket(int port, bool supportIPv6);

	public:
		NetGame();
		~NetGame();
		bool startListening(int port, bool supportIPv6);
		bool stopListening();
	};
}
