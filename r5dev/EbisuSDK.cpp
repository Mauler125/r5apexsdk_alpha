#include "stdafx.h"
#include "EbisuSDK.h"

//-----------------------------------------------------------------------------
// Purpose: sets the player name in the OriginSDK struct from the cfg file
// TODO   : recreate and define the struct instead
//-----------------------------------------------------------------------------
void HEbisuSDK_Init()
{
	DWORD64 splayerName = 0x1634F35D8;

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
	memcpy((char*)splayerName, sNameBuf, sizeof(sNameBuf));
}

void AttachEbisuSDKHooks()
{
	//
}

void DetachEbisuSDKHooks()
{
	//
}