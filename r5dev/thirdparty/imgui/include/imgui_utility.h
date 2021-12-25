#pragma once

/////////////////////////////////////////////////////////////////////////////
// Internals
int   Stricmp(const char* s1, const char* s2);
int   Strnicmp(const char* s1, const char* s2, int n);
char* Strdup(const char* s);
void  Strtrim(char* s);

class ImGuiConfig
{
public:
    struct
    {
        int m_nBind0          = VK_OEM_3;
        int m_nBind1          = VK_INSERT;
        int m_nAutoClearLimit = 300;
        bool m_bAutoClear     = true;
    } IConsole_Config;

    struct
    {
        int m_nBind0 = VK_HOME;
        int m_nBind1 = VK_F10;
    } IBrowser_Config;

    void Load();
    void Save();
};

extern ImGuiConfig* g_pImGuiConfig;
