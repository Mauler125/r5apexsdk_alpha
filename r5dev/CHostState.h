#pragma once
#include "stdafx.h"
#include "mathlib.h"

enum class HostStates_t : int
{
	HS_NEW_GAME        = 0x0,
	HS_LOAD_GAME       = 0x1,
	HS_CHANGE_LEVEL_SP = 0x2,
	HS_CHANGE_LEVEL_MP = 0x3,
	HS_RUN             = 0x4,
	HS_GAME_SHUTDOWN   = 0x5,
	HS_SHUTDOWN        = 0x6,
	HS_RESTART         = 0x7,
};

class CHostState
{
public:
	HostStates_t m_iCurrentState;            //0x0000
	HostStates_t m_iNextState;               //0x0004
	Vector3 m_vecLocation;                   //0x0008
	QAngle m_angLocation;                    //0x0014
	char m_levelName[64];                    //0x0020
	char m_mapGroupName[256];                //0x0060
	char m_landMarkName[256];                //0x0160
	float m_flShortFrameTime;                //0x0260
	bool m_bActiveGame;                      //0x0264
	bool m_bRememberLocation;                //0x0265
	bool m_bBackgroundLevel;                 //0x0266
	bool m_bWaitingForConnection;            //0x0267
	bool m_bSplitScreenConnect;              //0x0268
	bool m_bGameHasShutDownAndFlushedMemory; //0x0269
	bool m_bWorkshopMapDownloadPending;      //0x026A
};

namespace
{
	/* ==== CHOSTSTATE ====================================================================================================================================================== */
	DWORD64 p_CHostState_FrameUpdate = FindPatternV2("r5apex.exe", (const unsigned char*)"\x48\x89\x5C\x24\x08\x48\x89\x6C\x24\x20\xF3\x0F\x11\x54\x24\x18", "xxxxxxxxxxxxxxxx");
	void (*CHostState_FrameUpdate)(void* rcx, void* rdx, float time) = (void(*)(void*, void*, float))p_CHostState_FrameUpdate; /*48 89 5C 24 08 48 89 6C 24 20 F3 0F 11 54 24 18*/
}

///////////////////////////////////////////////////////////////////////////////
void HCHostState_FrameUpdate(void* rcx, void* rdx, float time);

void AttachCHostStateHooks();
void DetachCHostStateHooks();

///////////////////////////////////////////////////////////////////////////////
extern CHostState* g_pHostState;
