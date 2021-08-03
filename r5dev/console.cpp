#include "stdafx.h"

#include "id3dx.h"
#include "hooks.h"
#include "opcptc.h"
#include "console.h"

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
// WORKER THREAD
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
		if (sCommand == "toggle net") { ToggleNetTrace(); CommandExecute(NULL, "exec autoexec_dev"); continue; }
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
		if (sCommand == "1") { CommandExecute(NULL, "exec autoexec_dev"); continue; }
		if (sCommand == "2") { CommandExecute(NULL, "exec connect5_dev"); continue; }
		///////////////////////////////////////////////////////////////////////
		// Execute the command in the r5 SQVM
		CommandExecute(NULL, sCommand.c_str());
		sCommand.clear();

		///////////////////////////////////////////////////////////////////////
		// Sleep and loop
		Sleep(50);
	}

	return 0;
}
