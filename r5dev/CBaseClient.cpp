#include "stdafx.h"
#include "CBaseClient.h"

//---------------------------------------------------------------------------------
// Purpose: throw away any residual garbage in the channel
//---------------------------------------------------------------------------------
__int64* HCBaseClient_Clear(__int64 client)
{
	return CBaseClient_Clear(client);
}

///////////////////////////////////////////////////////////////////////////////
void AttachCBaseClientHooks()
{
	DetourAttach((LPVOID*)&CBaseClient_Clear, &HCBaseClient_Clear);
}
void DetachCBaseClientHooks()
{
	DetourDetach((LPVOID*)&CBaseClient_Clear, &HCBaseClient_Clear);
}
