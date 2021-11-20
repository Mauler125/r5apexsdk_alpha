#include "stdafx.h"
#include "IVEngineServer.h"
#include "CClient.h"
#include "sys_utils.h"
#include "cvar.h"

//-----------------------------------------------------------------------------
// Purpose: sets the persistence var in the CClient instance to 'ready'
//-----------------------------------------------------------------------------
bool HIVEngineServer_PersistenceAvailable(std::int64_t thisptr, int index)
{
	CClient* client = g_pClient->GetClientInstance(index); // Get client instance.
	std::uintptr_t targetInstance = (std::uintptr_t)client;
	*(char*)(targetInstance + g_dwPersistenceVar) = (char)0x5; // Set the client instance to 'ready'.

	if (!g_bIsPersistenceVarSet[index] && sv_showconnecting->m_iValue > 0)
	{
		void* clientNamePtr = (void**)(((std::uintptr_t)client->GetNetChan()) + 0x1A8D); // Get client name from netchan.
		std::string clientName((char*)clientNamePtr, 32);                                // Get full name.
		std::int64_t originID = client->m_iOriginID;
		std::int64_t clientID = static_cast<std::int64_t>(client->m_iUserID + 1);

		std::string ipAddress = "null"; // If this stays null they modified the packet somehow.
		ADDRESS ipAddressField = ADDRESS(((std::uintptr_t)client->GetNetChan()) + 0x1AC0); // Get client ip from netchan.
		if (ipAddressField)
		{
			std::stringstream ss;
			ss  << std::to_string(ipAddressField.GetValue<std::uint8_t>()) << "."
				<< std::to_string(ipAddressField.Offset(0x1).GetValue<std::uint8_t>()) << "."
				<< std::to_string(ipAddressField.Offset(0x2).GetValue<std::uint8_t>()) << "."
				<< std::to_string(ipAddressField.Offset(0x3).GetValue<std::uint8_t>());

			ipAddress = ss.str();
		}

		Sys_Print(SYS_DLL::SERVER, "\n");
		Sys_Print(SYS_DLL::SERVER, "______________________________________________________________\n");
		Sys_Print(SYS_DLL::SERVER, "] CLIENT_INSTANCE_DETAILS ------------------------------------\n");
		Sys_Print(SYS_DLL::SERVER, "] INDEX: | '#%d'\n", index);
		Sys_Print(SYS_DLL::SERVER, "] NAME : | '%s'\n", clientName.c_str());
		Sys_Print(SYS_DLL::SERVER, "] OID  : | '%lld'\n", originID);
		Sys_Print(SYS_DLL::SERVER, "] UID  : | '%lld'\n", clientID);
		Sys_Print(SYS_DLL::SERVER, "] IPADR: | '%s'\n", ipAddress.c_str());
		Sys_Print(SYS_DLL::SERVER, "--------------------------------------------------------------\n");
		Sys_Print(SYS_DLL::SERVER, "\n");

		g_bIsPersistenceVarSet[index] = true;
	}
	///////////////////////////////////////////////////////////////////////////
	return IVEngineServer_PersistenceAvailable(thisptr, index);
}

void IVEngineServer_Attach()
{
	DetourAttach((LPVOID*)&IVEngineServer_PersistenceAvailable, &HIVEngineServer_PersistenceAvailable);
}

void IVEngineServer_Detach()
{
	DetourDetach((LPVOID*)&IVEngineServer_PersistenceAvailable, &HIVEngineServer_PersistenceAvailable);
}

///////////////////////////////////////////////////////////////////////////////
bool g_bIsPersistenceVarSet[128];
