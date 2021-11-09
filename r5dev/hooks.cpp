#include "stdafx.h"
#include "hooks.h"
#include "opcptc.h"
#include "IAppSystem.h"
#include "IConVar.h"
#include "ConCommand.h"
#include "IVEngineServer.h"
#include "IVEngineClient.h"
#include "CEngineVGui.h"
#include "CHLClient.h"
#include "CNetChan.h"
#include "EbisuSDK.h"
#include "basefilesystem.h"
#include "QHull.h"
#include "rtech.h"
#include "sqvm.h"
#include "sys_dll.h"
#include "sys_utils.h"
#include "CBaseClient.h"

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
	AttachIVEngineServerHooks();
	AttachCEngineVGuiHooks();
	AttachCHLClientHooks();
	AttachCNetChanHooks();
	AttachEbisuSDKHooks();
	AttachSQVMHooks();
	AttachSysUtilsHooks();
	AttachSysDllHooks();
	AttachCBaseFileSystemHooks();
	AttachCBaseClientHooks();

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
	DetachIVEngineServerHooks();
	DetachCEngineVGuiHooks();
	DetachCHLClientHooks();
	DetachCNetChanHooks();
	DetachEbisuSDKHooks();
	DetachSQVMHooks();
	DetachSysUtilsHooks();
	DetachSysDllHooks();
	DetachCBaseFileSystemHooks();
	DetachCBaseClientHooks();

	///////////////////////////////////////////////////////////////////////////////
	// Commit the transaction
	DetourTransactionCommit();
}

void PrintHAddress() // Test the sigscan results
{
	std::cout << "+--------------------------------------------------------+" << std::endl;
	std::cout << "| IAppSystem::Main                     : " << std::hex << std::uppercase << p_IAppSystem_Main                     << std::setw(8) << " |" << std::endl;
	std::cout << "| IAppSystem::Create                   : " << std::hex << std::uppercase << p_IAppSystem_Create                   << std::setw(8) << " |" << std::endl;
	std::cout << "+--------------------------------------------------------+" << std::endl;
	std::cout << "| IVEngineServer::PersistenceAvailable : " << std::hex << std::uppercase << p_IVEngineServer_PersistenceAvailable << std::setw(8) << " |" << std::endl;
	std::cout << "+--------------------------------------------------------+" << std::endl;
	std::cout << "| IVEngineClient::CommandExecute       : " << std::hex << std::uppercase << p_IVEngineClient_CommandExecute       << std::setw(8) << " |" << std::endl;
	std::cout << "+--------------------------------------------------------+" << std::endl;
	std::cout << "| CHLClient::FrameStageNotify          : " << std::hex << std::uppercase << p_FrameStageNotify                    << std::setw(8) << " |" << std::endl;
	std::cout << "+--------------------------------------------------------+" << std::endl;
	std::cout << "| ConVar::IsFlagSet                    : " << std::hex << std::uppercase << p_IConVar_IsFlagSet                   << std::setw(8) << " |" << std::endl;
	std::cout << "+--------------------------------------------------------+" << std::endl;
	std::cout << "| ConCommand::IsFlagSet                : " << std::hex << std::uppercase << p_ConCommand_IsFlagSet                << std::setw(8) << " |" << std::endl;
	std::cout << "+--------------------------------------------------------+" << std::endl;
	std::cout << "| SQVM_PrintFunc                       : " << std::hex << std::uppercase << p_SQVM_PrintFunc                      << std::setw(8) << " |" << std::endl;
	std::cout << "| SQVM_LoadScript                      : " << std::hex << std::uppercase << p_SQVM_LoadScript                     << std::setw(8) << " |" << std::endl;
	std::cout << "| SQVM_LoadRson                        : " << std::hex << std::uppercase << p_SQVM_LoadRson                       << std::setw(8) << " |" << std::endl;
	std::cout << "+--------------------------------------------------------+" << std::endl;
	std::cout << "| NET_ReceiveDatagram                  : " << std::hex << std::uppercase << p_NET_ReceiveDatagram                 << std::setw(8) << " |" << std::endl;
	std::cout << "| NET_SendDatagram                     : " << std::hex << std::uppercase << p_NET_SendDatagram                    << std::setw(8) << " |" << std::endl;
	std::cout << "| NET_PrintFunc                        : " << std::hex << std::uppercase << p_NET_PrintFunc                       << std::setw(8) << " |" << std::endl;
	std::cout << "+--------------------------------------------------------+" << std::endl;
	std::cout << "| CBaseFileSystem::Warning             : " << std::hex << std::uppercase << p_CBaseFileSystem_Warning             << std::setw(8) << " |" << std::endl;
	std::cout << "+--------------------------------------------------------+" << std::endl;
	std::cout << "| QHull_PrintError                     : " << std::hex << std::uppercase << p_QHull_PrintError                    << std::setw(8) << " |" << std::endl;
	std::cout << "| QHull_PrintDebug                     : " << std::hex << std::uppercase << p_QHull_PrintDebug                    << std::setw(8) << " |" << std::endl;
	std::cout << "+--------------------------------------------------------+" << std::endl;
	std::cout << "| Sys_Error_Internal                   : " << std::hex << std::uppercase << p_Sys_Error_Internal                  << std::setw(8) << " |" << std::endl;
	std::cout << "| Sys_Error                            : " << std::hex << std::uppercase << p_Sys_Error                           << std::setw(8) << " |" << std::endl;
	std::cout << "+--------------------------------------------------------+" << std::endl;

	// TODO implement error handling when sigscan fails or result is 0
}
