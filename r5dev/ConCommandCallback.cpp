#include "stdafx.h"
#include "id3dx.h"
#include "rtech.h"
#include "classes.h"
#include "basetypes.h"
#include "CNetChan.h"
#include "IConVar.h"
#include "CGameConsole.h"
#include "ConCommandCallback.h"
#include "sys_utils.h"

void CGameConsole_Callback(const CCommand& cmd)
{
	g_bShowConsole = !g_bShowConsole;
}

void CCompanion_Callback(const CCommand& cmd)
{
	g_bShowBrowser = !g_bShowBrowser;
}

void Kick_Callback(CCommand* cmd)
{
	std::int32_t argSize = *(std::int32_t*)((std::uintptr_t)cmd + 0x4);
	if (argSize < 2) // Do we atleast have 2 arguments?
	{
		return;
	}

	CCommand& cmdReference = *cmd; // Get reference.
	const char* firstArg = cmdReference[1]; // Get first arg.

	for (int i = 0; i < MAX_PLAYERS; i++) // Loop through all possible client instances.
	{
		CClient* client = g_pClient->GetClientInstance(i); // Get client instance.
		if (!client)
		{
			continue;
		}

		if (!client->GetNetChan()) // Netchan valid?
		{
			continue;
		}

		void* clientNamePtr = (void**)(((std::uintptr_t)client->GetNetChan()) + 0x1A8D); // Get client name from netchan.
		std::string clientName((char*)clientNamePtr, 32); // Get full name.

		if (clientName.empty()) // Empty name?
		{
			continue;
		}

		if (strcmp(firstArg, clientName.c_str()) != 0) // Our wanted name?
		{
			continue;
		}

		NET_DisconnectClient(client, i, "Kicked from Server", 0, 1); // Disconnect client.
	}
}

void KickID_Callback(CCommand* cmd)
{
	static auto HasOnlyDigits = [](const std::string& string)
	{
		for (const char& character : string)
		{
			if (std::isdigit(character) == 0)
			{
				return false;
			}
		}
		return true;
	};

	std::int32_t argSize = *(std::int32_t*)((std::uintptr_t)cmd + 0x4);
	if (argSize < 2) // Do we atleast have 2 arguments?
	{
		return;
	}

	CCommand& cmdReference = *cmd; // Get reference.
	std::string firstArg = cmdReference[1]; // Get first arg.

	try
	{
		bool onlyDigits = HasOnlyDigits(firstArg); // Only has digits?
		for (int i = 0; i < MAX_PLAYERS; i++) // Loop through all possible client instances.
		{
			CClient* client = g_pClient->GetClientInstance(i); // Get client instance.
			if (!client)
			{
				continue;
			}

			if (!client->GetNetChan()) // Netchan valid?
			{
				continue;
			}

			std::string finalIPAddress = "null"; // If this stays null they modified the packet somehow.
			Address ipAddressField = Address(((std::uintptr_t)client->GetNetChan()) + 0x1AC0); // Get client ip from netchan.
			if (ipAddressField)
			{
				std::stringstream ss;
				ss << std::to_string(ipAddressField.GetValue<std::uint8_t>()) << "."
					<< std::to_string(ipAddressField.Offset(0x1).GetValue<std::uint8_t>()) << "."
					<< std::to_string(ipAddressField.Offset(0x2).GetValue<std::uint8_t>()) << "."
					<< std::to_string(ipAddressField.Offset(0x3).GetValue<std::uint8_t>());

				finalIPAddress = ss.str();
			}

			if (onlyDigits)
			{
				std::int64_t ID = static_cast<std::int64_t>(std::stoll(firstArg));
				if (ID > MAX_PLAYERS) // Is it a possible originID?
				{
					std::int64_t originID = client->m_iOriginID;
					if (originID != ID) // See if they match.
					{
						continue;
					}
				}
				else // If its not try by userID.
				{
					std::int64_t clientID = static_cast<std::int64_t>(client->m_iUserID + 1); // Get UserID + 1.
					if (clientID != ID) // See if they match.
					{
						continue;
					}
				}

				NET_DisconnectClient(client, i, "Kicked from Server", 0, 1); // Disconnect client.
			}
			else
			{
				if (firstArg.compare(finalIPAddress) != NULL) // Do the string equal?
				{
					continue;
				}

				NET_DisconnectClient(client, i, "Kicked from Server", 0, 1); // Disconnect client.
			}
		}
	}
	catch (std::exception& e)
	{
		Sys_Print(SYS_DLL::SERVER, "sv_kickid requires a UserID or OriginID. You can get the UserID with the 'status' command. Error: %s", e.what());
		return;
	}
}

