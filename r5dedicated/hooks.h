#pragma once
#include "stdafx.h"
#include "classes.h"
#include "IAppSystem.h"
#include "IConVar.h"
#include "ConCommand.h"
#include "CVEngineServer.h"
#include "CNetChan.h"
#include "EbisuSDK.h"
#include "sqvm.h"
#include "sys.h"
#include "msgbox.h"
#include "opcodes.h"

// Define the signatures or offsets to be searched and hooked
namespace
{
	/* ==== CAPPSYSTEMGROUP ================================================================================================================================================= */
	DWORD64 p_IAppSystem_Main = FindPatternV2("r5apex.exe", (const unsigned char*)"\x48\x8B\xC4\x55\x41\x54\x41\x55\x41\x56\x41\x57\x48\x8B\xEC\x48\x83\xEC\x60", "xxxxxxxxxxxxxxxxxxx");
	__int64 (*IAppSystem_Main)(__int64 a1, __int64 a2) = (__int64(*)(__int64, __int64))p_IAppSystem_Main; /*40 53 48 83 EC 20 80 B9 ? ? ? ? ? BB ? ? ? ?*/

	DWORD64 p_IAppSystem_Create = FindPatternV2("r5apex.exe", (const unsigned char*)"\x40\x53\x48\x83\xEC\x20\x80\xB9\x00\x00\x00\x00\x00\xBB\x00\x00\x00\x00", "xxxxxxxx?????x????");
	char (*IAppSystem_Create)(__int64 a1) = (char(*)(__int64))p_IAppSystem_Create; /*48 8B C4 55 41 54 41 55 41 56 41 57 48 8B EC 48 83 EC 60*/

	/* ==== CVENGINECLIENT ================================================================================================================================================== */
	DWORD64 p_CommandExecute = FindPatternV2("r5apex.exe", (const unsigned char*)"\x48\x89\x5C\x24\x08\x57\x48\x83\xEC\x20\x48\x8D\x0D\x27\x61\xa5\x1E\x41\x8B\xD8", "xxxx?xxxxxxxx????xxx");
	void (*CommandExecute)(void* self, const char* cmd) = (void (*)(void*, const char*))p_CommandExecute; /*48 89 5C 24 ?? 57 48 83 EC 20 48 8D 0D ?? ?? ?? ?? 41 8B D8*/

	/* ==== CVENGINESERVER ================================================================================================================================================== */
	DWORD64 p_Persistence_IsAvailable = FindPatternV2("r5apex.exe", (const unsigned char*)"\x3B\x15\x00\x00\x00\x00\x7D\x33", "xx????xx");
	bool (*Persistence_IsAvailable)(__int64 entidx, int clientidx) = (bool (*)(__int64, int))p_Persistence_IsAvailable; /*3B 15 ?? ?? ?? ?? 7D 33*/

	/* ==== CONCOMMAND ====================================================================================================================================================== */
	DWORD64 p_ConCommand_IsFlagSet = FindPatternV2("r5apex.exe", (const unsigned char*)"\x85\x51\x38\x0F\x95\xC0\xC3", "xxxxxxx");
	bool (*ConCommand_IsFlagSet)(ConCommandBase* cmd, int flag) = (bool (*)(ConCommandBase*, int))p_ConCommand_IsFlagSet; /*85 51 38 0F 95 C0 C3*/

	/* ==== ICONVAR ========================================================================================================================================================= */
	DWORD64 p_IConVar_IsFlagSet = FindPatternV2("r5apex.exe", (const unsigned char*)"\x48\x8B\x41\x48\x85\x50\x38", "xxxxxxx");
	bool (*IConVar_IsFlagSet)(ConVar* cvar, int flag) = (bool (*)(ConVar*, int))p_IConVar_IsFlagSet; /*48 8B 41 48 85 50 38*/

