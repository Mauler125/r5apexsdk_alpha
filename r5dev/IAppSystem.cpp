#include "stdafx.h"
#include "IAppSystem.h"
#include "EbisuSDK.h"
#include "classes.h"

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
__int64 HIAppSystem_Main(__int64 a1, __int64 a2)
{
	HEbisuSDK_Init();
	return IAppSystem_Main(a1, a2);
}

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
__int64 HIAppSystem_Create(__int64 a1)
{
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
