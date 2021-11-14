#pragma once
#include "stdafx.h"

namespace
{
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	DWORD64 p_Sys_Error = FindPatternV2("r5apex.exe", (const unsigned char*)"\x48\x89\x4C\x24\x08\x48\x89\x54\x24\x10\x4C\x89\x44\x24\x18\x4C\x89\x4C\x24\x20\x53\x55\x41\x54\x41\x56\xB8\x58\x10\x00\x00\xE8", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
	void* Sys_Error = (void*)p_Sys_Error; /*48 89 4C 24 08 48 89 54 24 10 4C 89 44 24 18 4C 89 4C 24 20 53 55 41 54 41 56 B8 58 10 00 00 E8*/

	DWORD64 p_Sys_LoadAsset = FindPatternV2("r5apex.exe", (const unsigned char*)"\x48\x89\x74\x24\x10\x48\x89\x7C\x24\x18\x41\x56\x48\x83\xEC\x40\x33", "xxxxxxxxxxxxxxxxx");
	std::int64_t(*Sys_LoadAsset)(const CHAR* lpFileName, std::int64_t a2, LARGE_INTEGER* a3) = (std::int64_t(*)(const CHAR*, std::int64_t, LARGE_INTEGER*))p_Sys_LoadAsset;/*48 89 74 24 10 48 89 7C 24 18 41 56 48 83 EC 40 33*/

	DWORD64 p_MemAlloc_Wrapper = FindPatternV2("r5apex.exe", (const unsigned char*)"\x40\x53\x48\x83\xEC\x20\x48\x8B\x05\x6B\x83\x25\x0D\x48\x8B\xD9", "xxxxxxxxxxxxxxxx");
	void* (*MemAlloc_Wrapper)(std::int64_t size) = (void* (*)(std::int64_t))p_MemAlloc_Wrapper; /*40 53 48 83 EC 20 48 8B 05 6B 83 25 0D 48 8B D9*/
	/* ==== ------- ========================================================================================================================================================= */
}

enum class SYS_DLL : int
{
	SERVER = 0, // Game DLL
	CLIENT = 1, // Game DLL
	UI     = 2, // Game DLL
	ENGINE = 3, // Wrapper
	FS     = 4, // File System
	RTECH  = 5, // RTech API
	MS     = 6  // Material System
};

///////////////////////////////////////////////////////////////////////////////
void HSys_Error(char* fmt, ...);
void Sys_Print(SYS_DLL idx, const char* fmt, ...);

void AttachSysUtilsHooks();
void DetachSysUtilsHooks();
