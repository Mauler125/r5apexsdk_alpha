#pragma once
#include "stdafx.h"

namespace
{
	/* ==== CBASECLIENT ===================================================================================================================================================== */
	DWORD64 p_CBaseClient_Clear = FindPatternV2("r5apex.exe", (const unsigned char*)"\x40\x53\x41\x56\x41\x57\x48\x83\xEC\x20\x48\x8B\xD9\x48\x89\x74", "xxxxxxxxxxxxxxxx");
	std::int64_t* (*CBaseClient_Clear)(std::int64_t client) = (std::int64_t * (*)(std::int64_t))p_CBaseClient_Clear; /*40 53 41 56 41 57 48 83 EC 20 48 8B D9 48 89 74*/
}

///////////////////////////////////////////////////////////////////////////////
std::int64_t* HCBaseClient_Clear(std::int64_t client);

void AttachCBaseClientHooks();
void DetachCBaseClientHooks();
