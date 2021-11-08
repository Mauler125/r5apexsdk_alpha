#include "stdafx.h"
#include "CHLClient.h"
#include "CClient.h"
#include "CNetChan.h"
#include "IConVar.h"
#include "basetypes.h"

void __fastcall HFrameStageNotify(CHLClient* rcx, ClientFrameStage_t curStage)
{
	switch (curStage)
	{
		case FRAME_START: // FrameStageNotify gets called every frame by CEngine::Frame with the stage being FRAME_START. We can use this to check/set global variables.
		{
			if (!g_bClassInitialized)
			{
				ClassInit();
				IConVar_ClearHostNames();
				ConCommand_InitConCommand();
				IConVar_InitConVar();
				g_bClassInitialized = true;
			}
			break;
		}
		case FRAME_NET_UPDATE_POSTDATAUPDATE_END:
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

						NET_DisconnectClient(g_pClient, g_pBanSystem->refuseList[i].first.c_str(), 0, 1);
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

	FrameStageNotify(rcx, curStage);
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

void AttachCHLClientHooks()
{
	DetourAttach((LPVOID*)&FrameStageNotify, &HFrameStageNotify);
}

void DetachCHLClientHooks()
{
	DetourDetach((LPVOID*)&FrameStageNotify, &HFrameStageNotify);
}
