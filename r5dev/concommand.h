#pragma once
#include "hooks.h"

bool HConCommand_IsFlagSet(ConCommandBase* cmd, int flag);

void AttachConCommandHooks();
void DetachConCommandHooks();
