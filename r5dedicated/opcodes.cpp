#include "stdafx.h"
#include "opcodes.h"

/*-----------------------------------------------------------------------------
 * _opcodes.cpp
 *-----------------------------------------------------------------------------*/

void InstallOpcodes() /* .TEXT */
{
	//-------------------------------------------------------------------------
	// JNZ --> JMP | Prevent OriginSDK from initializing on the client
	WriteProcessMemory(GameProcess, LPVOID(dst000 + 0x0B), "\xE9\x63\x02\x00\x00\x00", 6, NULL);
	WriteProcessMemory(GameProcess, LPVOID(dst001 + 0x0E), "\xE9\xCB\x03\x00\x00", 5, NULL);
	//-------------------------------------------------------------------------
	// JNE --> JMP | Allow games to be loaded without the optional texture streaming file
	WriteProcessMemory(GameProcess, LPVOID(dst002 + 0x8E5), "\xEB\x19", 2, NULL);
	//-------------------------------------------------------------------------
	// JNE --> JMP | Prevent connect command from crashing by invalid call to UI function
	WriteProcessMemory(GameProcess, LPVOID(dst003 + 0x1D6), "\xEB\x27", 2, NULL);
	//-------------------------------------------------------------------------
	// JNE --> JMP | Prevent connect localhost from being executed after listenserver init
	//WriteProcessMemory(GameProcess, LPVOID(dst004 + 0x637), "\xE9\xC1\x00\x00\x00", 5, NULL);
	//-------------------------------------------------------------------------
	// JA  --> JMP | Disable server-side accpunt and packet verification
	WriteProcessMemory(GameProcess, LPVOID(dst005 + 0x53), "\x90\x90", 2, NULL);
	WriteProcessMemory(GameProcess, LPVOID(dst005 + 0x284), "\x90\x90", 2, NULL);
	//-------------------------------------------------------------------------
	// JA  --> JMP | Prevent FairFight anti-cheat from initializing on the server
	WriteProcessMemory(GameProcess, LPVOID(dst006 + 0x61), "\xE9\xED\x00\x00\x00\x00", 6, NULL);

	//-------------------------------------------------------------------------
	// JNE -- NOP | TODO: jumping over the instructions causes access violation when parsing settings layouts
	WriteProcessMemory(GameProcess, LPVOID(dst009 + 389), "\x90\x90", 2, NULL);
}

void ToggleOpcodes() /* .TEXT */
{
	static bool g_nop = true;

	if (g_nop)
	{
		//-------------------------------------------------------------------------
		// CALL --> NOP | Allow some maps to be loaded by nopping out a call in LoadProp function
		WriteProcessMemory(GameProcess, LPVOID(dst007 + 0x5E8), "\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90", 11, NULL);
		//-------------------------------------------------------------------------
		// CALL --> NOP | Disable the viewmodel rendered to avoid a crash from a certain entity in desertlands_mu1
		WriteProcessMemory(GameProcess, LPVOID(dst008 + 0x67), "\x90\x90\x90\x90\x90", 5, NULL);
		printf("\n");
		printf("+--------------------------------------------------------+\n");
		printf("|>>>>>>>>>>>>>>| TEXT OPCODES OVERWRITTEN |<<<<<<<<<<<<<<|\n");
		printf("+--------------------------------------------------------+\n");
		printf("\n");
	}
	else
	{
		//-------------------------------------------------------------------------
		// NOP --> CALL | Recover function DST007
		WriteProcessMemory(GameProcess, LPVOID(dst007 + 0x5E8), "\x48\x8B\x03\xFF\x90\xB0\x02\x00\x00\x84\xC0", 11, NULL);
		//-------------------------------------------------------------------------
		// NOP --> CALL | Recover function DST008
		WriteProcessMemory(GameProcess, LPVOID(dst008 + 0x67), "\xE8\x54\xD8\xFF\xFF", 5, NULL);
		printf("\n");
		printf("+--------------------------------------------------------+\n");
		printf("|>>>>>>>>>>>>>>>| TEXT OPCODES RECOVERED |<<<<<<<<<<<<<<<|\n");
		printf("+--------------------------------------------------------+\n");
		printf("\n");
	}
	g_nop = !g_nop;
}

void InstallGlobals() /* .DATA */
{
	//-------------------------------------------------------------------------
	//  00 --> 01  | Set Origin globals to allow execution of restricted commands
	WriteProcessMemory(GameProcess, LPVOID(ofs000), "\x01", 1, NULL);
	WriteProcessMemory(GameProcess, LPVOID(ofs001), "\x01", 1, NULL);
	WriteProcessMemory(GameProcess, LPVOID(ofs002), "\x01", 1, NULL);

	//-------------------------------------------------------------------------
	//  00 --> 01  | Set bDedicated to true to launch in dedicated mode
	WriteProcessMemory(GameProcess, LPVOID(ofs003), "\x01", 1, NULL);
}
