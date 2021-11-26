#include "core/stdafx.h"
#include "tier0/cvar.h"
#include "tier0/IConVar.h"
#include "engine/sys_utils.h"
#include "engine/sys_dll2.h"

//-----------------------------------------------------------------------------
// Purpose: test each ConVar query before setting the cvar
// Input  : **cvar - flag
// Output : true if change is not permitted, false if permitted
//-----------------------------------------------------------------------------
bool HIConVar_IsFlagSet(ConVar* cvar, int flag)
{
	if (cm_debug_cmdquery->m_iValue > 0)
	{
		printf("--------------------------------------------------\n");
		printf(" Flaged: %08X\n", cvar->m_ConCommandBase.m_nFlags);
	}
	// Mask off FCVAR_CHEATS and FCVAR_DEVELOPMENTONLY
	cvar->m_ConCommandBase.m_nFlags &= 0xFFFFBFFD;
	if (cm_debug_cmdquery->m_iValue > 0)
	{
		printf(" Masked: %08X\n", cvar->m_ConCommandBase.m_nFlags);
		printf(" Verify: %08X\n", flag);
		printf("--------------------------------------------------\n");
	}
	if (flag & 0x80000 && cm_return_false_cmdquery_all->m_iValue <= 0)
	{
		return IConVar_IsFlagSet(cvar, flag);
	}
	if (cm_return_false_cmdquery_all->m_iValue > 0)
	{
		// Returning false on all queries may cause problems.
		return false;
	}
	if (cm_return_false_cmdquery_dev_cheat->m_iValue > 0)
	{
		// Return false on every FCVAR_DEVELOPMENTONLY || FCVAR_CHEAT query.
		return (cvar->m_ConCommandBase.m_nFlags & flag) != 0;
	}
	// Default behaviour.
	return IConVar_IsFlagSet(cvar, flag);
}

//-----------------------------------------------------------------------------
// Purpose: register ConVar's
//-----------------------------------------------------------------------------
ConVar* IConVar_RegisterConVar(const char* name, const char* defaultValue, int flags, const char* helpString, bool bMin, float fMin, bool bMax, float fMax, void* callback, void* unk)
{
	ConVar* allocatedConvar = reinterpret_cast<ConVar*>(MemAlloc_Wrapper(0xA0)); // Allocate new memory with StdMemAlloc else we crash.
	memset(allocatedConvar, 0, 0xA0);                                            // Set all to null.
	std::uintptr_t cvarPtr = reinterpret_cast<std::uintptr_t>(allocatedConvar);  // To ptr.

	*(void**)(cvarPtr + 0x40) = g_pIConVarVtable.RCast<void*>(); // 0x40 to ICvar table.
	*(void**)cvarPtr = g_pConVarVtable.RCast<void*>();           // 0x0 to ConVar vtable.

	p_ConVar_Register.RCast<void(*)(ConVar*, const char*, const char*, int, const char*, bool, float, bool, float, void*, void*)>() // Call to create ConVar.
		(allocatedConvar, name, defaultValue, flags, helpString, bMin, fMin, bMax, fMax, callback, unk);

	return allocatedConvar; // Return allocated ConVar.
}

