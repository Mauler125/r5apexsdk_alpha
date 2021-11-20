#pragma once
#include "stdafx.h"

#define MAKE_3_BYTES_FROM_1_AND_2( x1, x2 ) (( (( std::uint16_t )x2) << 8 ) | (std::uint8_t)(x1))
extern std::vector<std::string> g_szAllPlaylists;
typedef int HKeySymbol;

enum KeyValuesTypes
{
	TYPE_NONE              = 0x0,
	TYPE_STRING            = 0x1,
	TYPE_INT               = 0x2,
	TYPE_FLOAT             = 0x3,
	TYPE_PTR               = 0x4,
	TYPE_WSTRING           = 0x5,
	TYPE_COLOR             = 0x6,
	TYPE_UINT64            = 0x7,
	TYPE_COMPILED_INT_BYTE = 0x8,
	TYPE_COMPILED_INT_0    = 0x9,
	TYPE_COMPILED_INT_1    = 0xA,
	TYPE_NUMTYPES          = 0xB,
};

namespace
{
	/* ==== KEYVALUES ======================================================================================================================================================= */
	ADDRESS p_KeyValues_FindKey = g_mGameDll.FindPatternSIMD((std::uint8_t*)"\x40\x56\x57\x41\x57\x48\x81\xEC\x00\x00\x00\x00\x45", "xxxxxxxx????x");
	void* (*KeyValues_FindKey)(void* a1, const char* a2, bool a3) = (void* (*)(void*, const char*, bool))p_KeyValues_FindKey.GetPtr(); /*40 56 57 41 57 48 81 EC 30 01 00 00 45 0F B6 F8*/
}

class CKeyValuesSystem // VTABLE @ 0x1413AA1E8 in R5pc_r5launch_N1094_CL456479_2019_10_30_05_20_PM
{
public:

	void RegisterSizeofKeyValues(std::int64_t size) //@0x1413AA1F0 in R5pc_r5launch_N1094_CL456479_2019_10_30_05_20_PM
	{
		using OriginalFn = void(__thiscall*)(CKeyValuesSystem*, std::int64_t);
		(*reinterpret_cast<OriginalFn**>(this))[0](this, size);
	}

	void* AllocKeyValuesMemory(std::int64_t size) // @0x1413AA1F8 in R5pc_r5launch_N1094_CL456479_2019_10_30_05_20_PM
	{
		using OriginalFn = void* (__thiscall*)(CKeyValuesSystem*, std::int64_t);
		return (*reinterpret_cast<OriginalFn**>(this))[1](this, size);
	}

	void FreeKeyValuesMemory(void* pMem) // @0x1413AA200 in R5pc_r5launch_N1094_CL456479_2019_10_30_05_20_PM
	{
		using OriginalFn = void(__thiscall*)(CKeyValuesSystem*, void*);
		(*reinterpret_cast<OriginalFn**>(this))[2](this, pMem);
	}

	HKeySymbol GetSymbolForString(const char* name, bool bCreate) // @0x1413AA208 in R5pc_r5launch_N1094_CL456479_2019_10_30_05_20_PM
	{
		using OriginalFn = HKeySymbol(__thiscall*)(CKeyValuesSystem*, const char*, bool);
		return (*reinterpret_cast<OriginalFn**>(this))[3](this, name, bCreate);
	}

	const char* GetStringForSymbol(HKeySymbol symbol) // @0x1413AA210 in R5pc_r5launch_N1094_CL456479_2019_10_30_05_20_PM
	{
		using OriginalFn = const char* (__thiscall*)(CKeyValuesSystem*, HKeySymbol);
		return (*reinterpret_cast<OriginalFn**>(this))[4](this, symbol);
	}

	//	void __fastcall CKeyValuesSystem::FreeKeyValuesMemory(CKeyValuesSystem* this_arg, void* ptr_mem_arg)
	//	{
	//		__int64* v2; // rax
	//		__int64 v4; // rax
	//		__int64* v5; // rax
	// 
	//		v2 = qword_14D40B538;
	//		if (!qword_14D40B538)
	//		{
	//			v2 = sub_140462930();
	//			qword_14D40B538 = v2;
	//		}
	//		v4 = (*(*v2 + 48))(v2, ptr_mem_arg);
	//		if (v4 > 0)
	//			CKeyValuesSystem::m_pMemPool -= v4;
	//		v5 = qword_14D40B538;
	//		if (!qword_14D40B538)
	//		{
	//			v5 = sub_140462930();
	//			qword_14D40B538 = v5;
	//		}
	//		(*(*v5 + 40))(v5, ptr_mem_arg);
	//	}

