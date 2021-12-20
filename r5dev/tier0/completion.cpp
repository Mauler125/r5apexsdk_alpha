#include "core/stdafx.h"
#include "windows/id3dx.h"
#include "tier0/basetypes.h"
#include "tier0/IConVar.h"
#include "tier0/completion.h"
#include "engine/net_chan.h"
#include "engine/sys_utils.h"
#include "rtech/rtech.h"
#include "vpklib/packedstore.h"
#include "gameui/IConsole.h"
#include "public/include/bansystem.h"

#ifndef DEDICATED
void CGameConsole_Callback(const CCommand& cmd)
{
	g_bShowConsole = !g_bShowConsole;
}

void CCompanion_Callback(const CCommand& cmd)
{
	g_bShowBrowser = !g_bShowBrowser;
}
#endif // !DEDICATED

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
			ADDRESS ipAddressField = ADDRESS(((std::uintptr_t)client->GetNetChan()) + 0x1AC0); // Get client ip from netchan.
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
		ADDRESS ipAddressField = ADDRESS(((std::uintptr_t)client->GetNetChan()) + 0x1AC0); // Get client ip from netchan.
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
			ADDRESS ipAddressField = ADDRESS(((std::uintptr_t)client->GetNetChan()) + 0x1AC0); // Get client ip from netchan.
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

	Sys_Print(SYS_DLL::RTECH, "______________________________________________________________\n");
	Sys_Print(SYS_DLL::RTECH, "] RTECH_HASH -------------------------------------------------\n");
	Sys_Print(SYS_DLL::RTECH, "] GUID: '0x%llX'\n", guid);
}

void RTech_Decompress_Callback(CCommand* cmd)
{
	std::int32_t argSize = *(std::int32_t*)((std::uintptr_t)cmd + 0x4);

	if (argSize < 2) // Do we atleast have 2 arguments?
	{
		return;
	}

	CCommand& cmdReference = *cmd; // Get reference.
	std::string firstArg  = cmdReference[1]; // Get first arg.
	std::string secondArg = cmdReference[2]; // Get second arg.

	const std::string mod_dir = "paks\\Win32\\";
	const std::string base_dir = "paks\\Win64\\";

	std::string pak_name_out = mod_dir + firstArg + ".rpak";
	std::string pak_name_in = base_dir + firstArg + ".rpak";

	Sys_Print(SYS_DLL::RTECH, "______________________________________________________________\n");
	Sys_Print(SYS_DLL::RTECH, "] RTECH_DECOMPRESS -------------------------------------------\n");

	if (!FileExists(pak_name_in.c_str()))
	{
		Sys_Print(SYS_DLL::RTECH, "Error: pak file '%s' does not exist!\n", pak_name_in.c_str());
		return;
	}

	Sys_Print(SYS_DLL::RTECH, "] Processing: '%s'\n", pak_name_in.c_str());

	std::vector<std::uint8_t> upak; // Compressed region.
	std::ifstream ipak(pak_name_in, std::fstream::binary);

	ipak.seekg(0, std::fstream::end);
	upak.resize(ipak.tellg());
	ipak.seekg(0, std::fstream::beg);
	ipak.read((char*)upak.data(), upak.size());

	rpak_h* rheader = (rpak_h*)upak.data();
	uint16_t flags = (rheader->m_nFlags[0] << 8) | rheader->m_nFlags[1];

	Sys_Print(SYS_DLL::RTECH, "______________________________________________________________\n");
	Sys_Print(SYS_DLL::RTECH, "] HEADER_DETAILS ---------------------------------------------\n");
	Sys_Print(SYS_DLL::RTECH, "] Magic    : '%08X'\n", rheader->m_nMagic);
	Sys_Print(SYS_DLL::RTECH, "] Version  : '%u'\n", (rheader->m_nVersion));
	Sys_Print(SYS_DLL::RTECH, "] Flags    : '%04X'\n", (flags));
	Sys_Print(SYS_DLL::RTECH, "] Hash     : '%llu'\n", rheader->m_nHash);
	Sys_Print(SYS_DLL::RTECH, "] Entries  : '%zu'\n", rheader->m_nAssetEntryCount);
	Sys_Print(SYS_DLL::RTECH, "______________________________________________________________\n");
	Sys_Print(SYS_DLL::RTECH, "] COMPRESSION_DETAILS ----------------------------------------\n");
	Sys_Print(SYS_DLL::RTECH, "] Size disk: '%lld'\n", rheader->m_nSizeDisk);
	Sys_Print(SYS_DLL::RTECH, "] Size decp: '%lld'\n", rheader->m_nSizeMemory);
	Sys_Print(SYS_DLL::RTECH, "] Ratio    : '%.02f'\n", (rheader->m_nSizeDisk * 100.f) / rheader->m_nSizeMemory);

	if (rheader->m_nMagic != 'kaPR')
	{
		Sys_Print(SYS_DLL::RTECH, "Error: pak file '%s' has invalid magic!\n", pak_name_in.c_str());
		return;
	}
	if ((rheader->m_nFlags[1] & 1) != 1)
	{
		Sys_Print(SYS_DLL::RTECH, "Error: pak file '%s' already decompressed!\n", pak_name_in.c_str());
		return;
	}
	if (rheader->m_nSizeDisk != upak.size())
	{
		Sys_Print(SYS_DLL::RTECH, "Error: pak file '%s' decompressed size '%u' doesn't match expected value '%u'!\n", pak_name_in.c_str(), upak.size(), rheader->m_nSizeMemory);
		return;
	}

	std::int64_t params[18];
	std::uint32_t dsize = g_pRtech->DecompressedSize((std::int64_t)(params), upak.data(), upak.size(), 0, PAK_HEADER_SIZE);

	if (dsize == rheader->m_nSizeDisk)
	{
		Sys_Print(SYS_DLL::RTECH, "Error: calculated size: '%zu' expected: '%zu'!\n", dsize, rheader->m_nSizeMemory);
		return;
	}
	else
	{
		Sys_Print(SYS_DLL::RTECH, "] Calculated size: '%zu'\n", dsize);
	}

	std::vector<std::uint8_t> pakbuf(rheader->m_nSizeMemory, 0);

	params[1] = std::int64_t(pakbuf.data());
	params[3] = -1i64;

	std::uint8_t decomp_result = g_pRtech->Decompress(params, upak.size(), pakbuf.size());
	if (decomp_result != 1)
	{
		Sys_Print(SYS_DLL::RTECH, "Error: decompression failed for '%s' return value: '%u'!\n", pak_name_in.c_str(), +decomp_result);
		return;
	}

	rheader->m_nFlags[1] = 0x0; // Set compressed flag to false for the decompressed pak file
	rheader->m_nSizeDisk = rheader->m_nSizeMemory; // Equal compressed size with decompressed

	std::ofstream out_block(pak_name_out, std::fstream::binary);
	std::ofstream out_header(pak_name_out, std::fstream::binary);

	out_block.write((char*)pakbuf.data(), params[5]);
	out_header.write((char*)rheader, PAK_HEADER_SIZE);

	Sys_Print(SYS_DLL::RTECH, "] Decompressed rpak to: '%s'\n", pak_name_out.c_str());
	Sys_Print(SYS_DLL::RTECH, "--------------------------------------------------------------\n");
}

