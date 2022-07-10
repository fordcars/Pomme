#pragma once

#include <string>
#include <atomic>
#include <thread>
#include <vector>
#include "Network/NetGame.h"
#include "PommeTypes.h"

namespace Pomme::Network
{
	class NetGameJoin : public NetGame
	{
	private:
		int mSock = -1;
		static int getConnectionSocket(NSpAddressReference addr);

	public:
		bool joinGame(NSpAddressReference addr);
	};
}
