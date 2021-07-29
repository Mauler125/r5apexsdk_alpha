#include "pch.h"

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
    printf("+-----------------------------------------------------------------------------+\n");
    printf("|   R5 DEVELOPER CONSOLE -- INITIALIZED -----------------------------------   |\n");
    printf("+-----------------------------------------------------------------------------+\n");
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
