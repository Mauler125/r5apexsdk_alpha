#include "core/stdafx.h"
#include "core/init.h"
#include "common/opcodes.h"
#include "tier0/ConCommand.h"
#include "tier0/IConVar.h"
#include "launcher/IApplication.h"
#include "vpc/basefilesystem.h"
#include "vpc/keyvalues.h"
#include "vphysics/QHull.h"
#ifndef DEDICATED
#include "vgui/CEngineVGui.h"
#include "vgui/vgui_fpspanel.h"
#include "vguimatsurface/MatSystemSurface.h"
#include "client/CHLClient.h"
#endif // !DEDICATED
#include "client/IVEngineClient.h"
#include "server/server.h"
#include "server/IVEngineServer.h"
#include "squirrel/sqapi.h"
#include "squirrel/sqvm.h"
#include "engine/baseclient.h"
#include "engine/host_state.h"
#include "engine/net_chan.h"
#include "engine/sys_dll.h"
#include "engine/sys_utils.h"


/////////////////////////////////////////////////////////////////////////////////////////////////
//
// ██╗███╗   ██╗██╗████████╗██╗ █████╗ ██╗     ██╗███████╗ █████╗ ████████╗██╗ ██████╗ ███╗   ██╗
// ██║████╗  ██║██║╚══██╔══╝██║██╔══██╗██║     ██║╚══███╔╝██╔══██╗╚══██╔══╝██║██╔═══██╗████╗  ██║
// ██║██╔██╗ ██║██║   ██║   ██║███████║██║     ██║  ███╔╝ ███████║   ██║   ██║██║   ██║██╔██╗ ██║
// ██║██║╚██╗██║██║   ██║   ██║██╔══██║██║     ██║ ███╔╝  ██╔══██║   ██║   ██║██║   ██║██║╚██╗██║
// ██║██║ ╚████║██║   ██║   ██║██║  ██║███████╗██║███████╗██║  ██║   ██║   ██║╚██████╔╝██║ ╚████║
// ╚═╝╚═╝  ╚═══╝╚═╝   ╚═╝   ╚═╝╚═╝  ╚═╝╚══════╝╚═╝╚══════╝╚═╝  ╚═╝   ╚═╝   ╚═╝ ╚═════╝ ╚═╝  ╚═══╝
//
/////////////////////////////////////////////////////////////////////////////////////////////////

void Systems_Init()
{
	// Begin the detour transaction to hook the the process
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());

	// Hook functions
	IApplication_Attach();
	CBaseClient_Attach();
	CBaseFileSystem_Attach();
#ifndef DEDICATED
	CEngineVGui_Attach();
	CFPSPanel_Attach();
	CHLClient_Attach();
#endif // !DEDICATED
	CServer_Attach();
	CHostState_Attach();
	CNetChan_Attach();
	ConCommand_Attach();
	IConVar_Attach();
	IVEngineServer_Attach();
	SQAPI_Attach();
	SQVM_Attach();
	SysDll_Attach();
	SysUtils_Attach();

	// Patch instructions
	RuntimePtc_Init();

	// Commit the transaction
	if (DetourTransactionCommit() != NO_ERROR)
	{
		// Failed to hook into the process, terminate
		TerminateProcess(GetCurrentProcess(), 0xBAD0C0DE);
	}

	IConVar_InitConVar();

#ifdef DEDICATED
	Dedicated_Init();
#endif // DEDICATED

}

//////////////////////////////////////////////////////////////////////////
//
// ███████╗██╗  ██╗██╗   ██╗████████╗██████╗  ██████╗ ██╗    ██╗███╗   ██╗
// ██╔════╝██║  ██║██║   ██║╚══██╔══╝██╔══██╗██╔═══██╗██║    ██║████╗  ██║
// ███████╗███████║██║   ██║   ██║   ██║  ██║██║   ██║██║ █╗ ██║██╔██╗ ██║
// ╚════██║██╔══██║██║   ██║   ██║   ██║  ██║██║   ██║██║███╗██║██║╚██╗██║
// ███████║██║  ██║╚██████╔╝   ██║   ██████╔╝╚██████╔╝╚███╔███╔╝██║ ╚████║
// ╚══════╝╚═╝  ╚═╝ ╚═════╝    ╚═╝   ╚═════╝  ╚═════╝  ╚══╝╚══╝ ╚═╝  ╚═══╝
//
//////////////////////////////////////////////////////////////////////////

