#pragma once
#include "stdafx.h"
#include "basetypes.h"

namespace
{
#if defined (GAMEDLL_S1) || defined (GAMEDLL_S1)

#elif defined (GAMEDLL_S2) || defined (GAMEDLL_S3)
	/* ==== CAPPSYSTEMGROUP ================================================================================================================================================= */
	DWORD64 p_IAppSystem_Main = FindPatternV2("r5apex.exe", (const unsigned char*)"\x48\x8B\xC4\x55\x41\x54\x41\x55\x41\x56\x41\x57\x48\x8B\xEC\x48\x83\xEC\x60", "xxxxxxxxxxxxxxxxxxx");
	std::int64_t(*IAppSystem_Main)(std::int64_t a1, std::int64_t a2) = (__int64(*)(std::int64_t, std::int64_t))p_IAppSystem_Main; /*48 8B C4 55 41 54 41 55 41 56 41 57 48 8B EC 48 83 EC 60*/

	DWORD64 p_IAppSystem_Create = FindPatternV2("r5apex.exe", (const unsigned char*)"\x40\x53\x48\x83\xEC\x20\x80\xB9\x00\x00\x00\x00\x00\xBB\x00\x00\x00\x00", "xxxxxxxx?????x????");
	char (*IAppSystem_Create)(std::int64_t a1) = (char(*)(std::int64_t))p_IAppSystem_Create; /*40 53 48 83 EC 20 80 B9 ?? ?? ?? ?? ?? BB ?? ?? ?? ??*/
#endif
}

///////////////////////////////////////////////////////////////////////////////
std::int64_t HIAppSystem_Main(std::int64_t a1, std::int64_t a2);
std::int64_t HIAppSystem_Create(std::int64_t a1);

void AttachIAppSystemHooks();
void DetachIAppSystemHooks();

///////////////////////////////////////////////////////////////////////////////
