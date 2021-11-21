#include "core/stdafx.h"
#include "core/r5dev.h"
#include "core/init.h"
/*****************************************************************************/
#ifndef DEDICATED
#include "windows/id3dx.h"
#include "windows/input.h"
#endif // !DEDICATED
#include "windows/console.h"

//#############################################################################
// INITIALIZATION
//#############################################################################

void InitializeR5Dev()
{
    SetupConsole();
    InstallHooks();
#ifndef DEDICATED
    InstallIPHooks();
    SetupDXSwapChain();
#endif // !DEDICATED

    spdlog::get("console")->set_pattern("%v");
    spdlog::info("+-----------------------------------------------------------------------------+\n");
    spdlog::info("|   R5 DEVELOPER CONSOLE -- INITIALIZED -----------------------------------   |\n");
    spdlog::info("+-----------------------------------------------------------------------------+\n");
    spdlog::get("console")->set_pattern("[%S.%e] %v");
}

void TerminateR5Dev()
{
#ifndef DEDICATED
    RemoveIPHooks();
    RemoveDXHooks();
#endif // !DEDICATED
    FreeConsole();
}

//#############################################################################
// ENTRYPOINT
//#############################################################################

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  dwReason, LPVOID lpReserved)
{
    switch (dwReason)
    {
        case DLL_PROCESS_ATTACH:
        {
            InitializeR5Dev();
            break;
        }

        case DLL_PROCESS_DETACH:
        {
            TerminateR5Dev();
            break;
        }
    }

    return TRUE;
}
