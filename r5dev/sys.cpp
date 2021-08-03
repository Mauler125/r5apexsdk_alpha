#include "stdafx.h"
#include "sys.h"

void HSys_PrintFunc(char* fmt, ...)
{
	char buf[1024];
	va_list args;
	va_start(args, fmt);
	vprintf(fmt, args);
	vsnprintf(buf, IM_ARRAYSIZE(buf), fmt, args);
	buf[IM_ARRAYSIZE(buf) - 1] = 0;
	va_end(args);
	Items.push_back(Strdup(buf));
}

void AttachSysHooks()
{
	DetourAttach((LPVOID*)&Sys_PrintFunc, &HSys_PrintFunc);
}

void DetachSysHooks()
{
	DetourDetach((LPVOID*)&Sys_PrintFunc, &HSys_PrintFunc);
}