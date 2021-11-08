#include "stdafx.h"
#include "hooks.h"

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
	AttachEbisuSDKHooks();
	AttachIConVarHooks();
	AttachConCommandHooks();
	AttachCEngineServerHooks();
	AttachSQVMHooks();
	AttachSysHooks();
	AttachMSGBoxHooks();

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
	AttachIAppSystemHooks();
	DetachEbisuSDKHooks();
	DetachIConVarHooks();
	DetachConCommandHooks();
	DetachCEngineServerHooks();
	DetachSQVMHooks();
	DetachSysHooks();
	DetachMSGBoxHooks();

	///////////////////////////////////////////////////////////////////////////////
	// Commit the transaction
	DetourTransactionCommit();
}
