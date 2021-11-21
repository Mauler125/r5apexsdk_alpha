#pragma once
#pragma message("Pre-compiling headers.\n")

#define WIN32_LEAN_AND_MEAN // Prevent winsock2 redefinition.
#include <windows.h>
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
#include <string>
#include <Psapi.h>
#include <emmintrin.h>
#include <vector>
#include <cmath>

#if !defined(DEDICATED)
#include <d3d11.h>
#endif // DEDICATED

#include "thirdparty/detours/include/detours.h"
#include "thirdparty/detours/include/idetour.h"

#if !defined(DEDICATED)
#include "thirdparty/imgui/include/imgui.h"
#include "thirdparty/imgui/include/imgui_utility.h"
#include "thirdparty/imgui/include/imgui_stdlib.h"
#include "thirdparty/imgui/include/imgui_impl_dx11.h"
#include "thirdparty/imgui/include/imgui_impl_win32.h"
#endif // DEDICATED

#include "thirdparty/spdlog/include/spdlog.h"
#include "thirdparty/spdlog/include/sinks/basic_file_sink.h"
#include "thirdparty/spdlog/include/sinks/stdout_sinks.h"
#include "thirdparty/spdlog/include/sinks/ostream_sink.h"
#include "public/include/utility.h"
#include "public/include/address.h"
#include "public/include/httplib.h"
#include "public/include/json.hpp"

#ifndef SDKLAUNCHER
namespace
{
	Module g_mGameDll            = Module("r5apex.exe");
	Module g_mRadVideoToolsDll   = Module("bink2w64.dll");
	Module g_mRadAudioDecoderDll = Module("binkawin64.dll");
	Module g_mRadAudioSystemDll  = Module("mileswin64.dll");
}
#endif // SDKLAUNCHER

//#include "address.h"
/*
#define FUNC_AT_ADDRESS(name, funcbody, addr) \
   using _##name = funcbody; \
   _##name name = (funcbody)addr \

#define PRINT_ADDRESS(name, address) std::cout << name << ": " << std::hex << std::uppercase << address << std::endl;
*/