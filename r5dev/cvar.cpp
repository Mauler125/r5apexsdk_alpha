#include "stdafx.h"
#include "cvar.h"
#include "IConVar.h"

//-------------------------------------------------------------------------
// ENGINE DLL                                                             |
ConVar* cm_debug_cmdquery;
ConVar* cm_return_false_cmdquery_all;
ConVar* cm_return_false_cmdquery_dev_cheat;
//-------------------------------------------------------------------------
// SERVER DLL                                                             |
ConVar* sv_showconnecting = NULL;
//-------------------------------------------------------------------------
// CLIENT DLL                                                             |
ConVar* cl_drawconsoleoverlay;
ConVar* cl_consoleoverlay_lines;
ConVar* cl_consoleoverlay_offset_x;
ConVar* cl_consoleoverlay_offset_y;
//-------------------------------------------------------------------------
// FILESYSTEM DLL                                                         |
ConVar* fs_warning_level_native;
//-------------------------------------------------------------------------
// SQUIRREL API                                                           |
ConVar* sq_showrsonloading;
ConVar* sq_showscriptloading;
ConVar* sq_showvmoutput;
ConVar* sq_showvmwarning;
//-------------------------------------------------------------------------
// NETCHANNEL                                                             |
ConVar* net_userandomkey;
