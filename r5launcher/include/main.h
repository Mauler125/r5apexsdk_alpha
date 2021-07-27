#pragma once

//-----------------------------------------------------------------------------
// Launch and inject specified dll based on launchmode
//-----------------------------------------------------------------------------
enum class LAUNCHMODE : int
{
    DLL_GAME, // Listen server
    DLL_DEDI, // Dedicated server
    DLL_IDBG  // Custom debug DLL
};

//-----------------------------------------------------------------------------
// [TODO] Launch with FCVAR_DEVELOPMENTONLY and FCVAR_CHEATS disabled/enabled
//-----------------------------------------------------------------------------
enum class LAUNCHSTATE : int
{
    LS_NOCHEATS, // Disabled cheats
    LS_CHEATS,   // Enable cheats
    LS_DEBUG     // Enable debug
};
