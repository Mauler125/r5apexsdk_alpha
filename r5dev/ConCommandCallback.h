#pragma once
#include "stdafx.h"
#include "basetypes.h"
#include "ConCommand.h"
#include "IConVar.h"
#include "CClient.h"
#include "CNetChan.h"

namespace
{
	/* ==== CONCOMMANDCALLBACK ============================================================================================================================================== */
#if defined (GAMEDLL_S1)
	ADDRESS p_Map_Callback = g_mGameDll.FindPatternSIMD((std::uint8_t*)"\x48\x89\x5C\x24\x18\x55\x41\x56\x41\x00\x00\x00\x00\x40\x02", "xxxxxxxxx????xx");
	void (*Map_Callback)(CCommand* cmd) = (void (*)(CCommand * cmd))p_Map_Callback.GetPtr(); /*48 89 5C 24 18 55 41 56 41 ?? ?? ?? ?? 40 02*/
#elif defined (GAMEDLL_S2) || defined (GAMEDLL_S3)
	ADDRESS p_Map_Callback = g_mGameDll.FindPatternSIMD((std::uint8_t*)"\x40\x55\x41\x56\x41\x57\x48\x81\xEC\x00\x00\x00\x00\x83\x3D", "xxxxxxxxx????xx");
	void (*Map_Callback)(CCommand* cmd) = (void (*)(CCommand * cmd))p_Map_Callback.GetPtr(); /*40 55 41 56 41 57 48 81 EC ?? ?? ?? ?? 83 3D*/
#endif
}

///////////////////////////////////////////////////////////////////////////////
void CGameConsole_Callback(const CCommand& cmd);
void CCompanion_Callback(const CCommand& cmd);
void Kick_Callback(CCommand* cmd);
void KickID_Callback(CCommand* cmd);
void Ban_Callback(CCommand* cmd);
void BanID_Callback(CCommand* cmd);
void Unban_Callback(CCommand* cmd);
void ReloadBanList_Callback(CCommand* cmd);
void RTech_GenerateGUID_Callback(CCommand* cmd);
void RTech_Decompress_Callback(CCommand* cmd);
void NET_TraceNetChan_Callback(CCommand* cmd);
void NET_SetKey_Callback(CCommand* cmd);
void NET_GenerateKey_Callback(CCommand* cmd);

///////////////////////////////////////////////////////////////////////////////
