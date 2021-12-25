#pragma once
#include "tier0/basetypes.h"

namespace
{
	/* ==== CCLIENTSTATE ==================================================================================================================================================== */
#if defined (GAMEDLL_S0) || defined (GAMEDLL_S1)
	//ADDRESS p_CClientState__CheckForResend = g_mGameDll.FindPatternSIMD((std::uint8_t*)"\x48\x89\x5C\x24\x00\x56\x57\x41\x57\x00\x81\xEC\x20\x04\x00\x00\x45\x0F\xB6\xF9\x00\x00\x00\x00\x8B\xF1\x48", "xxxx?xxxx?xxxx?xxxxx????xxx"); /*48 89 5C 24 ?? 56 57 41 57 ?? 81 EC 20 04 ?? 00 45 0F B6 F9 ?? ?? ?? ?? 8B F1 48*/
	//void (*CClientState__CheckForResend)(std::int64_t a1, const char* a2, std::int64_t a3, char a4, int a5, std::uint8_t* a6) = (void(*)(std::int64_t, const char*, std::int64_t, char, int, std::uint8_t*))p_CClientState__CheckForResend.GetPtr();
#elif defined (GAMEDLL_S2)
	//ADDRESS p_CClientState__CheckForResend = g_mGameDll.FindPatternSIMD((std::uint8_t*)"\x48\x89\x5C\x24\x00\x48\x89\x74\x24\x00\x48\x89\x7C\x24\x00\x41\x56\x48\x81\xEC\x00\x00\x00\x00\x45\x0F\xB6", "xxxx?xxxx?xxxx?xxxxx????xxx"); /*48 89 5C 24 ?? 48 89 74 24 ?? 48 89 7C 24 ?? 41 56 48 81 EC ?? ?? ?? ?? 45 0F B6*/
	//void (*CClientState__CheckForResend)(std::int64_t a1, const char* a2, std::int64_t a3, char a4, int a5, std::uint8_t* a6) = (void(*)(std::int64_t, const char*, std::int64_t, char, int, std::uint8_t*))p_CClientState__CheckForResend.GetPtr();
#elif defined (GAMEDLL_S3)
	//ADDRESS p_CClientState__CheckForResend = g_mGameDll.FindPatternSIMD((std::uint8_t*)"\x48\x89\x5C\x24\x00\x48\x89\x74\x24\x00\x48\x89\x7C\x24\x00\x41\x56\x48\x81\xEC\x00\x00\x00\x00\x48\x8B\x32", "xxxx?xxxx?xxxx?xxxxx????xxx"); /*48 89 5C 24 ?? 48 89 74 24 ?? 48 89 7C 24 ?? 41 56 48 81 EC ?? ?? ?? ?? 48 8B 32*/
	//void (*CClientState__CheckForResend)(std::int64_t a1, const char* a2, std::int64_t a3, char a4, int a5, std::uint8_t* a6) = (void(*)(std::int64_t, const char*, std::int64_t, char, int, std::uint8_t*))p_CClientState__CheckForResend.GetPtr();
#endif
}


///////////////////////////////////////////////////////////////////////////////
class HClientState : public IDetour
{
	virtual void debugp()
	{
		//std::cout << "| FUN: CClientState::CheckForResend         : 0x" << std::hex << std::uppercase << p_CClientState__CheckForResend.GetPtr() << std::setw(npad) << " |" << std::endl;
		//std::cout << "+----------------------------------------------------------------+" << std::endl;
	}
};
///////////////////////////////////////////////////////////////////////////////

REGISTER(HClientState);
