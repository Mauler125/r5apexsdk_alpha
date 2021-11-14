#include "stdafx.h"
#include "CBaseClient.h"

//---------------------------------------------------------------------------------
// Purpose: throw away any residual garbage in the channel.
//---------------------------------------------------------------------------------
std::int64_t* HCBaseClient_Clear(std::int64_t client)
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
