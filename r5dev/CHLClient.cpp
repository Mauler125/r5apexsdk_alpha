#include "stdafx.h"
#include "CHLClient.h"

void __fastcall HFrameStageNotify(CHLClient* rcx, ClientFrameStage_t curStage)
{
	switch (curStage)
	{
		case FRAME_START: // FrameStageNotify gets called every frame by CEngine::Frame with the stage being FRAME_START. We can use this to check/set global variables.
		{
			if (!GameGlobals::IsInitialized)
			{
				GameGlobals::InitGameGlobals();
			}
			break;
		}
		default:
		{
			break;
		}
	}

	FrameStageNotify(rcx, curStage);
}

void AttachCHLClientHooks()
{
	DetourAttach((LPVOID*)&FrameStageNotify, &HFrameStageNotify);
}

void DetachCHLClientHooks()
{
	DetourDetach((LPVOID*)&FrameStageNotify, &HFrameStageNotify);
}