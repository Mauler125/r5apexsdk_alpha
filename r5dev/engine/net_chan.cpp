#include "core/stdafx.h"
#include "core/logdef.h"
#include "engine/sys_utils.h"
#include "engine/net_chan.h"
#include "engine/baseclient.h"
#include "server/IVEngineServer.h"
#include "gameui/IGameConsole.h"
#include "tier0/cvar.h"

//-----------------------------------------------------------------------------
// Purpose: hook and log the receive datagram
//-----------------------------------------------------------------------------
char HNET_ReceiveDatagram(int sock, void* inpacket, bool raw)
{
	if (!g_bTraceNetChannel)
	{
		return NET_ReceiveDatagram(sock, inpacket, raw);
	}

	const int i = NULL;
	char result = NET_ReceiveDatagram(sock, inpacket, raw);
	if (result)
	{
		netpacket_t* pkt = (netpacket_t*)inpacket;

		// Log received packet data
		HexDump("[+] NET_ReceiveDatagram", 0, &pkt->data[i], pkt->wiresize);
	}
	return result;
}

//-----------------------------------------------------------------------------
// Purpose: hook and log the send datagram
//-----------------------------------------------------------------------------
std::int64_t HNET_SendDatagram(SOCKET s, const char* buf, int len, int flags)
{
	if (!g_bTraceNetChannel)
	{
		return NET_SendDatagram(s, buf, len, flags);
	}

	std::int64_t result = NET_SendDatagram(s, buf, len, flags);
	if (result)
	{
		// Log transmitted packet data
		HexDump("[+] NET_SendDatagram", 0, buf, len);
	}
	return result;
}

//-----------------------------------------------------------------------------
// Purpose: sets the encryption key
//-----------------------------------------------------------------------------
void HNET_SetKey(std::string key)
{
	g_szNetKey.clear();
	g_szNetKey = key;

	Sys_Print(SYS_DLL::ENGINE, "______________________________________________________________\n");
	Sys_Print(SYS_DLL::ENGINE, "] NET_KEY ----------------------------------------------------\n");
	Sys_Print(SYS_DLL::ENGINE, "] BASE64: '%s'\n", g_szNetKey.c_str());
	Sys_Print(SYS_DLL::ENGINE, "--------------------------------------------------------------\n");

	NET_SetKey(g_uNetKeyPtr, g_szNetKey.c_str());
}

//-----------------------------------------------------------------------------
// Purpose: calculates and sets the encryption key
//-----------------------------------------------------------------------------
void HNET_GenerateKey()
{
	g_szNetKey.clear();
	net_userandomkey->m_iValue = 1;

	BCRYPT_ALG_HANDLE hAlgorithm;
	if (BCryptOpenAlgorithmProvider(&hAlgorithm, L"RNG", 0, 0) < 0)
	{
		Sys_Print(SYS_DLL::ENGINE, "Failed to open rng algorithm\n");
	}
	unsigned char pBuffer[0x10u];
	if (BCryptGenRandom(hAlgorithm, pBuffer, 0x10u, 0) < 0)
	{
		Sys_Print(SYS_DLL::ENGINE, "Failed to generate random data\n");
	}

	for (int i = 0; i < 0x10u; i++)
	{
		g_szNetKey += pBuffer[i];
	}

	g_szNetKey = base64_encode(g_szNetKey);

	Sys_Print(SYS_DLL::ENGINE, "______________________________________________________________\n");
	Sys_Print(SYS_DLL::ENGINE, "] NET_KEY ----------------------------------------------------\n");
	Sys_Print(SYS_DLL::ENGINE, "] BASE64: '%s'\n", g_szNetKey.c_str());
	Sys_Print(SYS_DLL::ENGINE, "--------------------------------------------------------------\n");

	NET_SetKey(g_uNetKeyPtr, g_szNetKey.c_str());
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

//-----------------------------------------------------------------------------
// Purpose: disconnect the client and shutdown netchannel
//-----------------------------------------------------------------------------
void NET_DisconnectClient(CClient* client, int index, const char* reason, uint8_t unk1, char unk2)
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

	NET_Shutdown(client->GetNetChan(), reason, unk1, unk2); // Shutdown netchan.
	client->GetNetChan() = nullptr;                         // Null netchan.
	CBaseClient_Clear((std::int64_t)client);                // Reset CClient instance for client.
	g_bIsPersistenceVarSet[index] = false;                  // Reset Persistence var.
}

///////////////////////////////////////////////////////////////////////////////
void CNetChan_Attach()
{
	DetourAttach((LPVOID*)&NET_PrintFunc, &HNET_PrintFunc);
	DetourAttach((LPVOID*)&NET_ReceiveDatagram, &HNET_ReceiveDatagram);
	DetourAttach((LPVOID*)&NET_SendDatagram, &HNET_SendDatagram);
}

void CNetChan_Detach()
{
	DetourDetach((LPVOID*)&NET_PrintFunc, &HNET_PrintFunc);
	DetourDetach((LPVOID*)&NET_ReceiveDatagram, &HNET_ReceiveDatagram);
	DetourDetach((LPVOID*)&NET_SendDatagram, &HNET_SendDatagram);
}

///////////////////////////////////////////////////////////////////////////////
bool g_bTraceNetChannel;
std::string g_szNetKey = "WDNWLmJYQ2ZlM0VoTid3Yg==";
std::uintptr_t g_uNetKeyPtr = g_mGameDll.StringSearch("client:NetEncryption_NewKey").FindPatternSelf("48 8D ? ? ? ? ? 48 3B", ADDRESS::Direction::UP, 150).ResolveRelativeAddressSelf(0x3, 0x7).GetPtr();
