#pragma once
#include "hooks.h"

void HSys_PrintFunc(char* fmt, ...);

void AttachSysHooks();
void DetachSysHooks();
