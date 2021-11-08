#pragma once
#include "stdafx.h"
#include "msgbox.h"
#include "opcptc.h"

// Define the signatures or offsets to be searched and hooked
namespace
{
	/* ==== UTILITY ========================================================================================================================================================= */
	DWORD64 p_MSG_EngineError = FindPatternV2("r5apex.exe", (const unsigned char*)"\x48\x89\x5C\x24\x08\x48\x89\x74\x24\x10\x57\x48\x81\xEC\x30\x08\x00\x00\x48\x8B\xDA\x48\x8B\xF9\xE8\x00\x00\x00\xFF\x33\xF6\x48", "xxxxxxxxxxxxxxxxxxxxxxxxx???xxxx");
	int (*MSG_EngineError)(char* fmt, va_list args) = (int (*)(char*, va_list))p_MSG_EngineError; /*48 89 5C 24 08 48 89 74 24 10 57 48 81 EC 30 08 00 00 48 8B DA 48 8B F9 E8 ?? ?? ?? FF 33 F6 48*/

	DWORD64 p_QHull_PrintError = FindPatternV2("r5apex.exe", (const unsigned char*)"\x48\x89\x4C\x24\x08\x48\x89\x54\x24\x10\x4C\x89\x44\x24\x18\x4C\x89\x4C\x24\x20\x53\xB8\x40\x27\x00\x00\x00\x00\x00\x00\x00\x48", "xxxxxxxxxxxxxxxxxxxxxxxxxx????xx");
	int (*QHull_PrintError)(char* fmt, va_list args) = (int (*)(char*, va_list))p_QHull_PrintError; /*48 89 4C 24 08 48 89 54 24 10 4C 89 44 24 18 4C 89 4C 24 20 53 B8 40 27 00 00 ?? ?? ?? ?? 00 48*/

	DWORD64 p_QHull_PrintDebug = FindPatternV2("r5apex.exe", (const unsigned char*)"\x48\x89\x54\x24\x10\x4C\x89\x44\x24\x18\x4C\x89\x4C\x24\x20\x53\x56\x57\x48\x83\xEC\x30\x48\x8B\xFA\x48\x8D\x74\x24\x60\x48\x8B", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
	int (*QHull_PrintDebug)(char* fmt, va_list args) = (int (*)(char*, va_list))p_QHull_PrintDebug; /*48 89 54 24 10 4C 89 44 24 18 4C 89 4C 24 20 53 56 57 48 83 EC 30 48 8B FA 48 8D 74 24 60 48 8B*/

	DWORD64 p_Sys_PrintFunc = FindPatternV2("r5apex.exe", (const unsigned char*)"\x48\x89\x4C\x24\x08\x48\x89\x54\x24\x10\x4C\x89\x44\x24\x18\x4C\x89\x4C\x24\x20\x53\x55\x41\x54\x41\x56\xB8\x58\x10\x00\x00\xE8", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
	void* Sys_PrintFunc = (void*)p_Sys_PrintFunc; /*48 89 4C 24 08 48 89 54 24 10 4C 89 44 24 18 4C 89 4C 24 20 53 55 41 54 41 56 B8 58 10 00 00 E8*/

	DWORD64 p_NET_PrintFunc = FindPatternV2("r5apex.exe", (const unsigned char*)"\x48\x89\x54\x24\x10\x4C\x89\x44\x24\x18\x4C\x89\x4C\x24\x20\xC3\x48", "xxxxxxxxxxxxxxxxx");
	void (*NET_PrintFunc)(const char* a1) = (void(*)(const char*))p_NET_PrintFunc; /*48 89 54 24 10 4C 89 44 24 18 4C 89 4C 24 20 C3 48*/

	DWORD64 p_MemAlloc_Wrapper = FindPatternV2("r5apex.exe", (const unsigned char*)"\x40\x53\x48\x83\xEC\x20\x48\x8B\x05\x6B\x83\x25\x0D\x48\x8B\xD9", "xxxxxxxxxxxxxxxx");
	void* (*MemAlloc_Wrapper)(__int64 size) = (void* (*)(__int64))p_MemAlloc_Wrapper; /*40 53 48 83 EC 20 48 8B 05 6B 83 25 0D 48 8B D9*/

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/* ==== ------- ========================================================================================================================================================= */
}

void InstallHooks();
void RemoveHooks();
void PrintHAddress();
