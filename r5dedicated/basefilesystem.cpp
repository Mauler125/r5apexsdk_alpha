#include "stdafx.h"
#include "classes.h"
#include "basefilesystem.h"
#include "IConVar.h"

//---------------------------------------------------------------------------------
// Purpose: prints the output of the filesystem based on the warning level
//---------------------------------------------------------------------------------
static std::ostringstream fs_oss;
static auto fs_ostream_sink = std::make_shared<spdlog::sinks::ostream_sink_st>(fs_oss);
void HCBaseFileSystem_Warning(void* thisptr, FileWarningLevel_t level, const char* fmt, ...)
{
	if (g_pCvar == nullptr || !g_bClassInitialized) // TODO: HACK.
	{
		return;
	}
	else if (g_pCvar->FindVar("fs_warning_level_native")->m_iValue < (int)level)
	{
		return;
	}

	static bool initialized = false;
	static char buf[1024];

	static auto wconsole = spdlog::stdout_logger_mt("fs_warn_wconsole"); // windows console.

	fs_oss.str("");
	fs_oss.clear();

	if (!initialized)
	{
		wconsole = std::make_shared<spdlog::logger>("fs_warn_ostream", fs_ostream_sink);
		wconsole->set_pattern("[%S.%e] %v");
		wconsole->set_level(spdlog::level::debug);
		initialized = true;
	}

	va_list args;
	va_start(args, fmt);

	vsnprintf(buf, sizeof(buf), fmt, args);

	buf[sizeof(buf) - 1] = 0;
	va_end(args);

	wconsole->debug(buf);
}

void AttachCBaseFileSystemHooks()
{
	DetourAttach((LPVOID*)&CBaseFileSystem_Warning, &HCBaseFileSystem_Warning);
}

void DetachCBaseFileSystemHooks()
{
	DetourDetach((LPVOID*)&CBaseFileSystem_Warning, &HCBaseFileSystem_Warning);
}
