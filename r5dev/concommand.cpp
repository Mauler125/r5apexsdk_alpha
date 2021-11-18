#include "stdafx.h"
#include "hooks.h"
#include "ConCommand.h"
#include "CClient.h"
#include "IConVar.h"
#include "ConCommandCallback.h"
#include "sys_utils.h"

//-----------------------------------------------------------------------------
// Purpose: test each ConCommand query before execution
// Input  : *cmd - flag
// Output : true if execution is not permitted, false if permitted
//-----------------------------------------------------------------------------
bool HConCommand_IsFlagSet(ConCommandBase* cmd, int flag)
{
	if (g_cm_debug_cmdquery > 0)
	{
		printf("--------------------------------------------------\n");
		printf(" Flaged: %08X\n", cmd->m_nFlags);
	}
	// Mask off FCVAR_CHEATS and FCVAR_DEVELOPMENTONLY
	cmd->m_nFlags &= 0xFFFFBFFD;
	if (g_cm_debug_cmdquery > 0)
	{
		printf(" Masked: %08X\n", cmd->m_nFlags);
		printf(" Verify: %08X\n", flag);
		printf("--------------------------------------------------\n");
	}
	if (flag & 0x80000 && g_cm_return_false_cmdquery_all <= 0)
	{
		return ConCommand_IsFlagSet(cmd, flag);
	}
	if (g_cm_return_false_cmdquery_all > 0)
	{
		// Returning false on all queries may cause problems.
		return false;
	}
	if (g_cm_return_false_cmdquery_dev_cheat > 0)
	{
		// Return false on every FCVAR_DEVELOPMENTONLY || FCVAR_CHEAT query.
		return (cmd->m_nFlags & flag) != 0;
	}
	// Default behaviour.
	return ConCommand_IsFlagSet(cmd, flag);
}

//-----------------------------------------------------------------------------
// Purpose: register concommands
//-----------------------------------------------------------------------------
void* ConCommand_RegisterCommand(const char* name, const char* helpString, int flags, void* callback, void* callbackAfterExecution)
{
	static ADDRESS ConCommandVtable   = ADDRESS(0x14136BD70);
	static ADDRESS NullSub            = ADDRESS(0x1401B3280);
	static ADDRESS CallbackCompletion = ADDRESS(0x1401E3990);
	static ADDRESS RegisterConCommand = ADDRESS(0x14046F470);

	void* command = reinterpret_cast<void*>(MemAlloc_Wrapper(0x68));       // Allocate new memory with StdMemAlloc else we crash.
	memset(command, 0, 0x68);                                              // Set all to null.
	std::uintptr_t commandPtr = reinterpret_cast<std::uintptr_t>(command); // To ptr.

	*(void**)commandPtr = ConCommandVtable.RCast<void*>(); // 0x0 to ConCommand vtable.
	*(const char**)(commandPtr + 0x18) = name;             // 0x18 to ConCommand Name.
	*(const char**)(commandPtr + 0x20) = helpString;       // 0x20 to ConCommand help string.
	*(std::int32_t*)(commandPtr + 0x38) = flags;           // 0x38 to ConCommand Flags.
	*(void**)(commandPtr + 0x40) = NullSub.RCast<void*>(); // 0x40 Nullsub since every concommand has it.
	*(void**)(commandPtr + 0x50) = callback;               // 0x50 has function callback.
	*(std::int32_t*)(commandPtr + 0x60) = 2;               // 0x60 Set to use callback and newcommand callback.

	if (callbackAfterExecution) // callback after execution desired?
	{
		*(void**)(commandPtr + 0x58) = callbackAfterExecution; // 0x58 to our callback after execution.
	}
	else
	{
		*(void**)(commandPtr + 0x58) = CallbackCompletion.RCast<void*>(); // 0x58 nullsub.
	}

	RegisterConCommand.RCast<void(*)(void*)>()((void*)commandPtr); // Register command in ConVarAccessor.

	return command;
}

//-----------------------------------------------------------------------------
// Purpose: concommands definitions to be registered
//-----------------------------------------------------------------------------
void ConCommand_InitConCommand()
{
	//-------------------------------------------------------------------------
	// CLIENT DLL                                                             |
	void* gameConsoleCommand   = ConCommand_RegisterCommand("cl_showconsole", "Opens the game console.", 0, CGameConsole_Callback, nullptr);
	void* serverBrowserCommand = ConCommand_RegisterCommand("cl_showbrowser", "Opens the server browser.", 0, CCompanion_Callback, nullptr);
	//-------------------------------------------------------------------------
	// SERVER DLL                                                             |
	void* kickCommand          = ConCommand_RegisterCommand("sv_kick", "Kick a client from the server by name. | Usage: kick <name>.", 0, Kick_Callback, nullptr);
	void* kickIDCommand        = ConCommand_RegisterCommand("sv_kickid", "Kick a client from the server by UserID or OriginID | Usage: kickid <OriginID>/<UserID>.", 0, KickID_Callback, nullptr);
	void* banCommand           = ConCommand_RegisterCommand("sv_ban", "Bans a client from the server by name. | Usage: ban <name>.", 0, Ban_Callback, nullptr);
	void* banIDCommand         = ConCommand_RegisterCommand("sv_banid", "Bans a client from the server by OriginID, UserID or IPAddress | Usage: banid <OriginID>/<IPAddress>/<UserID>.", 0, BanID_Callback, nullptr);
	void* unbanIDCommand       = ConCommand_RegisterCommand("sv_unban", "Unbans a client from the Server by IPAddress or OriginID | Usage: unban <OriginID>/<IPAddress>.", 0, Unban_Callback, nullptr);
	void* reloadBanListCommand = ConCommand_RegisterCommand("sv_reloadbanlist", "Reloads the ban list from the disk.", 0, ReloadBanList_Callback, nullptr);
	//-------------------------------------------------------------------------
	// RTECH API                                                              |
	void* rTechGenerateGuidCommand = ConCommand_RegisterCommand("rtech_toguid", "Calculates the GUID from input data.", 0, RTech_GenerateGUID_Callback, nullptr);
	void* rTechDecompressCommand   = ConCommand_RegisterCommand("rtech_decompress", "Decompresses user specified 'RPak' file.", 0, RTech_Decompress_Callback, nullptr);
	//-------------------------------------------------------------------------
	// NETCHANNEL                                                             |
	void* netTraceCommand  = ConCommand_RegisterCommand("net_toggletrace", "Logs the sending and receiving datagram to a file on the disk.", 0, NET_TraceNetChan_Callback, nullptr);
	void* netSetKeyCommand = ConCommand_RegisterCommand("net_setkey", "Sets user specified base64 net key.", 0, NET_SetKey_Callback, nullptr);
	void* netGenKeyCommand = ConCommand_RegisterCommand("net_generatekey", "Generates and sets a random base64 net key.", 0, NET_GenerateKey_Callback, nullptr);
}

void ConCommand_Attach()
{
	DetourAttach((LPVOID*)&ConCommand_IsFlagSet, &HConCommand_IsFlagSet);
}

void ConCommand_Detach()
{
	DetourDetach((LPVOID*)&ConCommand_IsFlagSet, &HConCommand_IsFlagSet);
}
