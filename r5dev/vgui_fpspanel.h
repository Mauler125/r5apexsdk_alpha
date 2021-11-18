#pragma once
#include "stdafx.h"
#include "IConVar.h"

namespace
{
	/* ==== CFPSPANEL ======================================================================================================================================================= */
	ADDRESS p_CFPSPanel_Paint = g_mGameDll.FindPatternSIMD((std::uint8_t*)"\x48\x8B\xC4\x55\x56\x41\x00\x48\x8D\xA8\x00\xFD\xFF\xFF\x48\x81\xEC\x80", "xxxxxx?xxx?xxxxxxx");
	ConVar* (*CFPSPanel_Paint)(std::int64_t a1) = (ConVar* (*)(std::int64_t))p_CFPSPanel_Paint.GetPtr(); /*48 8B C4 55 56 41 ?? 48 8D A8 ?? FD FF FF 48 81 EC 80*/
}

void CFPSPanel_Attach();
void CFPSPanel_Detach();
