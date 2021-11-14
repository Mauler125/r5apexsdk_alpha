#include "stdafx.h"
#include "sys_dll.h"

//-----------------------------------------------------------------------------
//	Sys_Error_Internal
//
//-----------------------------------------------------------------------------
int HSys_Error_Internal(char* fmt, va_list args)
{
	printf("\n_____________________________________________________________\n");
	printf("ENGINE ERROR ##################################################\n");
	vprintf(fmt, args);

	///////////////////////////////////////////////////////////////////////////
	return Sys_Error_Internal(fmt, args);
}

void AttachSysDllHooks()
{
	DetourAttach((LPVOID*)&Sys_Error_Internal, &HSys_Error_Internal);
}

void DetachSysDllHooks()
{
	DetourDetach((LPVOID*)&Sys_Error_Internal, &HSys_Error_Internal);
}