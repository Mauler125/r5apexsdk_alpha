#include "stdafx.h"
#include "sys_utils.h"

//-----------------------------------------------------------------------------
//	Sys_Error
//
//-----------------------------------------------------------------------------
void HSys_Error(char* fmt, ...)
{
	printf("\n_____________________________________________________________\n");
	printf("SYS ERROR #####################################################\n");
	char buf[1024];
	va_list args;
	va_start(args, fmt);
	vprintf(fmt, args);
	vsnprintf(buf, IM_ARRAYSIZE(buf), fmt, args);
	buf[IM_ARRAYSIZE(buf) - 1] = 0;
	va_end(args);
	Items.push_back(Strdup(buf));
}

void AttachSysUtilsHooks()
{
	DetourAttach((LPVOID*)&Sys_Error, &HSys_Error);
}

void DetachSysUtilsHooks()
{
	DetourDetach((LPVOID*)&Sys_Error, &HSys_Error);
}
