#pragma once
#include <thirdparty/spdlog/include/sinks/basic_file_sink.h>

/////////////////////////////////////////////////////////////////////////////
// Internals
BOOL FileExists(LPCTSTR szPath);
MODULEINFO GetModuleInfo(const char* szModule);
DWORD64 FindPatternV1(const char* szModule, const unsigned char* szPattern, const char* szMask);
DWORD64 FindPatternV2(const char* szModule, const unsigned char* szPattern, const char* szMask);

/////////////////////////////////////////////////////////////////////////////
// Utility
void DbgPrint(LPCSTR sFormat, ...);
void HexDump(const char* szHeader, int nFunc, const void* pData, int nSize);
std::string base64_encode(const std::string& in);
std::string base64_decode(const std::string& in);

/////////////////////////////////////////////////////////////////////////////