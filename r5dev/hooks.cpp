#include "stdafx.h"
#include "hooks.h"
#include "opcptc.h"
#include "IAppSystem.h"
#include "CBaseClient.h"
#include "basefilesystem.h"
#include "CEngineVGui.h"
#include "CHLClient.h"
#include "CHostState.h"
#include "CKeyValuesSystem.h"
#include "CMatSystemSurface.h"
#include "CNetChan.h"
#include "ConCommand.h"
#include "IConVar.h"
#include "IVEngineServer.h"
#include "IVEngineClient.h"
#include "QHull.h"
#include "sqapi.h"
#include "sqvm.h"
#include "sys_dll.h"
#include "sys_utils.h"


//#################################################################################
// MANAGEMENT
//#################################################################################

void InstallHooks()
{
	///////////////////////////////////////////////////////////////////////////////
	// Begin the detour transaction to hook the the process
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());

	///////////////////////////////////////////////////////////////////////////////
	// Hook functions
	AttachIAppSystemHooks();
	AttachCBaseClientHooks();
	AttachCBaseFileSystemHooks();
	AttachCEngineVGuiHooks();
	AttachCHLClientHooks();
	AttachCHostStateHooks();
	AttachCNetChanHooks();
	AttachConCommandHooks();
	AttachIConVarHooks();
	AttachIVEngineServerHooks();
	AttachSQAPIHooks();
	AttachSQVMHooks();
	AttachSysDllHooks();
	AttachSysUtilsHooks();

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
	// Begin the detour transaction to unhook the the process
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());

	///////////////////////////////////////////////////////////////////////////////
	// Unhook functions
	DetachIAppSystemHooks();
	DetachCBaseClientHooks();
	DetachCBaseFileSystemHooks();
	DetachCEngineVGuiHooks();
	DetachCHLClientHooks();
	DetachCHostStateHooks();
	DetachCNetChanHooks();
	DetachConCommandHooks();
	DetachIConVarHooks();
	DetachIVEngineServerHooks();
	DetachSQAPIHooks();
	DetachSQVMHooks();
	DetachSysDllHooks();
	DetachSysUtilsHooks();

	///////////////////////////////////////////////////////////////////////////////
	// Commit the transaction
	DetourTransactionCommit();
}

