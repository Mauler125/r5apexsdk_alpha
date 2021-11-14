#pragma once
#include "stdafx.h"

namespace
{
	/* ==== CMATSYSTEMSURFACE =============================================================================================================================================== */
	ADDRESS p_CMatSystemSurface_DrawColoredText = g_mGameDll.FindPatternSIMD((std::uint8_t*)"\x41\x55\x41\x56\x48\x83\xEC\x78\x44\x8B\xEA", "xxxxxxxxxxx"); /*41 55 41 56 48 83 EC 78 44 8B EA*/
	void* (*CMatSystemSurface_DrawColoredText)(void* thisptr, int a0, int fontHeight, int a3, int a4, int red, int green, int blue, int alpha, const char* text, ...) = (void* (*)(void*, int, int, int, int, int, int, int, int, const char*, ...))p_CMatSystemSurface_DrawColoredText.GetPtr();
}
