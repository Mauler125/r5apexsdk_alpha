#include "stdafx.h"
#include "hooks.h"
#include "IConVar.h"
#include "sys_utils.h"

//-----------------------------------------------------------------------------
// Purpose: test each ConVar query before setting the cvar
// Input  : **cvar - flag
// Output : true if change is not permitted, false if permitted
//-----------------------------------------------------------------------------
bool HIConVar_IsFlagSet(ConVar* cvar, int flag)
{
	if (g_bClassInitialized && g_pCvar->FindVar("cm_debug_cmdquery")->m_iValue > 0)
	{
		printf("--------------------------------------------------\n");
		printf(" Flaged: %08X\n", cvar->m_ConCommandBase.m_nFlags);
	}
	// Mask off FCVAR_CHEATS and FCVAR_DEVELOPMENTONLY
	cvar->m_ConCommandBase.m_nFlags &= 0xFFFFBFFD;
	if (g_bClassInitialized && g_pCvar->FindVar("cm_debug_cmdquery")->m_iValue > 0)
	{
		printf(" Masked: %08X\n", cvar->m_ConCommandBase.m_nFlags);
		printf(" Verify: %08X\n", flag);
		printf("--------------------------------------------------\n");
	}
	if (flag & 0x80000 && g_bClassInitialized && g_pCvar->FindVar("cm_return_false_cmdquery_all")->m_iValue > 0)
	{
		return IConVar_IsFlagSet(cvar, flag);
	}
	if (g_bClassInitialized && g_pCvar->FindVar("cm_return_false_cmdquery_all")->m_iValue > 0)
	{
		// Returning false on all queries may cause problems.
		return false;
	}
	if (g_bClassInitialized && g_pCvar->FindVar("cm_return_false_cmdquery_dev_cheat")->m_iValue > 0)
	{
		// Return false on every FCVAR_DEVELOPMENTONLY || FCVAR_CHEAT query.
		return (cvar->m_ConCommandBase.m_nFlags & flag) != 0;
	}
	// Default behaviour.
	return IConVar_IsFlagSet(cvar, flag);
}

//-----------------------------------------------------------------------------
// Purpose: register convar's
//-----------------------------------------------------------------------------
ConVar* IConVar_RegisterConVar(const char* name, const char* defaultValue, int flags, const char* helpString, bool bMin, float fMin, bool bMax, float fMax, void* callback, void* unk)
{
	static Address ConVarVtable = Address(0x14046FB50).Offset(0x12).ResolveRelativeAddress(); // Get vtable ptr for ConVar table.
	static Address ICvarVtable  = Address(0x14046FB50).Offset(0x29).ResolveRelativeAddress(); // Get vtable ptr for ICvar table.
	static Address CreateConVar = Address(0x140470540);                                       // Get CreateConvar address. TODO: CREATE SIG

	ConVar* allocatedConvar = reinterpret_cast<ConVar*>(MemAlloc_Wrapper(0xA0)); // Allocate new memory with StdMemAlloc else we crash.
	memset(allocatedConvar, 0, 0xA0);                                            // Set all to null.
	std::uintptr_t cvarPtr = reinterpret_cast<std::uintptr_t>(allocatedConvar);  // To ptr.

	*(void**)(cvarPtr + 0x40) = ICvarVtable.RCast<void*>(); // 0x40 to ICvar table.
	*(void**)cvarPtr = ConVarVtable.RCast<void*>();         // 0x0 to ConVar vtable.

	CreateConVar.RCast<void(*)(ConVar*, const char*, const char*, int, const char*, bool, float, bool, float, void*, void*)>() // Call to create ConVar.
		(allocatedConvar, name, defaultValue, flags, helpString, bMin, fMin, bMax, fMax, callback, unk);

	return allocatedConvar; // Return allocated ConVar.
}

