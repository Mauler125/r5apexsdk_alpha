#pragma once
#include "hooks.h"
#include "classes.h"

void __fastcall HFrameStageNotify(CHLClient* rcx, ClientFrameStage_t curStage);

void AttachCHLClientHooks();
void DetachCHLClientHooks();
