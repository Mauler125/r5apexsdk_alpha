#include "stdafx.h"
#include "sys.h"

void HSys_PrintFunc(char* fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	vprintf(fmt, args);
	va_end(args);
}

void AttachSysHooks()
{
	DetourAttach((LPVOID*)&Sys_PrintFunc, &HSys_PrintFunc);
}

void DetachSysHooks()
{
	DetourDetach((LPVOID*)&Sys_PrintFunc, &HSys_PrintFunc);
}