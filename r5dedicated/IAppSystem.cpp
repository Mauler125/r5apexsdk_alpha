#include "stdafx.h"
#include "IAppSystem.h"
#include "EbisuSDK.h"

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
std::int64_t HIApplication_Main(std::int64_t a1, std::int64_t a2)
{
	HEbisuSDK_Init();
	return IAppSystem_Main(a1, a2);
}
auto g_bIsDedicated = (uint8_t*)0x162C61208;
//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
char HIApplication_Create(std::int64_t a1)
{
	*g_bIsDedicated = 1; // HAS TO BE HERE!!!
	HEbisuSDK_Init();
	return IAppSystem_Create(a1);
}

void AttachIApplicationHooks()
{
	DetourAttach((LPVOID*)&IAppSystem_Main, &HIApplication_Main);
	DetourAttach((LPVOID*)&IAppSystem_Create, &HIApplication_Create);
}

void DetachIApplicationHooks()
{
	DetourDetach((LPVOID*)&IAppSystem_Main, &HIApplication_Main);
	DetourDetach((LPVOID*)&IAppSystem_Create, &HIApplication_Create);
}
