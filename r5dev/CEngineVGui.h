#pragma once
#include "stdafx.h"
#include "basetypes.h"

enum class LogType_t : int
{
	SCRIPT_SERVER,
	SCRIPT_CLIENT,
	SCRIPT_UI,
	SCRIPT_WARNING,
	NATIVE
};

struct Log
{
	Log(const std::string Message, const int Ticks, const LogType_t Type)
	{
		this->Message = Message;
		this->Ticks = Ticks;
		this->Type = Type;
	}
	std::string Message = "";
	int Ticks = 1024;
	LogType_t Type = LogType_t::NATIVE;
};

class CLogSystem
{
public:
	void AddLog(LogType_t type, std::string text);
	void Update();

private:
	std::array<int, 3> GetLogColorForType(LogType_t type);
	std::vector<Log> m_vLogs{};
};

namespace
{
	/* ==== CENGINEVGUI ===================================================================================================================================================== */
#if defined (GAMEDLL_S0) || defined (GAMEDLL_S1)
	ADDRESS p_CEngineVGui_Paint = g_mGameDll.FindPatternSIMD((std::uint8_t*)"\x89\x54\x24\x10\x55\x56\x41\x55\x48\x81\xEC\x00\x00\x00\x00", "xxxxxxxxxxx????");
	int (*CEngineVGui_Paint)(void* thisptr, int mode) = (int (*)(void*, int))p_CEngineVGui_Paint.GetPtr(); /*41 55 41 56 48 83 EC 78 44 8B EA*/
#elif defined (GAMEDLL_S2) || defined (GAMEDLL_S3)
	ADDRESS p_CEngineVGui_Paint = g_mGameDll.FindPatternSIMD((std::uint8_t*)"\x41\x55\x41\x56\x48\x83\xEC\x78\x44\x8B\xEA", "xxxxxxxxxxx");
	int (*CEngineVGui_Paint)(void* thisptr, int mode) = (int (*)(void*, int))p_CEngineVGui_Paint.GetPtr(); /*41 55 41 56 48 83 EC 78 44 8B EA*/
#endif
}

///////////////////////////////////////////////////////////////////////////////
int HCEngineVGui_Paint(void* thisptr, int mode);
void AttachCEngineVGuiHooks();
void DetachCEngineVGuiHooks();

///////////////////////////////////////////////////////////////////////////////
extern CLogSystem g_pLogSystem;