void NET_TraceNetChan_Callback(CCommand* cmd)
{
	static bool bTraceNetChannel = false;
	if (!bTraceNetChannel)
	{
		g_pCvar->FindVar("net_usesocketsforloopback")->m_iValue = 1;
		Sys_Print(SYS_DLL::ENGINE, "\n");
		Sys_Print(SYS_DLL::ENGINE, "+--------------------------------------------------------+\n");
		Sys_Print(SYS_DLL::ENGINE, "|>>>>>>>>>>>>>| NETCHANNEL TRACE ACTIVATED |<<<<<<<<<<<<<|\n");
		Sys_Print(SYS_DLL::ENGINE, "+--------------------------------------------------------+\n");
		Sys_Print(SYS_DLL::ENGINE, "\n");

		// Begin the detour transaction to hook the the process
		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());

		CNetChan_Trace_Attach();
		// Commit the transaction
		if (DetourTransactionCommit() != NO_ERROR)
		{
			// Failed to hook into the process, terminate
			TerminateProcess(GetCurrentProcess(), 0xBAD0C0DE);
		}
	}
	else
	{
		Sys_Print(SYS_DLL::ENGINE, "\n");
		Sys_Print(SYS_DLL::ENGINE, "+--------------------------------------------------------+\n");
		Sys_Print(SYS_DLL::ENGINE, "|>>>>>>>>>>>>| NETCHANNEL TRACE DEACTIVATED |<<<<<<<<<<<<|\n");
		Sys_Print(SYS_DLL::ENGINE, "+--------------------------------------------------------+\n");
		Sys_Print(SYS_DLL::ENGINE, "\n");

		// Begin the detour transaction to hook the the process
		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());

		CNetChan_Trace_Detach();

		// Commit the transaction
		DetourTransactionCommit();
	}
	bTraceNetChannel = !bTraceNetChannel;
}

void VPK_Decompress_Callback(CCommand* cmd)
{
	std::int32_t argSize = *(std::int32_t*)((std::uintptr_t)cmd + 0x4);

	if (argSize < 2) // Do we atleast have 2 arguments?
	{
		return;
	}

	CCommand& cmdReference = *cmd; // Get reference.
	std::string firstArg = cmdReference[1]; // Get first arg.
	std::string szPathOut = "platform\\vpk";

	std::chrono::milliseconds msStart = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());

	Sys_Print(SYS_DLL::FS, "______________________________________________________________\n");
	Sys_Print(SYS_DLL::FS, "] FS_DECOMPRESS ----------------------------------------------\n");
	Sys_Print(SYS_DLL::FS, "] Processing: '%s'\n", firstArg.c_str());

	vpk_dir_h vpk = g_pPackedStore->GetPackDirFile(firstArg);
	g_pPackedStore->InitLzParams();

	std::thread th([&] { g_pPackedStore->UnpackAll(vpk, szPathOut); });
	th.join();

	std::chrono::milliseconds msEnd = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
	float duration = msEnd.count() - msStart.count();

	Sys_Print(SYS_DLL::FS, "______________________________________________________________\n");
	Sys_Print(SYS_DLL::FS, "] OPERATION_DETAILS ------------------------------------------\n");
	Sys_Print(SYS_DLL::FS, "] Time elapsed: '%.3f' seconds\n", (duration / 1000));
	Sys_Print(SYS_DLL::FS, "] Decompressed vpk to: '%s'\n", szPathOut.c_str());
	Sys_Print(SYS_DLL::FS, "--------------------------------------------------------------\n");
}

void NET_SetKey_Callback(CCommand* cmd)
{
	CCommand& cmdReference = *cmd; // Get reference.
	std::string firstArg = cmdReference[1]; // Get first arg.

	std::int32_t argSize = *(std::int32_t*)((std::uintptr_t)cmd + 0x4);

	if (argSize < 2) // Do we atleast have 2 arguments?
	{
		return;
	}

	HNET_SetKey(firstArg);
}

void NET_GenerateKey_Callback(CCommand* cmd)
{
	HNET_GenerateKey();
}
