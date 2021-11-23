#include "core/stdafx.h"
#include "ebisusdk/EbisuSDK.h"
#include "client/client.h"
#include "engine/sys_utils.h"

//-----------------------------------------------------------------------------
// Purpose: sets the player name in the OriginSDK struct from the cfg file
// TODO   : recreate and define the struct instead
//-----------------------------------------------------------------------------
void HEbisuSDK_Init()
{
	FILE* sNameCfg;
	CHAR sNameBuf[64] = { 0 };
	fopen_s(&sNameCfg, "platform\\cfg\\name.cfg", "r");

	if (sNameCfg)
	{
		while (fgets(sNameBuf, sizeof(sNameBuf), sNameCfg) != NULL)
		{
			fclose(sNameCfg);
		}
	}

	*(char*)g_bEbisuSDKInitialized.GetPtr()     = (char)0x1; // <- 1st EbisuSDK check
	*(char*)g_bEbisuSDKCvarInitialized.GetPtr() = (char)0x1; // <- 2nd EbisuSDK check
	*(char*)g_qEbisuSDKCvarInitialized.GetPtr() = (char)0x1; // <- 3rd EbisuSDK check
	memcpy((char*)g_sPlayerName.GetPtr(), sNameBuf, sizeof(sNameBuf));
}

void EbisuSDK_Attach()
{
	//
}

void EbisuSDK_Detach()
{
	//
}
