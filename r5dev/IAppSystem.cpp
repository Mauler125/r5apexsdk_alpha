#include "stdafx.h"
#include "IAppSystem.h"
#include "EbisuSDK.h"
#include "classes.h"

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
std::int64_t HIAppSystem_Main(std::int64_t a1, std::int64_t a2)
{
	HEbisuSDK_Init();
	return IAppSystem_Main(a1, a2);
}

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
char HIAppSystem_Create(std::int64_t a1)
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
