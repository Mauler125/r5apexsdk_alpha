#pragma once
#include "hooks.h"

bool HIConVar_IsFlagSet(ConVar* cvar, int flag);

void AttachIConVarHooks();
void DetachIConVarHooks();