	/* ==== SQUIRREL ======================================================================================================================================================== */
	DWORD64 p_SQVM_PrintFunc = FindPatternV2("r5apex.exe", (const unsigned char*)"\x48\x8B\xC4\x48\x89\x50\x10\x4C\x89\x40\x18\x4C\x89\x48\x20\x53\x56\x57\x48\x81\xEC\x30\x08\x00\x00\x48\x8B\xDA\x48\x8D\x70\x18\x48\x8B\xF9\xE8\x00\x00\x00\xFF\x48\x89\x74\x24\x28\x48\x8D\x54\x24\x30\x33", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx???xxxxxxxxxxxx");
	void* SQVM_PrintFunc = (void*)p_SQVM_PrintFunc; /*48 8B C4 48 89 50 10 4C 89 40 18 4C 89 48 20 53 56 57 48 81 EC 30 08 00 00 48 8B DA 48 8D 70 18 48 8B F9 E8 ?? ?? ?? FF 48 89 74 24 28 48 8D 54 24 30 33*/

	//DWORD64 p_SQVM_LoadScript = FindPatternV2("r5apex.exe", (const unsigned char*)"\x48\x89\x5C\x24\x10\x48\x89\x74\x24\x18\x48\x89\x7C\x24\x20\x48\x89\x4C\x24\x08\x55\x41\x54\x41\x55\x41\x56\x41\x57\x48\x8D\x6C", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"); // For S0 and S1
	DWORD64 p_SQVM_LoadScript = FindPatternV2("r5apex.exe", (const unsigned char*)"\x48\x8B\xC4\x48\x89\x48\x08\x55\x41\x56\x48\x8D\x68", "xxxxxxxxxxxxx"); // For anything S2 and above (current S8)
	bool (*SQVM_LoadScript)(void* sqvm, const char* script_path, const char* script_name, int flag) = (bool (*)(void*, const char*, const char*, int))p_SQVM_LoadScript; /*E8 ?? ?? ?? ?? 84 C0 74 1C 41 B9 ?? ?? ?? ??*/

	DWORD64 p_SQVM_LoadRson = FindPatternV2("r5apex.exe", (const unsigned char*)"\x4C\x8B\xDC\x49\x89\x5B\x08\x57\x48\x81\xEC\xA0\x00\x00\x00\x33", "xxxxxxxxxxxxxxxx");
	int (*SQVM_LoadRson)(const char* rson_name) = (int (*)(const char*))p_SQVM_LoadRson; /*4C 8B DC 49 89 5B 08 57 48 81 EC A0 00 00 00 33*/

	/* ==== CNETCHAN ======================================================================================================================================================== */
	DWORD64 p_NET_ReceiveDatagram = FindPatternV2("r5apex.exe", (const unsigned char*)"\x48\x89\x74\x24\x18\x48\x89\x7C\x24\x20\x55\x41\x54\x41\x55\x41\x56\x41\x57\x48\x8D\xAC\x24\x50\xEB", "xxxxxxxxxxxxxxxxxxxxxxxxx");
	bool (*NET_ReceiveDatagram)(int, void*, bool) = (bool (*)(int, void*, bool))p_NET_ReceiveDatagram; /*E8 ?? ?? ?? ?? 84 C0 75 35 48 8B D3*/

	DWORD64 p_NET_SendDatagram = FindPatternV2("r5apex.exe", (const unsigned char*)"\x48\x89\x5C\x24\x08\x48\x89\x6C\x24\x10\x48\x89\x74\x24\x18\x57\x41\x56\x41\x57\x48\x81\xEC\x00\x05\x00\x00", "xxxxxxxxxxxxxxxxxxxxxxx?xxx");
	int (*NET_SendDatagram)(SOCKET s, const char* buf, int len, int flags) = (int (*)(SOCKET, const char*, int, int))p_NET_SendDatagram; /*48 89 5C 24 08 48 89 6C 24 10 48 89 74 24 18 57 41 56 41 57 48 81 EC ?? 05 00 00*/

	/* ==== UTILITY ========================================================================================================================================================= */
	DWORD64 p_MSG_EngineError = FindPatternV2("r5apex.exe", (const unsigned char*)"\x48\x89\x5C\x24\x08\x48\x89\x74\x24\x10\x57\x48\x81\xEC\x30\x08\x00\x00\x48\x8B\xDA\x48\x8B\xF9\xE8\x00\x00\x00\xFF\x33\xF6\x48", "xxxxxxxxxxxxxxxxxxxxxxxxx???xxxx");
	int (*MSG_EngineError)(char* fmt, va_list args) = (int (*)(char*, va_list))p_MSG_EngineError; /*48 89 5C 24 08 48 89 74 24 10 57 48 81 EC 30 08 00 00 48 8B DA 48 8B F9 E8 ?? ?? ?? FF 33 F6 48*/

	DWORD64 p_QHull_PrintError = FindPatternV2("r5apex.exe", (const unsigned char*)"\x48\x89\x4C\x24\x08\x48\x89\x54\x24\x10\x4C\x89\x44\x24\x18\x4C\x89\x4C\x24\x20\x53\xB8\x40\x27\x00\x00\x00\x00\x00\x00\x00\x48", "xxxxxxxxxxxxxxxxxxxxxxxxxx????xx");
	int (*QHull_PrintError)(char* fmt, va_list args) = (int (*)(char*, va_list))p_QHull_PrintError; /*48 89 4C 24 08 48 89 54 24 10 4C 89 44 24 18 4C 89 4C 24 20 53 B8 40 27 00 00 ?? ?? ?? ?? 00 48*/

	DWORD64 p_QHull_PrintDebug = FindPatternV2("r5apex.exe", (const unsigned char*)"\x48\x89\x54\x24\x10\x4C\x89\x44\x24\x18\x4C\x89\x4C\x24\x20\x53\x56\x57\x48\x83\xEC\x30\x48\x8B\xFA\x48\x8D\x74\x24\x60\x48\x8B", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
	int (*QHull_PrintDebug)(char* fmt, va_list args) = (int (*)(char*, va_list))p_QHull_PrintDebug; /*48 89 54 24 10 4C 89 44 24 18 4C 89 4C 24 20 53 56 57 48 83 EC 30 48 8B FA 48 8D 74 24 60 48 8B*/

	DWORD64 p_Sys_PrintFunc = FindPatternV2("r5apex.exe", (const unsigned char*)"\x48\x89\x4C\x24\x08\x48\x89\x54\x24\x10\x4C\x89\x44\x24\x18\x4C\x89\x4C\x24\x20\x53\x55\x41\x54\x41\x56\xB8\x58\x10\x00\x00\xE8", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
	void* Sys_PrintFunc = (void*)p_Sys_PrintFunc; /*48 89 4C 24 08 48 89 54 24 10 4C 89 44 24 18 4C 89 4C 24 20 53 55 41 54 41 56 B8 58 10 00 00 E8*/

	/* ==== KEYVALUES ======================================================================================================================================================= */
	// DWORD64 p_KeyValues_FindKey = /*1404744E0*/ reinterpret_cast<DWORD64>(PatternScan("r5apex.exe", "40 56 57 41 57 48 81 EC ?? ?? ?? ?? 45"));

	/* ==== ------- ========================================================================================================================================================= */

	void PrintHAddress() // Test the sigscan results
	{
		std::cout << "+--------------------------------------------------------+" << std::endl;
		std::cout << "| IAppSystem_Main          : " << std::hex << std::uppercase << p_IAppSystem_Main         << std::setw(20) << " |" << std::endl;
		std::cout << "| IAppSystem_Create        : " << std::hex << std::uppercase << p_IAppSystem_Create       << std::setw(20) << " |" << std::endl;
		std::cout << "+--------------------------------------------------------+" << std::endl;
		std::cout << "| CommandExecute           : " << std::hex << std::uppercase << p_CommandExecute          << std::setw(20) << " |" << std::endl;
		std::cout << "| ConVar_IsFlagSet         : " << std::hex << std::uppercase << p_IConVar_IsFlagSet       << std::setw(20) << " |" << std::endl;
		std::cout << "| ConCommand_IsFlagSet     : " << std::hex << std::uppercase << p_ConCommand_IsFlagSet    << std::setw(20) << " |" << std::endl;
		std::cout << "+--------------------------------------------------------+" << std::endl;
		std::cout << "| Persistence_IsAvailable  : " << std::hex << std::uppercase << p_Persistence_IsAvailable << std::setw(20) << " |" << std::endl;
		std::cout << "+--------------------------------------------------------+" << std::endl;
		std::cout << "| SQVM_PrintFunc           : " << std::hex << std::uppercase << p_SQVM_PrintFunc          << std::setw(20) << " |" << std::endl;
		std::cout << "| SQVM_LoadScript          : " << std::hex << std::uppercase << p_SQVM_LoadScript         << std::setw(20) << " |" << std::endl;
		std::cout << "| SQVM_LoadRson            : " << std::hex << std::uppercase << p_SQVM_LoadRson           << std::setw(20) << " |" << std::endl;
		std::cout << "+--------------------------------------------------------+" << std::endl;
		std::cout << "| NET_ReceiveDatagram      : " << std::hex << std::uppercase << p_NET_ReceiveDatagram     << std::setw(20) << " |" << std::endl;
		std::cout << "| NET_SendDatagram         : " << std::hex << std::uppercase << p_NET_SendDatagram        << std::setw(20) << " |" << std::endl;
		std::cout << "+--------------------------------------------------------+" << std::endl;
		std::cout << "| MSG_EngineError          : " << std::hex << std::uppercase << p_MSG_EngineError         << std::setw(20) << " |" << std::endl;
		std::cout << "| QHull_PrintError         : " << std::hex << std::uppercase << p_QHull_PrintError        << std::setw(20) << " |" << std::endl;
		std::cout << "| QHull_PrintDebug         : " << std::hex << std::uppercase << p_QHull_PrintDebug        << std::setw(20) << " |" << std::endl;
		std::cout << "| Sys_PrintFunc            : " << std::hex << std::uppercase << Sys_PrintFunc             << std::setw(20) << " |" << std::endl;
		std::cout << "+--------------------------------------------------------+" << std::endl;

		// TODO implement error handling when sigscan fails or result is 0
	}

	inline bool g_bDebugConsole   = false;
	inline bool g_bReturnAllFalse = false;
	inline bool g_bDebugLoading   = false;
}

void InstallHooks();
void RemoveHooks();

void ToggleDevCommands();
void ToggleNetTrace();
