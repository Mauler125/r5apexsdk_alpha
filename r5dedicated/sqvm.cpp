#include "stdafx.h"
#include "logdef.h"
#include "sys_utils.h"
#include "sqvm.h"
#include "classes.h"
#include "IConVar.h"

//---------------------------------------------------------------------------------
// Purpose: prints the output of each VM to the console
//---------------------------------------------------------------------------------
void* HSQVM_PrintFunc(void* sqvm, char* fmt, ...)
{
	int vmIdx = *(int*)((std::uintptr_t)sqvm + 0x18);
	static bool initialized = false;

	static char buf[1024];
	static std::string vmType[3] = { "Script(S):", "Script(C):", "Script(U):" };

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

	va_end(args);

	vmStr.append(buf);

	iconsole->debug(vmStr);
	wconsole->debug(vmStr);
	sqlogger->debug(vmStr);

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
	static std::string vmType[3] = { "Script(S): WARNING: ", "Script(C): WARNING: ", "Script(U): WARNING: " };

	static auto iconsole = spdlog::stdout_logger_mt("sqvm_warn_iconsole"); // in-game console.
	static auto wconsole = spdlog::stdout_logger_mt("sqvm_warn_wconsole"); // windows console.
	static auto sqlogger = spdlog::basic_logger_mt("sqvm_warn_logger", "platform\\logs\\sqvm_warn.log"); // file logger.

	int vmIdx = *(int*)((std::uintptr_t)sqvm + 0x18);
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

	if (g_bClassInitialized && g_pCvar->FindVar("sq_showvmwarning")->m_iValue > 0)
	{
		sqlogger->debug(vmStr); // Emit to file.
	}
	if (g_bClassInitialized && g_pCvar->FindVar("sq_showvmwarning")->m_iValue > 1)
	{
		iconsole->debug(vmStr); // Emit to in-game console.
		wconsole->debug(vmStr); // Emit to windows console.
	}

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
		if (g_bClassInitialized && g_pCvar->FindVar("sq_showrsonloading")->m_iValue > 0)
		{
			Sys_Print(SYS_DLL::ENGINE, "\n");
			Sys_Print(SYS_DLL::ENGINE, "______________________________________________________________\n");
			Sys_Print(SYS_DLL::ENGINE, "] RSON_DISK_PATH ---------------------------------------------\n");
			Sys_Print(SYS_DLL::ENGINE, "] '%s'\n", filepath);
			Sys_Print(SYS_DLL::ENGINE, "--------------------------------------------------------------\n");
			Sys_Print(SYS_DLL::ENGINE, "\n");
		}
		return SQVM_LoadRson(filepath);
	}
	else
	{
		if (g_bClassInitialized && g_pCvar->FindVar("sq_showrsonloading")->m_iValue > 0)
		{
			Sys_Print(SYS_DLL::ENGINE, "\n");
			Sys_Print(SYS_DLL::ENGINE, "______________________________________________________________\n");
			Sys_Print(SYS_DLL::ENGINE, "] RSON_VPK_PATH ----------------------------------------------\n");
			Sys_Print(SYS_DLL::ENGINE, "] '%s'\n", rson_name);
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

	if (g_bClassInitialized && g_pCvar->FindVar("sq_showscriptloading")->m_iValue > 0)
	{
		spdlog::debug("Native(E):Loading SQVM Script '{}'\n", filepath);
	}

	// Returns true if the script exists on the disk
	if (FileExists(filepath) && SQVM_LoadScript(sqvm, filepath, script_name, flag))
	{
		return true;
	}

	if (g_bClassInitialized && g_pCvar->FindVar("sq_showscriptloading")->m_iValue > 0)
	{
		spdlog::debug("Native(E):FAILED. Try SP / VPK for '{}'\n", filepath);
	}

	///////////////////////////////////////////////////////////////////////////////
	return SQVM_LoadScript(sqvm, script_path, script_name, flag);
}

void SQVM_RegisterFunction(void* sqvm, const char* name, const char* helpString, const char* retValType, const char* argTypes, void* funcPtr)
{
	static ADDRESS Script_RegisterFunction = ADDRESS(0x141056040); // TODO: CONVERT TO SIG!

	SQFuncRegistration* func = new SQFuncRegistration();

	func->scriptName = name;
	func->nativeName = name;
	func->helpString = helpString;
	func->retValType = retValType;
	func->argTypes = argTypes;
	func->funcPtr = funcPtr;

	Script_RegisterFunction.RCast<void(*)(void*, SQFuncRegistration*, char unk)>()(sqvm, func, 1);
}

int SQVM_NativeTest(void* sqvm)
{
	// Function code goes here.
	return 1;
}

void RegisterUIScriptFunctions(void* sqvm)
{
	SQVM_RegisterFunction(sqvm, "UINativeTest", "native ui function", "void", "", &SQVM_NativeTest);
}

void RegisterClientScriptFunctions(void* sqvm)
{
	SQVM_RegisterFunction(sqvm, "ClientNativeTest", "native client function", "void", "", &SQVM_NativeTest);
}

void RegisterServerScriptFunctions(void* sqvm)
{
	SQVM_RegisterFunction(sqvm, "ServerNativeTest", "native server function", "void", "", &SQVM_NativeTest);
}

void AttachSQVMHooks()
{
	DetourAttach((LPVOID*)&SQVM_PrintFunc, &HSQVM_PrintFunc);
	DetourAttach((LPVOID*)&SQVM_WarningFunc, &HSQVM_WarningFunc);
	DetourAttach((LPVOID*)&SQVM_WarningCmd, &HSQVM_WarningCmd);
	DetourAttach((LPVOID*)&SQVM_LoadRson, &HSQVM_LoadRson);
	DetourAttach((LPVOID*)&SQVM_LoadScript, &HSQVM_LoadScript);
}

void DetachSQVMHooks()
{
	DetourDetach((LPVOID*)&SQVM_PrintFunc, &HSQVM_PrintFunc);
	DetourDetach((LPVOID*)&SQVM_WarningFunc, &HSQVM_WarningFunc);
	DetourDetach((LPVOID*)&SQVM_LoadRson, &HSQVM_LoadRson);
	DetourDetach((LPVOID*)&SQVM_LoadScript, &HSQVM_LoadScript);
}

///////////////////////////////////////////////////////////////////////////////
bool g_bSQVM_WarnFuncCalled;
