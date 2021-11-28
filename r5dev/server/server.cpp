#include "core/stdafx.h"
#include "tier0/cvar.h"
#include "engine/sys_utils.h"
#include "server/server.h"
#include "client/client.h"
#include "networksystem/r5net.h"
#include "public/include/bansystem.h"

void IsClientBanned(R5Net::Client* r5net, const std::string ipaddr, std::int64_t nucleus_id)
{
	std::string err = std::string();
	bool compBanned = r5net && r5net->GetClientIsBanned(ipaddr, nucleus_id, err);
	if (compBanned)
	{
		while (compBanned)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(500));
			for (int i = 0; i < MAX_PLAYERS; i++) // Loop through all possible client instances.
			{
				CClient* client = g_pClient->GetClientInstance(i); // Get client instance.
				if (!client) // Client instance valid?
				{
					continue;
				}

				if (!client->GetNetChan()) // Netchan valid?
				{
					continue;
				}

				std::int64_t originID = client->m_iOriginID; // Get originID.
				if (originID != nucleus_id) // See if they match.
				{
					continue;
				}

				g_pBanSystem->AddConnectionRefuse(err, client->m_iUserID + 1); // Add to the vector.
				compBanned = false;
				break;
			}
		}
	}
}

void* HCServer_Authenticate(void* cserver, user_creds* inpacket)
{
	std::string ipAddress = "null";
	ADDRESS ipAddressField = ADDRESS(&inpacket->ipaddr);

	if (ipAddressField && ipAddressField.GetValue<int>() != 0x0)
	{
		std::stringstream ss;
		ss << std::to_string(ipAddressField.GetValue<std::uint8_t>()) << "."
			<< std::to_string(ipAddressField.Offset(0x1).GetValue<std::uint8_t>()) << "."
			<< std::to_string(ipAddressField.Offset(0x2).GetValue<std::uint8_t>()) << "."
			<< std::to_string(ipAddressField.Offset(0x3).GetValue<std::uint8_t>());

		ipAddress = ss.str();
	}

	if (sv_showconnecting->m_iValue > 0)
	{
		Sys_Print(SYS_DLL::SERVER, "\n");
		Sys_Print(SYS_DLL::SERVER, "______________________________________________________________\n");
		Sys_Print(SYS_DLL::SERVER, "] AUTHENTICATION_DETAILS -------------------------------------\n");
		Sys_Print(SYS_DLL::SERVER, "] UserID   : | '%s'\n", inpacket->user_id);
		Sys_Print(SYS_DLL::SERVER, "] OriginID : | '%lld'\n", inpacket->nucleus_id);
		Sys_Print(SYS_DLL::SERVER, "] IP-ADDR  : | '%s'\n", ipAddress.c_str());
		Sys_Print(SYS_DLL::SERVER, "--------------------------------------------------------------\n");
	}

	if (g_pBanSystem->IsBanListValid()) // Is the banlist vector valid?
	{
		if (g_pBanSystem->IsBanned(ipAddress, inpacket->nucleus_id)) // Is the client trying to connect banned?
		{
			CServer_RejectConnection(cserver, *(unsigned int*)((std::uintptr_t)cserver + 0xC), inpacket, "You have been banned from this Server."); // RejectConnection for the client.

			if (sv_showconnecting->m_iValue > 0)
			{
				Sys_Print(SYS_DLL::SERVER, "] NOTICE   : | THIS CLIENT IS BANNED!\n");
				Sys_Print(SYS_DLL::SERVER, "--------------------------------------------------------------\n");
				Sys_Print(SYS_DLL::SERVER, "\n");
			}
			return nullptr;
		}
	}
	if (sv_showconnecting->m_iValue > 0)
	{
		Sys_Print(SYS_DLL::SERVER, "\n");
	}

	if (g_bCheckCompBanDB)
	{
		if (r5net)
		{
			std::thread t1(IsClientBanned, r5net, ipAddress, inpacket->nucleus_id);
			t1.detach();
		}
	}

	return CServer_Authenticate(cserver, inpacket);
}
bool g_bCheckCompBanDB = true;

void CServer_Attach()
{
	DetourAttach((LPVOID*)&CServer_Authenticate, &HCServer_Authenticate);
}

void CServer_Detach()
{
	DetourDetach((LPVOID*)&CServer_Authenticate, &HCServer_Authenticate);
}