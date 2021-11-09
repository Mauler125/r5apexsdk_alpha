#pragma once
#include "stdafx.h"

namespace
{
	/* ==== CVENGINESERVER ================================================================================================================================================== */
	DWORD64 p_IVEngineServer_PersistenceAvailable = FindPatternV2("r5apex.exe", (const unsigned char*)"\x3B\x15\x00\x00\x00\x00\x7D\x33", "xx????xx");
	bool (*IVEngineServer_PersistenceAvailable)(__int64 entidx, int clientidx) = (bool (*)(__int64, int))p_IVEngineServer_PersistenceAvailable; /*3B 15 ?? ?? ?? ?? 7D 33*/
}

///////////////////////////////////////////////////////////////////////////////
bool HIVEngineServer_PersistenceAvailable(__int64 thisptr, int client);

void AttachIVEngineServerHooks();
void DetachIVEngineServerHooks();

///////////////////////////////////////////////////////////////////////////////
extern bool g_bIsPersistenceVarSet[128];
