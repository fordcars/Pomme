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
		NSpPlayerInfo mOurPlayerInfo{};
		static int getConnectionSocket(NSpAddressReference addr);

	public:
		NetGameJoin(const NSpPlayerInfo &ourPlayerInfo);
		bool joinGame(NSpAddressReference addr, const std::string& password,
			unsigned userDataLen = 0, void *userData = nullptr);
	};
}
