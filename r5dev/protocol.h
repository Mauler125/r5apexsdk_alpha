#pragma once
#include "stdafx.h"

/*-----------------------------------------------------------------------------
 * _protocol.h
 *-----------------------------------------------------------------------------*/

enum SIGNONSTATE
{
	SIGNONSTATE_NONE         = 0, // no state yet; about to connect
	SIGNONSTATE_CHALLENGE    = 1, // client challenging server; all OOB packets
	SIGNONSTATE_CONNECTED    = 2, // client is connected to server; netchans ready
	SIGNONSTATE_NEW          = 3, // just got serverinfo and string tables
	SIGNONSTATE_PRESPAWN     = 4, // received signon buffers
	SIGNONSTATE_GETTING_DATA = 5, // getting persistence data
	SIGNONSTATE_SPAWN        = 6, // ready to receive entity packets
	SIGNONSTATE_FIRST_SNAP   = 7, // received baseline snapshot
	SIGNONSTATE_FULL         = 8, // we are fully connected; first non-delta packet received
	SIGNONSTATE_CHANGELEVEL  = 9, // server is changing level; please wait
};
