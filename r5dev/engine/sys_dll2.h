#pragma once
#include "tier0/basetypes.h"
// TODO [ AMOS ]: implement 'tier0.h'
// TODO [ AMOS ]: implement 'tier1.h'

typedef void* (*CreateInterfaceFn)(const char* pName, int* pReturnCode); // TODO [ AMOS ]: implement 'interface.h'
class CEngineAPI
{
	// TODO [ AMOS ]:
};

namespace
{
	ADDRESS p_CEngineAPI_Connect = g_mGameDll.FindPatternSIMD((std::uint8_t*)"\x48\x83\xEC\x28\x48\x8B\x05\x00\x00\x00\x00\x48\x8D\x0D\x00\x00\x00\x00\x48\x85\xC0\x48\x89\x15", "xxxxxxx????xxx????xxxxxx");
	bool (*CEngineAPI_Connect)(CEngineAPI* thisptr, CreateInterfaceFn factory) = (bool (*)(CEngineAPI*, CreateInterfaceFn))p_CEngineAPI_Connect.GetPtr(); /*48 83 EC 28 48 8B 05 ? ? ? ? 48 8D 0D ? ? ? ? 48 85 C0 48 89 15 ? ? ? ?*/
}
