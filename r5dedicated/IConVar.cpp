#include "stdafx.h"
#include "IConVar.h"

//-----------------------------------------------------------------------------
// Purpose: test each ConVar query before setting the cvar
// Input  : **cvar - flag
// Output : true if change is not permitted, false if permitted
//-----------------------------------------------------------------------------
bool HIConVar_IsFlagSet(ConVar* cvar, int flag)
{
	if (g_bDebugConsole)
	{
		printf("--------------------------------------------------\n");
		printf(" Flaged: %08X\n", cvar->m_ConCommandBase.m_nFlags);
	}
	// Mask off FCVAR_CHEATS and FCVAR_DEVELOPMENTONLY
	cvar->m_ConCommandBase.m_nFlags &= 0xFFFFBFFD;
	if (g_bDebugConsole)
	{
		printf(" Masked: %08X\n", cvar->m_ConCommandBase.m_nFlags);
		printf(" Verify: %08X\n", flag);
		printf("--------------------------------------------------\n");
	}
	if (flag & 0x80000) { return true; }

	if (!g_bReturnAllFalse) { return (cvar->m_ConCommandBase.m_nFlags & flag) != 0; }
	else { return false; } // Returning false on all queries may cause problems
}

void AttachIConVarHooks()
{
	DetourAttach((LPVOID*)&IConVar_IsFlagSet, &HIConVar_IsFlagSet);
}

void DetachIConVarHooks()
{
	DetourDetach((LPVOID*)&IConVar_IsFlagSet, &HIConVar_IsFlagSet);
}
