#pragma once
#include "stdafx.h"

namespace
{
	/* ==== CVENGINECLIENT ================================================================================================================================================== */
	ADDRESS p_IVEngineClient_CommandExecute = g_mGameDll.FindPatternSIMD((std::uint8_t*)"\x48\x89\x5C\x24\x08\x57\x48\x83\xEC\x20\x48\x8D\x0D\x27\x61\xa5\x1E\x41\x8B\xD8", "xxxx?xxxxxxxx????xxx");
	void (*IVEngineClient_CommandExecute)(void* self, const char* cmd) = (void (*)(void*, const char*))p_IVEngineClient_CommandExecute.GetPtr(); /*48 89 5C 24 ?? 57 48 83 EC 20 48 8D 0D ?? ?? ?? ?? 41 8B D8*/
}

///////////////////////////////////////////////////////////////////////////////
extern DWORD64 m_bRestrictServerCommands;

///////////////////////////////////////////////////////////////////////////////
