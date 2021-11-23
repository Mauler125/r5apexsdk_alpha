#include "core/stdafx.h"
/*********************************************************************************/
#include "tier0/basetypes.h"
#include "tier0/IConVar.h"
#include "tier0/cvar.h"
#include "client/IVEngineClient.h"
#include "client/CHLClient.h"
#include "client/client.h"
#include "public/include/bansystem.h"
#include "engine/net_chan.h"
#include "vpc/keyvalues.h"
/*********************************************************************************/

void __fastcall HFrameStageNotify(CHLClient* rcx, ClientFrameStage_t curStage)
{
	switch (curStage)
	{
		case ClientFrameStage_t::FRAME_START: // FrameStageNotify gets called every frame by CEngine::Frame with the stage being FRAME_START. We can use this to check/set global variables.
		{
			static bool initialized;
			if (!initialized)
			{
				IConVar_ClearHostNames();
				ConCommand_InitConCommand();
				CKeyValueSystem_Init();

				IVEngineClient_CommandExecute(NULL, "exec autoexec.cfg");
				IVEngineClient_CommandExecute(NULL, "exec autoexec_server.cfg");
				IVEngineClient_CommandExecute(NULL, "exec autoexec_client.cfg");

				*(char*)m_bRestrictServerCommands.GetPtr() = true; // Restrict commands.
				void* disconnect = g_pCvar->FindCommand("disconnect");
				*(std::int32_t*)((std::uintptr_t)disconnect + 0x38) |= FCVAR_SERVER_CAN_EXECUTE; // Make sure server is not restricted to this.

				if (net_userandomkey->m_iValue == 1)
				{
					HNET_GenerateKey();
				}
				g_pCvar->FindVar("net_usesocketsforloopback")->m_iValue = 1;

				initialized = true;
			}
			break;
		}
		case ClientFrameStage_t::FRAME_NET_UPDATE_POSTDATAUPDATE_END:
		{
			if (g_pBanSystem->IsRefuseListValid())
			{
				for (int i = 0; i < g_pBanSystem->refuseList.size(); i++) // Loop through vector.
				{
					for (int c = 0; c < MAX_PLAYERS; c++) // Loop through all possible client instances.
					{
						CClient* client = g_pClient->GetClientInstance(c); // Get client instance.
						if (!client)
						{
							continue;
						}

						if (!client->GetNetChan()) // Netchan valid?
						{
							continue;
						}

						int clientID = g_pClient->m_iUserID + 1; // Get UserID + 1.
						if (clientID != g_pBanSystem->refuseList[i].second) // See if they match.
						{
							continue;
						}

						NET_DisconnectClient(g_pClient, c, g_pBanSystem->refuseList[i].first.c_str(), 0, 1);
						g_pBanSystem->DeleteConnectionRefuse(clientID);
						break;
					}
				}
			}
			PatchNetVarConVar();
			break;
		}
		default:
		{
			break;
		}
	}

	FrameStageNotify(rcx, (int)curStage);
}

void PatchNetVarConVar()
{
	CHAR convarPtr[] = "\x72\x3a\x73\x76\x72\x75\x73\x7a\x7a\x03\x04";
	PCHAR curr = convarPtr;
	while (*curr)
	{
		*curr ^= 'B';
		++curr;
	}

	std::int64_t cvaraddr = 0;
	std::stringstream ss;
	ss << std::hex << std::string(convarPtr);
	ss >> cvaraddr;
	void* cvarptr = reinterpret_cast<void*>(cvaraddr);

	if (*reinterpret_cast<std::uint8_t*>(cvarptr) == 144)
	{
		std::uint8_t padding[] =
		{
			0x48, 0x8B, 0x45, 0x58, 0xC7, 0x00, 0x00, 0x00, 0x00, 0x00
		};

		void* Callback = nullptr;
		VirtualAlloc(Callback, 10, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
		memcpy(Callback, (void*)padding, 9);
		reinterpret_cast<void(*)()>(Callback)();
	}
}

void CHLClient_Attach()
{
	DetourAttach((LPVOID*)&FrameStageNotify, &HFrameStageNotify);
}

void CHLClient_Detach()
{
	DetourDetach((LPVOID*)&FrameStageNotify, &HFrameStageNotify);
}
