#define __STDC_WANT_LIB_EXT1__ // For strcpy_s
#include <string.h>
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

OSStatus NSpProtocolList_Create(NSpProtocolReference inProtocolRef, NSpProtocolListReference *outList)
{
	*outList = new NSpListPrivate{};
	(*outList)->protocol = (inProtocolRef == NULL) ? NSpProtocolPrivate{} : *inProtocolRef;
	return 0;
}

void NSpProtocolList_Delete(NSpProtocolListReference inProtocolList)
{
	delete inProtocolList;
}

Boolean NSpDoModalHostDialog(NSpProtocolListReference ioProtocolList, Str31 ioGameName,
	Str31 ioPlayerName, Str31 ioPassword, NSpEventProcPtr /*inEventProcPtr*/)
{
	// These will be set in the dialog
	ioProtocolList->protocol.port = 2500;
	strcpy_s(ioGameName, sizeof(Str31), "MyGame");
	strcpy_s(ioPlayerName, sizeof(Str31), "ImTheHost");
	strcpy_s(ioPassword, sizeof(Str31), "password");
	return true;
}

OSStatus NSpGame_Host(
	NSpGameReference *outGame,
	NSpProtocolListReference inProtocolList,
	UInt32                   inMaxPlayers,
	ConstStr31Param          inGameName,
	ConstStr31Param          inPassword,
	ConstStr31Param          /*inPlayerName*/,
	NSpPlayerType            /*inPlayerType*/,
	NSpTopology              /*inTopology*/,
	NSpFlags                 /*inFlags*/)
{
	*outGame = new NSpGamePrivate{};
	(*outGame)->game = new NetGame(inGameName);
	(*outGame)->game->startListening(inPassword, inMaxPlayers, inProtocolList->protocol.port, false);
	return 0;
}

OSStatus NSpGame_EnableAdvertising(NSpGameReference inGame, NSpProtocolReference /*inProtocol*/, Boolean inEnable)
{
	if(!inEnable)
		inGame->game->stopListening();
	return 0;
}
