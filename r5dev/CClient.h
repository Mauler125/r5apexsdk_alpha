#pragma once
#include "stdafx.h"

class CClient
{
public:
	inline CClient* GetClientInstance(int index)
	{
		return (CClient*)(std::uintptr_t)(0x16073B200 + (index * (std::uintptr_t)0x4A4C0));
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
	char pad_03C0[303360]; //0x03C0
};

///////////////////////////////////////////////////////////////////////////////
extern CClient* g_pClient;
