#include "stdafx.h"
#include "IAppSystem.h"

__int64 HIAppSystem_Main(__int64 a1, __int64 a2)
{
	HEbisuSDK_Init();
	return IAppSystem_Main(a1, a2);
}

void AttachIAppSystemHooks()
{
	DetourAttach((LPVOID*)&IAppSystem_Main, &HIAppSystem_Main);
}

void DetachIAppSystemHooks()
{
	DetourDetach((LPVOID*)&IAppSystem_Main, &HIAppSystem_Main);
}
