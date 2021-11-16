#include "stdafx.h"
#include "IApplication.h"
#include "EbisuSDK.h"
#include "classes.h"

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
std::int64_t HIApplication_Main(std::int64_t a1, std::int64_t a2)
{
	HEbisuSDK_Init();
	return IAppSystem_Main(a1, a2);
}

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
char HIApplication_Create(std::int64_t a1)
{
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
