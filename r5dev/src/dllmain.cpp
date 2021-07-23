#include <string>
#include <Windows.h>

#include "r5dev.h"
#include "id3dx.h"
#include "input.h"
#include "hooks.h"
#include "opcptc.h"
#include "console.h"
#include "utility.h"

//#############################################################################
// INITIALIZATION
//#############################################################################

void InitializeR5Dev()
{
	SetupConsole();
    InstallENHooks();
    InstallCNHooks();
    InstallIPHooks();
    InstallDXHooks();
    InstallOpcodes();
    InstallGlobals();
    SetupDXSwapChain();
    printf("+-----------------------------------------------------------------------------+\n");
    printf("|   R5 DEVELOPER CONSOLE -- INITIALIZED -----------------------------------   |\n");
    printf("+-----------------------------------------------------------------------------+\n");
}

void TerminateR5Dev()
{
    RemoveENHooks();
    RemoveIPHooks();
    RemoveDXHooks();
    RemoveCNHooks();
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
