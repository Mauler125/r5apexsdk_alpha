#include "stdafx.h"
#include "EbisuSDK.h"

//-----------------------------------------------------------------------------
// Purpose: sets the player name in the OriginSDK struct from the cfg file
// TODO   : recreate and define the struct instead
//-----------------------------------------------------------------------------
void HEbisuSDK_Init()
{
	DWORD64 ofs000 = 0x1634F1690;
	DWORD64 ofs001 = 0x1634F16B0;
	DWORD64 ofs002 = 0x1634F1695;
	DWORD64 ofs003 = 0x1634F30D8;
	DWORD64 ofs004 = 0x1634F31D8;

	*(char*)(ofs000) = (char)0x1; // <- First Origin boolean check
	*(char*)(ofs001) = (char)0x1; // <- Second Origin boolean check
	*(char*)(ofs002) = (char)0x1; // <- Third Origin bolean check
	*(char*)(ofs003) = (char)0x1; // <- Gets tested on listenserver for certain concommands
	*(char*)(ofs004) = (char)0x0; // <- TODO: Enforces Necleus cvar's when not equal to 0
}

void AttachEbisuSDKHooks()
{
	//
}

void DetachEbisuSDKHooks()
{
	//
}