#pragma once

void InstallOpcodes();
void InstallGlobals();
void ToggleOpcodes();
void SetCHostState();
inline HANDLE GameProcess = GetCurrentProcess();

namespace
{
	/* -------------- ORIGIN ------------------------------------------------------------------------------------------------------------------------------------------------ */
	DWORD64 dst000 = /*0x14032EEA0*/ FindPatternV2("r5apex.exe", (const unsigned char*)"\x48\x83\xEC\x28\x80\x3D\x00\x00\x00\x23\x00\x0F\x85\x00\x02\x00", "xxxxxx???xxxx?xx");
	DWORD64 dst001 = /*0x140330290*/ FindPatternV2("r5apex.exe", (const unsigned char*)"\x48\x81\xEC\x58\x04\x00\x00\x80\x3D\x00\x00\x00\x00\x00\x0F\x84", "xxxxxxxxx????xxx");

	/* -------------- ENGINE ------------------------------------------------------------------------------------------------------------------------------------------------ */
	DWORD64 dst002 = /*0x14043FB90*/ FindPatternV2("r5apex.exe", (const unsigned char*)"\x48\x89\x4C\x24\x08\x56\x41\x55\x48\x81\xEC\x68\x03\x00\x00\x4C", "xxxx?xxxxxxxxxxx");
	DWORD64 dst003 = /*0x14022A4A0*/ FindPatternV2("r5apex.exe", (const unsigned char*)"\x48\x83\xEC\x38\x0F\x29\x74\x24\x20\x48\x89\x5C\x24\x40\x48\x8B", "xxxxxxxxxxxxxxxx");
	DWORD64 dst004 = /*0x140238DA0*/ FindPatternV2("r5apex.exe", (const unsigned char*)"\x48\x8B\xC4\x00\x41\x54\x41\x00\x48\x81\xEC\x00\x00\x00\x00\xF2", "xxx?xxx?xxx??xxx");

	/* -------------- NETCHAN ----------------------------------------------------------------------------------------------------------------------------------------------- */
	DWORD64 dst005 = /*0x14030D000*/ FindPatternV2("r5apex.exe", (const unsigned char*)"\x40\x55\x57\x41\x55\x41\x57\x48\x8D\xAC\x24\x28\xFF\xFF\xFF\x48", "xxxxxxxxxxxxxxxx");

	/* -------------- FAIRFIGHT --------------------------------------------------------------------------------------------------------------------------------------------- */
	DWORD64 dst006 = /*0x140303AE0*/ FindPatternV2("r5apex.exe", (const unsigned char*)"\x40\x53\x48\x83\xEC\x20\x8B\x81\xB0\x03\x00\x00\x48\x8B\xD9\xC6", "xxxxxxxxxxxxxxxx");

	/* -------------- OTHER ------------------------------------------------------------------------------------------------------------------------------------------------- */
	DWORD64 dst007 = /*0x14028F3B0*/ FindPatternV2("r5apex.exe", (const unsigned char*)"\x48\x8B\xC4\x44\x89\x40\x18\x48\x89\x50\x10\x55\x53\x56\x57\x41", "xxxxxxxxxxxxxxxx");
	DWORD64 dst008 = /*0x140E3E110*/ FindPatternV2("r5apex.exe", (const unsigned char*)"\x48\x83\xEC\x78\x48\x8B\x84\x24\x00\x00\x00\x00\x4D\x8B\xD8\x00", "xxxxxxxx????xxx?");

	DWORD64 dst009 = FindPatternV2("r5apex.exe", (const unsigned char*)"\x48\x8B\xC4\x55\x41\x54\x41\x55\x41\x56\x41\x57\x48\x8B\xEC\x48\x83\xEC\x60", "xxxxxxxxxxxxxxxxxxx");

	/* -------------- ------- ----------------------------------------------------------------------------------------------------------------------------------------------- */

	/* -------------- GLOBALS ----------------------------------------------------------------------------------------------------------------------------------------------- */
	DWORD64 ofs000 = 0x0000000000BAF5E8; //bDedicated

	// Renderer
	DWORD64 d0 = 0x00000001402FE280;
	DWORD64 d1 = 0x00000001403B3A50;
	DWORD64 d2 = 0x00000001403DEE90;
	DWORD64 d3 = 0x0000000140404380;
	DWORD64 d4 = 0x000000014040D850;
	DWORD64 d5 = 0x0000000140413260;
	DWORD64 d6 = 0x00000001402974F0;

	DWORD64 r0 = 0x00000001404093F0;
	DWORD64 r1 = 0x00000001403D2E60;
	DWORD64 r2 = 0x0000000140282E40;
	DWORD64 r3 = 0x00000001403BD142;

	DWORD64 c0 = 0x0000000140299100;
	DWORD64 c1 = 0x000000014039EFB0;
	DWORD64 c2 = 0x00000001403B40C0;
	DWORD64	c3 = 0x00000001403BBBFA;
	DWORD64	c4 = 0x00000001403DF870;
	DWORD64	c5 = 0x00000001403EDCD0;
	DWORD64	c6 = 0x00000001403EF6E0;
	DWORD64	c7 = 0x0000000140401CD0;

	DWORD64 d3d11init = 0x000000014043CDF0; // f-start
	DWORD64 x0 = 0x00000001403F4360; // f-start
	DWORD64 x1 = 0x00000001403F8A80; // f-start
	DWORD64 x2 = 0x0000000140FB2F10; // f-start
	DWORD64 x3 = 0x0000000140236E40; // Host_Init()

	DWORD64 x4 = 0x0000000140236640; // bdedicated inves
	DWORD64 x5 = 0x0000000140237B00; // bdedicated inves // custom respawn init implementation
	DWORD64 x6 = 0x0000000140231C00; // f-start
	DWORD64 x7 = 0x00000001402312A0; // HeartBeat

	DWORD64 CCreateWindow = 0x0000000140343DE0;
	DWORD64 SCR_BeginLoadingPlaque = 0x000000014023E870;

	void PrintOAddress() // Test the sigscan results
	{
		std::cout << "+--------------------------------------------------------+" << std::endl;
		std::cout << "| dst000                   : " << std::hex << std::uppercase << dst000 << std::setw(20) << " |" << std::endl;
		std::cout << "| dst001                   : " << std::hex << std::uppercase << dst001 << std::setw(20) << " |" << std::endl;
		std::cout << "+--------------------------------------------------------+" << std::endl;
		std::cout << "| dst002                   : " << std::hex << std::uppercase << dst002 << std::setw(20) << " |" << std::endl;
		std::cout << "| dst003                   : " << std::hex << std::uppercase << dst003 << std::setw(20) << " |" << std::endl;
		std::cout << "| dst004                   : " << std::hex << std::uppercase << dst004 << std::setw(20) << " |" << std::endl;
		std::cout << "+--------------------------------------------------------+" << std::endl;
		std::cout << "| dst005                   : " << std::hex << std::uppercase << dst005 << std::setw(20) << " |" << std::endl;
		std::cout << "| dst006                   : " << std::hex << std::uppercase << dst006 << std::setw(20) << " |" << std::endl;
		std::cout << "| dst007                   : " << std::hex << std::uppercase << dst007 << std::setw(20) << " |" << std::endl;
		std::cout << "| dst008                   : " << std::hex << std::uppercase << dst008 << std::setw(20) << " |" << std::endl;
		std::cout << "| dst009                   : " << std::hex << std::uppercase << dst009 << std::setw(20) << " |" << std::endl;
		std::cout << "+--------------------------------------------------------+" << std::endl;

		// TODO implement error handling when sigscan fails or result is 0
	}
}
