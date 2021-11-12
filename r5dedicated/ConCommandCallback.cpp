#include "stdafx.h"
#include "rtech.h"
#include "classes.h"
#include "basetypes.h"
#include "CNetChan.h"
#include "IConVar.h"
#include "ConCommandCallback.h"
#include "sys_utils.h"

#pragma comment(lib, "bcrypt.lib")

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

void RTech_GenerateGUID_Callback(CCommand* cmd)
{
	std::int32_t argSize = *(std::int32_t*)((std::uintptr_t)cmd + 0x4);

	if (argSize < 2) // Do we atleast have 2 arguments?
	{
		return;
	}

	CCommand& cmdReference = *cmd; // Get reference.
	const char* firstArg = cmdReference[1]; // Get first arg.
	unsigned long long guid = g_pRtech->StringToGuid(firstArg);

	Sys_Print(SYS_DLL::ENGINE, "______________________________________________________________\n");
	Sys_Print(SYS_DLL::ENGINE, "# RTECH_HASH |#################################################\n");
	Sys_Print(SYS_DLL::ENGINE, "] GUID: '0x%llX'\n", guid);
}

void RTech_DecompRpak_Callback(CCommand* cmd)
{
	std::int32_t argSize = *(std::int32_t*)((std::uintptr_t)cmd + 0x4);

	if (argSize < 2) // Do we atleast have 2 arguments?
	{
		return;
	}

	CCommand& cmdReference = *cmd; // Get reference.
	const char* firstArg = cmdReference[1]; // Get first arg.
	const char* secondArg = cmdReference[2]; // Get second arg.

	Sys_Print(SYS_DLL::ENGINE, "______________________________________________________________\n");
	Sys_Print(SYS_DLL::ENGINE, "] RTECH_DECOMPRESS |##########################################\n");
	Sys_Print(SYS_DLL::ENGINE, "] Processing: '%s'\n", firstArg);

	const std::string mod_dir = "paks\\Win32\\";
	const std::string base_dir = "paks\\Win64\\";
	const std::string pak_name_out = mod_dir + firstArg;
	const std::string pak_name_in = base_dir + firstArg;

	std::vector<uint8_t> upak; // Compressed region.
	std::ifstream ipak(pak_name_in, std::fstream::binary);

	ipak.seekg(0, std::fstream::end);
	upak.resize(ipak.tellg());
	ipak.seekg(0, std::fstream::beg);
	ipak.read((char*)upak.data(), upak.size());

	//std::vector<uint8_t>::iterator it = std::next(upak.begin(), PAK_HEADER_SIZE);
	//std::move(upak.begin(), it, std::back_inserter(uheader));

	auto rheader = (rpak_h*)upak.data();

	if (rheader->m_nMagic != 'kaPR')
	{
		Sys_Print(SYS_DLL::ENGINE, "] Error: pak file '%s' has invalid magic!\n", pak_name_in.c_str());
		return;
	}
	if ((rheader->m_bCompressed & 1) != 1)
	{
		Sys_Print(SYS_DLL::ENGINE, "] Error: pak file '%s' already decompressed!\n", pak_name_in.c_str());
		return;
	}
	if (rheader->m_nSizeComp != upak.size())
	{
		Sys_Print(SYS_DLL::ENGINE, "] Error: pak file '%s' decompressed size '%u' doesn't match expected value '%u'!\n", pak_name_in.c_str(), upak.size(), rheader->m_nSizeDecomp);
		return;
	}

	Sys_Print(SYS_DLL::ENGINE, "______________________________________________________________\n");
	Sys_Print(SYS_DLL::ENGINE, "] HEADER_DETAILS |############################################\n");
	Sys_Print(SYS_DLL::ENGINE, "] Magic    : '%08X'\n", rheader->m_nMagic);
	Sys_Print(SYS_DLL::ENGINE, "] Version  : '%u'\n", (rheader->m_nVersion));
	Sys_Print(SYS_DLL::ENGINE, "] Flags    : '%u'\n", (rheader->m_nFlag));
	Sys_Print(SYS_DLL::ENGINE, "] Type     : '%llu'\n", rheader->m_nType);
	Sys_Print(SYS_DLL::ENGINE, "] Entries  : '%zu'\n", rheader->m_nEntry);
	Sys_Print(SYS_DLL::ENGINE, "______________________________________________________________\n");
	Sys_Print(SYS_DLL::ENGINE, "] COMPRESSION_DETAILS |#######################################\n");
	Sys_Print(SYS_DLL::ENGINE, "] Size disk: '%lld'\n", rheader->m_nSizeComp);
	Sys_Print(SYS_DLL::ENGINE, "] Size decp: '%lld'\n", rheader->m_nSizeDecomp);
	Sys_Print(SYS_DLL::ENGINE, "] Ratio    : '%.02f'\n", (rheader->m_nSizeComp * 100.f) / rheader->m_nSizeDecomp);


	int64_t parameters[18];
	uint32_t dsize = g_pRtech->DecompressedSize((int64_t)(parameters), upak.data(), upak.size(), 0, PAK_HEADER_SIZE);
	if (dsize == rheader->m_nSizeComp)
	{
		Sys_Print(SYS_DLL::ENGINE, "] Error: calculated size: '%zu' expected: '%zu'!\n", dsize, rheader->m_nSizeDecomp);
		return;
	}
	else
	{
		Sys_Print(SYS_DLL::ENGINE, "] Calculated size: '%zu'\n", dsize);
	}

	std::vector<uint8_t> decompress_buffer(rheader->m_nSizeDecomp, 0); //(0x400000, 0);

	parameters[1] = int64_t(decompress_buffer.data());
	parameters[3] = -1i64;
	auto dret = g_pRtech->Decompress(parameters, upak.size(), decompress_buffer.size());
	if (dret != 1)
	{
		Sys_Print(SYS_DLL::ENGINE, "] Error: decompression failed for '%s' return value: '%d'!\n", pak_name_in.c_str(), +dret);
		return;
	}

	rheader->m_bCompressed = false; // Set compressed to false for the decompressed pak file
	rheader->m_nSizeComp = rheader->m_nSizeDecomp; // Equal compressed size with decompressed

	std::ofstream out_block(pak_name_out, std::fstream::binary);
	std::ofstream out_header(pak_name_out, std::fstream::binary);

	out_block.write((char*)decompress_buffer.data(), parameters[5]);
	//out_header.write((char*)uheader.data(), uheader.size());
	out_header.write((char*)rheader, PAK_HEADER_SIZE);

	Sys_Print(SYS_DLL::ENGINE, "] Wrote unpacked file to '%s'\n", pak_name_out.c_str());
}

void NET_TraceNetChan_Callback(CCommand* cmd)
{
	if (!g_bTraceNetChannel)
	{
		g_pCvar->FindVar("net_usesocketsforloopback")->m_iValue = 1;
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

void NET_SetKey_Callback(CCommand* cmd)
{
	CCommand& cmdReference = *cmd; // Get reference.
	const char* firstArg = cmdReference[1]; // Get first arg.

	std::int32_t argSize = *(std::int32_t*)((std::uintptr_t)cmd + 0x4);

	if (argSize < 1) // Do we atleast have 1 argument?
	{
		return;
	}

	HNET_SetKey(firstArg);
}

void NET_GenerateKey_Callback(CCommand* cmd)
{
	HNET_GenerateKey();
}
