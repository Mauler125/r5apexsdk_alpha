#include "stdafx.h"

#include "r5dev.h"
#include "id3dx.h"
#include "input.h"
#include "hooks.h"
#include "console.h"

//#############################################################################
// INITIALIZATION
//#############################################################################

void InitializeR5Dev()
{
    SetupConsole();
    InstallHooks();
    InstallIPHooks();
    SetupDXSwapChain();

    spdlog::get("console")->set_pattern("%v");
    spdlog::info("+-----------------------------------------------------------------------------+\n");
    spdlog::info("|   R5 DEVELOPER CONSOLE -- INITIALIZED -----------------------------------   |\n");
    spdlog::info("+-----------------------------------------------------------------------------+\n");
    spdlog::get("console")->set_pattern("[%S.%e] %v");
}

void TerminateR5Dev()
{
    RemoveIPHooks();
    RemoveDXHooks();
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
