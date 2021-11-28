#pragma once
#pragma message("Pre-compiling headers.\n")

#define WIN32_LEAN_AND_MEAN // Prevent winsock2 redefinition.
#include <windows.h>
#include <WinSock2.h>

#include <stdio.h>
#include <Psapi.h>
#include <shlobj.h>
#include <objbase.h>
#include <emmintrin.h>
#include <cmath>
#include <vector>
#include <thread>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <cassert>
#include <filesystem>

#if !defined(DEDICATED)
#include <d3d11.h>
#endif // !DEDICATED

#include "thirdparty/detours/include/detours.h"
#include "thirdparty/detours/include/idetour.h"

#if !defined(DEDICATED)
#include "thirdparty/imgui/include/imgui.h"
#include "thirdparty/imgui/include/imgui_utility.h"
#include "thirdparty/imgui/include/imgui_stdlib.h"
#include "thirdparty/imgui/include/imgui_impl_dx11.h"
#include "thirdparty/imgui/include/imgui_impl_win32.h"
#endif // !DEDICATED

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
	MODULE g_mGameDll            = MODULE("r5apex.exe");
	MODULE g_mRadVideoToolsDll   = MODULE("bink2w64.dll");
	MODULE g_mRadAudioDecoderDll = MODULE("binkawin64.dll");
	MODULE g_mRadAudioSystemDll  = MODULE("mileswin64.dll");
}
#endif // SDKLAUNCHER
