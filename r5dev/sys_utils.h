#pragma once
#include "hooks.h"
#include "CGameConsole.h"

namespace
{
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	DWORD64 p_Sys_Error = FindPatternV2("r5apex.exe", (const unsigned char*)"\x48\x89\x4C\x24\x08\x48\x89\x54\x24\x10\x4C\x89\x44\x24\x18\x4C\x89\x4C\x24\x20\x53\x55\x41\x54\x41\x56\xB8\x58\x10\x00\x00\xE8", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
	void* Sys_Error = (void*)p_Sys_Error; /*48 89 4C 24 08 48 89 54 24 10 4C 89 44 24 18 4C 89 4C 24 20 53 55 41 54 41 56 B8 58 10 00 00 E8*/

	DWORD64 p_MemAlloc_Wrapper = FindPatternV2("r5apex.exe", (const unsigned char*)"\x40\x53\x48\x83\xEC\x20\x48\x8B\x05\x6B\x83\x25\x0D\x48\x8B\xD9", "xxxxxxxxxxxxxxxx");
	void* (*MemAlloc_Wrapper)(__int64 size) = (void* (*)(__int64))p_MemAlloc_Wrapper; /*40 53 48 83 EC 20 48 8B 05 6B 83 25 0D 48 8B D9*/
	/* ==== ------- ========================================================================================================================================================= */
}

///////////////////////////////////////////////////////////////////////////////
void HSys_Error(char* fmt, ...);

void AttachSysUtilsHooks();
void DetachSysUtilsHooks();
