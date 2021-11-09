#pragma once

void InstallOpcodes();
inline HANDLE GameProcess = GetCurrentProcess();

namespace
{
	/* -------------- ORIGIN ------------------------------------------------------------------------------------------------------------------------------------------------ */
	Address Origin_Init     = /*0x14032EEA0*/ FindPatternV2("r5apex.exe", (const unsigned char*)"\x48\x83\xEC\x28\x80\x3D\x00\x00\x00\x23\x00\x0F\x85\x00\x02\x00", "xxxxxx???xxxx?xx");
	Address Origin_SetState = /*0x140330290*/ FindPatternV2("r5apex.exe", (const unsigned char*)"\x48\x81\xEC\x58\x04\x00\x00\x80\x3D\x00\x00\x00\x00\x00\x0F\x84", "xxxxxxxxx????xxx");

	/* -------------- ENGINE ------------------------------------------------------------------------------------------------------------------------------------------------ */
	Address dst002 = /*0x14043FB90*/ FindPatternV2("r5apex.exe", (const unsigned char*)"\x48\x89\x4C\x24\x08\x56\x41\x55\x48\x81\xEC\x68\x03\x00\x00\x4C", "xxxx?xxxxxxxxxxx");
	Address dst003 = /*0x14022A4A0*/ FindPatternV2("r5apex.exe", (const unsigned char*)"\x48\x83\xEC\x38\x0F\x29\x74\x24\x20\x48\x89\x5C\x24\x40\x48\x8B", "xxxxxxxxxxxxxxxx");
	Address Host_NewGame    = /*0x140238DA0*/ FindPatternV2("r5apex.exe", (const unsigned char*)"\x48\x8B\xC4\x00\x41\x54\x41\x00\x48\x81\xEC\x00\x00\x00\x00\xF2", "xxx?xxx?xxx??xxx");

	/* -------------- NETCHAN ----------------------------------------------------------------------------------------------------------------------------------------------- */
	Address CServer_Auth    = /*0x14030D000*/ FindPatternV2("r5apex.exe", (const unsigned char*)"\x40\x55\x57\x41\x55\x41\x57\x48\x8D\xAC\x24\x28\xFF\xFF\xFF\x48", "xxxxxxxxxxxxxxxx");

	/* -------------- FAIRFIGHT --------------------------------------------------------------------------------------------------------------------------------------------- */
	Address FairFight_Init  = /*0x140303AE0*/ FindPatternV2("r5apex.exe", (const unsigned char*)"\x40\x53\x48\x83\xEC\x20\x8B\x81\xB0\x03\x00\x00\x48\x8B\xD9\xC6", "xxxxxxxxxxxxxxxx");

	/* -------------- OTHER ------------------------------------------------------------------------------------------------------------------------------------------------- */
	Address dst007 = /*0x14028F3B0*/ FindPatternV2("r5apex.exe", (const unsigned char*)"\x48\x8B\xC4\x44\x89\x40\x18\x48\x89\x50\x10\x55\x53\x56\x57\x41", "xxxxxxxxxxxxxxxx");
	Address dst008 = /*0x140E3E110*/ FindPatternV2("r5apex.exe", (const unsigned char*)"\x48\x83\xEC\x78\x48\x8B\x84\x24\x00\x00\x00\x00\x4D\x8B\xD8\x00", "xxxxxxxx????xxx?");

	Address dst009 = FindPatternV2("r5apex.exe", (const unsigned char*)"\x48\x8B\xC4\x55\x41\x54\x41\x55\x41\x56\x41\x57\x48\x8B\xEC\x48\x83\xEC\x60", "xxxxxxxxxxxxxxxxxxx");

	/* -------------- ------- ----------------------------------------------------------------------------------------------------------------------------------------------- */

	/* -------------- GLOBALS ----------------------------------------------------------------------------------------------------------------------------------------------- */
#pragma once

	inline HANDLE GameProcess = GetCurrentProcess();

	namespace
	{
		// TODO: create patterns instead and rename to function names.
		// Renderer
		Address r0 = 0x00000001402FE280; //
		Address gCShaderGlue__Init = 0x00000001403B3A50; //
		Address gMatSync = 0x00000001403DEE90; //
		Address gCMaterialSystem__MatsysMode_Init = 0x00000001403BD120; //
		Address r4 = 0x0000000140404380; //
		Address r5 = 0x000000014040D850; //
		Address r6 = 0x0000000140413260; //
		Address r7 = 0x00000001404093F0; //
		Address r8 = 0x00000001403D2E60; //
		Address d3d11init = 0x000000014043CDF0; //