void Ban_Callback(CCommand* cmd)
{
	std::int32_t argSize = *(std::int32_t*)((std::uintptr_t)cmd + 0x4);
	if (argSize < 2) // Do we atleast have 2 arguments?
	{
		return;
	}

	CCommand& cmdReference = *cmd; // Get reference.
	const char* firstArg = cmdReference[1]; // Get first arg.

	for (int i = 0; i < MAX_PLAYERS; i++) // Loop through all possible client instances.
	{
		CClient* client = g_pClient->GetClientInstance(i); // Get client instance.
		if (!client)
		{
			continue;
		}

		if (!client->GetNetChan()) // Netchan valid?
		{
			continue;
		}

		void* clientNamePtr = (void**)(((std::uintptr_t)client->GetNetChan()) + 0x1A8D); // Get client name from netchan.
		std::string clientName((char*)clientNamePtr, 32); // Get full name.

		if (clientName.empty()) // Empty name?
		{
			continue;
		}

		if (strcmp(firstArg, clientName.c_str()) != 0) // Our wanted name?
		{
			continue;
		}

		std::string finalIPAddress = "null"; // If this stays null they modified the packet somehow.
		Address ipAddressField = Address(((std::uintptr_t)client->GetNetChan()) + 0x1AC0); // Get client ip from netchan.
		if (ipAddressField && ipAddressField.GetValue<int>() != 0x0)
		{
			std::stringstream ss;
			ss << std::to_string(ipAddressField.GetValue<std::uint8_t>()) << "."
				<< std::to_string(ipAddressField.Offset(0x1).GetValue<std::uint8_t>()) << "."
				<< std::to_string(ipAddressField.Offset(0x2).GetValue<std::uint8_t>()) << "."
				<< std::to_string(ipAddressField.Offset(0x3).GetValue<std::uint8_t>());

			finalIPAddress = ss.str();
		}

		g_pBanSystem->AddEntry(finalIPAddress, client->m_iOriginID); // Add ban entry.
		g_pBanSystem->Save(); // Save ban list.
		NET_DisconnectClient(client, i, "Banned from Server", 0, 1); // Disconnect client.
	}
}

void BanID_Callback(CCommand* cmd)
{
	static auto HasOnlyDigits = [](const std::string& string)
	{
		for (const char& character : string)
		{
			if (std::isdigit(character) == 0)
			{
				return false;
			}
		}
		return true;
	};

	std::int32_t argSize = *(std::int32_t*)((std::uintptr_t)cmd + 0x4);
	if (argSize < 2) // Do we atleast have 2 arguments?
	{
		return;
	}

	CCommand& cmdReference = *cmd; // Get reference.
	std::string firstArg = cmdReference[1];

	try
	{
		bool onlyDigits = HasOnlyDigits(firstArg); // Only has digits?
		for (int i = 0; i < MAX_PLAYERS; i++) // Loop through all possible client instances.
		{
			CClient* client = g_pClient->GetClientInstance(i); // Get client instance.
			if (!client)
			{
				continue;
			}

			if (!client->GetNetChan()) // Netchan valid?
			{
				continue;
			}

			std::string finalIPAddress = "null"; // If this stays null they modified the packet somehow.
			Address ipAddressField = Address(((std::uintptr_t)client->GetNetChan()) + 0x1AC0); // Get client ip from netchan.
			if (ipAddressField)
			{
				std::stringstream ss;
				ss << std::to_string(ipAddressField.GetValue<std::uint8_t>()) << "."
					<< std::to_string(ipAddressField.Offset(0x1).GetValue<std::uint8_t>()) << "."
					<< std::to_string(ipAddressField.Offset(0x2).GetValue<std::uint8_t>()) << "."
					<< std::to_string(ipAddressField.Offset(0x3).GetValue<std::uint8_t>());

				finalIPAddress = ss.str();
			}

			if (onlyDigits)
			{
				std::int64_t ID = static_cast<std::int64_t>(std::stoll(firstArg));
				if (ID > MAX_PLAYERS) // Is it a possible originID?
				{
					std::int64_t originID = client->m_iOriginID;
					if (originID != ID) // See if they match.
					{
						continue;
					}
				}
				else // If its not try by userID.
				{
					std::int64_t clientID = static_cast<std::int64_t>(client->m_iUserID + 1); // Get UserID + 1.
					if (clientID != ID) // See if they match.
					{
						continue;
					}
				}

				g_pBanSystem->AddEntry(finalIPAddress, client->m_iOriginID); // Add ban entry.
				g_pBanSystem->Save(); // Save ban list.
				NET_DisconnectClient(client, i, "Banned from Server", 0, 1); // Disconnect client.
			}
			else
			{
				if (firstArg.compare(finalIPAddress) != NULL) // Do the string equal?
				{
					continue;
				}

				g_pBanSystem->AddEntry(finalIPAddress, client->m_iOriginID); // Add ban entry.
				g_pBanSystem->Save(); // Save ban list.
				NET_DisconnectClient(client, i, "Banned from Server", 0, 1); // Disconnect client.
			}
		}
	}
	catch (std::exception& e)
	{
		Sys_Print(SYS_DLL::SERVER, "Banid Error: %s", e.what());
		return;
	}
}

