#include "stdafx.h"
#include "classes.h"
#include "cvar.h"
#include "CEngineVGui.h"
#include "CHostState.h"
#include "CInputSystem.h"
#include "IConVar.h"
#include "CClient.h"
#include "CKeyValuesSystem.h"
#include "IVEngineClient.h"
#include "CClient.h"

#include "stryder.h"
#include "IAppSystem.h"

void ClassInit()
{
#if defined (GAMEDLL_S0) || defined (GAMEDLL_S1)
	g_pCvar = *reinterpret_cast<CCVar**>(p_CCvar_Disconnect.FindPatternSelf("4C 8B", ADDRESS::Direction::DOWN, 40).ResolveRelativeAddressSelf(0x3, 0x7).GetPtr()); // VERIFY S1 '0x14C1AD8D0'.
	g_pPlaylistKeyValues = reinterpret_cast<KeyValues**>(p_Stryder_StitchRequest.FindPatternSelf("48 8B 2D", ADDRESS::Direction::DOWN, 100).ResolveRelativeAddressSelf(0x3, 0x7).GetPtr()); // Get the KeyValue for the playlist file.
#elif defined (GAMEDLL_S2) || defined (GAMEDLL_S3)
	g_pCvar = *reinterpret_cast<CCVar**>(p_CCvar_Disconnect.FindPatternSelf("48 8B", ADDRESS::Direction::DOWN, 40).ResolveRelativeAddressSelf(0x3, 0x7).GetPtr());
	g_pPlaylistKeyValues = reinterpret_cast<KeyValues**>(p_Stryder_StitchRequest.FindPatternSelf("48 8B 0D", ADDRESS::Direction::DOWN, 100).ResolveRelativeAddressSelf(0x3, 0x7).GetPtr()); // Get the KeyValue for the playlist file.
#endif
	g_pKeyValuesSystem = reinterpret_cast<CKeyValuesSystem*>(p_KeyValues_Init.FindPatternSelf("48 8D ?? ?? ?? ?? 01", ADDRESS::Direction::DOWN, 100).ResolveRelativeAddressSelf(0x3, 0x7).GetPtr());
	g_pHostState       = reinterpret_cast<CHostState*>(p_CHostState_FrameUpdate.FindPatternSelf("48 8D ?? ?? ?? ?? 01", ADDRESS::Direction::DOWN, 100).ResolveRelativeAddressSelf(0x3, 0x7).GetPtr());
	g_pInputSystem     = reinterpret_cast<CInputSystem*>(p_IAppSystem_LoadLibrary.FindPatternSelf("48 89 05", ADDRESS::Direction::DOWN, 40).ResolveRelativeAddressSelf(0x3, 0x7).GetPtr());
	g_pClient          = reinterpret_cast<CClient*>(g_pClientStart.GetPtr());
	g_pBanSystem       = new CBanSystem();

	*(char*)m_bRestrictServerCommands.GetPtr() = true; // Restrict commands.
	void* disconnect = g_pCvar->FindCommand("disconnect");
	*(std::int32_t*)((std::uintptr_t)disconnect + 0x38) |= FCVAR_SERVER_CAN_EXECUTE; // Make sure server is not restricted to this.

	if (g_pCvar->FindVar("net_usesocketsforloopback")->m_iValue != 1)
	{
		g_pCvar->FindVar("net_usesocketsforloopback")->m_iValue = 1; // Set 'net_usesocketsforloopback' to 1 for dedicated server.
	}
}