		// Engine
		Address gHost_Init_0 = 0x0000000140236E40; // main Host_Init()?
		Address e1 = 0x0000000140FB2F10; // also used by CServerGameDLL
		Address addr_CEngine_Frame = 0x00000001402970E0;
		Address e3 = 0x0000000140231C00;
		Address e4 = 0x0000000140BE1970;
		Address e5 = 0x0000000140DBBAF0;
		Address e6 = 0x0000000140DBE610;
		Address e7 = 0x000000014044AFA0;
		Address e8 = 0x000000014027EC50; // RenderFrame?
		Address gCEngineAPI__Init = 0x0000000140342FB0; //
		Address gCEngineAPI__ModInit = 0x0000000140343DE0; //
		Address gCEngineAPI__Connect = 0x0000000140342BA0; //
		Address gCEngineAPI__OnStartup = 0x0000000140343860; //
		Address gCSourceAppSystemGroup__Create = 0x000000014044AFA0; //
		Address gCShaderSystem__Init = 0x00000001403DF870; //
		Address gInitMaterialSystem = 0x000000014024B390; //
		Address gCVideoMode_Common__DrawStartupGraphic = 0x000000014027F0F0; //
		Address gShaderDispatch = 0x00000001403EE5C0;
		Address gShaderCreate = 0x00000001403ECD00; //
		Address gTextureCreate = 0x00000001403EDCD0;

		Address gCShaderSystem__9 = 0x00000001403DFC30;
		Address gBSP_LUMP_INIT = 0x00000001402546F0; // BSP.


		Address e9 = 0x00000001404066E0;
		Address e10 = 0x00000001403B49E0; // CMaterialGlue?

		// SERVER
		Address gHost_Init_1 = 0x0000000140237B00; // server Host_Init()?
		Address s1 = 0x0000000140231C00; // _Host_RunFrame() with inlined CFrameTimer::MarkFrame()?
		Address s2 = 0x00000001402312A0; // server HeartBeat? (baseserver.cpp)
		Address s3 = 0x0000000140FB36D0; // TEMP??

		// CLIENT
		Address gHost_Init_2 = 0x0000000140236640; // client Host_Init()?
		Address gCGame__CreateGameWindow = 0x0000000140299100; //
		Address c2 = 0x00000001403F4360; // 1403DF870 --> 1403F4360
		Address c3 = 0x00000001403F8A80; // 1403DF870 --> 1403F8A40
		Address gCHLClient__1000 = 0x00000001405C27B0; // CHLClient + 1000
		Address gCHLClient__HudMessage = 0x00000001405BAC00; // CHudMessage
		Address c6 = 0x00000001403CA2D0; //

		// VGUI
		Address gCEngineVGui__Init = 0x0000000140282E40; // jumptable
		Address gCEngineVGui__OnLevelLoadingStarted = 0x00000001402830D0;
		Address SCR_BeginLoadingPlaque = 0x000000014023E870;
	}

	void PrintOAddress() // Test the sigscan results
	{
		std::cout << "+--------------------------------------------------------+" << std::endl;
		std::cout << "| Origin_Init              : " << std::hex << std::uppercase << Origin_Init.GetPtr() << std::setw(20) << " |" << std::endl;
		std::cout << "| Origin_SetState          : " << std::hex << std::uppercase << Origin_SetState.GetPtr() << std::setw(20) << " |" << std::endl;
		std::cout << "+--------------------------------------------------------+" << std::endl;
		std::cout << "| dst002                   : " << std::hex << std::uppercase << dst002.GetPtr() << std::setw(20) << " |" << std::endl;
		std::cout << "| dst003                   : " << std::hex << std::uppercase << dst003.GetPtr() << std::setw(20) << " |" << std::endl;
		std::cout << "| Host_NewGame             : " << std::hex << std::uppercase << Host_NewGame.GetPtr() << std::setw(20) << " |" << std::endl;
		std::cout << "+--------------------------------------------------------+" << std::endl;
		std::cout << "| CServer_Auth             : " << std::hex << std::uppercase << CServer_Auth.GetPtr()   << std::setw(20) << " |" << std::endl;
		std::cout << "| FairFight_Init           : " << std::hex << std::uppercase << FairFight_Init.GetPtr() << std::setw(20) << " |" << std::endl;
		std::cout << "| dst007                   : " << std::hex << std::uppercase << dst007.GetPtr() << std::setw(20) << " |" << std::endl;
		std::cout << "| dst008                   : " << std::hex << std::uppercase << dst008.GetPtr() << std::setw(20) << " |" << std::endl;
		std::cout << "| dst009                   : " << std::hex << std::uppercase << dst009.GetPtr() << std::setw(20) << " |" << std::endl;
		std::cout << "+--------------------------------------------------------+" << std::endl;

		// TODO implement error handling when sigscan fails or result is 0
	}
}
