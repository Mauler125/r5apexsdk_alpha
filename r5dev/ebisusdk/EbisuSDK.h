#pragma once
#include "tier0/basetypes.h"
#include "tier0/completion.h"
#include "public/include/utility.h"

namespace
{
#ifdef DEDICATED
	ADDRESS p_EbisuSDK_Init_Tier0 = g_mGameDll.FindPatternSIMD((std::uint8_t*)"\x48\x83\xEC\x28\x80\x3D\x00\x00\x00\x00\x00\x0F\x85\x00\x02\x00\x00\x48\x89\x5C\x24\x20", "xxxxxx????xxx?xxxxxxxx").GetPtr();
	void(*EbisuSDK_Init_Tier0) = (void(*))p_EbisuSDK_Init_Tier0.GetPtr(); /*48 83 EC 28 80 3D ?? ?? ?? ?? 00 0F 85 ?? 02 00 00 48 89 5C 24 20*/

	ADDRESS p_EbisuSDK_CVar_Init = g_mGameDll.FindPatternSIMD((std::uint8_t*)"\x40\x57\x48\x83\xEC\x40\x83\x3D", "xxxxxxxx");
	void(*EbisuSDK_CVar_Init) = (void(*))p_EbisuSDK_CVar_Init.GetPtr(); /*40 57 48 83 EC 40 83 3D*/
#else
	ADDRESS p_OriginGetErrorDescription = g_mGameDll.FindPatternSIMD((std::uint8_t*)"\x40\x53\x48\x83\xEC\x20\x8B\xD9\x48\x8D\x15\x00\x00\x00\x00", "xxxxxxxxxxx????").GetPtr();
	void(*OriginGetErrorDescription) = (void(*))p_OriginGetErrorDescription.GetPtr(); /*40 53 48 83 EC 20 8B D9 48 8D 15 ?? ?? ?? ??*/
#endif
}

namespace
{
#ifdef DEDICATED
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
#endif // GAMEDLL_*
#endif // DEDICATED
}
///////////////////////////////////////////////////////////////////////////////
void HEbisuSDK_Init();

void EbisuSDK_Attach();
void EbisuSDK_Detach();


///////////////////////////////////////////////////////////////////////////////
class HEbisuSDK : public IDetour
{
	virtual void debugp()
	{
#ifdef DEDICATED
		std::cout << "| FUN: EbisuSDK_Init_Tier0                  : 0x" << std::hex << std::uppercase << p_EbisuSDK_Init_Tier0.GetPtr()       << std::setw(npad) << " |" << std::endl;
		std::cout << "| FUN: EbisuSDK_CVar_Init                   : 0x" << std::hex << std::uppercase << p_EbisuSDK_CVar_Init.GetPtr()        << std::setw(npad) << " |" << std::endl;
		std::cout << "| VAR: g_bEbisuSDKInitialized               : 0x" << std::hex << std::uppercase << g_bEbisuSDKInitialized.GetPtr()      << std::setw(npad) << " |" << std::endl;
		std::cout << "| VAR: g_bEbisuSDKCvarInitialized           : 0x" << std::hex << std::uppercase << g_bEbisuSDKCvarInitialized.GetPtr()  << std::setw(npad) << " |" << std::endl;
		std::cout << "| VAR: g_qEbisuSDKCvarInitialized           : 0x" << std::hex << std::uppercase << g_qEbisuSDKCvarInitialized.GetPtr()  << std::setw(npad) << " |" << std::endl;
#else
		std::cout << "| FUN: OriginGetErrorDescription            : 0x" << std::hex << std::uppercase << p_OriginGetErrorDescription.GetPtr() << std::setw(npad) << " |" << std::endl;
#endif // DEDICATED
		std::cout << "+----------------------------------------------------------------+" << std::endl;
	}
};
///////////////////////////////////////////////////////////////////////////////

REGISTER(HEbisuSDK);
