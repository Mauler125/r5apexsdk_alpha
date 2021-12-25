#include "core/stdafx.h"
#include "launcher/IApplication.h"
#include "ebisusdk/EbisuSDK.h"

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void* HIApplication_Main(void* a1, void* a2)
{
	HEbisuSDK_Init();
	return IAppSystem_Main(a1, a2);
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
bool HIApplication_Create(void* a1)
{
	return IAppSystem_Create(a1);
}

void IApplication_Attach()
{
	DetourAttach((LPVOID*)&IAppSystem_Main, &HIApplication_Main);
	DetourAttach((LPVOID*)&IAppSystem_Create, &HIApplication_Create);
}

void IApplication_Detach()
{
	DetourDetach((LPVOID*)&IAppSystem_Main, &HIApplication_Main);
	DetourDetach((LPVOID*)&IAppSystem_Create, &HIApplication_Create);
}
