#include "core/stdafx.h"
#include "core/logdef.h"
#include "public/include/utility.h"

/*-----------------------------------------------------------------------------
 * _utility
 *-----------------------------------------------------------------------------*/

///////////////////////////////////////////////////////////////////////////////
// For checking if a specific file exists.
BOOL FileExists(LPCTSTR szPath)
{
    DWORD dwAttrib = GetFileAttributes(szPath);

    return (dwAttrib != INVALID_FILE_ATTRIBUTES &&
        !(dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
}

///////////////////////////////////////////////////////////////////////////////
// For getting information about the executing module.
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
// For finding a byte pattern in memory of the process.
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
// For finding a pattern in memory of the process with SIMD.
DWORD64 FindPatternSIMD(const char* szModule, const unsigned char* szPattern, const char* szMask)
{
    MODULEINFO mInfo = GetModuleInfo(szModule);
    DWORD64 base = (DWORD64)mInfo.lpBaseOfDll;
    DWORD64 size = (DWORD64)mInfo.SizeOfImage;

    unsigned char* pData = (unsigned char*)base;
    unsigned int length  = (unsigned int)size;

    const unsigned char* end = pData + length - strlen(szMask);
    int num_masks = (int)ceil((float)strlen(szMask) / (float)16);
    int masks[32]; // 32*16 = enough masks for 512 bytes.
    memset(masks, 0, num_masks * sizeof(int));
    for (int64_t i = 0; i < num_masks; ++i)
    {
        for (int64_t j = strnlen(szMask + i * 16, 16) - 1; j >= 0; --j)
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
// For printing output to the debugger.
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
    static unsigned char ascii[17] = { 0 };
    static auto logger = spdlog::get("default_logger");

    // Loop until the function returned to the first caller.
    while (k == 1) { /*Sleep(75);*/ }

    k = 1;
    ascii[16] = '\0';

    // Add new loggers here to replace the placeholder.
    if (nFunc == 0) { logger = g_spd_netchan_logger; }

    // Add timestamp.
    logger->set_level(spdlog::level::trace);
    logger->set_pattern("%v [%H:%M:%S.%f]\n");
    logger->trace("---------------------------------------------------------");

    // Disable EOL and create block header.
    logger->set_pattern("%v");
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
                    logger->trace("---------------------------------------------------------------------------\n");
                    logger->trace("\n");
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
                logger->trace("---------------------------------------------------------------------------\n");
                logger->trace("\n");
            }
        }
    }
    k = 0;
    ///////////////////////////////////////////////////////////////////////////
}

///////////////////////////////////////////////////////////////////////////////
// For encoding data in base64.
std::string base64_encode(const std::string& in)
{
    std::string out;
    int val = 0, valb = -6;

    for (unsigned char c : in)
    {
        val = (val << 8) + c;
        valb += 8;
        while (valb >= 0)
        {
            out.push_back("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[(val >> valb) & 0x3F]);
            valb -= 6;
        }
    }
    if (valb > -6)
    {
        out.push_back("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[((val << 8) >> (valb + 8)) & 0x3F]);
    }
    while (out.size() % 4)
    {
        out.push_back('=');
    }
    return out;
}

///////////////////////////////////////////////////////////////////////////////
// For decoding data in base64.
std::string base64_decode(const std::string& in)
{
    std::string out;
    int val = 0, valb = -8;

    std::vector<int> T(256, -1);
    for (int i = 0; i < 64; i++)
    {
        T["ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[i]] = i;
    }

    for (unsigned char c : in)
    {
        if (T[c] == -1)
        {
            break;
        }
        val = (val << 6) + T[c];
        valb += 6;
        if (valb >= 0)
        {
            out.push_back(char((val >> valb) & 0xFF));
            valb -= 8;
        }
    }
    return out;
}
