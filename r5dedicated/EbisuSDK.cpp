#include "stdafx.h"
#include "EbisuSDK.h"

//-----------------------------------------------------------------------------
// Purpose: sets the player name in the OriginSDK struct from the cfg file
// TODO   : recreate and define the struct instead
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
