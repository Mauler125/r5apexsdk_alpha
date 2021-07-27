#include "pch.h"
#include "hooks.h"

//-----------------------------------------------------------------------------
// Purpose: Engine Error message box
//-----------------------------------------------------------------------------
int HMSG_EngineError(char* fmt, va_list args)
{
	printf("\nENGINE ERROR #####################################\n");
	vprintf(fmt, args);

	///////////////////////////////////////////////////////////////////////////
	return MSG_EngineError(fmt, args);
}

void AttachMSGBoxHooks()
{
	DetourAttach((LPVOID*)&MSG_EngineError, &HMSG_EngineError);
}

void DetachMSGBoxHooks()
{
	DetourDetach((LPVOID*)&MSG_EngineError, &HMSG_EngineError);
}