void PrintHAddress() // Test the sigscan results
{
	std::cout << "+--------------------------------------------------------+" << std::endl;
	std::cout << "| IAppSystem::Main                     : " << std::hex << std::uppercase << p_IAppSystem_Main.GetPtr()                     << std::setw(8) << " |" << std::endl;
	std::cout << "| IAppSystem::Create                   : " << std::hex << std::uppercase << p_IAppSystem_Create.GetPtr()                   << std::setw(8) << " |" << std::endl;
	std::cout << "+--------------------------------------------------------+" << std::endl;
	std::cout << "| CBaseClient::Clear                   : " << std::hex << std::uppercase << p_CBaseClient_Clear.GetPtr()                   << std::setw(8) << " |" << std::endl;
	std::cout << "+--------------------------------------------------------+" << std::endl;
	std::cout << "| CBaseFileSystem::Warning             : " << std::hex << std::uppercase << p_CBaseFileSystem_Warning.GetPtr()             << std::setw(8) << " |" << std::endl;
	std::cout << "+--------------------------------------------------------+" << std::endl;
	std::cout << "| CEngineVGui::Paint                   : " << std::hex << std::uppercase << p_CEngineVGui_Paint.GetPtr()                   << std::setw(8) << " |" << std::endl;
	std::cout << "+--------------------------------------------------------+" << std::endl;
	std::cout << "| CHLClient::FrameStageNotify          : " << std::hex << std::uppercase << p_FrameStageNotify.GetPtr()                    << std::setw(8) << " |" << std::endl;
	std::cout << "+--------------------------------------------------------+" << std::endl;
	std::cout << "| CHostState::FrameUpdate              : " << std::hex << std::uppercase << p_CHostState_FrameUpdate.GetPtr()              << std::setw(8) << " |" << std::endl;
	std::cout << "+--------------------------------------------------------+" << std::endl;
	std::cout << "| KeyValues::FindKey                   : " << std::hex << std::uppercase << p_KeyValues_FindKey.GetPtr()                   << std::setw(8) << " |" << std::endl;
	std::cout << "+--------------------------------------------------------+" << std::endl;
	std::cout << "| CMatSystemSurface::DrawColoredText   : " << std::hex << std::uppercase << p_CMatSystemSurface_DrawColoredText.GetPtr()   << std::setw(8) << " |" << std::endl;
	std::cout << "+--------------------------------------------------------+" << std::endl;
	std::cout << "| NET_Init                             : " << std::hex << std::uppercase << p_NET_Init.GetPtr()                            << std::setw(8) << " |" << std::endl;
	std::cout << "| NET_Shutdown                         : " << std::hex << std::uppercase << p_NET_Shutdown.GetPtr()                        << std::setw(8) << " |" << std::endl;
	std::cout << "| NET_SetKey                           : " << std::hex << std::uppercase << p_NET_SetKey.GetPtr()                          << std::setw(8) << " |" << std::endl;
	std::cout << "| NET_ReceiveDatagram                  : " << std::hex << std::uppercase << p_NET_ReceiveDatagram.GetPtr()                 << std::setw(8) << " |" << std::endl;
	std::cout << "| NET_SendDatagram                     : " << std::hex << std::uppercase << p_NET_SendDatagram.GetPtr()                    << std::setw(8) << " |" << std::endl;
	std::cout << "| NET_PrintFunc                        : " << std::hex << std::uppercase << p_NET_PrintFunc.GetPtr()                       << std::setw(8) << " |" << std::endl;
	std::cout << "+--------------------------------------------------------+" << std::endl;
	std::cout << "| ConCommand::IsFlagSet                : " << std::hex << std::uppercase << p_ConCommand_IsFlagSet.GetPtr()                << std::setw(8) << " |" << std::endl;
	std::cout << "+--------------------------------------------------------+" << std::endl;
	std::cout << "| ConVar::IsFlagSet                    : " << std::hex << std::uppercase << p_IConVar_IsFlagSet.GetPtr()                   << std::setw(8) << " |" << std::endl;
	std::cout << "+--------------------------------------------------------+" << std::endl;
	std::cout << "| IVEngineServer::PersistenceAvailable : " << std::hex << std::uppercase << p_IVEngineServer_PersistenceAvailable.GetPtr() << std::setw(8) << " |" << std::endl;
	std::cout << "+--------------------------------------------------------+" << std::endl;
	std::cout << "| IVEngineClient::CommandExecute       : " << std::hex << std::uppercase << p_IVEngineClient_CommandExecute.GetPtr()       << std::setw(8) << " |" << std::endl;
	std::cout << "+--------------------------------------------------------+" << std::endl;
	std::cout << "| QHull_PrintError                     : " << std::hex << std::uppercase << p_QHull_PrintError.GetPtr()                    << std::setw(8) << " |" << std::endl;
	std::cout << "| QHull_PrintDebug                     : " << std::hex << std::uppercase << p_QHull_PrintDebug.GetPtr()                    << std::setw(8) << " |" << std::endl;
	std::cout << "+--------------------------------------------------------+" << std::endl;
	std::cout << "| sq_pushbool                          : " << std::hex << std::uppercase << p_sq_pushbool.GetPtr()                         << std::setw(8) << " |" << std::endl;
	std::cout << "| sq_pushstring                        : " << std::hex << std::uppercase << p_sq_pushstring.GetPtr()                       << std::setw(8) << " |" << std::endl;
	std::cout << "| sq_pushinteger                       : " << std::hex << std::uppercase << p_sq_pushinteger.GetPtr()                      << std::setw(8) << " |" << std::endl;
	std::cout << "| sq_newarray                          : " << std::hex << std::uppercase << p_sq_newarray.GetPtr()                         << std::setw(8) << " |" << std::endl;
	std::cout << "| sq_arrayappend                       : " << std::hex << std::uppercase << p_sq_arrayappend.GetPtr()                      << std::setw(8) << " |" << std::endl;
	std::cout << "| sq_newtable                          : " << std::hex << std::uppercase << p_sq_newtable.GetPtr()                         << std::setw(8) << " |" << std::endl;
	std::cout << "| sq_newslot                           : " << std::hex << std::uppercase << p_sq_newslot.GetPtr()                          << std::setw(8) << " |" << std::endl;
	std::cout << "+--------------------------------------------------------+" << std::endl;
	std::cout << "| SQVM_PrintFunc                       : " << std::hex << std::uppercase << p_SQVM_PrintFunc.GetPtr()                      << std::setw(8) << " |" << std::endl;
	std::cout << "| SQVM_LoadScript                      : " << std::hex << std::uppercase << p_SQVM_LoadScript.GetPtr()                     << std::setw(8) << " |" << std::endl;
	std::cout << "| SQVM_LoadRson                        : " << std::hex << std::uppercase << p_SQVM_LoadRson.GetPtr()                       << std::setw(8) << " |" << std::endl;
	std::cout << "+--------------------------------------------------------+" << std::endl;
	std::cout << "| Sys_Error_Internal                   : " << std::hex << std::uppercase << p_Sys_Error_Internal.GetPtr()                  << std::setw(8) << " |" << std::endl;
	std::cout << "+--------------------------------------------------------+" << std::endl;
	std::cout << "| Sys_Error                            : " << std::hex << std::uppercase << p_Sys_Error.GetPtr()                           << std::setw(8) << " |" << std::endl;
	std::cout << "| Sys_LoadAsset                        : " << std::hex << std::uppercase << p_Sys_LoadAsset.GetPtr()                       << std::setw(8) << " |" << std::endl;
	std::cout << "| MemAlloc_Wrapper                     : " << std::hex << std::uppercase << p_MemAlloc_Wrapper.GetPtr()                    << std::setw(8) << " |" << std::endl;
	std::cout << "+--------------------------------------------------------+" << std::endl;
}
