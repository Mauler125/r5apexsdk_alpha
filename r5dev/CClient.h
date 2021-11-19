#pragma once
#include "stdafx.h"
#include "basetypes.h"
#include "IVEngineServer.h"

namespace
{
#if defined (GAMEDLL_S0) || defined (GAMEDLL_S1)
	const std::uintptr_t g_dwCClientSize    = 0x4A440;
	const std::uintptr_t g_dwPersistenceVar = 0x5B4;
	const std::uintptr_t g_dwCClientPadding = 303232;
#elif defined (GAMEDLL_S2) || defined (GAMEDLL_S3)
	const std::uintptr_t g_dwCClientSize    = 0x4A4C0;
	const std::uintptr_t g_dwPersistenceVar = 0x5BC;
	const std::uintptr_t g_dwCClientPadding = 303360;
#endif
	static ADDRESS g_pClientStart = p_IVEngineServer_PersistenceAvailable.FindPatternSelf("48 8D 0D", ADDRESS::Direction::DOWN, 150).ResolveRelativeAddressSelf(0x3, 0x7);
}

class CClient
{
public:
	inline CClient* GetClientInstance(int index)
	{
		return (CClient*)(std::uintptr_t)(g_pClientStart.GetPtr() + (index * g_dwCClientSize));
	}

	void*& GetNetChan()
	{
		return m_nNetChannel;
	}
private:
	char pad_0000[16]; //0x0000
public:
	int m_iUserID; //0x0010
private:
	char pad_0014[908]; //0x0014
public:
	void* m_nNetChannel; //0x03A0
private:
	char pad_03A8[8]; //0x03A8
public:
	int m_iSignonstate; //0x03B0
private:
	char pad_03B4[4]; //0x03B4
public:
	std::int64_t m_iOriginID; //0x03B8
private:
	char pad_03C0[g_dwCClientPadding]; //0x03C0
};

///////////////////////////////////////////////////////////////////////////////
extern CClient* g_pClient;
