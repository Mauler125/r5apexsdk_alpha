#include "stdafx.h"
#include "logdef.h"
#include "CNetChan.h"
#include "CGameConsole.h"
#include "sys_utils.h"

//-----------------------------------------------------------------------------
// Purpose: hook and log the receive datagram
//-----------------------------------------------------------------------------
bool HNET_ReceiveDatagram(int sock, void* inpacket, bool raw)
{
	bool result = NET_ReceiveDatagram(sock, inpacket, raw);
	if (!g_bTraceNetChannel)
	{
		return result;
	}

	if (result)
	{
		int i = NULL;
		netpacket_t* pkt = (netpacket_t*)inpacket;

		// Log received packet data
		HexDump("[+] NET_ReceiveDatagram", 0, &pkt->data[i], pkt->wiresize);
	}
	return result;
}

//-----------------------------------------------------------------------------
// Purpose: hook and log the send datagram
//-----------------------------------------------------------------------------
unsigned int HNET_SendDatagram(SOCKET s, const char* buf, int len, int flags)
{
	unsigned int result = NET_SendDatagram(s, buf, len, flags);
	if (!g_bTraceNetChannel)
	{
		return result;
	}

	if (result)
	{
		// Log transmitted packet data
		HexDump("[+] NET_SendDatagram", 0, buf, len);
	}
	return result;
}

//-----------------------------------------------------------------------------
// Purpose: disconnect the client and shutdown netchannel
//-----------------------------------------------------------------------------
void NET_DisconnectClient(CClient* client, const char* reason, unsigned __int8 unk1, char unk2)
{
	if (!client) //	Client valid?
	{
		return;
	}
	if (std::strlen(reason) == NULL) // Is reason null?
	{
		return;
	}
	if (!client->GetNetChan())
	{
		return;
	}

	NetChan_Shutdown(client->GetNetChan(), reason, unk1, unk2); // Shutdown netchan.
	client->GetNetChan() = nullptr;                             // Null netchan.
	Address(0x140302FD0).RCast<void(*)(CClient*)>()(client);    // Reset CClient instance for client.
}

//-----------------------------------------------------------------------------
// Purpose: hook and log the client's signonstate to the console
//-----------------------------------------------------------------------------
void HNET_PrintFunc(const char* fmt, ...)
{
	static char buf[1024];

	va_list args;
	va_start(args, fmt);

	vsnprintf(buf, sizeof(buf), fmt, args);

	buf[sizeof(buf) -1] = 0;
	va_end(args);

	Sys_Print(SYS_DLL::CLIENT, "%s\n", buf);
}

///////////////////////////////////////////////////////////////////////////////
void AttachCNetChanHooks()
{
	DetourAttach((LPVOID*)&NET_PrintFunc, &HNET_PrintFunc);
	DetourAttach((LPVOID*)&NET_ReceiveDatagram, &HNET_ReceiveDatagram);
	DetourAttach((LPVOID*)&NET_SendDatagram, &HNET_SendDatagram);
}

void DetachCNetChanHooks()
{
	DetourDetach((LPVOID*)&NET_PrintFunc, &HNET_PrintFunc);
	DetourDetach((LPVOID*)&NET_ReceiveDatagram, &HNET_ReceiveDatagram);
	DetourDetach((LPVOID*)&NET_SendDatagram, &HNET_SendDatagram);
}

bool g_bTraceNetChannel;
