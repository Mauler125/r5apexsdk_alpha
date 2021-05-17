#include <string>
#include <Windows.h>

#include "r5dev.h"
#include "console.h"
#include "utilities.h"
#include "hooks.h"

//---------------------------------------------------------------------------------
// Main
//---------------------------------------------------------------------------------

void InitializeR5Dev()
{
	SetupConsole();
    InstallHooks();
    printf("+-----------------------------------------------------------------------------+\n");
    printf("|   R5 DEV -- INITIALIZED -------------------------------------------------   |\n");
    printf("+-----------------------------------------------------------------------------+\n");
}

void TerminateR5Dev()
{
    RemoveHooks();
	FreeConsole();
}

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