#include "core/stdafx.h"
#include "ebisusdk/EbisuSDK.h"
#include "client/client.h"
#include "engine/sys_utils.h"

//-----------------------------------------------------------------------------
// Purpose: sets the EbisuSDK globals for dedicated
// TODO   : recreate the struct instead
//-----------------------------------------------------------------------------
void HEbisuSDK_Init()
{
	*(char*)g_bEbisuSDKInitialized.GetPtr()     = (char)0x1; // <- 1st EbisuSDK check
	*(char*)g_bEbisuSDKCvarInitialized.GetPtr() = (char)0x1; // <- 2nd EbisuSDK check
	*(char*)g_qEbisuSDKCvarInitialized.GetPtr() = (char)0x1; // <- 3rd EbisuSDK check
}

void EbisuSDK_Attach()
{
	//
}

void EbisuSDK_Detach()
{
	//
}
