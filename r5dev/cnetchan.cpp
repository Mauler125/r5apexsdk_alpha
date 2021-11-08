#include "stdafx.h"
#include "logdef.h"
#include "CNetChan.h"
#include "CGameConsole.h"

//-----------------------------------------------------------------------------
// Purpose: hook and log the client's signonstate to the console
//-----------------------------------------------------------------------------
void HNET_PrintFunc(const char* fmt, ...)
{
	static bool initialized = false;
	static char buf[1024];

	static auto iconsole = spdlog::stdout_logger_mt("net_print_iconsole"); // in-game console
	static auto wconsole = spdlog::stdout_logger_mt("net_print_wconsole"); // windows console

	g_spd_net_p_oss.str("");
	g_spd_net_p_oss.clear();

	if (!initialized)
	{
		iconsole = std::make_shared<spdlog::logger>("net_print_ostream", g_spd_net_p_ostream_sink);
		iconsole->set_pattern("[%S.%e] Native(C):%v\n");
		iconsole->set_level(spdlog::level::debug);
		wconsole->set_pattern("[%S.%e] Native(C):%v\n");
		wconsole->set_level(spdlog::level::debug);
	}

	va_list args;
	va_start(args, fmt);

	vsnprintf(buf, sizeof(buf), fmt, args);

	buf[sizeof(buf) - 1] = 0;
	va_end(args);

	iconsole->debug(buf);
	wconsole->debug(buf);

	std::string s = g_spd_net_p_oss.str();
	const char* c = s.c_str();

	Items.push_back(Strdup((const char*)c));
}

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
		HexDump("[+] NET_ReceiveDatagram", "netchan_logger", &pkt->data[i], pkt->wiresize);
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
		HexDump("[+] NET_SendDatagram", "netchan_logger", buf, len);
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
