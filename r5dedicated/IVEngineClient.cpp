#include "stdafx.h"
#include "basetypes.h"
#include "IVEngineClient.h"

//#ifdef GAMEDLL_S3
ADDRESS m_bRestrictServerCommands = g_mGameDll.StringSearch("DevShotGenerator_Init()").FindPatternSelf("88 05", ADDRESS::Direction::UP).ResolveRelativeAddressSelf(0x2).OffsetSelf(0x2).GetPtr();
//#endif
