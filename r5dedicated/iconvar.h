#pragma once
#include "hooks.h"

bool HIConVar_IsFlagSet(int** cvar, int flag);

void AttachIConVarHooks();
void DetachIConVarHooks();
