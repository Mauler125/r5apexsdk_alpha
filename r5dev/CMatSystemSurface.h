#pragma once
#include "stdafx.h"
#include "basetypes.h"
#include "CHLClient.h"

namespace
{
	/* ==== CMATSYSTEMSURFACE =============================================================================================================================================== */
	ADDRESS p_CMatSystemSurface_DrawColoredText = g_mGameDll.FindPatternSIMD((std::uint8_t*)"\x4C\x8B\xDC\x48\x83\xEC\x68\x49\x8D\x43\x58\x0F\x57\xC0", "xxxxxxxxxxxxxx"); /*4C 8B DC 48 83 EC 68 49 8D 43 58 0F 57 C0*/
	void* (*CMatSystemSurface_DrawColoredText)(void* thisptr, int font, int fontHeight, int offsetX, int offsetY, int red, int green, int blue, int alpha, const char* text, ...) = (void* (*)(void*, int, int, int, int, int, int, int, int, const char*, ...))p_CMatSystemSurface_DrawColoredText.GetPtr();
#if defined (GAMEDLL_S0) || defined (GAMEDLL_S1)
	ADDRESS p_CMatSystemSurface_Unknown0 = g_mGameDll.FindPatternSIMD((std::uint8_t*)"\x48\x8B\x0D\x00\x00\x00\x00\x48\x8B\x01\x48\xFF\xA0\x18\x01\x00\x00", "xxx????xxxxxxxxxx"); /*48 8B 0D ?? ?? ?? ?? 48 8B 01 48 FF A0 18 01 00 00*/
	std::int64_t(*CMatSystemSurface_Unknown0)() = (std::int64_t(*)())p_CMatSystemSurface_Unknown0.GetPtr(); // [ AMOS ] DELETE
#elif defined (GAMEDLL_S2) || defined (GAMEDLL_S3)
	ADDRESS p_CMatSystemSurface_Unknown0 = g_mGameDll.FindPatternSIMD((std::uint8_t*)"\x48\x8B\x0D\x00\x00\x00\x00\x48\x8B\x01\x48\xFF\xA0\x20\x01\x00\x00", "xxx????xxxxxxxxxx"); /*48 8B 0D ?? ?? ?? ?? 48 8B 01 48 FF A0 20 01 00 00*/
	std::int64_t(*CMatSystemSurface_Unknown0)() = (std::int64_t(*)())p_CMatSystemSurface_Unknown0.GetPtr(); // [ AMOS ] DELETE
#endif
	ADDRESS g_pMatSystemSurface = p_CHLClient_PostInit.FindPatternSelf("48 83 3D", ADDRESS::Direction::DOWN, 40).ResolveRelativeAddressSelf(0x3, 0x8).GetPtr();
}
