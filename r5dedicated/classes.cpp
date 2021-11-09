#include "stdafx.h"
#include "classes.h"

#include "CHostState.h"
#include "IConVar.h"
#include "CClient.h"
#include "CKeyValuesSystem.h"
#include "IVEngineClient.h"

void ClassInit()
{
	g_pHostState         = reinterpret_cast<CHostState*>      (0x141736120); // Get CHostState from memory.
	g_pCvar              = *reinterpret_cast<CCVar**>         (0x14D40B348); // Get CCVar from memory.
	g_pKeyValuesSystem   = reinterpret_cast<CKeyValuesSystem*>(0x141F105C0); // Get CKeyValuesSystem from memory.
	g_pPlaylistKeyValues = reinterpret_cast<KeyValues**>      (0x16705B980); // Get the KeyValue for the playlist file.
	g_pClient            = reinterpret_cast<CClient*>         (0x16073B200);
	g_pBanSystem         = new CBanSystem();

	*(char*)m_bRestrictServerCommands = true; // Restrict commands.
	void* disconnect = g_pCvar->FindCommand("disconnect");
	*(std::int32_t*)((std::uintptr_t)disconnect + 0x38) |= FCVAR_SERVER_CAN_EXECUTE; // Make sure server is not restricted to this.
}