void Systems_Shutdown()
{
	// Begin the detour transaction to unhook the the process
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());

	// Unhook functions
	IApplication_Detach();
	CBaseClient_Detach();
	CBaseFileSystem_Detach();
#ifndef DEDICATED
	CEngineVGui_Detach();
	CFPSPanel_Detach();
	CHLClient_Detach();
#endif // !DEDICATED
	CServer_Detach();
	CHostState_Detach();
	CNetChan_Detach();
	ConCommand_Detach();
	IConVar_Detach();
	IVEngineServer_Detach();
	SQAPI_Detach();
	SQVM_Detach();
	SysDll_Detach();
	SysUtils_Detach();

	// Commit the transaction
	DetourTransactionCommit();
}

//////////////////////////////////////////////////////////
//
// ██████╗ ███████╗███████╗██╗   ██╗██╗  ████████╗███████╗
// ██╔══██╗██╔════╝██╔════╝██║   ██║██║  ╚══██╔══╝██╔════╝
// ██████╔╝█████╗  ███████╗██║   ██║██║     ██║   ███████╗
// ██╔══██╗██╔══╝  ╚════██║██║   ██║██║     ██║   ╚════██║
// ██║  ██║███████╗███████║╚██████╔╝███████╗██║   ███████║
// ╚═╝  ╚═╝╚══════╝╚══════╝ ╚═════╝ ╚══════╝╚═╝   ╚══════╝
//
//////////////////////////////////////////////////////////

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
#ifndef DEDICATED
	std::cout << "| CEngineVGui::Paint                   : " << std::hex << std::uppercase << p_CEngineVGui_Paint.GetPtr()                   << std::setw(8) << " |" << std::endl;
	std::cout << "+--------------------------------------------------------+" << std::endl;
	std::cout << "| CFPSPannel::Paint                    : " << std::hex << std::uppercase << p_CFPSPanel_Paint.GetPtr()                     << std::setw(8) << " |" << std::endl;
	std::cout << "+--------------------------------------------------------+" << std::endl;
	std::cout << "| CHLClient::FrameStageNotify          : " << std::hex << std::uppercase << p_FrameStageNotify.GetPtr()                    << std::setw(8) << " |" << std::endl;
	std::cout << "+--------------------------------------------------------+" << std::endl;
#endif // !DEDICATED
	std::cout << "| CHostState::FrameUpdate              : " << std::hex << std::uppercase << p_CHostState_FrameUpdate.GetPtr()              << std::setw(8) << " |" << std::endl;
	std::cout << "+--------------------------------------------------------+" << std::endl;
	std::cout << "| KeyValues::FindKey                   : " << std::hex << std::uppercase << p_KeyValues_FindKey.GetPtr()                   << std::setw(8) << " |" << std::endl;
	std::cout << "+--------------------------------------------------------+" << std::endl;
#ifndef DEDICATED
	std::cout << "| CMatSystemSurface::DrawColoredText   : " << std::hex << std::uppercase << p_CMatSystemSurface_DrawColoredText.GetPtr()   << std::setw(8) << " |" << std::endl;
	std::cout << "+--------------------------------------------------------+" << std::endl;
#endif // !DEDICATED
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
	std::cout << "| SQVM_RegisterFunc                    : " << std::hex << std::uppercase << p_SQVM_RegisterFunc.GetPtr()                   << std::setw(8) << " |" << std::endl;
	std::cout << "+--------------------------------------------------------+" << std::endl;
	std::cout << "| Sys_Error_Internal                   : " << std::hex << std::uppercase << p_Sys_Error_Internal.GetPtr()                  << std::setw(8) << " |" << std::endl;
	std::cout << "+--------------------------------------------------------+" << std::endl;
	std::cout << "| Sys_Error                            : " << std::hex << std::uppercase << p_Sys_Error.GetPtr()                           << std::setw(8) << " |" << std::endl;
	std::cout << "| Sys_LoadAsset                        : " << std::hex << std::uppercase << p_Sys_LoadAsset.GetPtr()                       << std::setw(8) << " |" << std::endl;
	std::cout << "| MemAlloc_Wrapper                     : " << std::hex << std::uppercase << p_MemAlloc_Wrapper.GetPtr()                    << std::setw(8) << " |" << std::endl;
	std::cout << "+--------------------------------------------------------+" << std::endl;
	std::cout << "| g_uNetKeyPtr                         : " << std::hex << std::uppercase << g_pNetKey                                   << std::setw(8) << " |" << std::endl;
	std::cout << "+--------------------------------------------------------+" << std::endl;
}
