#include "core/stdafx.h"
#include "tier0/cvar.h"
#include "engine/sys_utils.h"
#include "server/server.h"
#include "client/client.h"
#include "networksystem/r5net.h"
#include "public/include/bansystem.h"

//-----------------------------------------------------------------------------
// Purpose: checks is particular client is banned on the comp server
//-----------------------------------------------------------------------------
void IsClientBanned(R5Net::Client* pR5net, const std::string svIPAddr, std::int64_t nNucleusID)
{
	std::string svError = std::string();
	bool bCompBanned = pR5net && pR5net->GetClientIsBanned(svIPAddr, nNucleusID, svError);
	if (bCompBanned)
	{
		while (bCompBanned)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(500));
			for (int i = 0; i < MAX_PLAYERS; i++) // Loop through all possible client instances.
			{
				CClient* pClient = g_pClient->GetClientInstance(i); // Get client instance.
				if (!pClient) // Client instance valid?
				{
					continue;
				}

				if (!pClient->GetNetChan()) // Netchan valid?
				{
					continue;
				}

				std::int64_t nOriginID = pClient->m_iOriginID; // Get originID.
				if (nOriginID != nNucleusID) // See if they match.
				{
					continue;
				}

				g_pBanSystem->AddConnectionRefuse(svError, pClient->m_iUserID + 1); // Add to the vector.
				bCompBanned = false;
				break;
			}
		}
	}
}

//-----------------------------------------------------------------------------
// Purpose: client to server authentication
//-----------------------------------------------------------------------------
void* HCServer_Authenticate(void* pServer, user_creds* pInpacket)
{
	std::string svIpAddress = "null";
	ADDRESS ipAddressField = ADDRESS(&pInpacket->m_nIpAddr);

	if (ipAddressField && ipAddressField.GetValue<int>() != 0x0)
	{
		std::stringstream ss;
		ss << std::to_string(ipAddressField.GetValue<std::uint8_t>()) << "."
			<< std::to_string(ipAddressField.Offset(0x1).GetValue<std::uint8_t>()) << "."
			<< std::to_string(ipAddressField.Offset(0x2).GetValue<std::uint8_t>()) << "."
			<< std::to_string(ipAddressField.Offset(0x3).GetValue<std::uint8_t>());

		svIpAddress = ss.str();
	}

	if (sv_showconnecting->m_pParent->m_iValue > 0)
	{
		DevMsg(eDLL_T::SERVER, "\n");
		DevMsg(eDLL_T::SERVER, "______________________________________________________________\n");
		DevMsg(eDLL_T::SERVER, "] AUTHENTICATION_DETAILS -------------------------------------\n");
		DevMsg(eDLL_T::SERVER, "] UserID   : | '%s'\n", pInpacket->m_nUserID);
		DevMsg(eDLL_T::SERVER, "] OriginID : | '%lld'\n", pInpacket->m_nNucleusID);
		DevMsg(eDLL_T::SERVER, "] IP-ADDR  : | '%s'\n", svIpAddress.c_str());
		DevMsg(eDLL_T::SERVER, "--------------------------------------------------------------\n");
	}

	if (g_pBanSystem->IsBanListValid()) // Is the banlist vector valid?
	{
		if (g_pBanSystem->IsBanned(svIpAddress, pInpacket->m_nNucleusID)) // Is the client trying to connect banned?
		{
			CServer_RejectConnection(pServer, *(unsigned int*)((std::uintptr_t)pServer + 0xC), pInpacket, "You have been banned from this Server."); // RejectConnection for the client.

			if (sv_showconnecting->m_pParent->m_iValue > 0)
			{
				DevMsg(eDLL_T::SERVER, "] NOTICE   : | THIS CLIENT IS BANNED!\n");
				DevMsg(eDLL_T::SERVER, "--------------------------------------------------------------\n\n");
			}
			return nullptr;
		}
	}
	if (sv_showconnecting->m_pParent->m_iValue > 0)
	{
		DevMsg(eDLL_T::SERVER, "\n");
	}

	if (g_bCheckCompBanDB)
	{
		if (g_pR5net)
		{
			std::thread th(IsClientBanned, g_pR5net, svIpAddress, pInpacket->m_nNucleusID);
			th.detach();
		}
	}

	return CServer_Authenticate(pServer, pInpacket);
}

///////////////////////////////////////////////////////////////////////////////
void CServer_Attach()
{
	DetourAttach((LPVOID*)&CServer_Authenticate, &HCServer_Authenticate);
}

void CServer_Detach()
{
	DetourDetach((LPVOID*)&CServer_Authenticate, &HCServer_Authenticate);
}

///////////////////////////////////////////////////////////////////////////////
bool g_bCheckCompBanDB = true;