#pragma once
#include "stdafx.h"

namespace
{
	/* ==== CMATSYSTEMSURFACE =============================================================================================================================================== */
	ADDRESS p_CMatSystemSurface_DrawColoredText = g_mGameDll.FindPatternSIMD((std::uint8_t*)"\x4C\x8B\xDC\x48\x83\xEC\x68\x49\x8D\x43\x58\x0F\x57\xC0", "xxxxxxxxxxxxxx"); /*4C 8B DC 48 83 EC 68 49 8D 43 58 0F 57 C0*/
	void* (*CMatSystemSurface_DrawColoredText)(void* thisptr, int font, int fontHeight, int offsetX, int offsetY, int red, int green, int blue, int alpha, const char* text, ...) = (void* (*)(void*, int, int, int, int, int, int, int, int, const char*, ...))p_CMatSystemSurface_DrawColoredText.GetPtr();
}
