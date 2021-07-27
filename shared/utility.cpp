#include "pch.h"

#include "utility.h"
#include "hooks.h"

/*-----------------------------------------------------------------------------
 * _utility.cpp
 *-----------------------------------------------------------------------------*/

///////////////////////////////////////////////////////////////////////////////
//
BOOL FileExists(LPCTSTR szPath)
{
    DWORD dwAttrib = GetFileAttributes(szPath);

    return (dwAttrib != INVALID_FILE_ATTRIBUTES &&
        !(dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
}

///////////////////////////////////////////////////////////////////////////////
// For getting information about the executing module
MODULEINFO GetModuleInfo(const char* szModule)
{
    MODULEINFO modinfo = { 0 };
    HMODULE hModule = GetModuleHandle(szModule);
    if (hModule == 0)
    {
        return modinfo;
    }
    GetModuleInformation(GetCurrentProcess(), hModule, &modinfo, sizeof(MODULEINFO));
    return modinfo;
}

///////////////////////////////////////////////////////////////////////////////
// For finding a byte pattern in memory of the game process
BOOL Compare(const unsigned char* pData, const unsigned char* szPattern, const char* szMask)
{
    for (; *szMask; ++szMask, ++pData, ++szPattern)
    {
        if (*szMask == 'x' && *pData != *szPattern)
        {
            return false;
        }
    }
    return (*szMask) == NULL;
}
DWORD64 FindPatternV1(const char* szModule, const unsigned char* szPattern, const char* szMask)
{
    MODULEINFO mInfo = GetModuleInfo(szModule);
    DWORD64 dwAddress = (DWORD64)mInfo.lpBaseOfDll;
    DWORD64 dwLen = (DWORD64)mInfo.SizeOfImage;

    size_t maskLen = strlen(szMask);
    for (int i = 0; i < dwLen - maskLen; i++)
    {
        if (Compare((unsigned char*)(dwAddress + i), szPattern, szMask))
        {
            return (DWORD64)(dwAddress + i);
        }
    }
    return 0;
}

///////////////////////////////////////////////////////////////////////////////
// For finding a pattern in memory of the game process with SIMD
DWORD64 FindPatternV2(const char* szModule, const unsigned char* szPattern, const char* szMask)
{
    MODULEINFO mInfo = GetModuleInfo(szModule);
    DWORD64 base = (DWORD64)mInfo.lpBaseOfDll;
    DWORD64 size = (DWORD64)mInfo.SizeOfImage;

    unsigned char* pData = (unsigned char*)base;
    unsigned int length  = (unsigned int)size;

    const unsigned char* end = pData + length - strlen(szMask);
    int num_masks = (int)ceil((float)strlen(szMask) / (float)16);
    int masks[32]; // 32*16 = enough masks for 512 bytes
    memset(masks, 0, num_masks * sizeof(int));
    for (__int64 i = 0; i < num_masks; ++i)
    {
        for (__int64 j = strnlen(szMask + i * 16, 16) - 1; j >= 0; --j)
        {
            if (szMask[i * 16 + j] == 'x')
            {
                masks[i] |= 1 << j;
            }
        }
    }
    __m128i xmm1 = _mm_loadu_si128((const __m128i*) szPattern);
    __m128i xmm2, xmm3, msks;
    for (; pData != end; _mm_prefetch((const char*)(++pData + 64), _MM_HINT_NTA))
    {
        if (szPattern[0] == pData[0])
        {
            xmm2 = _mm_loadu_si128((const __m128i*) pData);
            msks = _mm_cmpeq_epi8(xmm1, xmm2);
            if ((_mm_movemask_epi8(msks) & masks[0]) == masks[0])
            {
                for (DWORD64 i = 1; i < num_masks; ++i)
                {
                    xmm2 = _mm_loadu_si128((const __m128i*) (pData + i * 16));
                    xmm3 = _mm_loadu_si128((const __m128i*) (szPattern + i * 16));
                    msks = _mm_cmpeq_epi8(xmm2, xmm3);
                    if ((_mm_movemask_epi8(msks) & masks[i]) == masks[i])
                    {
                        if ((i + 1) == num_masks)
                        {
                            return (DWORD64)pData;
                        }
                    }
                    else goto cont;
                }
                return (DWORD64)pData;
            }
        }cont:;
    }
    return NULL;
}

///////////////////////////////////////////////////////////////////////////////
// For finding a string pattern in memory of the game process
std::uint8_t* PatternScan(const char* module, const char* signature)
{
    static auto PatternToBytes = [](const char* pattern)
    {
        char* PatternStart = const_cast<char*>(pattern); // Cast const away and get start of pattern.
        char* PatternEnd = PatternStart + std::strlen(pattern); // Get end of pattern.

        std::vector<std::int32_t> Bytes = std::vector<std::int32_t>{ }; // Initialize byte vector.

        for (char* CurrentByte = PatternStart; CurrentByte < PatternEnd; ++CurrentByte)
        {
            if (*CurrentByte == '?') // Is current char(byte) a wildcard?
            {
                ++CurrentByte; // Skip 1 character.

                if (*CurrentByte == '?') // Is it a double wildcard pattern?
                {
                    ++CurrentByte; // If so skip the next space that will come up so we can reach the next byte.
                }

                Bytes.push_back(-1); // Push the byte back as invalid.
            }
            else
            {
                // https://stackoverflow.com/a/43860875/12541255
                // Here we convert our string to a unsigned long integer. We pass our string then we use 16 as the base because we want it as hexadecimal.
                // Afterwards we push the byte into our bytes vector.
                Bytes.push_back(std::strtoul(CurrentByte, &CurrentByte, 16));
            }
        }
        return Bytes;
    };

    const MODULEINFO mInfo = GetModuleInfo(module); // Get module info.
    const DWORD64 SizeOfModule = (DWORD64)mInfo.SizeOfImage; // Grab the module size.
    std::uint8_t* ScanBytes = reinterpret_cast<std::uint8_t*>(mInfo.lpBaseOfDll); // Get the base of the module.

    const std::vector<int> PatternBytes = PatternToBytes(signature); // Convert our pattern to a byte array.
    const std::pair BytesInfo = std::make_pair(PatternBytes.size(), PatternBytes.data()); // Get the size and data of our bytes.

    for (DWORD i = 0ul; i < SizeOfModule - BytesInfo.first; ++i)
    {
        bool FoundAddress = true;

        for (DWORD j = 0ul; j < BytesInfo.first; ++j)
        {
            // If either the current byte equals to the byte in our pattern or our current byte in the pattern is a wildcard
            // our if clause will be false.
            if (ScanBytes[i + j] != BytesInfo.second[j] && BytesInfo.second[j] != -1)
            {
                FoundAddress = false;
                break;
            }
        }

        if (FoundAddress)
        {
            return &ScanBytes[i];
        }

    }

    return nullptr;
}

///////////////////////////////////////////////////////////////////////////////
//
void DbgPrint(LPCSTR sFormat, ...)
{
    CHAR sBuffer[512] = { 0 };
    va_list sArgs;

    // Get the variable arg pointer.
    va_start(sArgs, sFormat);

    // Format print the string.
    int length = vsnprintf(sBuffer, sizeof(sBuffer), sFormat, sArgs);
    va_end(sArgs);

    // Output the string to the debugger.
    OutputDebugString(sBuffer);
}

///////////////////////////////////////////////////////////////////////////////
// For dumping data from a buffer to a file on the disk
void HexDump(const char* szHeader, int nFunc, const void* pData, int nSize)
{
    static std::atomic<int> i, j, k = 0;
    static char ascii[17] = { 0 };
    static auto logger = spdlog::get("default_logger");
    auto pattern = std::make_unique<spdlog::pattern_formatter>("%v", spdlog::pattern_time_type::local, std::string(""));

    // Loop until the function returned to the first caller.
    while (k == 1) { /*Sleep(75);*/ }

    k = 1;
    ascii[16] = '\0';

    // Add new loggers here to replace the placeholder.
    if (nFunc == 0) { logger = g_spdnetchan_logger; }

    // Add timestamp.
    logger->set_level(spdlog::level::trace);
    logger->set_pattern("%v [%H:%M:%S.%f]");
    logger->trace("---------------------------------------------------------");

    // Disable EOL and create block header.
    logger->set_formatter(std::move(pattern));
    logger->trace("{:s} ---- LEN BYTES: {}\n:\n", szHeader, nSize);
    logger->trace("--------  0  1  2  3  4  5  6  7   8  9  A  B  C  D  E  F  0123456789ABCDEF\n");

    // Output the buffer to the file.
    for (i = 0; i < nSize; i++)
    {
        if (i % nSize == 0) { logger->trace(" 0x{:04X}  ", i); }
        logger->trace("{:02x} ", ((unsigned char*)pData)[i]);

        if (((unsigned char*)pData)[i] >= ' ' && ((unsigned char*)pData)[i] <= '~') { ascii[i % 16] = ((unsigned char*)pData)[i]; }
        else { ascii[i % 16] = '.'; }

        if ((i + 1) % 8 == 0 || i + 1 == nSize)
        {
            logger->trace(" ");

            if ((i + 1) % 16 == 0)
            {
                if (i + 1 == nSize)
                {
                    logger->trace("{:s}\n", ascii);
                    logger->trace("---------------------------------------------------------------------------\n\n");
                }
                else
                {
                    i++;
                    logger->trace("{:s}\n ", ascii);
                    logger->trace("0x{:04X}  ", i--);
                }
            }
            else if (i + 1 == nSize)
            {
                ascii[(i + 1) % 16] = '\0';
                if ((i + 1) % 16 <= 8)
                {
                    logger->trace(" ");
                }
                for (j = (i + 1) % 16; j < 16; j++)
                {
                    logger->trace("   ");
                }
                logger->trace("{:s}\n", ascii);
                logger->trace("---------------------------------------------------------------------------\n\n");
            }
        }
    }
    k = 0;
    ///////////////////////////////////////////////////////////////////////////
}

unsigned __int64 __fastcall RHash(DWORD* a1)
{
    DWORD* v1;           // r8
    unsigned __int64 v2; // r10
    int v3;              // er11
    unsigned int v4;     // er9
    unsigned int i;      // edx
    __int64 v6;          // rcx
    int v7;              // er9
    int v8;              // edx
    int v9;              // eax
    unsigned int v10;    // er8
    int v12;             // ecx

    v1 = a1;
    v2 = 0i64;
    v3 = 0;
    v4 = (*a1 - 45 * ((~(*a1 ^ 0x5C5C5C5Cu) >> 7) & (((*a1 ^ 0x5C5C5C5Cu) - 0x1010101) >> 7) & 0x1010101)) & 0xDFDFDFDF;
    for (i = ~*a1 & (*a1 - 0x1010101) & 0x80808080; !i; i = v8 & 0x80808080)
    {
        v6 = v4;
        v7 = v1[1];
        ++v1;
        v3 += 4;
        v2 = ((((unsigned __int64)(0xFB8C4D96501i64 * v6) >> 24) + 0x633D5F1 * v2) >> 61) ^ (((unsigned __int64)(0xFB8C4D96501i64 * v6) >> 24)
            + 0x633D5F1 * v2);
        v8 = ~v7 & (v7 - 0x1010101);
        v4 = (v7 - 45 * ((~(v7 ^ 0x5C5C5C5Cu) >> 7) & (((v7 ^ 0x5C5C5C5Cu) - 0x1010101) >> 7) & 0x1010101)) & 0xDFDFDFDF;
    }
    v9 = -1;
    v10 = (i & -(signed)i) - 1;
    if (_BitScanReverse((unsigned long*)&v12, v10))
    {
        v9 = v12;
    }
    return 0x633D5F1 * v2 + ((0xFB8C4D96501i64 * (unsigned __int64)(v4 & v10)) >> 24) - 0xAE502812AA7333i64 * (unsigned int)(v3 + v9 / 8);
}

/*
//static void HexDump(const void* pData, void* addr, int len)
void HexDump(const char* szHeader, const char* szFile, int nFunc, const void* pData, int nSize)
{
    static int i = 0;
    static int j = 0;
    static int k = 0;
    static unsigned char hexBuf[1028];
    static unsigned char* pc = (unsigned char*)pData;

    static auto logger = spdlog::get("default_logger");
    auto pattern = std::make_unique<spdlog::pattern_formatter>("%v", spdlog::pattern_time_type::local, std::string(""));

    // Loop until the function returned to the first caller.
    while (k == 1) { Sleep(75); }
    k = 1;

    // Add new loggers here to replace the placeholder.
    if (nFunc == 0) { logger = spdlog::get("netchan_logger"); }

    // Add timestamp before block header.
    logger->set_level(spdlog::level::trace);
    logger->set_pattern("%v [%H:%M:%S.%f]");
    logger->trace("---------------------------------------------------------");

    // Disable EOL and create block header.
    logger->set_formatter(std::move(pattern));
    logger->trace("{:s} ---- LEN BYTES: {}\n:\n", szHeader, nSize);
    logger->trace("--------  0  1  2  3  4  5  6  7   8  9  A  B  C  D  E  F  0123456789ABCDEF\n");

    // Process all bytes in the buffer.
    for (i = 0; i < nSize; i++)
    {
        if ((i) % 8 == 0 || i == nSize)
        {
            if (i != 0)
            {
                if (!(i % 16) == 0)
                {
                    logger->trace(" ");
                }
            }
        }
        // New line when multiple of 16 has been reached.
        if ((i % 16) == 0)
        {
            // Don't print ASCII for the zeroth line.
            if (i != 0)
            {
                logger->trace("  {:s}\n", hexBuf);
            }
            // Output the offset.
            logger->trace(" 0x{:04X} ", i);
        }

        // Print the hex code for the specific character.
        logger->trace(" {:02x}", pc[i]);

        // Store a printable ASCII character for later.
        if ((pc[i] < 0x20) || (pc[i] > 0x7e))
        {
            hexBuf[i % 16] = '.';
        }
        else
        {
            hexBuf[i % 16] = pc[i];
        }

        hexBuf[(i % 16) + 1] = '\0';

        for (j = (i + 1) % 16; j < 16; j++)
        {
            logger->trace("   ");
        }
    }

    // Pad out last line if not exactly 16 characters.
    //while ((i % 16) != 0)
    //{
    //    logger->trace("   ");
    //    i++;
    //}

    // Print the final ASCII data.
    logger->trace("  {:s}\n", hexBuf);
    logger->trace("-------------------------------------------------------------------------\n");
    logger->trace("\n");
    k = 0;
}
*/
