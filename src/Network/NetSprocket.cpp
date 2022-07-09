#include "Pomme.h"
#include "Network/NetGame.h"

using namespace Pomme::Network;
static UInt32 gStandardMessageSize = 0;
static UInt32 gBufferSize          = 0;

OSStatus NSpInitialize(UInt32 inStandardMessageSize, UInt32 inBufferSize,
	UInt32 /*inQElements*/, NSpGameID /*inGameID*/, UInt32 /*inTimeout*/)
{
	gStandardMessageSize = inStandardMessageSize;
	gBufferSize          = inBufferSize;
	return 0;
}

OSStatus NSpGame_Delete(NSpGameReference inGame, NSpFlags /*inFlags*/)
{
	delete inGame->game;
	delete inGame;
	return 0;
}

Boolean NSpDoModalHostDialog(NSpProtocolListReference /*ioProtocolList*/, Str31 /*ioGameName*/,
	Str31 /*ioPlayerName*/, Str31 /*ioPassword*/, NSpEventProcPtr /*inEventProcPtr*/)
{
	return true;
}

OSStatus NSpGame_Host(
	NSpGameReference *outGame,
	NSpProtocolListReference /*inProtocolList*/,
	UInt32                   /*inMaxPlayers*/,
	ConstStr31Param          /*inGameName*/,
	ConstStr31Param          /*inPassword*/,
	ConstStr31Param          /*inPlayerName*/,
	NSpPlayerType            /*inPlayerType*/,
	NSpTopology              /*inTopology*/,
	NSpFlags                 /*inFlags*/)
{
	*outGame = new NSpGamePrivate{};
	(*outGame)->game = new NetGame();
	(*outGame)->game->startListening(2500, false);
	return 0;
}

OSStatus NSpGame_EnableAdvertising(NSpGameReference inGame, NSpProtocolReference /*inProtocol*/, Boolean inEnable)
{
	if(!inEnable)
		inGame->game->stopListening();
	return 0;
}
