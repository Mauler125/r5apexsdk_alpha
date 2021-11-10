#include "stdafx.h"
#include "IAppSystem.h"
#include "EbisuSDK.h"

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
__int64 HIAppSystem_Main(__int64 a1, __int64 a2)
{
	HEbisuSDK_Init();
	return IAppSystem_Main(a1, a2);
}
auto g_bIsDedicated = (uint8_t*)0x162C61208;
//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
__int64 HIAppSystem_Create(__int64 a1)
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
