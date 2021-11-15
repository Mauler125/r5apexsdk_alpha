#include "stdafx.h"
#include "CEngineVGui.h"
#include "CMatSystemSurface.h"
#include "IConVar.h"

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
int HCEngineVGui_Paint(void* thisptr, int mode)
{
	int result = CEngineVGui_Paint(thisptr, mode);

	static void* pCMatSystemSurface = ADDRESS(0x14D40B3B0).RCast<void* (*)()>();
	static auto fnRenderStart       = ADDRESS(0x14053EFC0).RCast<void(*)(void*)>();
	static auto fnRenderEnd         = ADDRESS(0x14053F1B0).RCast<void* (*)()>();

	if (mode == 1 || mode == 2) // Render in-main menu and in-game.
	{
		fnRenderStart(pCMatSystemSurface);

		g_pLogSystem.Update();

		fnRenderEnd();
	}

	return result;
}

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
void CLogSystem::Update()
{
	if (g_pCvar->FindVar("cl_drawconsoleoverlay")->m_iValue < 1)
	{
		return;
	}
	if (m_vLogs.empty())
	{
		return;
	}
	static void* pCMatSystemSurface = ADDRESS(0x14D40B360).RCast<void* (*)()>();
	if (!pCMatSystemSurface)
	{
		return;
	}

	static int fontHeight = 16;

	for (int i = 0; i < m_vLogs.size(); ++i)
	{
		if (m_vLogs[i].Ticks >= 0)
		{
			if (i < g_pCvar->FindVar("cl_consoleoverlay_lines")->m_iValue)
			{
				float fadepct = fminf(static_cast<float>(m_vLogs[i].Ticks) / 64.f, 1.0);
				float ptc = static_cast<int>(ceilf(fadepct * 255.f));
				int alpha = static_cast<int>(ptc);
				int y = (g_pCvar->FindVar("cl_consoleoverlay_offset_y")->m_iValue + (fontHeight * i));
				int x = g_pCvar->FindVar("cl_consoleoverlay_offset_x")->m_iValue;

				std::array<int, 3> color = GetLogColorForType(m_vLogs[i].Type);
				CMatSystemSurface_DrawColoredText(pCMatSystemSurface, 0x13, fontHeight, x, y, color[0], color[1], color[2], alpha, m_vLogs[i].Message.c_str());
			}
			else
			{
				m_vLogs.erase(m_vLogs.begin());
				continue;
			}

			m_vLogs[i].Ticks--;
		}
		else
		{
			m_vLogs.erase(m_vLogs.begin() + i);
		}
	}
}

void CLogSystem::AddLog(LogType_t type, std::string message)
{
	if (message.length() > 0)
	{
		m_vLogs.push_back(Log{ message, 1024, type });
	}
}

std::array<int, 3> CLogSystem::GetLogColorForType(LogType_t type)
{
	switch (type)
	{
		case LogType_t::NATIVE:
			return { 255, 255, 255 };
		case LogType_t::SCRIPT_SERVER:
			return { 190, 183, 240 };
		case LogType_t::SCRIPT_CLIENT:
			return { 117, 116, 139 };
		case LogType_t::SCRIPT_UI:
			return { 197, 160, 177 };
		default:
			return { 255, 255, 255 };
	}

	return { 255, 255, 255 };
}

///////////////////////////////////////////////////////////////////////////////
void AttachCEngineVGuiHooks()
{
	DetourAttach((LPVOID*)&CEngineVGui_Paint, &HCEngineVGui_Paint);
}

void DetachCEngineVGuiHooks()
{
	DetourDetach((LPVOID*)&CEngineVGui_Paint, &HCEngineVGui_Paint);
}

///////////////////////////////////////////////////////////////////////////////
CLogSystem g_pLogSystem;
