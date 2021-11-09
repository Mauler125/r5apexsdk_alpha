#include "stdafx.h"

#include "rtech.h"
#include "hooks.h"
#include "opcodes.h"
#include "console.h"

#include "IVEngineClient.h"

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

	// Initialize global spdlog.
	auto console = spdlog::stdout_logger_mt("console");
	console->set_pattern("[%S.%e] %v"); // Set pattern.
	spdlog::set_level(spdlog::level::trace);
	spdlog::set_default_logger(console); // Set as default.
	spdlog::flush_every(std::chrono::seconds(5)); // Flush buffers every 5 seconds for every logger.
	
	if (hThread0)
	{
		spdlog::debug("THREAD ID: {}\n\n", threadId0);
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
		// Debug toggles
		if (sCommand == "pattern test") { PrintHAddress(); PrintOAddress(); continue; }
		///////////////////////////////////////////////////////////////////////
		// Exec toggles
		if (sCommand == "1") { IVEngineClient_CommandExecute(NULL, "exec autoexec_dev"); continue; }
		if (sCommand == "2") { IVEngineClient_CommandExecute(NULL, "exec connect5_dev"); continue; }
		///////////////////////////////////////////////////////////////////////
		// Execute the command in the r5 SQVM
		IVEngineClient_CommandExecute(NULL, sCommand.c_str());
		sCommand.clear();

		///////////////////////////////////////////////////////////////////////
		// Sleep and loop
		Sleep(50);
	}

	return 0;
}