void Unban_Callback(CCommand* cmd)
{
	static auto HasOnlyDigits = [](const std::string& string)
	{
		for (const char& character : string)
		{
			if (std::isdigit(character) == 0)
			{
				return false;
			}
		}
		return true;
	};

	std::int32_t argSize = *(std::int32_t*)((std::uintptr_t)cmd + 0x4);
	if (argSize < 2) // Do we atleast have 2 arguments?
	{
		return;
	}

	CCommand& cmdReference = *cmd; // Get reference.

	try
	{
		const char* firstArg = cmdReference[1];
		if (HasOnlyDigits(firstArg)) // Check if we have an ip address or origin ID.
		{
			g_pBanSystem->DeleteEntry("noIP", std::stoll(firstArg)); // Delete ban entry.
			g_pBanSystem->Save(); // Save modified vector to file.
		}
		else
		{
			g_pBanSystem->DeleteEntry(firstArg, 1); // Delete ban entry.
			g_pBanSystem->Save(); // Save modified vector to file.
		}
	}
	catch (std::exception& e)
	{
		Sys_Print(SYS_DLL::SERVER, "Unban Error: %s", e.what());
		return;
	}
}

void ReloadBanList_Callback(CCommand* cmd)
{
	g_pBanSystem->Load(); // Reload banlist.
}

void ToHash_Callback(CCommand* cmd)
{
	CCommand& cmdReference = *cmd; // Get reference.
	const char* firstArg = cmdReference[1]; // Get first arg.
	unsigned long long guid = g_pRtech->ToGuid(firstArg);
	Sys_Print(SYS_DLL::ENGINE, "______________________________________________________________\n");
	Sys_Print(SYS_DLL::ENGINE, "# RTECH_HASH #################################################\n");
	Sys_Print(SYS_DLL::ENGINE, "GUID: 0x%llX\n", guid);
}

void NET_TraceNetChan(CCommand* cmd)
{
	if (!g_bTraceNetChannel)
	{
		if (g_pCvar->FindVar("net_usesocketsforloopback")->m_iValue != 1)
		{
			g_pCvar->FindVar("net_usesocketsforloopback")->m_iValue = 1;
		}
		Sys_Print(SYS_DLL::ENGINE, "\n");
		Sys_Print(SYS_DLL::ENGINE, "+--------------------------------------------------------+\n");
		Sys_Print(SYS_DLL::ENGINE, "|>>>>>>>>>>>>>| NETCHANNEL TRACE ACTIVATED |<<<<<<<<<<<<<|\n");
		Sys_Print(SYS_DLL::ENGINE, "+--------------------------------------------------------+\n");
		Sys_Print(SYS_DLL::ENGINE, "\n");
	}
	else
	{
		Sys_Print(SYS_DLL::ENGINE, "\n");
		Sys_Print(SYS_DLL::ENGINE, "+--------------------------------------------------------+\n");
		Sys_Print(SYS_DLL::ENGINE, "|>>>>>>>>>>>>| NETCHANNEL TRACE DEACTIVATED |<<<<<<<<<<<<|\n");
		Sys_Print(SYS_DLL::ENGINE, "+--------------------------------------------------------+\n");
		Sys_Print(SYS_DLL::ENGINE, "\n");
	}
	g_bTraceNetChannel = !g_bTraceNetChannel;
}