	// GetMemPool return a global variable called m_pMemPool it gets modified by AllocKeyValuesMemory and FreeKeyValuesMemory above you can see where the find it in FreeKeyValuesMemory.
	void* GetMemPool() // @0x1413AA228 in R5pc_r5launch_N1094_CL456479_2019_10_30_05_20_PM
	{
		return reinterpret_cast<void*>(0x14D412768); // May need to dereference this once more not sure right now.
	}

	void SetKeyValuesExpressionSymbol(const char* name, bool bValue) // @0x1413AA230 in R5pc_r5launch_N1094_CL456479_2019_10_30_05_20_PM
	{
		using OriginalFn = void(__thiscall*)(CKeyValuesSystem*, const char*, bool);
		(*reinterpret_cast<OriginalFn**>(this))[8](this, name, bValue);
	}

	bool GetKeyValuesExpressionSymbol(const char* name) // @0x1413AA238 in R5pc_r5launch_N1094_CL456479_2019_10_30_05_20_PM
	{
		using OriginalFn = bool(__thiscall*)(CKeyValuesSystem*, const char*);
		return (*reinterpret_cast<OriginalFn**>(this))[9](this, name);
	}

	HKeySymbol GetSymbolForStringCaseSensitive(HKeySymbol& hCaseInsensitiveSymbol, const char* name, bool bCreate) // @0x1413AA240 in R5pc_r5launch_N1094_CL456479_2019_10_30_05_20_PM
	{
		using OriginalFn = HKeySymbol(__thiscall*)(CKeyValuesSystem*, HKeySymbol&, const char*, bool);
		return (*reinterpret_cast<OriginalFn**>(this))[10](this, hCaseInsensitiveSymbol, name, bCreate);
	}

// Datatypes aren't accurate. But full fill the actual byte distance.
public:
	void*        vtable;                     // 0x0000
	std::int64_t m_iMaxKeyValuesSize;        // 0x0008
private:
	char         gap10[240];                 // 0x0010
public:
	int          m_KvConditionalSymbolTable; // 0x0100
private:
	char         gap104[4];                  // 0x0104
public:
	std::int64_t field_108;                  // 0x0108
private:
	char         gap110[32];                 // 0x0110
public:
	int          m_mutex;                    // 0x0130
};

class KeyValues
{
public:

	KeyValues* FindKey(const char* keyName, bool bCreate)
	{
		static auto func = reinterpret_cast<KeyValues * (__thiscall*)(KeyValues*, const char*, bool)>(KeyValues_FindKey);
		return func(this, keyName, bCreate);
	}

	const char* GetName();

	int GetInt(const char* keyName, int defaultValue)
	{
		KeyValues* dat = FindKey(keyName, false);

		if (!dat)
			return defaultValue;

		switch (dat->m_iDataType)
		{
			case TYPE_STRING:
				return atoi(dat->m_sValue);
			case TYPE_FLOAT:
				return static_cast<int>(m_flValue());
			case TYPE_WSTRING:
				return _wtoi(dat->m_wsValue);
			case TYPE_UINT64:
				return 0;
		default:
			return dat->m_iValue();
		}

		return defaultValue;
	}

	void SetInt(const char* keyName, int iValue)
	{
		KeyValues* dat = FindKey(keyName, true);
		if (dat)
		{
			dat->m_iValue() = iValue;
			dat->m_iDataType = TYPE_INT;
		}
	}

	void SetFloat(const char* keyName, float flValue)
	{
		KeyValues* dat = FindKey(keyName, true);
		if (dat)
		{
			dat->m_flValue() = flValue;
			dat->m_iDataType = TYPE_FLOAT;
		}
	}

	// Compiler makes it so m_Value shares the offset spot with m_flValue that why we cast it like this.
	float& m_flValue()
	{
		static std::int32_t offset = 0x18;
		return *(float*)((std::uintptr_t)this + offset);
	}

	int& m_iValue()
	{
		static std::int32_t offset = 0x18;
		return *(int*)((std::uintptr_t)this + offset);
	}

public:
	std::uint32_t    m_iKeyName              : 24; // 0x0000
	std::uint32_t    m_iKeyNameCaseSensitive : 8;  // 0x0003
	char*            m_sValue;                     // 0x0008
	wchar_t*         m_wsValue;                    // 0x0010
	int              m_Value;                      // 0x0018
private:
	char             gap1C[12];                    // 0x0020
public:
	char             m_iDataType;                  // 0x0028
	std::uint16_t    m_iKeyNameCaseSensitive2;     // 0x002A
	KeyValues*       m_pPeer;                      // 0x0030
	KeyValues*       m_pSub;                       // 0x0038
	KeyValues*       m_pChain;                     // 0x0040
};

///////////////////////////////////////////////////////////////////////////////
extern CKeyValuesSystem* g_pKeyValuesSystem;
extern KeyValues** g_pPlaylistKeyValues;
