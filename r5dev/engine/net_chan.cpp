//=============================================================================//
//
// Purpose: Netchannel system utilities
//
//=============================================================================//

#include "core/stdafx.h"
#include "core/logdef.h"
#include "tier0/cvar.h"
#include "tier0/completion.h"
#include "engine/sys_utils.h"
#include "engine/net_chan.h"
#include "engine/baseclient.h"
#include "server/IVEngineServer.h"

#ifndef DEDICATED
#include "gameui/IConsole.h"
#endif // !DEDICATED

//-----------------------------------------------------------------------------
// Purpose: shutdown netchannel
//-----------------------------------------------------------------------------
void NET_ShutDown(void* thisptr, const char* szReason, std::uint8_t a1, char a2)
{
	DownloadPlaylists_Callback(); // Re-load playlist from disk after getting disconnected from the server.
	NET_ShutDown(thisptr, szReason, a1, a2);
}

//-----------------------------------------------------------------------------
// Purpose: hook and log the receive datagram
//-----------------------------------------------------------------------------
bool HNET_ReceiveDatagram(int iSocket, netpacket_s* pInpacket, bool bRaw)
{
	bool result = NET_ReceiveDatagram(iSocket, pInpacket, bRaw);
	if (result)
	{
		// Log received packet data
		HexDump("[+] NET_ReceiveDatagram", 0, &pInpacket->data[NULL], pInpacket->wiresize);
	}
	return result;
}

//-----------------------------------------------------------------------------
// Purpose: hook and log the send datagram
//-----------------------------------------------------------------------------
void* HNET_SendDatagram(SOCKET s, const char* szPayload, int iLenght, int nFlags)
{
	void* result = NET_SendDatagram(s, szPayload, iLenght, nFlags);
	if (result)
	{
		// Log transmitted packet data
		HexDump("[+] NET_SendDatagram", 0, szPayload, iLenght);
	}
	return result;
}

//-----------------------------------------------------------------------------
// Purpose: sets the user specified encryption key
//-----------------------------------------------------------------------------
void HNET_SetKey(std::string svNetKey)
{
	g_szNetKey.clear();
	g_szNetKey = svNetKey;

	DevMsg(eDLL::ENGINE, "______________________________________________________________\n");
	DevMsg(eDLL::ENGINE, "] NET_KEY ----------------------------------------------------\n");
	DevMsg(eDLL::ENGINE, "] BASE64: '%s'\n", g_szNetKey.c_str());
	DevMsg(eDLL::ENGINE, "--------------------------------------------------------------\n");

	NET_SetKey(g_pNetKey, g_szNetKey.c_str());
}

//-----------------------------------------------------------------------------
// Purpose: calculates and sets the encryption key
//-----------------------------------------------------------------------------
void HNET_GenerateKey()
{
	g_szNetKey.clear();
	net_userandomkey->m_pParent->m_iValue = 1;

	BCRYPT_ALG_HANDLE hAlgorithm;
	if (BCryptOpenAlgorithmProvider(&hAlgorithm, L"RNG", 0, 0) < 0)
	{
		DevMsg(eDLL::ENGINE, "Failed to open rng algorithm\n");
		return;
	}
	unsigned char pBuffer[0x10u];
	if (BCryptGenRandom(hAlgorithm, pBuffer, 0x10u, 0) < 0)
	{
		DevMsg(eDLL::ENGINE, "Failed to generate random data\n");
		return;
	}

	for (int i = 0; i < 0x10u; i++)
	{
		g_szNetKey += pBuffer[i];
	}

	g_szNetKey = Base64Encode(g_szNetKey);

	DevMsg(eDLL::ENGINE, "______________________________________________________________\n");
	DevMsg(eDLL::ENGINE, "] NET_KEY ----------------------------------------------------\n");
	DevMsg(eDLL::ENGINE, "] BASE64: '%s'\n", g_szNetKey.c_str());
	DevMsg(eDLL::ENGINE, "--------------------------------------------------------------\n");

	NET_SetKey(g_pNetKey, g_szNetKey.c_str());
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

	DevMsg(eDLL::CLIENT, "%s\n", buf);
}

//-----------------------------------------------------------------------------
// Purpose: disconnect the client and shutdown netchannel
//-----------------------------------------------------------------------------
void NET_DisconnectClient(CClient* pClient, int nIndex, const char* szReason, uint8_t unk1, char unk2)
{
	if (!pClient) //	Client valid?
	{
		return;
	}
	if (std::strlen(szReason) == NULL) // Is reason null?
	{
		return;
	}
	if (!pClient->GetNetChan())
	{
		return;
	}

	NET_Shutdown(pClient->GetNetChan(), szReason, unk1, unk2); // Shutdown netchan.
	pClient->GetNetChan() = nullptr;                           // Null netchan.
	CBaseClient_Clear((std::int64_t)pClient);                  // Reset CClient instance for client.
	g_bIsPersistenceVarSet[nIndex] = false;                    // Reset Persistence var.
}

///////////////////////////////////////////////////////////////////////////////
void CNetChan_Attach()
{
	DetourAttach((LPVOID*)&NET_PrintFunc, &HNET_PrintFunc);
}

void CNetChan_Detach()
{
	DetourDetach((LPVOID*)&NET_PrintFunc, &HNET_PrintFunc);
}

void CNetChan_Trace_Attach()
{
	DetourAttach((LPVOID*)&NET_ReceiveDatagram, &HNET_ReceiveDatagram);
	DetourAttach((LPVOID*)&NET_SendDatagram, &HNET_SendDatagram);
}

void CNetChan_Trace_Detach()
{
	DetourDetach((LPVOID*)&NET_ReceiveDatagram, &HNET_ReceiveDatagram);
	DetourDetach((LPVOID*)&NET_SendDatagram, &HNET_SendDatagram);
}

///////////////////////////////////////////////////////////////////////////////
std::string g_szNetKey = "WDNWLmJYQ2ZlM0VoTid3Yg==";
std::uintptr_t g_pNetKey = g_mGameDll.StringSearch("client:NetEncryption_NewKey").FindPatternSelf("48 8D ? ? ? ? ? 48 3B", ADDRESS::Direction::UP, 150).ResolveRelativeAddressSelf(0x3, 0x7).GetPtr();
