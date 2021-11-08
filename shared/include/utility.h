#pragma once
#include <Windows.h>
#include <Psapi.h>
#include <sinks/basic_file_sink.h>

/////////////////////////////////////////////////////////////////////////////
// Internals
BOOL FileExists(LPCTSTR szPath);
MODULEINFO GetModuleInfo(const char* szModule);
DWORD64 FindPatternV1(const char* szModule, const unsigned char* szPattern, const char* szMask);
DWORD64 FindPatternV2(const char* szModule, const unsigned char* szPattern, const char* szMask);

/////////////////////////////////////////////////////////////////////////////
// Utility
void DbgPrint(LPCSTR sFormat, ...);
void HexDump(const char* szHeader, const char* szLogger, const void* pData, int nSize);

/////////////////////////////////////////////////////////////////////////////
