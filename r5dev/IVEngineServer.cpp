#include "stdafx.h"
#include "IVEngineServer.h"
#include "CClient.h"

//-----------------------------------------------------------------------------
// Purpose: sets the persistence var in the CClient instance to 'ready'
//-----------------------------------------------------------------------------
bool HIVEngineServer_PersistenceAvailable(__int64 thisptr, int index)
{
	static bool isPersistenceVarSet[128];

	CClient* client = g_pClient->GetClientInstance(index); // Get client instance.
	std::uintptr_t targetInstance = (std::uintptr_t)client;
	std::uintptr_t persistenceVar = 0x5BC;                 // TODO: Add to CClient struct.
	*(char*)(targetInstance + persistenceVar) = (char)0x5; // Set the client instance to 'ready'.

	if (!isPersistenceVarSet[index])
	{
		void* clientNamePtr = (void**)(((std::uintptr_t)client->GetNetChan()) + 0x1A8D); // Get client name from netchan.
		std::string clientName((char*)clientNamePtr, 32);                                // Get full name.
		std::int64_t originID = client->m_iOriginID;
		std::int64_t clientID = static_cast<std::int64_t>(client->m_iUserID + 1);

		std::string ipAddress = "null"; // If this stays null they modified the packet somehow.
		Address ipAddressField = Address(((std::uintptr_t)client->GetNetChan()) + 0x1AC0); // Get client ip from netchan.
		if (ipAddressField)
		{
			std::stringstream ss;
			ss << std::to_string(ipAddressField.GetValue<std::uint8_t>()) << "."
				<< std::to_string(ipAddressField.Offset(0x1).GetValue<std::uint8_t>()) << "."
				<< std::to_string(ipAddressField.Offset(0x2).GetValue<std::uint8_t>()) << "."
				<< std::to_string(ipAddressField.Offset(0x3).GetValue<std::uint8_t>());

			ipAddress = ss.str();
		}

		spdlog::debug("\n");
		spdlog::debug("Native(S):______________________________________________________________\n");
		spdlog::debug("Native(S):# CLIENT_INSTANCE_DETAILS ####################################\n");
		spdlog::debug("Native(S):] INDEX: | '#{}'\n", index);
		spdlog::debug("Native(S):] NAME : | '{}'\n", clientName.c_str());
		spdlog::debug("Native(S):] OID  : | '{}'\n", originID);
		spdlog::debug("Native(S):] UID  : | '{}'\n", clientID);
		spdlog::debug("Native(S):] IPADR: | '{}'\n", ipAddress.c_str());
		spdlog::debug("Native(S):##############################################################\n");
		spdlog::debug("\n");
		isPersistenceVarSet[index] = true;
	}
	///////////////////////////////////////////////////////////////////////////
	return IVEngineServer_PersistenceAvailable(thisptr, index);
}

void AttachIVEngineServerHooks()
{
	DetourAttach((LPVOID*)&IVEngineServer_PersistenceAvailable, &HIVEngineServer_PersistenceAvailable);
}

void DetachIVEngineServerHooks()
{
	DetourDetach((LPVOID*)&IVEngineServer_PersistenceAvailable, &HIVEngineServer_PersistenceAvailable);
}
