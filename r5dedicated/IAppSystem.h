#pragma once
#include "hooks.h"

__int64 HIAppSystem_Main(__int64 a1, __int64 a2);
__int64 HIAppSystem_Create(__int64 a1);

void AttachIAppSystemHooks();
void DetachIAppSystemHooks();
