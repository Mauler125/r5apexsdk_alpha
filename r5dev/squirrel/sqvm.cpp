#include "core/stdafx.h"
#include "core/logdef.h"
#include "tier0/basetypes.h"
#include "tier0/cvar.h"
#include "tier0/IConVar.h"
#include "engine/sys_utils.h"
#include "squirrel/sqvm.h"
#include "vgui/CEngineVGui.h"
#include "gameui/IGameConsole.h"

//---------------------------------------------------------------------------------
// Purpose: prints the output of each VM to the console
//---------------------------------------------------------------------------------
void* HSQVM_PrintFunc(void* sqvm, char* fmt, ...)
{
#ifdef GAMEDLL_S3
	int vmIdx = *(int*)((std::uintptr_t)sqvm + 0x18);
#elif // TODO [ AMOS ]: nothing equal to 'rdx + 18h' exist in the vm pointers for anything below S3.
	int vmIdx = 3;
#endif
	static bool initialized = false;

	static char buf[1024];
	static std::string vmType[4] = { "Script(S):", "Script(C):", "Script(U):", "Script(X):" };

	static auto iconsole = spdlog::stdout_logger_mt("sqvm_print_iconsole"); // in-game console.
	static auto wconsole = spdlog::stdout_logger_mt("sqvm_print_wconsole"); // windows console.
	static auto sqlogger = spdlog::basic_logger_mt("sqvm_print_logger", "platform\\logs\\sqvm_print.log"); // file logger.

	std::string vmStr = vmType[vmIdx].c_str();

	g_spd_sqvm_p_oss.str("");
	g_spd_sqvm_p_oss.clear();

	if (!initialized)
	{
		iconsole = std::make_shared<spdlog::logger>("sqvm_print_ostream", g_spd_sqvm_p_ostream_sink);
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

	if (sq_showvmoutput->m_iValue > 0)
	{
		sqlogger->debug(vmStr);
	}
	if (sq_showvmoutput->m_iValue > 1)
	{
		iconsole->debug(vmStr);
		wconsole->debug(vmStr);
#ifndef DEDICATED
		std::string s = g_spd_sqvm_p_oss.str();
		const char* c = s.c_str();
		Items.push_back(Strdup((const char*)c));
#endif // !DEDICATED
	}
#ifndef DEDICATED
	if (sq_showvmoutput->m_iValue > 2)
	{
		std::string s = g_spd_sqvm_p_oss.str();
		g_pLogSystem.AddLog((LogType_t)vmIdx, s);
	}
#endif // !DEDICATED
	return NULL;
}

//---------------------------------------------------------------------------------
// Purpose: prints the warning output of each VM to the console
//---------------------------------------------------------------------------------
std::int64_t HSQVM_WarningFunc(void* sqvm, int a2, int a3, int* stringSize, void** string)
{
	std::int64_t result = SQVM_WarningFunc(sqvm, a2, a3, stringSize, string);
	if (g_bSQVM_WarnFuncCalled) // Check if its SQVM_Warning calling.
	{
		return result; // If not return.
	}

	static bool initialized = false;
	static std::string vmType[4] = { "Script(S): WARNING: ", "Script(C): WARNING: ", "Script(U): WARNING: ", "Script(X): WARNING: " };

	static auto iconsole = spdlog::stdout_logger_mt("sqvm_warn_iconsole"); // in-game console.
	static auto wconsole = spdlog::stdout_logger_mt("sqvm_warn_wconsole"); // windows console.
	static auto sqlogger = spdlog::basic_logger_mt("sqvm_warn_logger", "platform\\logs\\sqvm_warn.log"); // file logger.

#ifdef GAMEDLL_S3
	int vmIdx = *(int*)((std::uintptr_t)sqvm + 0x18);
#elif // TODO [ AMOS ]: nothing equal to 'rdx + 18h' exist in the vm pointers for anything below S3.
	int vmIdx = 3;
#endif
	std::string vmStr = vmType[vmIdx].c_str();

	g_spd_sqvm_w_oss.str("");
	g_spd_sqvm_w_oss.clear();

	if (!initialized)
	{
		iconsole = std::make_shared<spdlog::logger>("sqvm_warn_ostream", g_spd_sqvm_p_ostream_sink);
		iconsole->set_pattern("[%S.%e] %v\n");
		iconsole->set_level(spdlog::level::debug);
		wconsole->set_pattern("[%S.%e] %v\n");
		wconsole->set_level(spdlog::level::debug);
		sqlogger->set_pattern("[%S.%e] %v\n");
		sqlogger->set_level(spdlog::level::debug);
		initialized = true;
	}

	std::string stringConstructor((char*)*string, *stringSize); // Get string from memory via std::string constructor.
	vmStr.append(stringConstructor);

	std::string s = g_spd_sqvm_w_oss.str();
	const char* c = s.c_str();

	if (sq_showvmwarning->m_iValue > 0)
	{
		sqlogger->debug(vmStr); // Emit to file.
	}
	if (sq_showvmwarning->m_iValue > 1)
	{
		iconsole->debug(vmStr); // Emit to in-game console.
		wconsole->debug(vmStr); // Emit to windows console.
#ifndef DEDICATED
		std::string s = g_spd_sqvm_w_oss.str();
		const char* c = s.c_str();
		Items.push_back(Strdup(c));
#endif // !DEDICATED
	}
#ifndef DEDICATED
	if (sq_showvmwarning->m_iValue > 2)
	{
		g_pLogSystem.AddLog((LogType_t)vmIdx, s);
		const char* c = s.c_str();
		Items.push_back(Strdup(c));
	}
#endif // !DEDICATED
	g_bSQVM_WarnFuncCalled = false;

	return result;
}

//---------------------------------------------------------------------------------
// Purpose:
//---------------------------------------------------------------------------------
std::int64_t HSQVM_WarningCmd(int a1, int a2)
{
	g_bSQVM_WarnFuncCalled = true;
	return SQVM_WarningCmd(a1, a2);
}

//---------------------------------------------------------------------------------
// Purpose: loads the include file from the mods directory
//---------------------------------------------------------------------------------
std::int64_t HSQVM_LoadRson(const char* rson_name)
{
	char filepath[MAX_PATH] = { 0 };
	sprintf_s(filepath, MAX_PATH, "platform\\%s", rson_name);

	// Flip forward slashes in filepath to windows-style backslash
	for (int i = 0; i < strlen(filepath); i++)
	{
		if (filepath[i] == '/')
		{
			filepath[i] = '\\';
		}
	}

	// Returns the new path if the rson exists on the disk
	if (FileExists(filepath) && SQVM_LoadRson(rson_name))
	{
		if (sq_showrsonloading->m_iValue > 0)
		{
			Sys_Print(SYS_DLL::ENGINE, "\n");
			Sys_Print(SYS_DLL::ENGINE, "______________________________________________________________\n");
			Sys_Print(SYS_DLL::ENGINE, "] RSON_DISK --------------------------------------------------\n");
			Sys_Print(SYS_DLL::ENGINE, "] PATH: '%s'\n", filepath);
			Sys_Print(SYS_DLL::ENGINE, "--------------------------------------------------------------\n");
			Sys_Print(SYS_DLL::ENGINE, "\n");
		}
		return SQVM_LoadRson(filepath);
	}
	else
	{
		if (sq_showrsonloading->m_iValue > 0)
		{
			Sys_Print(SYS_DLL::ENGINE, "\n");
			Sys_Print(SYS_DLL::ENGINE, "______________________________________________________________\n");
			Sys_Print(SYS_DLL::ENGINE, "] RSON_VPK ---------------------------------------------------\n");
			Sys_Print(SYS_DLL::ENGINE, "] PATH: '%s'\n", rson_name);
			Sys_Print(SYS_DLL::ENGINE, "--------------------------------------------------------------\n");
			Sys_Print(SYS_DLL::ENGINE, "\n");
		}
	}
	return SQVM_LoadRson(rson_name);
}

//---------------------------------------------------------------------------------
// Purpose: loads the script file from the mods directory
//---------------------------------------------------------------------------------
bool HSQVM_LoadScript(void* sqvm, const char* script_path, const char* script_name, int flag)
{
	char filepath[MAX_PATH] = { 0 };
	sprintf_s(filepath, MAX_PATH, "platform\\%s", script_path);

	// Flip forward slashes in filepath to windows-style backslash
	for (int i = 0; i < strlen(filepath); i++)
	{
		if (filepath[i] == '/')
		{
			filepath[i] = '\\';
		}
	}

	if (sq_showscriptloading->m_iValue > 0)
	{
		Sys_Print(SYS_DLL::ENGINE, "Loading SQVM Script '%s'\n", filepath);
	}

	// Returns true if the script exists on the disk
	if (FileExists(filepath) && SQVM_LoadScript(sqvm, filepath, script_name, flag))
	{
		return true;
	}

	if (sq_showscriptloading->m_iValue > 0)
	{
		Sys_Print(SYS_DLL::ENGINE, "FAILED. Try SP / VPK for '%s'\n", filepath);
	}

	///////////////////////////////////////////////////////////////////////////////
	return SQVM_LoadScript(sqvm, script_path, script_name, flag);
}

void HSQVM_RegisterFunction(void* sqvm, const char* name, const char* helpString, const char* retValType, const char* argTypes, void* funcPtr)
{
	SQFuncRegistration* func = new SQFuncRegistration();

	func->scriptName = name;
	func->nativeName = name;
	func->helpString = helpString;
	func->retValType = retValType;
	func->argTypes = argTypes;
	func->funcPtr = funcPtr;

	SQVM_RegisterFunc(sqvm, func, 1);
}

int HSQVM_NativeTest(void* sqvm)
{
	// Function code goes here.
	return 1;
}

void RegisterUIScriptFunctions(void* sqvm)
{
	HSQVM_RegisterFunction(sqvm, "UINativeTest", "native ui function", "void", "", &HSQVM_NativeTest);
}

void RegisterClientScriptFunctions(void* sqvm)
{
	HSQVM_RegisterFunction(sqvm, "ClientNativeTest", "native client function", "void", "", &HSQVM_NativeTest);
}

void RegisterServerScriptFunctions(void* sqvm)
{
	HSQVM_RegisterFunction(sqvm, "ServerNativeTest", "native server function", "void", "", &HSQVM_NativeTest);
}

void SQVM_Attach()
{
	DetourAttach((LPVOID*)&SQVM_PrintFunc, &HSQVM_PrintFunc);
	DetourAttach((LPVOID*)&SQVM_WarningFunc, &HSQVM_WarningFunc);
	DetourAttach((LPVOID*)&SQVM_WarningCmd, &HSQVM_WarningCmd);
	DetourAttach((LPVOID*)&SQVM_LoadRson, &HSQVM_LoadRson);
	DetourAttach((LPVOID*)&SQVM_LoadScript, &HSQVM_LoadScript);
}

void SQVM_Detach()
{
	DetourDetach((LPVOID*)&SQVM_PrintFunc, &HSQVM_PrintFunc);
	DetourDetach((LPVOID*)&SQVM_WarningFunc, &HSQVM_WarningFunc);
	DetourDetach((LPVOID*)&SQVM_LoadRson, &HSQVM_LoadRson);
	DetourDetach((LPVOID*)&SQVM_LoadScript, &HSQVM_LoadScript);
}

///////////////////////////////////////////////////////////////////////////////
bool g_bSQVM_WarnFuncCalled;
