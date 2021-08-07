#pragma once
#include "hooks.h"
#include "CGameConsole.h"

void HSys_PrintFunc(char* fmt, ...);

void AttachSysHooks();
void DetachSysHooks();