//-----------------------------------------------------------------------------
// Purpose: convar definitions to be registered
//-----------------------------------------------------------------------------
void IConVar_InitConVar()
{
	spdlog::debug("Native(E): IConVar::InitConVar();\n");

	//-------------------------------------------------------------------------
	// ENGINE DLL                                                             |
	ConVar* engineDebugCmdQuery = IConVar_RegisterConVar("cm_debug_cmdquery", "0", FCVAR_DEVELOPMENTONLY || FCVAR_CHEAT, "Prints the flags of each ConVar/ConCommand query to the console ( !slower! ).", false, 0.f, false, 0.f, nullptr, nullptr);
	ConVar* engineDebugReturnFalseAll      = IConVar_RegisterConVar("cm_return_false_cmdquery_all", "0", FCVAR_DEVELOPMENTONLY || FCVAR_CHEAT, "Returns false on every ConVar/ConCommand query ( !warning! ).", false, 0.f, false, 0.f, nullptr, nullptr);
	ConVar* engineDebugReturnFalseDevCheat = IConVar_RegisterConVar("cm_return_false_cmdquery_dev_cheat", "1", FCVAR_RELEASE, "Returns false on all FCVAR_DEVELOPMENTONLY and FCVAR_CHEAT ConVar/ConCommand queries ( !warning! ).", false, 0.f, false, 0.f, nullptr, nullptr);
	//-------------------------------------------------------------------------
	// SERVER DLL                                                             |
	ConVar* serverShowConnecting = IConVar_RegisterConVar("sv_showconnecting", "1", FCVAR_DEVELOPMENTONLY || FCVAR_CHEAT, "Logs information about the connecting client to the console.", false, 0.f, false, 0.f, nullptr, nullptr);
	//-------------------------------------------------------------------------
	// CLIENT DLL                                                             |
	ConVar* drawConsoleOverlay  = IConVar_RegisterConVar("cl_drawconsoleoverlay", "0", FCVAR_RELEASE, "Draw the console overlay at the top of the screen.", false, 0.f, false, 0.f, nullptr, nullptr);
	ConVar* consoleOverlayLines = IConVar_RegisterConVar("cl_consoleoverlay_lines", "3", FCVAR_RELEASE, "Number of lines of console output to draw.", false, 1.f, false, 50.f, nullptr, nullptr);
	//-------------------------------------------------------------------------
	// FILESYSTEM API                                                         |
	ConVar* fileSystemWarningLevel = IConVar_RegisterConVar("fs_warning_level_native", "0", FCVAR_DEVELOPMENTONLY || FCVAR_CHEAT, "Set the filesystem warning level ( !slower! ).", false, 0.f, false, 0.f, nullptr, nullptr);
	//-------------------------------------------------------------------------
	// SQUIRREL API                                                           |
	ConVar* squirrelShowRsonLoading   = IConVar_RegisterConVar("sq_showrsonloading", "1", FCVAR_DEVELOPMENTONLY || FCVAR_CHEAT, "Logs all 'rson' files loaded by the SQVM.", false, 0.f, false, 0.f, nullptr, nullptr);
	ConVar* squirrelShowScriptLoading = IConVar_RegisterConVar("sq_showscriptloading", "0", FCVAR_DEVELOPMENTONLY || FCVAR_CHEAT, "Logs all scripts loaded by the SQVM to be pre-compiled.", false, 0.f, false, 0.f, nullptr, nullptr);
	ConVar* squirrelShowVMOutput      = IConVar_RegisterConVar("sq_showvmoutput", "1", FCVAR_DEVELOPMENTONLY || FCVAR_CHEAT, "Prints the VM output to the console.", false, 0.f, false, 0.f, nullptr, nullptr);
	ConVar* squirrelShowVMWarning     = IConVar_RegisterConVar("sq_showvmwarning", "1", FCVAR_DEVELOPMENTONLY || FCVAR_CHEAT, "Prints the VM warning output to the console. 1 = Log to file. 2 = 1 + log to console. 3 = 1 + 2 + log to overhead console.", false, 0.f, false, 0.f, nullptr, nullptr);
}

//-----------------------------------------------------------------------------
// Purpose: clear all hostname convar's
//-----------------------------------------------------------------------------
void IConVar_ClearHostNames()
{
	spdlog::debug("Native(E):Clearing host names\n");
	const char* hostnameArray[] =
	{
		"pin_telemetry_hostname",
		"assetdownloads_hostname",
		"users_hostname",
		"persistence_hostname",
		"speechtotexttoken_hostname",
		"communities_hostname",
		"persistenceDef_hostname",
		"party_hostname",
		"speechtotext_hostname",
		"serverReports_hostname",
		"subscription_hostname",
		"steamlink_hostname",
		"staticfile_hostname",
		"matchmaking_hostname",
		"skill_hostname",
		"publication_hostname",
		"stats_hostname"
	};

	for (int i = 0; i < 17; i++)
	{
		const char* name = hostnameArray[i];
		g_pCvar->FindVar(name)->m_pzsCurrentValue = "0.0.0.0";
	}
}

///////////////////////////////////////////////////////////////////////////////
void AttachIConVarHooks()
{
	DetourAttach((LPVOID*)&IConVar_IsFlagSet, &HIConVar_IsFlagSet);
}

void DetachIConVarHooks()
{
	DetourDetach((LPVOID*)&IConVar_IsFlagSet, &HIConVar_IsFlagSet);
}

///////////////////////////////////////////////////////////////////////////////
CCVar* g_pCvar = nullptr;
