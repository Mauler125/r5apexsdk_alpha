#include <string>
#include <sstream>
#include <iostream>

#include <Windows.h>
#include <detours.h>

#include "id3dx.h"
#include "hooks.h"
#include "opcptc.h"
#include "console.h"
#include "patterns.h"

//#############################################################################
// INITIALIZATION
//#############################################################################

void SetupConsole()
{
	///////////////////////////////////////////////////////////////////////////
	// Create the console window
	if (AllocConsole() == FALSE)
	{
		OutputDebugString("Failed to create console window!\n");
		return;
	}

	///////////////////////////////////////////////////////////////////////////
	// Set the window title
	FILE* sBuildTxt;
	CHAR sBuildBuf[1024] = { 0 };
#ifdef NDEBUG
	fopen_s(&sBuildTxt, "build.txt", "r");
#elif _DEBUG
	fopen_s(&sBuildTxt, "..\\build.txt", "r");
#endif

	if (sBuildTxt)
	{
		while (fgets(sBuildBuf, sizeof(sBuildBuf), sBuildTxt) != NULL)
		{
			fclose(sBuildTxt);
		}
	}
	SetConsoleTitle(sBuildBuf);

	///////////////////////////////////////////////////////////////////////////
	// Open input/output streams
	FILE* fDummy;
	freopen_s(&fDummy, "CONIN$", "r", stdin);
	freopen_s(&fDummy, "CONOUT$", "w", stdout);
	freopen_s(&fDummy, "CONOUT$", "w", stderr);

	///////////////////////////////////////////////////////////////////////////
	// Create a worker thread to process console commands
	DWORD threadId0;
	DWORD __stdcall ProcessConsoleWorker(LPVOID);
	HANDLE hThread0 = CreateThread(NULL, 0, ProcessConsoleWorker, NULL, 0, &threadId0);
	
	if (hThread0)
	{
		printf("THREAD ID: %ld\n\n", threadId0);
		CloseHandle(hThread0);
	}
}

//#############################################################################
// HOOKS
//#############################################################################

bool HConVar_IsFlagSet(int** ppnCvar, int nFlag)
{
	int real_flags = *(*(ppnCvar + (72 / (sizeof(void*)))) + (56 / sizeof(int)));
	if (g_bDebugConsole)
	{
		printf("--------------------------------------------------\n");
		printf(" Flaged: %08X\n", real_flags);
	}
	///////////////////////////////////////////////////////////////////////////
	// Mask off FCVAR_CHEATS and FCVAR_DEVELOPMENTONLY
	real_flags &= 0xFFFFBFFD;
	if (g_bDebugConsole)
	{
		printf(" Masked: %08X\n", real_flags);
		printf(" Verify: %08X\n", nFlag);
		printf("--------------------------------------------------\n");
	}
	if (nFlag & 0x80000) { return true; }

	if (!g_bReturnAllFalse) { return (real_flags & nFlag) != 0; }
	else { return false; }
}

bool HConCommand_IsFlagSet(int* pnCmd, int nFlag)
{
	int real_flags = *((pnCmd + (56 / sizeof(int))));
	if (g_bDebugConsole)
	{
		printf("--------------------------------------------------\n");
		printf(" Flaged: %08X\n", real_flags);
	}
	///////////////////////////////////////////////////////////////////////////
	// Mask off FCVAR_CHEATS and FCVAR_DEVELOPMENTONLY
	real_flags &= 0xFFFFBFFD;
	if (g_bDebugConsole)
	{
		printf(" Masked: %08X\n", real_flags);
		printf(" Verify: %08X\n", nFlag);
		printf("--------------------------------------------------\n");
	}
	if (nFlag & 0x80000) { return true; }

	if (!g_bReturnAllFalse) { return (real_flags & nFlag) != 0; }
	else { return false; }
}

//#############################################################################
// WORKER
//#############################################################################

