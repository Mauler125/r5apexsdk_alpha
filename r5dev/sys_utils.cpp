#include "stdafx.h"
#include "logdef.h"
#include "sys_utils.h"
#include "CGameConsole.h"

//-----------------------------------------------------------------------------
//	Sys_Error
//
//-----------------------------------------------------------------------------
void HSys_Error(char* fmt, ...)
{
	static char buf[1024];

	va_list args;
	va_start(args, fmt);

	vsnprintf(buf, sizeof(buf), fmt, args);

	buf[sizeof(buf) -1] = 0;
	va_end(args);

	Sys_Print(SYS_DLL::ENGINE, "%s\n", buf);
}

//-----------------------------------------------------------------------------
//	Sys_Print
//
//-----------------------------------------------------------------------------
void Sys_Print(SYS_DLL idx, const char* fmt, ...)
{
	int vmIdx = (int)idx;
	static bool initialized = false;
	static char buf[1024];

	static std::string vmType[4] = { "Native(S):", "Native(C):", "Native(U):", "Native(E):" };

	static auto iconsole = spdlog::stdout_logger_mt("sys_print_iconsole"); // in-game console.
	static auto wconsole = spdlog::stdout_logger_mt("sys_print_wconsole"); // windows console.
	static auto sqlogger = spdlog::basic_logger_mt("sys_print_logger", "platform\\logs\\SYS_Print.log"); // file logger.

	std::string vmStr = vmType[vmIdx].c_str();

	g_spd_sys_w_oss.str("");
	g_spd_sys_w_oss.clear();

	if (!initialized)
	{
		iconsole = std::make_shared<spdlog::logger>("sys_print_ostream", g_spd_sys_p_ostream_sink);
		iconsole->set_pattern("[%S.%e] %v");
		iconsole->set_level(spdlog::level::debug);
		wconsole->set_pattern("[%S.%e] %v");
		wconsole->set_level(spdlog::level::debug);
		sqlogger->set_pattern("[%S.%e] %v");
		sqlogger->set_level(spdlog::level::debug);
		initialized = true;
	}

	va_list args;
	va_start(args, fmt);

	vsnprintf(buf, sizeof(buf), fmt, args);

	buf[sizeof(buf) - 1] = 0;
	va_end(args);

	vmStr.append(buf);

	iconsole->debug(vmStr);
	wconsole->debug(vmStr);
	sqlogger->debug(vmStr);

	std::string s = g_spd_sys_w_oss.str();
	const char* c = s.c_str();

	Items.push_back(Strdup((const char*)c));
}

void AttachSysUtilsHooks()
{
	DetourAttach((LPVOID*)&Sys_Error, &HSys_Error);
}

void DetachSysUtilsHooks()
{
	DetourDetach((LPVOID*)&Sys_Error, &HSys_Error);
}
