#pragma once
#include "stdafx.h"

int HCEngineVGui_Paint(void* thisptr, int mode);
void AttachCEngineVGuiHooks();
void DetachCEngineVGuiHooks();

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
	std::vector<Log> m_vLogs;
};

namespace
{
	/* ==== CENGINEVGUI ===================================================================================================================================================== */
	DWORD64 p_CEngineVGui_Paint = FindPatternV2("r5apex.exe", (const unsigned char*)"\x41\x55\x41\x56\x48\x83\xEC\x78\x44\x8B\xEA", "xxxxxxxxxxx");
	int (*CEngineVGui_Paint)(void* thisptr, int mode) = (int (*)(void*, int))p_CEngineVGui_Paint; /*41 55 41 56 48 83 EC 78 44 8B EA*/
}

extern CLogSystem g_LogSystem;