DWORD __stdcall ProcessConsoleWorker(LPVOID)
{
	// Loop forever
	while (true)
	{
		std::string sCommand;

		///////////////////////////////////////////////////////////////////////
		// Get the user input on the debug console
		printf(">");
		std::getline(std::cin, sCommand);

		///////////////////////////////////////////////////////////////////////
		// Engine toggles
		if (sCommand == "toggle net") { ToggleNetHooks(); CVEngineClient_CommandExecute(NULL, "exec autoexec_dev"); continue; }
		if (sCommand == "toggle dev") { ToggleDevCommands(); continue; }
		if (sCommand == "toggle rep") { g_bReturnAllFalse = !g_bReturnAllFalse; continue; }
		if (sCommand == "toggle opc") { ToggleOpcodes(); }
		///////////////////////////////////////////////////////////////////////
		// Debug toggles
		if (sCommand == "pattern test") { PrintHAddress(); PrintOAddress(); continue; }
		if (sCommand == "directx test") { PrintDXAddress(); continue; }
		if (sCommand == "console test") { g_bDebugConsole = !g_bDebugConsole; continue; }
		if (sCommand == "loading test") { g_bDebugLoading = !g_bDebugLoading; continue; }
		///////////////////////////////////////////////////////////////////////
		// Exec toggles
		if (sCommand == "1") { InstallGlobals(); CVEngineClient_CommandExecute(NULL, "exec autoexec_dev"); continue; }
		if (sCommand == "2") { InstallGlobals(); CVEngineClient_CommandExecute(NULL, "exec connect5_dev"); continue; }
		///////////////////////////////////////////////////////////////////////
		// Opcode toggles
		if (sCommand == "3") { InstallGlobals(); continue; }
		///////////////////////////////////////////////////////////////////////
		// Execute the command in the r5 SQVM
		CVEngineClient_CommandExecute(NULL, sCommand.c_str());
		sCommand.clear();

		///////////////////////////////////////////////////////////////////////
		// Sleep and loop
		Sleep(50);
	}

	return 0;
}

//#############################################################################
// MANAGEMENT
//#############################################################################

void InstallCNHooks()
{
	///////////////////////////////////////////////////////////////////////////
	// Begin the detour transaction, to unhook the the process
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());

	///////////////////////////////////////////////////////////////////////////
	// Hook Console functions
	DetourAttach((LPVOID*)&ConVar_IsFlagSet, &HConVar_IsFlagSet);
	DetourAttach((LPVOID*)&ConCommand_IsFlagSet, &HConCommand_IsFlagSet);

	///////////////////////////////////////////////////////////////////////////
	// Commit the transaction
	if (DetourTransactionCommit() != NO_ERROR)
	{
		// Failed to hook into the process, terminate
		TerminateProcess(GetCurrentProcess(), 0xBAD0C0DE);
	}
}

void RemoveCNHooks()
{
	///////////////////////////////////////////////////////////////////////////
	// Begin the detour transaction, to unhook the the process
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());

	///////////////////////////////////////////////////////////////////////////
	// Unhook Console functions
	DetourDetach((LPVOID*)&ConVar_IsFlagSet, &HConVar_IsFlagSet);
	DetourDetach((LPVOID*)&ConCommand_IsFlagSet, &HConCommand_IsFlagSet);

	///////////////////////////////////////////////////////////////////////////
	// Commit the transaction
	DetourTransactionCommit();
}

//#############################################################################
// TOGGLES
//#############################################################################

void ToggleDevCommands()
{
	static bool g_dev = false;

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());

	if (g_dev)
	{
		DetourAttach((LPVOID*)&ConVar_IsFlagSet, &HConVar_IsFlagSet);
		DetourAttach((LPVOID*)&ConCommand_IsFlagSet, &HConCommand_IsFlagSet);
		printf("\n");
		printf("+--------------------------------------------------------+\n");
		printf("|>>>>>>>>>>>>>| DEVONLY COMMANDS ACTIVATED |<<<<<<<<<<<<<|\n");
		printf("+--------------------------------------------------------+\n");
		printf("\n");
	}
	else
	{
		DetourDetach((LPVOID*)&ConVar_IsFlagSet, &HConVar_IsFlagSet);
		DetourDetach((LPVOID*)&ConCommand_IsFlagSet, &HConCommand_IsFlagSet);
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

	g_dev = !g_dev;
}
