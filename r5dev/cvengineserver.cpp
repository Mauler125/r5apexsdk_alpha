#include "pch.h"
#include "CVEngineServer.h"

//-----------------------------------------------------------------------------
// Sets the persistence var in the playerstruct to ready for each client
//-----------------------------------------------------------------------------
bool HPersistence_IsAvailable(__int64 thisptr, int client)
{
	static bool isPersistenceVarSet[256];

	// TODO: Maybe not hardcode
	std::uintptr_t playerStructBase = 0x16073B200;
	std::uintptr_t playerStructSize = 0x4A4C0;
	std::uintptr_t persistenceVar = 0x5BC;

	std::uintptr_t targetPlayerStruct = playerStructBase + client * playerStructSize;

	*(char*)(targetPlayerStruct + persistenceVar) = (char)0x5;

	if (!isPersistenceVarSet[client])
	{
		printf("\n");
		printf("##################################################\n");
		printf("] SETTING PERSISTENCE VAR FOR CLIENT #%d\n", client);
		printf("##################################################\n");
		printf("\n");
		isPersistenceVarSet[client] = true;
	}

	///////////////////////////////////////////////////////////////////////////
	return Persistence_IsAvailable(thisptr, client);
}

void AttachCEngineServerHooks()
{
	DetourAttach((LPVOID*)&Persistence_IsAvailable, &HPersistence_IsAvailable);
}

void DetachCEngineServerHooks()
{
	DetourDetach((LPVOID*)&Persistence_IsAvailable, &HPersistence_IsAvailable);
}
