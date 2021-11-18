#pragma once
#include "stdafx.h"
#include "basetypes.h"
#include "ConCommandCallback.h"

namespace
{
	ADDRESS p_EbisuSDK_Init_Tier0 = g_mGameDll.FindPatternSIMD((std::uint8_t*)"\x48\x83\xEC\x28\x80\x3D\x00\x00\x00\x00\x00\x0F\x85\x00\x02\x00\x00\x48\x89\x5C\x24\x20", "xxxxxx????xxx?xxxxxxxx").GetPtr();
	void(*EbisuSDK_Init_Tier0) = (void(*))p_EbisuSDK_Init_Tier0.GetPtr(); /*48 83 EC 28 80 3D ?? ?? ?? ?? 00 0F 85 ?? 02 00 00 48 89 5C 24 20*/

	ADDRESS p_EbisuSDK_CVar_Init = g_mGameDll.FindPatternSIMD((std::uint8_t*)"\x40\x57\x48\x83\xEC\x40\x83\x3D", "xxxxxxxx");
	void(*EbisuSDK_CVar_Init) = (void(*))p_EbisuSDK_CVar_Init.GetPtr(); /*40 57 48 83 EC 40 83 3D*/
}

namespace
{
#if defined (GAMEDLL_S1)
	ADDRESS g_bEbisuSDKInitialized = p_EbisuSDK_Init_Tier0.FindPatternSelf("80 3D ?? ?? ?? ?? 00", ADDRESS::Direction::DOWN, 150).ResolveRelativeAddressSelf(0x2, 0x7).GetPtr();
	ADDRESS g_bEbisuSDKCvarInitialized = p_Map_Callback.FindPatternSelf("80 3D 8F 7C 1E 22 00", ADDRESS::Direction::DOWN, 150).ResolveRelativeAddressSelf(0x2, 0x7).GetPtr();
	ADDRESS g_qEbisuSDKCvarInitialized = p_EbisuSDK_CVar_Init.FindPatternSelf("4C 89 05 C4 2B 0E 22", ADDRESS::Direction::DOWN, 150).ResolveRelativeAddressSelf(0x3, 0x7).GetPtr();
#elif defined (GAMEDLL_S2)
	ADDRESS g_bEbisuSDKInitialized = p_EbisuSDK_Init_Tier0.FindPatternSelf("80 3D ?? ?? ?? ?? 00", ADDRESS::Direction::DOWN, 150).ResolveRelativeAddressSelf(0x2, 0x7).GetPtr();
	ADDRESS g_bEbisuSDKCvarInitialized = p_Map_Callback.FindPatternSelf("80 3D 43 2D 41 22 00", ADDRESS::Direction::DOWN, 150).ResolveRelativeAddressSelf(0x2, 0x7).GetPtr();
	ADDRESS g_qEbisuSDKCvarInitialized = p_EbisuSDK_CVar_Init.FindPatternSelf("4C 89 05 74 2D 32 22", ADDRESS::Direction::DOWN, 150).ResolveRelativeAddressSelf(0x3, 0x7).GetPtr();
#elif defined (GAMEDLL_S3)
	ADDRESS g_bEbisuSDKInitialized = p_EbisuSDK_Init_Tier0.FindPatternSelf("80 3D ?? ?? ?? ?? 00", ADDRESS::Direction::DOWN, 150).ResolveRelativeAddressSelf(0x2, 0x7).GetPtr();
	ADDRESS g_bEbisuSDKCvarInitialized = p_Map_Callback.FindPatternSelf("80 3D 23 54 2B 23 00", ADDRESS::Direction::DOWN, 150).ResolveRelativeAddressSelf(0x2, 0x7).GetPtr();
	ADDRESS g_qEbisuSDKCvarInitialized = p_EbisuSDK_CVar_Init.FindPatternSelf("4C 89 05 B4 2C 1C 23", ADDRESS::Direction::DOWN, 150).ResolveRelativeAddressSelf(0x3, 0x7).GetPtr();
#endif
}

///////////////////////////////////////////////////////////////////////////////
void HEbisuSDK_Init();
void EbisuSDK_Attach();
void EbisuSDK_Detach();

///////////////////////////////////////////////////////////////////////////////
