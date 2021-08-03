#pragma once
#include "hooks.h"

int HQHull_PrintError(char* fmt, va_list args);
int HQHull_PrintDebug(char* fmt, va_list args);

void AttachQHullHooks();
void DetachQHullHooks();
