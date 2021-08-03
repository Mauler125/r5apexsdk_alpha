#pragma once
#include "hooks.h"
#include "overlay.h"

void HSys_PrintFunc(char* fmt, ...);

void AttachSysHooks();
void DetachSysHooks();
