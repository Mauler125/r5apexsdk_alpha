#include "stdafx.h"
#include "hooks.h"
#include "opcptc.h"

//#################################################################################
// MANAGEMENT
//#################################################################################

void InstallHooks()
{
	///////////////////////////////////////////////////////////////////////////////
	// Begin the detour transaction, to hook the the process
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());

	///////////////////////////////////////////////////////////////////////////////
	// Hook functions
	AttachIAppSystemHooks();
	AttachIConVarHooks();
	AttachConCommandHooks();
	AttachCEngineServerHooks();
	AttachCHLClientHooks();
	AttachCNetChanHooks();
	AttachEbisuSDKHooks();
	AttachSQVMHooks();
	//AttachSysHooks();
	AttachMSGBoxHooks();

	///////////////////////////////////////////////////////////////////////////////
	// Patch instructions
	InstallOpcodes();

	///////////////////////////////////////////////////////////////////////////////
	// Commit the transaction
	if (DetourTransactionCommit() != NO_ERROR)
	{
		// Failed to hook into the process, terminate
		TerminateProcess(GetCurrentProcess(), 0xBAD0C0DE);
	}

	InstallOpcodes();
}

void RemoveHooks()
{
	///////////////////////////////////////////////////////////////////////////////
	// Begin the detour transaction, to unhook the the process
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());

	///////////////////////////////////////////////////////////////////////////////
	// Unhook functions
	DetachIAppSystemHooks();
	DetachIConVarHooks();
	DetachConCommandHooks();
	DetachCEngineServerHooks();
	DetachCHLClientHooks();
	DetachCNetChanHooks();
	DetachEbisuSDKHooks();
	DetachSQVMHooks();
	//DetachSysHooks();
	DetachMSGBoxHooks();

	///////////////////////////////////////////////////////////////////////////////
	// Commit the transaction
	DetourTransactionCommit();
}

//#################################################################################
// TOGGLES
//#################################################################################

void ToggleDevCommands()
{
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());

	if (!g_bToggledDevFlags)
	{
		AttachIConVarHooks();
		AttachConCommandHooks();
		printf("\n");
		printf("+--------------------------------------------------------+\n");
		printf("|>>>>>>>>>>>>>| DEVONLY COMMANDS ACTIVATED |<<<<<<<<<<<<<|\n");
		printf("+--------------------------------------------------------+\n");
		printf("\n");

	}
	else
	{
		DetachIConVarHooks();
		DetachConCommandHooks();
		printf("\n");
		printf("+--------------------------------------------------------+\n");
		printf("|>>>>>>>>>>>>| DEVONLY COMMANDS DEACTIVATED |<<<<<<<<<<<<|\n");
		printf("+--------------------------------------------------------+\n");
		printf("\n");
	}

	if (DetourTransactionCommit() != NO_ERROR)
	{
		TerminateProcess(GetCurrentProcess(), 0xBAD0C0DE);
	}

	g_bToggledDevFlags = !g_bToggledDevFlags;
}

void ToggleNetTrace()
{
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());

	if (!g_bToggledNetTrace)
	{
		AttachCNetChanTraceHooks();
		printf("\n");
		printf("+--------------------------------------------------------+\n");
		printf("|>>>>>>>>>>>>>| NETCHANNEL TRACE ACTIVATED |<<<<<<<<<<<<<|\n");
		printf("+--------------------------------------------------------+\n");
		printf("\n");
	}
	else
	{
		DetachCNetChanTraceHooks();
		printf("\n");
		printf("+--------------------------------------------------------+\n");
		printf("|>>>>>>>>>>>>| NETCHANNEL TRACE DEACTIVATED |<<<<<<<<<<<<|\n");
		printf("+--------------------------------------------------------+\n");
		printf("\n");
	}

	if (DetourTransactionCommit() != NO_ERROR)
	{
		TerminateProcess(GetCurrentProcess(), 0xBAD0C0DE);
	}

	g_bToggledNetTrace = !g_bToggledNetTrace;
}
