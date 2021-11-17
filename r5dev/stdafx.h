#pragma once
#pragma message("Pre-compiling headers.\n")

#define WIN32_LEAN_AND_MEAN // Prevent winsock2 redefinition.
#include <windows.h>
#include <detours.h>
#include <WinSock2.h>
#include <thread>
#include <fstream>
#include <stdio.h>
#include <filesystem>
#include <sstream>
#include <shlobj.h>
#include <objbase.h>
#include <cassert>
#include <iomanip>
#include <iostream>
#include <d3d11.h>
#include <string>
#include <Psapi.h>
#include <emmintrin.h>
#include <vector>
#include <cmath>

// Our headers
#include "imgui.h"
#include "imgui_utility.h"
#include "imgui_stdlib.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"
#include "spdlog.h"
#include "sinks/basic_file_sink.h"
#include "sinks/stdout_sinks.h"
#include "sinks/ostream_sink.h"
#include "utility.h"
#include "address.h"
#include "classes.h"
#include "httplib.h"
#include "json.hpp"

class IDetour
{
public:
	virtual ~IDetour() { ; }
	virtual void debug() = 0;
	virtual void attach() = 0;
	virtual void detach() = 0;
};

namespace
{
	Module g_mGameDll            = Module("r5apex.exe");
	Module g_mRadVideoToolsDll   = Module("bink2w64.dll");
	Module g_mRadAudioDecoderDll = Module("binkawin64.dll");
	Module g_mRadAudioSystemDll  = Module("mileswin64.dll");

	std::vector<IDetour*> vdetour;
	std::int64_t ppadding = 0;

	size_t addTest(IDetour* id)
	{
		vdetour.push_back(id);
		return vdetour.size();
	}
}

#define XREGISTER2(x,y)	static size_t dummy_reg_##y = addTest( new x() );
#define XREGISTER(x,y)	XREGISTER2(x, y)
#define REGISTER(x)		XREGISTER(x, __LINE__)

class H : public IDetour
{
	virtual void attach()
	{
		//
	}
	virtual void detach()
	{
		//
	}
	virtual void debug()
	{
		//
	}
};
REGISTER(H);

//#include "address.h"
/*
#define FUNC_AT_ADDRESS(name, funcbody, addr) \
   using _##name = funcbody; \
   _##name name = (funcbody)addr \

#define PRINT_ADDRESS(name, address) std::cout << name << ": " << std::hex << std::uppercase << address << std::endl;
*/