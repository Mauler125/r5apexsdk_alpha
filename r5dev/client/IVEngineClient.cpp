#include "core/stdafx.h"
#include "tier0/basetypes.h"
#include "client/IVEngineClient.h"

//#ifdef GAMEDLL_S3
bool* m_bRestrictServerCommands = reinterpret_cast<bool*>(g_mGameDll.StringSearch("DevShotGenerator_Init()").FindPatternSelf("88 05", ADDRESS::Direction::UP).ResolveRelativeAddressSelf(0x2).OffsetSelf(0x2).GetPtr());
//#endif
