#pragma once
#include "stdafx.h"

extern DWORD64 m_bRestrictServerCommands;

namespace
{
	/* ==== CVENGINECLIENT ================================================================================================================================================== */
	DWORD64 p_IVEngineClient_CommandExecute = FindPatternV2("r5apex.exe", (const unsigned char*)"\x48\x89\x5C\x24\x08\x57\x48\x83\xEC\x20\x48\x8D\x0D\x27\x61\xa5\x1E\x41\x8B\xD8", "xxxx?xxxxxxxx????xxx");
	void (*IVEngineClient_CommandExecute)(void* self, const char* cmd) = (void (*)(void*, const char*))p_IVEngineClient_CommandExecute; /*48 89 5C 24 ?? 57 48 83 EC 20 48 8D 0D ?? ?? ?? ?? 41 8B D8*/
}
