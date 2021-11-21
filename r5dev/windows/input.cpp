#include "core/stdafx.h"
#include "windows/input.h"

/*-----------------------------------------------------------------------------
 * _input.cpp
 *-----------------------------------------------------------------------------*/

///////////////////////////////////////////////////////////////////////////////
typedef BOOL(WINAPI* IGetCursorPos)(LPPOINT lpPoint);
typedef BOOL(WINAPI* ISetCursorPos)(int nX, int nY);
typedef BOOL(WINAPI* IClipCursor)(const RECT* lpRect);
typedef BOOL(WINAPI* IShowCursor)(BOOL bShow);

///////////////////////////////////////////////////////////////////////////////
static IGetCursorPos            g_oGetCursorPos             = nullptr;
static ISetCursorPos            g_oSetCursorPos             = nullptr;
static IClipCursor              g_oClipCursor               = nullptr;
static IShowCursor              g_oShowCursor               = nullptr;

///////////////////////////////////////////////////////////////////////////////
static POINT                    g_pLastCursorPos              { 0 };
extern BOOL                     g_bBlockInput               = false;

//#############################################################################
// INITIALIZATION
//#############################################################################

void SetupIPHooks()
{
	g_oSetCursorPos = (ISetCursorPos)DetourFindFunction("user32.dll", "SetCursorPos");
	g_oClipCursor   = (IClipCursor  )DetourFindFunction("user32.dll", "ClipCursor"  );
	g_oGetCursorPos = (IGetCursorPos)DetourFindFunction("user32.dll", "GetCursorPos");
	g_oShowCursor   = (IShowCursor  )DetourFindFunction("user32.dll", "ShowCursor"  );
}

//#############################################################################
// INPUT HOOKS
//#############################################################################

BOOL WINAPI HGetCursorPos(LPPOINT lpPoint)
{
	if (g_bBlockInput)
	{
		assert(lpPoint != nullptr);
		*lpPoint = g_pLastCursorPos;
	}

	return g_oGetCursorPos(lpPoint);
}

BOOL WINAPI HSetCursorPos(int X, int Y)
{
	g_pLastCursorPos.x = X;
	g_pLastCursorPos.y = Y;

	if (g_bBlockInput)
	{
		return TRUE;
	}

	return g_oSetCursorPos(X, Y);
}

BOOL WINAPI HClipCursor(const RECT* lpRect)
{
	if (g_bBlockInput)
	{
		lpRect = nullptr;
	}

	return g_oClipCursor(lpRect);
}

BOOL WINAPI HShowCursor(BOOL bShow)
{
	if (g_bBlockInput)
	{
		bShow = TRUE;
	}

	return g_oShowCursor(bShow);
}

//#############################################################################
// MANAGEMENT
//#############################################################################

void InstallIPHooks()
{
	SetupIPHooks();
	///////////////////////////////////////////////////////////////////////////
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());

	///////////////////////////////////////////////////////////////////////////
	DetourAttach(&(LPVOID&)g_oGetCursorPos, (PBYTE)HGetCursorPos);
	DetourAttach(&(LPVOID&)g_oSetCursorPos, (PBYTE)HSetCursorPos);
	DetourAttach(&(LPVOID&)g_oClipCursor, (PBYTE)HClipCursor);
	DetourAttach(&(LPVOID&)g_oShowCursor, (PBYTE)HShowCursor);

	///////////////////////////////////////////////////////////////////////////
	DetourTransactionCommit();
}

void RemoveIPHooks()
{
	///////////////////////////////////////////////////////////////////////////
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());

	///////////////////////////////////////////////////////////////////////////
	DetourDetach(&(LPVOID&)g_oGetCursorPos, (PBYTE)HGetCursorPos);
	DetourDetach(&(LPVOID&)g_oSetCursorPos, (PBYTE)HSetCursorPos);
	DetourDetach(&(LPVOID&)g_oClipCursor, (PBYTE)HClipCursor);
	DetourDetach(&(LPVOID&)g_oShowCursor, (PBYTE)HShowCursor);

	///////////////////////////////////////////////////////////////////////////
	DetourTransactionCommit();
}
