#include "stdafx.h"
#include "opcodes.h"
#include <classes.h>

/*-----------------------------------------------------------------------------
 * _opcodes.cpp
 *-----------------------------------------------------------------------------*/

auto p_is_dedicated = (uint8_t*)0x162C61208;

void DisableRenderer()
{
	WriteProcessMemory(GameProcess, LPVOID(d0), "\xC3", 1, NULL);
	WriteProcessMemory(GameProcess, LPVOID(d1), "\xC3", 1, NULL);
	WriteProcessMemory(GameProcess, LPVOID(d2), "\xC3", 1, NULL);
	WriteProcessMemory(GameProcess, LPVOID(d3), "\xC3", 1, NULL);
	WriteProcessMemory(GameProcess, LPVOID(d4), "\xC3", 1, NULL);
	WriteProcessMemory(GameProcess, LPVOID(d5), "\xC3", 1, NULL);
	WriteProcessMemory(GameProcess, LPVOID(d6), "\x90\x90\x90\x90\x90\x90", 6, NULL);
}

PVOID sub_14044AFA0 = 0;
char __fastcall sub_14044AFA0_hk(__int64 a1) {
	*p_is_dedicated = 1; // HAS TO BE HERE!!!

	return reinterpret_cast<decltype(&sub_14044AFA0_hk)>(sub_14044AFA0)(a1);
}

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
	WriteProcessMemory(GameProcess, LPVOID(dst004 + 0x637), "\xE9\xC1\x00\x00\x00", 5, NULL);
	//-------------------------------------------------------------------------
	// JA  --> JMP | Disable server-side accpunt and packet verification
	WriteProcessMemory(GameProcess, LPVOID(dst005 + 0x53), "\x90\x90", 2, NULL);
	WriteProcessMemory(GameProcess, LPVOID(dst005 + 0x284), "\x90\x90", 2, NULL);
	//-------------------------------------------------------------------------
	// JA  --> JMP | Prevent FairFight anti-cheat from initializing on the server
	WriteProcessMemory(GameProcess, LPVOID(dst006 + 0x61), "\xE9\xED\x00\x00\x00\x00", 6, NULL);

	//-------------------------------------------------------------------------
	// JNE --> NOP | TODO: jumping over the instructions causes access violation when parsing settings layouts
	WriteProcessMemory(GameProcess, LPVOID(dst009 + 0x389), "\x90\x90", 2, NULL);

	//-------------------------------------------------------------------------
	// MOV --> NOP | Nop CreateGameWindow initialization code
	WriteProcessMemory(GameProcess, LPVOID(CCreateWindow + 0x41B), "\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90", 49, NULL);
	//-------------------------------------------------------------------------
	// JNE --> JMP | Jump over CreateGameWindow validation code
	WriteProcessMemory(GameProcess, LPVOID(CCreateWindow + 0x44C), "\xEB\x49", 2, NULL);
	//-------------------------------------------------------------------------
	// PUS --> XOR | Prevent ShowWindow and CreateGameWindow from being initialized
	WriteProcessMemory(GameProcess, LPVOID(c0), "\x30\xC0\xC3", 3, NULL);

	////WriteProcessMemory(GameProcess, LPVOID(c1), "\xC3", 1, NULL);
	//WriteProcessMemory(GameProcess, LPVOID(c2), "\xC3", 1, NULL);
	//WriteProcessMemory(GameProcess, LPVOID(c3), "\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90", 24, NULL);
	//WriteProcessMemory(GameProcess, LPVOID(c4), "\xC3", 1, NULL);
	//WriteProcessMemory(GameProcess, LPVOID(c5), "\xC3", 1, NULL);
	//WriteProcessMemory(GameProcess, LPVOID(c6), "\xC3", 1, NULL); // 1402378B0
	//WriteProcessMemory(GameProcess, LPVOID(c7), "\xC3", 1, NULL); // 1402378BD

	//WriteProcessMemory(GameProcess, LPVOID(d3d11init + 0x1A), "\xE9\xF2\x00\x00\x00", 5, NULL);

	WriteProcessMemory(GameProcess, LPVOID(x0 + 0x23C), "\x90\x90\x90\x90\x90\x90", 6, NULL);//d
	WriteProcessMemory(GameProcess, LPVOID(x0 + 0x2BD), "\x90\x90\x90", 3, NULL);//d

	WriteProcessMemory(GameProcess, LPVOID(x1 + 0xA9), "\x90\x90\x90\x90", 4, NULL);//d
	WriteProcessMemory(GameProcess, LPVOID(x2 + 0x213), "\x90\x90\x90\x90\x90\x90", 6, NULL); // <-- inves //d

	WriteProcessMemory(GameProcess, LPVOID(x3 + 0x182), "\x90\x90\x90\x90\x90", 5, NULL); // <-- inves //d
	WriteProcessMemory(GameProcess, LPVOID(x3 + 0xA7D), "\xE9\xF0\x01\x00\x00", 5, NULL);//d

	WriteProcessMemory(GameProcess, LPVOID(x4 + 0x26F), "\xE9\x4D\x05\x00\x00", 5, NULL);

	WriteProcessMemory(GameProcess, LPVOID(x5 + 0x19), "\xEB\x6E", 2, NULL);
	WriteProcessMemory(GameProcess, LPVOID(x5 + 0x609), "\xEB\x2B", 2, NULL);
	WriteProcessMemory(GameProcess, LPVOID(x5 + 0x621), "\xEB\x0C", 2, NULL);
	WriteProcessMemory(GameProcess, LPVOID(x5 + 0x658), "\xE9\x8C\x00\x00\x00", 5, NULL);
	WriteProcessMemory(GameProcess, LPVOID(x5 + 0x6E9), "\xE9\xB0\x00\x00\x00", 5, NULL);

	WriteProcessMemory(GameProcess, LPVOID(x6 + 0x1C6), "\xE9\xAD\x11\x00\x00", 5, NULL); // TODO: DISABLE THIS!
	WriteProcessMemory(GameProcess, LPVOID(x6 + 0x1023), "\x90\x90\x90", 3, NULL);
	WriteProcessMemory(GameProcess, LPVOID(x6 + 0x1010), "\xEB\x14", 2, NULL);

	WriteProcessMemory(GameProcess, LPVOID(x7 + 0xF), "\xE9\x22\x01\x00\x00", 5, NULL);

	WriteProcessMemory(GameProcess, LPVOID(SCR_BeginLoadingPlaque + 0x427), "\xEB\x09", 2, NULL);


	*p_is_dedicated = 1;

	*(uintptr_t*)0x14D415040 = 0x1417304E8;
	*(uintptr_t*)0x14B37C3C0 = 0x141F10CA0;

	DisableRenderer();

	{
		// binder for the app group - 2 reset dedicated to 1
		auto addr_AppGroupBind = PVOID(0x14044AFA0);

		DetourAttach((LPVOID*)&addr_AppGroupBind, &sub_14044AFA0_hk);
	}

	{
		// begin
		WriteProcessMemory(GameProcess, LPVOID(r0), "\xC3\x90\x90\x90\x90", 5, NULL);
		// end
		WriteProcessMemory(GameProcess, LPVOID(r1), "\xC3\x90\x90\x90\x90\x90", 6, NULL);
	}

	{
		WriteProcessMemory(GameProcess, LPVOID(r2), "\x48\x33\xC0\xC3\x90\x90\x90", 7, NULL);
	}

	// this is bs, TFO:R had same shit, but setting 1 and returning from some func worked cuz we had dedicated and were doing init ourselves lmfao...
	{
		//init_matshit_mode_based
		WriteProcessMemory(GameProcess, LPVOID(r3), "\xEB", 1, NULL);
	}
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
	//  00 --> 01  | Set bDedicated to true to launch in dedicated mode
	WriteProcessMemory(GameProcess, LPVOID(ofs000), "\x01", 1, NULL);
}

void SetCHostState()
{
	static std::string ServerMap = std::string();
	ServerMap = "mp_rr_canyonlands_64k_x_64k";
	strncpy_s(GameGlobals::HostState->m_levelName, ServerMap.c_str(), 64); // Copy new map into hoststate levelname. 64 is size of m_levelname.
	GameGlobals::HostState->m_iNextState = HostStates_t::HS_NEW_GAME; // Force CHostState::FrameUpdate to start a server.
	printf("HS_NEW_GAME\n");
}
