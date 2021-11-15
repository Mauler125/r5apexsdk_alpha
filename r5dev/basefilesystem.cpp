#include "stdafx.h"
#include "logdef.h"
#include "basefilesystem.h"
#include "IConVar.h"
#include "CGameConsole.h"

//---------------------------------------------------------------------------------
// Purpose: prints the output of the filesystem based on the warning level
//---------------------------------------------------------------------------------
void HCBaseFileSystem_Warning(void* thisptr, FileWarningLevel_t level, const char* fmt, ...)
{
	if (g_fs_warning_level_native < (int)level)
	{
		return;
	}

	static bool initialized = false;
	static char buf[1024];

	static auto iconsole = spdlog::stdout_logger_mt("fs_warn_iconsole"); // in-game console.
	static auto wconsole = spdlog::stdout_logger_mt("fs_warn_wconsole"); // windows console.

	fs_oss.str("");
	fs_oss.clear();

	if (!initialized)
	{
		iconsole = std::make_shared<spdlog::logger>("fs_warn_ostream", fs_ostream_sink);
		iconsole->set_pattern("[%S.%e] %v");
		iconsole->set_level(spdlog::level::debug);
		wconsole->set_pattern("[%S.%e] %v");
		wconsole->set_level(spdlog::level::debug);
		initialized = true;
	}

	va_list args;
	va_start(args, fmt);

	vsnprintf(buf, sizeof(buf), fmt, args);

	buf[sizeof(buf) - 1] = 0;
	va_end(args);

	iconsole->debug(buf);
	wconsole->debug(buf);

	std::string s = fs_oss.str();
	const char* c = s.c_str();

	Items.push_back(Strdup((const char*)c));
}

void AttachCBaseFileSystemHooks()
{
	DetourAttach((LPVOID*)&CBaseFileSystem_Warning, &HCBaseFileSystem_Warning);
}

void DetachCBaseFileSystemHooks()
{
	DetourDetach((LPVOID*)&CBaseFileSystem_Warning, &HCBaseFileSystem_Warning);
}
