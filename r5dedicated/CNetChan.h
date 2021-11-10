#pragma once
#include "stdafx.h"
#include "basetypes.h"
#include "hooks.h"
#include "protocol.h"
#include "CClient.h"

typedef unsigned __int64 QWORD;
struct __declspec(align(8)) netpacket_t
{
	DWORD family_maybe;
	sockaddr_in sin;
	WORD sin_port;
	BYTE gap16;
	BYTE byte17;
	DWORD source;
	double received;
	unsigned __int8* data;
	QWORD label;
	BYTE byte38;
	QWORD qword40;
	QWORD qword48;
	BYTE gap50[8];
	QWORD qword58;
	QWORD qword60;
	QWORD qword68;
	int less_than_12;
	DWORD wiresize;
	BYTE gap78[8];
	QWORD qword80;
};

namespace
{
	/* ==== CNETCHAN ======================================================================================================================================================== */
#if defined (GAMEDLL_S0) || defined (GAMEDLL_S1) || defined (GAMEDLL_S2)
	DWORD64 p_NET_Init = FindPatternV2("r5apex.exe", (const unsigned char*)"\x48\x89\x5C\x24\x08\x48\x89\x6C\x24\x10\x48\x89\x74\x24\x18\x48\x89\x7C\x24\x20\x41\x54\x41\x56\x41\x57\x48\x81\xEC\xC0\x01\x00", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
	__int64 (*NET_Init)(char a1) = (__int64 (*)(char))p_NET_Init; /*48 89 5C 24 08 48 89 6C 24 10 48 89 74 24 18 48 89 7C 24 20 41 54 41 56 41 57 48 81 EC C0 01 00*/
#elif defined (GAMEDLL_S3)
	DWORD64 p_NET_Init = FindPatternV2("r5apex.exe", (const unsigned char*)"\x48\x89\x5C\x24\x08\x48\x89\x6C\x24\x10\x48\x89\x74\x24\x18\x48\x89\x7C\x24\x20\x41\x54\x41\x56\x41\x57\x48\x81\xEC\xF0\x01\x00", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
	__int64 (*NET_Init)(char a1) = (__int64 (*)(char))p_NET_Init; /*48 89 5C 24 08 48 89 6C 24 10 48 89 74 24 18 48 89 7C 24 20 41 54 41 56 41 57 48 81 EC F0 01 00*/
#endif
	DWORD64 p_NET_Shutdown = FindPatternV2("r5apex.exe", (const unsigned char*)"\x48\x89\x6C\x24\x18\x56\x57\x41\x56\x48\x83\xEC\x30\x83\xB9\xD0", "xxxxxxxxxxxxxxxx");
	void (*NET_Shutdown)(void* thisptr, const char* a0, unsigned __int8 a1, char a2) = (void (*)(void*, const char*, unsigned __int8, char))p_NET_Shutdown; /*48 89 6C 24 18 56 57 41 56 48 83 EC 30 83 B9 D0*/

	DWORD64 p_NET_SetKey = FindPatternV2("r5apex.exe", (const unsigned char*)"\x48\x89\x5C\x24\x08\x48\x89\x6C\x24\x10\x48\x89\x74\x24\x18\x57\x48\x83\xEC\x20\x48\x8B\xF9\x41\xB8", "xxxxxxxxxxxxxxxxxxxxxxxxx");
	void (*NET_SetKey)(__int64 a1, const char* a2) = (void (*)(__int64, const char*))p_NET_SetKey; /*48 89 5C 24 08 48 89 6C 24 10 48 89 74 24 18 57 48 83 EC 20 48 8B F9 41 B8*/

	DWORD64 p_NET_ReceiveDatagram = FindPatternV2("r5apex.exe", (const unsigned char*)"\x48\x89\x74\x24\x18\x48\x89\x7C\x24\x20\x55\x41\x54\x41\x55\x41\x56\x41\x57\x48\x8D\xAC\x24\x50\xEB", "xxxxxxxxxxxxxxxxxxxxxxxxx");
	bool (*NET_ReceiveDatagram)(int, void*, bool) = (bool (*)(int, void*, bool))p_NET_ReceiveDatagram; /*E8 ?? ?? ?? ?? 84 C0 75 35 48 8B D3*/

	DWORD64 p_NET_SendDatagram = FindPatternV2("r5apex.exe", (const unsigned char*)"\x48\x89\x5C\x24\x08\x48\x89\x6C\x24\x10\x48\x89\x74\x24\x18\x57\x41\x56\x41\x57\x48\x81\xEC\x00\x05\x00\x00", "xxxxxxxxxxxxxxxxxxxxxxx?xxx");
	int (*NET_SendDatagram)(SOCKET s, const char* buf, int len, int flags) = (int (*)(SOCKET, const char*, int, int))p_NET_SendDatagram; /*48 89 5C 24 08 48 89 6C 24 10 48 89 74 24 18 57 41 56 41 57 48 81 EC ?? 05 00 00*/
}

///////////////////////////////////////////////////////////////////////////////
bool HNET_ReceiveDatagram(int sock, void* inpacket, bool raw);
unsigned int HNET_SendDatagram(SOCKET s, const char* buf, int len, int flags);
void HNET_SetKey(std::string key);
void HNET_GenerateKey();
void NET_DisconnectClient(CClient* client, int index, const char* reason, unsigned __int8 unk1, char unk2);

void AttachCNetChanHooks();
void DetachCNetChanHooks();

///////////////////////////////////////////////////////////////////////////////
extern bool g_bTraceNetChannel;
extern std::string g_szNetKey;
