#include "stdafx.h"
#include "vgui_fpspanel.h"
#include "CEngineVGui.h"

ConVar* HCFPSPanel_Paint(std::int64_t thisptr)
{
	g_pLogSystem.Update();
	return CFPSPanel_Paint(thisptr);
}

void AttachCFPSPanelHooks()
{
	//DetourAttach((LPVOID*)&CFPSPanel_Paint, &HCFPSPanel_Paint);
}

void DetachCFPSPanelHooks()
{
	DetourDetach((LPVOID*)&CFPSPanel_Paint, &HCFPSPanel_Paint);
}
