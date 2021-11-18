#pragma once
#include "stdafx.h"

namespace
{
	/* ==== CVENGINESERVER ================================================================================================================================================== */
	ADDRESS p_IVEngineServer_PersistenceAvailable = g_mGameDll.FindPatternSIMD((std::uint8_t*)"\x3B\x15\x00\x00\x00\x00\x7D\x33", "xx????xx");
	bool (*IVEngineServer_PersistenceAvailable)(std::int64_t entidx, int clientidx) = (bool (*)(std::int64_t, int))p_IVEngineServer_PersistenceAvailable.GetPtr(); /*3B 15 ?? ?? ?? ?? 7D 33*/
}

///////////////////////////////////////////////////////////////////////////////
bool HIVEngineServer_PersistenceAvailable(std::int64_t thisptr, int client);

void IVEngineServer_Attach();
void IVEngineServer_Detach();

///////////////////////////////////////////////////////////////////////////////
extern bool g_bIsPersistenceVarSet[128];
