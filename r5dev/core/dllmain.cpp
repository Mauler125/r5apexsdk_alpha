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

void R5Dev_Init()
{
    Console_Init();
    Systems_Init();

#ifndef DEDICATED
    Input_Init();
    DirectX_Init();
#endif // !DEDICATED

    spdlog::get("console")->set_pattern("%v");
    spdlog::info("+-----------------------------------------------------------------------------+\n");
    spdlog::info("|   R5 DEVELOPER CONSOLE -- INITIALIZED -----------------------------------   |\n");
    spdlog::info("+-----------------------------------------------------------------------------+\n");
    spdlog::get("console")->set_pattern("[%S.%e] %v");
}

void R5Dev_Shutdown()
{
    Systems_Shutdown();

#ifndef DEDICATED
    Input_Shutdown();
    DirectX_Shutdown();
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
            R5Dev_Init();
            break;
        }

        case DLL_PROCESS_DETACH:
        {
            R5Dev_Shutdown();
            break;
        }
    }

    return TRUE;
}
