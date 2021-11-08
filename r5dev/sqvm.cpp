#include "stdafx.h"
#include "sqvm.h"
#include "CEngineVGui.h"
#include "IConVar.h"

static std::ostringstream sqvm_p_oss;
static auto sqvm_p_ostream_sink = std::make_shared<spdlog::sinks::ostream_sink_st>(sqvm_p_oss);
static auto sqvm_p_log_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("platform/logs/SQVM_Print.log", true);
//---------------------------------------------------------------------------------
// Purpose: prints the output of each VM to the console
//---------------------------------------------------------------------------------
void* HSQVM_PrintFunc(void* sqvm, char* fmt, ...)
{
	static bool initialized = false;
	static char buf[1024];
	static std::string vmType[3] = { "Script(S):", "Script(C):", "Script(U):" };

	int vmIdx = *(int*)((std::uintptr_t)sqvm + 0x18);
	std::string vmStr = vmType[vmIdx].c_str();

	sqvm_p_oss.str("");
	sqvm_p_oss.clear();

	static spdlog::logger logger("sqvm_print", { sqvm_p_log_sink, sqvm_p_ostream_sink });

	if (!initialized)
	{
		sqvm_p_log_sink->set_level(spdlog::level::debug);
		sqvm_p_ostream_sink->set_level(spdlog::level::debug);
		logger.set_level(spdlog::level::debug);
		logger.set_pattern("[%S.%e] %v");

		initialized = true;
	}

	va_list args;
	va_start(args, fmt);

	vsnprintf(buf, sizeof(buf), fmt, args);

	buf[sizeof(buf) - 1] = 0;
	va_end(args);

	vmStr.append(buf);

	spdlog::debug(vmStr);
	logger.debug(vmStr);

	std::string s = sqvm_p_oss.str();
	const char* c = s.c_str();

	g_LogSystem.AddLog((LogType_t)vmIdx, s);

	Items.push_back(Strdup(c));

	return NULL;
}

static bool g_bSQVM_WarnFuncCalled;
static std::ostringstream sqvm_w_oss;
static auto ostream_sink_warning = std::make_shared<spdlog::sinks::ostream_sink_st>(sqvm_w_oss);
static auto log_sink_warning = std::make_shared<spdlog::sinks::basic_file_sink_mt>("platform/logs/SQVM_Warning.txt", true);
//---------------------------------------------------------------------------------
// Purpose: prints the warning output of each VM to the console
//---------------------------------------------------------------------------------
__int64 HSQVM_WarningFunc(void* sqvm, int a2, int a3, int* stringSize, void** string)
{
	__int64 result = SQVM_WarningFunc(sqvm, a2, a3, stringSize, string);

	void* retaddr = _ReturnAddress(); // Get return address.

	if (g_bSQVM_WarnFuncCalled) // Check if its SQVM_Warning calling.
	{
		return result; // If not return.
	}

	static bool initialized = false;
	static std::string vmType[3] = { "Script(S):WARNING: ", "Script(C):WARNING: ", "Script(U):WARNING: " };

	int vmIdx = *(int*)((std::uintptr_t)sqvm + 0x18);
	std::string vmStr = vmType[vmIdx].c_str();

	sqvm_w_oss.str("");
	sqvm_w_oss.clear();

	static spdlog::logger logger("sqvm_warning", { log_sink_warning, ostream_sink_warning });

	log_sink_warning->set_pattern("[%S.%e] %v\n");
	ostream_sink_warning->set_pattern("[%S.%e] %v\n");
	logger.set_pattern("[%S.%e] %v\n");
	if (!initialized)
	{
		log_sink_warning->set_level(spdlog::level::debug);
		log_sink_warning->set_pattern("[%S.%e] %v\n");
		ostream_sink_warning->set_level(spdlog::level::debug);
		ostream_sink_warning->set_pattern("[%S.%e] %v\n");
		initialized = true;
	}

	std::string stringConstructor((char*)*string, *stringSize); // Get string from memory via std::string constructor.
	vmStr.append(stringConstructor);

	spdlog::info(vmStr + "\n");
	logger.debug(vmStr + "\n");

	std::string s = sqvm_w_oss.str();
	const char* c = s.c_str();

	g_LogSystem.AddLog((LogType_t)vmIdx, s);

	Items.push_back(Strdup(c));
	g_bSQVM_WarnFuncCalled = false;

	return result;
}

//---------------------------------------------------------------------------------
// Purpose:
//---------------------------------------------------------------------------------
__int64 HSQVM_WarningCmd(int a1, int a2)
{
	g_bSQVM_WarnFuncCalled = true;
	return SQVM_WarningCmd(a1, a2);
}

//---------------------------------------------------------------------------------
// Purpose: loads the include file from the mods directory
//---------------------------------------------------------------------------------
__int64 HSQVM_LoadRson(const char* rson_name)
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
			printf("\n");
			printf("##################################################\n");
			printf("] '%s'\n", filepath);
			printf("##################################################\n");
			printf("\n");
		}
		return SQVM_LoadRson(filepath);
	}
	else
	{
		if (g_bClassInitialized && g_pCvar->FindVar("sq_showrsonloading")->m_iValue > 0)
		{
			printf("\n");
			printf("##################################################\n");
			printf("] '%s'\n", rson_name);
			printf("##################################################\n");
			printf("\n");
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
		printf("Native(E):Loading SQVM Script '%s'\n", filepath);
	}

	// Returns true if the script exists on the disk
	if (FileExists(filepath) && SQVM_LoadScript(sqvm, filepath, script_name, flag))
	{
		return true;
	}

	if (g_bClassInitialized && g_pCvar->FindVar("sq_showscriptloading")->m_iValue > 0)
	{
		printf("Native(E):FAILED. Try SP / VPK for '%s'\n", filepath);
	}

	///////////////////////////////////////////////////////////////////////////////
	return SQVM_LoadScript(sqvm, script_path, script_name, flag);
}

void SQVM_RegisterFunction(void* sqvm, const char* name, const char* helpString, const char* retValType, const char* argTypes, void* funcPtr)
{
	static Address Script_RegisterFunction = Address(0x141056040); // TODO: CONVERT TO SIG!

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
