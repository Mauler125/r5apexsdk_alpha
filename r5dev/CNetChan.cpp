#include "stdafx.h"
#include "CNetChan.h"

//-----------------------------------------------------------------------------
// Purpose: log the clients signonstate to the console
//-----------------------------------------------------------------------------
static std::ostringstream oss;
static auto ostream_sink = std::make_shared<spdlog::sinks::ostream_sink_st>(oss);
void HNET_PrintFunc(const char* fmt, ...)
{
	static bool initialized = false;
	static char buf[1024];

	static auto iconsole = spdlog::stdout_logger_mt("net_iconsole"); // in-game console
	static auto wconsole = spdlog::stdout_logger_mt("net_wconsole"); // windows console

	oss.str("");
	oss.clear();

	if (!initialized)
	{
		iconsole = std::make_shared<spdlog::logger>("ostream", ostream_sink);
		iconsole->set_pattern("[%S.%e] %v\n");
		iconsole->set_level(spdlog::level::debug);
		wconsole->set_pattern("[%S.%e] %v\n");
		wconsole->set_level(spdlog::level::debug);
	}

	va_list args;
	va_start(args, fmt);

	vsnprintf(buf, sizeof(buf), fmt, args);

	buf[sizeof(buf) - 1] = 0;
	va_end(args);

	iconsole->debug(buf);
	wconsole->debug(buf);

	std::string s = oss.str();
	const char* c = s.c_str();

	Items.push_back(Strdup((const char*)c));
}


//-----------------------------------------------------------------------------
// Purpose: hook and log the receive datagram
//-----------------------------------------------------------------------------
bool HNET_ReceiveDatagram(int sock, void* inpacket, bool raw)
{
	bool result = NET_ReceiveDatagram(sock, inpacket, raw);
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
	if (result)
	{
		// Log transmitted packet data
		HexDump("[+] NET_SendDatagram", 0, buf, len);
	}
	return result;
}

///////////////////////////////////////////////////////////////////////////////
void AttachCNetChanHooks()
{
	DetourAttach((LPVOID*)&NET_PrintFunc, &HNET_PrintFunc);
}

void DetachCNetChanHooks()
{
	DetourDetach((LPVOID*)&NET_PrintFunc, &HNET_PrintFunc);
}

void AttachCNetChanTraceHooks()
{
	DetourAttach((LPVOID*)&NET_ReceiveDatagram, &HNET_ReceiveDatagram);
	DetourAttach((LPVOID*)&NET_SendDatagram, &HNET_SendDatagram);
}

void DetachCNetChanTraceHooks()
{
	DetourDetach((LPVOID*)&NET_ReceiveDatagram, &HNET_ReceiveDatagram);
	DetourDetach((LPVOID*)&NET_SendDatagram, &HNET_SendDatagram);
}
