#define __STDC_WANT_LIB_EXT1__ // For strcpy_s
#include <string.h>
#include "Pomme.h"
#include "Network/NetGameHost.h"
#include "Network/NetGameJoin.h"

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
	strcpy_s(ioGameName,   sizeof(Str31), "MyGame");
	strcpy_s(ioPlayerName, sizeof(Str31), "ImTheHost");
	strcpy_s(ioPassword,   sizeof(Str31), "password");
	return true;
}

NSpAddressReference NSpDoModalJoinDialog(
	ConstStr31Param /*inGameType*/,
	ConstStr31Param /*inEntityListLabel*/,
	Str31 ioName,
	Str31 ioPassword,
	NSpEventProcPtr /*inEventProcPtr*/)
{
	// These will be set in the dialog
	NSPAddressPrivate *addr = new NSPAddressPrivate{};
	strcpy_s(addr->address, sizeof(addr->address), "127.0.0.1");
	addr->port = 2500;

	strcpy_s(ioName,     sizeof(Str31), "ImJoined");
	strcpy_s(ioPassword, sizeof(Str31), "password");
	return addr;
}

void NSpReleaseAddressReference(NSpAddressReference inAddress)
{
	delete inAddress;
}

OSStatus NSpGame_Delete(NSpGameReference inGame, NSpFlags /*inFlags*/)
{
	delete inGame->game;
	delete inGame;
	return 0;
}

OSStatus NSpGame_Host(
	NSpGameReference *outGame,
	NSpProtocolListReference inProtocolList,
	UInt32                   inMaxPlayers,
	ConstStr31Param          inGameName,
	ConstStr31Param          inPassword,
	ConstStr31Param          /*inPlayerName may be NULL, if NULL, no player*/,
	NSpPlayerType            /*inPlayerType*/,
	NSpTopology              /*inTopology*/,
	NSpFlags                 /*inFlags*/)
{
	NetGameHost *netGame = new NetGameHost(inGameName, inPassword, inMaxPlayers);
	netGame->startListening(inProtocolList->protocol.port);

	*outGame = new NSpGamePrivate{};
	(*outGame)->game = netGame;
	return 0;
}

OSStatus NSpGame_EnableAdvertising(NSpGameReference inGame, NSpProtocolReference /*inProtocol*/, Boolean inEnable)
{
	if(!inEnable)
	{
		NetGameHost *hostGame = dynamic_cast<NetGameHost *>(inGame->game);
		if(hostGame == nullptr)
			return kNSpInvalidGameRefErr;

		hostGame->stopListening();
	}
	return 0;
}

OSStatus NSpGame_Join(
	NSpGameReference *outGame,
	NSpAddressReference inAddress,
	ConstStr31Param inName,
	ConstStr31Param inPassword,
	NSpPlayerType inType,
	UInt32 inUserDataLen,
	void *inUserData,
	NSpFlags /*inFlags*/)
{
	// Create our player info
	NSpPlayerInfo ourPlayerInfo{};
	ourPlayerInfo.type = inType;
	strcpy_s(ourPlayerInfo.name, sizeof(Str31), inName);

	// Create NetGame and join game
	NetGameJoin *netGame = new NetGameJoin(ourPlayerInfo);
	netGame->joinGame(inAddress, inPassword, inUserDataLen, inUserData);

	// Set GameReference
	*outGame = new NSpGamePrivate{};
	(*outGame)->game = netGame;
	return 0;
}