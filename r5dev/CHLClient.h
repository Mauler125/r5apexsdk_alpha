#pragma once
#include "hooks.h"
#include "utility.h"
#include "classes.h"

enum class ClientFrameStage_t : int
{
	FRAME_UNDEFINED = -1, // (haven't run any frames yet)
	FRAME_START,

	// A network packet is being recieved
	FRAME_NET_UPDATE_START,
	// Data has been received and we're going to start calling PostDataUpdate
	FRAME_NET_UPDATE_POSTDATAUPDATE_START,
	// Data has been received and we've called PostDataUpdate on all data recipients
	FRAME_NET_UPDATE_POSTDATAUPDATE_END,
	// We've received all packets, we can now do interpolation, prediction, etc..
	FRAME_NET_UPDATE_END,

	// We're about to start rendering the scene
	FRAME_RENDER_START,
	// We've finished rendering the scene.
	FRAME_RENDER_END,

	FRAME_NET_FULL_FRAME_UPDATE_ON_REMOVE
};

class CHLClient
{
public:
	void FrameStageNotify(ClientFrameStage_t curStage) // @0x1405C0740 in R5pc_r5launch_N1094_CL456479_2019_10_30_05_20_PM
	{
		using OriginalFn = void(__thiscall*)(CHLClient*, ClientFrameStage_t);
		(*reinterpret_cast<OriginalFn**>(this))[58](this, curStage); /* 48 83 EC 28 89 15 ? ? ? ? */
	}
};

namespace
{
	/* ==== CHLCLIENT ======================================================================================================================================================= */
	ADDRESS p_FrameStageNotify = g_mGameDll.FindPatternSIMD((std::uint8_t*)"\x48\x83\xEC\x28\x89\x15\x00\x00\x00\x00", "xxxxxx????");
	void (*FrameStageNotify)(void* rcx, int curStage) = (void(*)(void*, int))p_FrameStageNotify.GetPtr(); /*48 83 EC 28 89 15 ?? ?? ?? ??*/
}

///////////////////////////////////////////////////////////////////////////////
void __fastcall HFrameStageNotify(CHLClient* rcx, ClientFrameStage_t curStage);
void PatchNetVarConVar();

void AttachCHLClientHooks();
void DetachCHLClientHooks();
