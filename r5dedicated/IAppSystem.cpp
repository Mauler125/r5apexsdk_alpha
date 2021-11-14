#include "stdafx.h"
#include "IAppSystem.h"
#include "EbisuSDK.h"

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
std::int64_t HIAppSystem_Main(std::int64_t a1, std::int64_t a2)
{
	HEbisuSDK_Init();
	return IAppSystem_Main(a1, a2);
}
auto g_bIsDedicated = (uint8_t*)0x162C61208;
//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
std::int64_t HIAppSystem_Create(std::int64_t a1)
{
	*g_bIsDedicated = 1; // HAS TO BE HERE!!!
	HEbisuSDK_Init();
	return IAppSystem_Create(a1);
}

void AttachIAppSystemHooks()
{
	DetourAttach((LPVOID*)&IAppSystem_Main, &HIAppSystem_Main);
	DetourAttach((LPVOID*)&IAppSystem_Create, &HIAppSystem_Create);
}

void DetachIAppSystemHooks()
{
	DetourDetach((LPVOID*)&IAppSystem_Main, &HIAppSystem_Main);
	DetourDetach((LPVOID*)&IAppSystem_Create, &HIAppSystem_Create);
}
