#pragma once
#include "stdafx.h"

namespace
{
	/* ==== CBASECLIENT ===================================================================================================================================================== */
	DWORD64 p_CBaseClient_Clear = FindPatternV2("r5apex.exe", (const unsigned char*)"\x40\x53\x41\x56\x41\x57\x48\x83\xEC\x20\x48\x8B\xD9\x48\x89\x74", "xxxxxxxxxxxxxxxx");
	__int64* (*CBaseClient_Clear)(__int64 client) = (__int64* (*)(__int64))p_CBaseClient_Clear; /*40 53 41 56 41 57 48 83 EC 20 48 8B D9 48 89 74*/
}

///////////////////////////////////////////////////////////////////////////////
__int64* HCBaseClient_Clear(__int64 client);

void AttachCBaseClientHooks();
void DetachCBaseClientHooks();
