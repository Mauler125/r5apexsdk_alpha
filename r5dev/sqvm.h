#pragma once
#include "stdafx.h"
#include "hooks.h"
#include "CGameConsole.h"

struct SQFuncRegistration
{
	const char* scriptName;   // 00
	const char* nativeName;   // 08
	const char* helpString;   // 10
	const char* retValType;   // 18
	const char* argTypes;     // 20
	std::int16_t unk28;       // 28
	std::int16_t padding1;    // 2A
	std::int32_t unk2c;       // 2C
	std::int64_t unk30;       // 30
	std::int32_t unk38;       // 38
	std::int32_t padding2;    // 3C
	std::int64_t unk40;       // 40
	std::int64_t unk48;       // 48
	std::int64_t unk50;       // 50
	std::int32_t unk58;       // 58
	std::int32_t padding3;    // 5C
	void* funcPtr;            // 60

	SQFuncRegistration()
	{
		memset(this, 0, sizeof(SQFuncRegistration));
		this->padding2 = 6;
	}
};

namespace
{
	/* ==== SQUIRREL ======================================================================================================================================================== */
	DWORD64 p_SQVM_PrintFunc = FindPatternV2("r5apex.exe", (const unsigned char*)"\x48\x8B\xC4\x48\x89\x50\x10\x4C\x89\x40\x18\x4C\x89\x48\x20\x53\x56\x57\x48\x81\xEC\x30\x08\x00\x00\x48\x8B\xDA\x48\x8D\x70\x18\x48\x8B\xF9\xE8\x00\x00\x00\xFF\x48\x89\x74\x24\x28\x48\x8D\x54\x24\x30\x33", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx???xxxxxxxxxxxx");
	void* SQVM_PrintFunc = (void*)p_SQVM_PrintFunc; /*48 8B C4 48 89 50 10 4C 89 40 18 4C 89 48 20 53 56 57 48 81 EC 30 08 00 00 48 8B DA 48 8D 70 18 48 8B F9 E8 ?? ?? ?? FF 48 89 74 24 28 48 8D 54 24 30 33*/

	DWORD64 p_SQVM_WarningFunc = FindPatternV2("r5apex.exe", (const unsigned char*)"\x4C\x89\x4C\x24\x20\x44\x89\x44\x24\x18\x89\x54\x24\x10\x53\x55\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x83\xEC\x00\x48\x8B", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxx?xx");
	__int64 (*SQVM_WarningFunc)(void* sqvm, int a2, int a3, int* stringSize, void** string) = (__int64 (*)(void*, int, int, int*, void**))p_SQVM_WarningFunc; /*4C 89 4C 24 20 44 89 44 24 18 89 54 24 10 53 55 56 57 41 54 41 55 41 56 41 57 48 83 EC ?? 48 8B*/

	DWORD64 p_SQVM_WarningCmd = FindPatternV2("r5apex.exe", (const unsigned char*)"\x40\x53\x48\x83\xEC\x30\x33\xDB\x48\x8D\x44\x24\x00\x4C\x8D\x4C\x24\x00", "xxxxxxxxxxxx?xxxx?");
	__int64 (*SQVM_WarningCmd)(int a1, int a2) = (__int64 (*)(int, int))p_SQVM_WarningCmd; /*40 53 48 83 EC 30 33 DB 48 8D 44 24 ?? 4C 8D 4C 24 ??*/

	//DWORD64 p_SQVM_LoadScript = FindPatternV2("r5apex.exe", (const unsigned char*)"\x48\x89\x5C\x24\x10\x48\x89\x74\x24\x18\x48\x89\x7C\x24\x20\x48\x89\x4C\x24\x08\x55\x41\x54\x41\x55\x41\x56\x41\x57\x48\x8D\x6C", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"); // For S0 and S1
	DWORD64 p_SQVM_LoadScript = FindPatternV2("r5apex.exe", (const unsigned char*)"\x48\x8B\xC4\x48\x89\x48\x08\x55\x41\x56\x48\x8D\x68", "xxxxxxxxxxxxx"); // For anything S2 and above (current S8)
	bool (*SQVM_LoadScript)(void* sqvm, const char* script_path, const char* script_name, int flag) = (bool (*)(void*, const char*, const char*, int))p_SQVM_LoadScript; /*E8 ?? ?? ?? ?? 84 C0 74 1C 41 B9 ?? ?? ?? ??*/

	DWORD64 p_SQVM_LoadRson = FindPatternV2("r5apex.exe", (const unsigned char*)"\x4C\x8B\xDC\x49\x89\x5B\x08\x57\x48\x81\xEC\xA0\x00\x00\x00\x33", "xxxxxxxxxxxxxxxx");
	int (*SQVM_LoadRson)(const char* rson_name) = (int (*)(const char*))p_SQVM_LoadRson; /*4C 8B DC 49 89 5B 08 57 48 81 EC A0 00 00 00 33*/
}

void* HSQVM_PrintFunc(void* sqvm, char* fmt, ...);
__int64 HSQVM_LoadRson(const char* rson_name);
bool HSQVM_LoadScript(void* sqvm, const char* script_path, const char* script_name, int flag);

void SQVM_RegisterFunction(void* sqvm, const char* name, const char* helpString, const char* retValType, const char* argTypes, void* funcPtr);
int SQVM_NativeTest(void* sqvm);


void AttachSQVMHooks();
void DetachSQVMHooks();

///////////////////////////////////////////////////////////////////////////////
extern bool g_bSQVM_WarnFuncCalled;
