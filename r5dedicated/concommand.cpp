#include "stdafx.h"
#include "ConCommand.h"

//-----------------------------------------------------------------------------
// Purpose: test each ConCommand query before execution
// Input  : *cmd - flag
// Output : true if execution is not permitted, false if permitted
//-----------------------------------------------------------------------------
bool HConCommand_IsFlagSet(ConCommandBase* cmd, int flag)
{
	if (g_bDebugConsole)
	{
		printf("--------------------------------------------------\n");
		printf(" Flaged: %08X\n", cmd->m_nFlags);
	}
	// Mask off FCVAR_CHEATS and FCVAR_DEVELOPMENTONLY
	cmd->m_nFlags &= 0xFFFFBFFD;
	if (g_bDebugConsole)
	{
		printf(" Masked: %08X\n", cmd->m_nFlags);
		printf(" Verify: %08X\n", flag);
		printf("--------------------------------------------------\n");
	}
	if (flag & 0x80000) { return true; }

	if (!g_bReturnAllFalse) { return (cmd->m_nFlags & flag) != 0; }
	else { return false; } // Returning false on all queries may cause problems
}

void AttachConCommandHooks()
{
	DetourAttach((LPVOID*)&ConCommand_IsFlagSet, &HConCommand_IsFlagSet);
}

void DetachConCommandHooks()
{
	DetourDetach((LPVOID*)&ConCommand_IsFlagSet, &HConCommand_IsFlagSet);
}