//-----------------------------------------------------------------------------
// Purpose: convar definitions to be registered
//-----------------------------------------------------------------------------
void IConVar_InitConVar()
{
	//-------------------------------------------------------------------------
	// ENGINE DLL                                                             |
	cm_debug_cmdquery                  = IConVar_RegisterConVar("cm_debug_cmdquery", "0", FCVAR_DEVELOPMENTONLY | FCVAR_CHEAT, "Prints the flags of each ConVar/ConCommand query to the console ( !slower! ).", false, 0.f, false, 0.f, nullptr, nullptr);
	cm_return_false_cmdquery_all       = IConVar_RegisterConVar("cm_return_false_cmdquery_all", "0", FCVAR_DEVELOPMENTONLY | FCVAR_CHEAT, "Returns false on every ConVar/ConCommand query ( !warning! ).", false, 0.f, false, 0.f, nullptr, nullptr);
	cm_return_false_cmdquery_dev_cheat = IConVar_RegisterConVar("cm_return_false_cmdquery_dev_cheat", "1", FCVAR_RELEASE, "Returns false on all FCVAR_DEVELOPMENTONLY and FCVAR_CHEAT ConVar/ConCommand queries ( !warning! ).", false, 0.f, false, 0.f, nullptr, nullptr);
	matchmaking_hostname_r5net         = IConVar_RegisterConVar("matchmaking_hostname_r5net", "r5a-comp-sv.herokuapp.com", FCVAR_RELEASE, "Holds the R5Net matchmaking hostname.", false, 0.f, false, 0.f, nullptr, nullptr);
	//-------------------------------------------------------------------------
	// SERVER DLL                                                             |
	sv_showconnecting = IConVar_RegisterConVar("sv_showconnecting", "1", FCVAR_DEVELOPMENTONLY | FCVAR_CHEAT, "Logs information about the connecting client to the console.", false, 0.f, false, 0.f, nullptr, nullptr);
	//-------------------------------------------------------------------------
	// CLIENT DLL                                                             |
	cl_drawconsoleoverlay      = IConVar_RegisterConVar("cl_drawconsoleoverlay", "1", FCVAR_RELEASE, "Draw the console overlay at the top of the screen.", false, 0.f, false, 0.f, nullptr, nullptr);
	cl_consoleoverlay_lines    = IConVar_RegisterConVar("cl_consoleoverlay_lines", "3", FCVAR_RELEASE, "Number of lines of console output to draw.", false, 1.f, false, 50.f, nullptr, nullptr);
	cl_consoleoverlay_offset_x = IConVar_RegisterConVar("cl_consoleoverlay_offset_x", "10", FCVAR_RELEASE, "X offset for console overlay.", false, 1.f, false, 50.f, nullptr, nullptr);
	cl_consoleoverlay_offset_y = IConVar_RegisterConVar("cl_consoleoverlay_offset_y", "10", FCVAR_RELEASE, "Y offset for console overlay.", false, 1.f, false, 50.f, nullptr, nullptr);
	//-------------------------------------------------------------------------
	// FILESYSTEM DLL                                                         |
	fs_warning_level_native = IConVar_RegisterConVar("fs_warning_level_native", "0", FCVAR_DEVELOPMENTONLY | FCVAR_CHEAT, "Set the filesystem warning level ( !slower! ).", false, 0.f, false, 0.f, nullptr, nullptr);
	//-------------------------------------------------------------------------
	// SQUIRREL API                                                           |
	sq_showrsonloading   = IConVar_RegisterConVar("sq_showrsonloading", "1", FCVAR_DEVELOPMENTONLY | FCVAR_CHEAT, "Logs all 'rson' files loaded by the SQVM ( !slower! ).", false, 0.f, false, 0.f, nullptr, nullptr);
	sq_showscriptloading = IConVar_RegisterConVar("sq_showscriptloading", "0", FCVAR_DEVELOPMENTONLY | FCVAR_CHEAT, "Logs all scripts loaded by the SQVM to be pre-compiled ( !slower! ).", false, 0.f, false, 0.f, nullptr, nullptr);
	sq_showvmoutput      = IConVar_RegisterConVar("sq_showvmoutput", "3", FCVAR_DEVELOPMENTONLY | FCVAR_CHEAT, "Prints the VM output to the console. 1 = Log to file. 2 = 1 + log to console. 3 = 1 + 2 + log to overhead console. 4 = only log to overhead console.", false, 0.f, false, 0.f, nullptr, nullptr);
	sq_showvmwarning     = IConVar_RegisterConVar("sq_showvmwarning", "0", FCVAR_DEVELOPMENTONLY | FCVAR_CHEAT, "Prints the VM warning output to the console. 1 = Log to file. 2 = 1 + log to console.", false, 0.f, false, 0.f, nullptr, nullptr);
	//-------------------------------------------------------------------------
	// NETCHANNEL                                                             |
	net_userandomkey = IConVar_RegisterConVar("net_userandomkey", "0", FCVAR_RELEASE, "If set to 1, the netchannel generates and sets a random base64 netkey.", false, 0.f, false, 0.f, nullptr, nullptr);
}

//-----------------------------------------------------------------------------
// Purpose: clear all hostname ConVar's
//-----------------------------------------------------------------------------
void IConVar_ClearHostNames()
{
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
void IConVar_Attach()
{
	DetourAttach((LPVOID*)&IConVar_IsFlagSet, &HIConVar_IsFlagSet);
}

void IConVar_Detach()
{
	DetourDetach((LPVOID*)&IConVar_IsFlagSet, &HIConVar_IsFlagSet);
}

///////////////////////////////////////////////////////////////////////////////
CCVar* g_pCvar = reinterpret_cast<CCVar*>(p_CEngineAPI_Connect.FindPatternSelf("48 8D 0D", ADDRESS::Direction::DOWN, 40).ResolveRelativeAddressSelf(0x3, 0x7).GetPtr());
