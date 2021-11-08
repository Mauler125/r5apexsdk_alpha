#pragma once
#include "stdafx.h"
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
	DWORD64 p_NET_ReceiveDatagram = FindPatternV2("r5apex.exe", (const unsigned char*)"\x48\x89\x74\x24\x18\x48\x89\x7C\x24\x20\x55\x41\x54\x41\x55\x41\x56\x41\x57\x48\x8D\xAC\x24\x50\xEB", "xxxxxxxxxxxxxxxxxxxxxxxxx");
	bool (*NET_ReceiveDatagram)(int, void*, bool) = (bool (*)(int, void*, bool))p_NET_ReceiveDatagram; /*E8 ?? ?? ?? ?? 84 C0 75 35 48 8B D3*/

	DWORD64 p_NET_SendDatagram = FindPatternV2("r5apex.exe", (const unsigned char*)"\x48\x89\x5C\x24\x08\x48\x89\x6C\x24\x10\x48\x89\x74\x24\x18\x57\x41\x56\x41\x57\x48\x81\xEC\x00\x05\x00\x00", "xxxxxxxxxxxxxxxxxxxxxxx?xxx");
	int (*NET_SendDatagram)(SOCKET s, const char* buf, int len, int flags) = (int (*)(SOCKET, const char*, int, int))p_NET_SendDatagram; /*48 89 5C 24 08 48 89 6C 24 10 48 89 74 24 18 57 41 56 41 57 48 81 EC ?? 05 00 00*/

	DWORD64 p_NetChan_Shutdown = FindPatternV2("r5apex.exe", (const unsigned char*)"\x48\x89\x6C\x24\x18\x56\x57\x41\x56\x48\x83\xEC\x30\x83\xB9\xD0", "xxxxxxxxxxxxxxxx");
	void (*NetChan_Shutdown)(void* thisptr, const char* a0, unsigned __int8 a1, char a2) = (void (*)(void*, const char*, unsigned __int8, char))p_NetChan_Shutdown; /*48 89 6C 24 18 56 57 41 56 48 83 EC 30 83 B9 D0*/

	DWORD64 p_NET_PrintFunc = FindPatternV2("r5apex.exe", (const unsigned char*)"\x48\x89\x54\x24\x10\x4C\x89\x44\x24\x18\x4C\x89\x4C\x24\x20\xC3\x48", "xxxxxxxxxxxxxxxxx");
	void (*NET_PrintFunc)(const char* a1) = (void(*)(const char*))p_NET_PrintFunc; /*48 89 54 24 10 4C 89 44 24 18 4C 89 4C 24 20 C3 48*/
}

///////////////////////////////////////////////////////////////////////////////
bool HNET_ReceiveDatagram(int sock, void* inpacket, bool raw);
unsigned int HNET_SendDatagram(SOCKET s, const char* buf, int len, int flags);
void HNET_PrintFunc(const char* fmt, ...);
void NET_DisconnectClient(CClient* client, const char* reason, unsigned __int8 unk1, char unk2);

void AttachCNetChanHooks();
void DetachCNetChanHooks();

///////////////////////////////////////////////////////////////////////////////
extern bool g_bTraceNetChannel;
