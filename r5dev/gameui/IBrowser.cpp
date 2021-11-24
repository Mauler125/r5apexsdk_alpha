#include "core/stdafx.h"
#include "core/init.h"
#include "core/resource.h"
#include "tier0/IConVar.h"
#include "windows/id3dx.h"
#include "windows/console.h"
#include "engine/net_chan.h"
#include "engine/sys_utils.h"
#include "engine/host_state.h"
#include "client/IVEngineClient.h"
#include "networksystem/serverlisting.h"
#include "networksystem/r5net.h"
#include "vpc/keyvalues.h"
#include "tier0/ConCommandCallback.h"
#include "squirrel/sqinit.h"
#include "gameui/IBrowser.h"

/******************************************************************************
-------------------------------------------------------------------------------
File   : IBrowser.cpp
Date   : 09:06:2021
Author : Sal
Purpose: Implements the in-game server browser frontend
-------------------------------------------------------------------------------
History:
- 09:06:2021   21:07 : Created by Sal
- 25:07:2021   14:26 : Implement private servers connect dialog and password field

******************************************************************************/

IBrowser::IBrowser() : m_szMatchmakingServerString("r5a-comp-sv.herokuapp.com"), r5net(new R5Net::Client("r5a-comp-sv.herokuapp.com"))
{
    memset(m_chServerConnStringBuffer, 0, sizeof(m_chServerConnStringBuffer));

    std::string path = "stbsp";
    for (const auto& entry : std::filesystem::directory_iterator(path))
    {
        std::string filename = entry.path().string();
        int slashPos = filename.rfind("\\", std::string::npos);
        filename = filename.substr(static_cast<std::basic_string<char, std::char_traits<char>, std::allocator<char>>::size_type>(slashPos) + 1, std::string::npos);
        filename = filename.substr(0, filename.size() - 6);

        auto it = mapArray.find(filename); // Find MapName in mapArray.
        if (it != mapArray.end())
        {
            m_vszMapsList.push_back(it->second);
        }
        else
        {
            m_vszMapsList.push_back(filename);
        }
    }

    m_Server.map = m_vszMapsList[0];
    static std::thread hostingServerRequestThread([this]()
    {
        while (true)
        {
            UpdateHostingStatus();
            std::this_thread::sleep_for(std::chrono::milliseconds(5000));
        }
    });

    hostingServerRequestThread.detach();

    static HGLOBAL rcData = NULL;
    HMODULE handle;
    GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, (LPCSTR)"unnamed", &handle);
    HRSRC rc = FindResource(handle, MAKEINTRESOURCE(IDB_PNG1), MAKEINTRESOURCE(PNG));

    if (rc != NULL) { rcData = LoadResource(handle, rc); }
    else { assert(rc == NULL); }
    if (rcData != NULL) { m_vucLockedIconBlob = (std::vector<unsigned char>*)LockResource(rcData); }
    else { assert(rcData == NULL); }
}

IBrowser::~IBrowser()
{
    delete r5net;
}

void IBrowser::UpdateHostingStatus()
{
    if (!g_pHostState || !g_pCvar)
    {
        return;
    }

    eHostingStatus = g_pHostState->m_bActiveGame ? EHostStatus::HOSTING : EHostStatus::NOT_HOSTING; // Are we hosting a server?
    switch (eHostingStatus)
    {
    case EHostStatus::NOT_HOSTING:
    {
        m_szHostRequestMessage = "";
        m_iv4HostRequestMessageColor = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
        break;
    }
    case EHostStatus::HOSTING:
    {
        if (eServerVisibility == EServerVisibility::OFFLINE)
        {
            break;
        }

        if (*reinterpret_cast<std::int32_t*>(g_nRemoteFunctionCallsChecksum.GetPtr()) == NULL) // Check if script checksum is valid yet.
        {
            break;
        }

        switch (eServerVisibility)
        {

        case EServerVisibility::HIDDEN:
            m_Server.hidden = true;
            break;
        case EServerVisibility::PUBLIC:
            m_Server.hidden = false;
            break;
        default:
            break;
        }

        SendHostingPostRequest();
        break;
    }
    default:
        break;
    }
}

void IBrowser::RefreshServerList()
{
    m_vServerList.clear();

    static bool bThreadLocked = false;

    if (!bThreadLocked)
    {
        std::thread t([this]()
        {
            Sys_Print(SYS_DLL::CLIENT, "Refreshing server list with string '%s'\n", m_szMatchmakingServerString.c_str());
            bThreadLocked = true;
            m_vServerList = r5net->GetServersList(m_szServerListMessage);
            bThreadLocked = false;
        });

        t.detach();
    }
}

void IBrowser::SendHostingPostRequest()
{
    m_szHostToken = std::string();
    Sys_Print(SYS_DLL::CLIENT, "Sending PostServerHost request\n");
    bool result = r5net->PostServerHost(m_szHostRequestMessage, m_szHostToken,
        ServerListing{
            m_Server.name,
            std::string(g_pHostState->m_levelName),
            "",
            g_pCvar->FindVar("hostport")->m_pzsCurrentValue,
            g_pCvar->FindVar("mp_gamemode")->m_pzsCurrentValue,
            m_Server.hidden,
            std::to_string(*reinterpret_cast<std::int32_t*>(g_nRemoteFunctionCallsChecksum.GetPtr())),

            std::string(),
            g_szNetKey.c_str()
        }
    );

    if (result)
    {
        m_iv4HostRequestMessageColor = ImVec4(0.00f, 1.00f, 0.00f, 1.00f);
        std::stringstream msg;
        msg << "Broadcasting! ";
        if (!m_szHostToken.empty())
        {
            msg << "Share the following token for clients to connect: ";
        }
        m_szHostRequestMessage = msg.str().c_str();
    }
    else
    {
        m_iv4HostRequestMessageColor = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
    }
}


void IBrowser::CompMenu()
{
    ImGui::BeginTabBar("CompMenu");
    if (ImGui::TabItemButton("Server Browser"))
    {
        SetSection(ESection::SERVER_BROWSER);
    }
    if (ImGui::TabItemButton("Host Server"))
    {
        SetSection(ESection::HOST_SERVER);
    }
    if (ImGui::TabItemButton("Settings"))
    {
        SetSection(ESection::SETTINGS);
    }
    ImGui::EndTabBar();
}

void IBrowser::ServerBrowserSection()
{
    ImGui::BeginGroup();
    m_imServerBrowserFilter.Draw();
    ImGui::SameLine();
    if (ImGui::Button("Refresh List"))
    {
        RefreshServerList();
    }
    ImGui::EndGroup();
    ImGui::TextColored(ImVec4(1.00f, 0.00f, 0.00f, 1.00f), m_szServerListMessage.c_str());
    ImGui::Separator();

    const float FooterHeight = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
    ImGui::BeginChild("ServerListChild", { 0, -FooterHeight }, true, ImGuiWindowFlags_AlwaysVerticalScrollbar);
    if (ImGui::BeginTable("##ServerBrowser_ServerList", 5, ImGuiTableFlags_Resizable))
    {
        ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_WidthStretch, 20);
        ImGui::TableSetupColumn("Map", ImGuiTableColumnFlags_WidthStretch, 25);
        ImGui::TableSetupColumn("Port", ImGuiTableColumnFlags_WidthStretch, 5);
        ImGui::TableSetupColumn("Playlist", ImGuiTableColumnFlags_WidthStretch, 5);
        ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthStretch, 5);
        ImGui::TableHeadersRow();

        for (ServerListing& server : m_vServerList)
        {
            const char* name = server.name.c_str();
            const char* map = server.map.c_str();
            const char* port = server.port.c_str();
            const char* playlist = server.playlist.c_str();

            if (m_imServerBrowserFilter.PassFilter(name)
                || m_imServerBrowserFilter.PassFilter(map)
                || m_imServerBrowserFilter.PassFilter(port))
            {
                ImGui::TableNextColumn();
                ImGui::Text(name);

                ImGui::TableNextColumn();
                ImGui::Text(map);

                ImGui::TableNextColumn();
                ImGui::Text(port);

                ImGui::TableNextColumn();
                ImGui::Text(playlist);

                ImGui::TableNextColumn();
                std::string selectButtonText = "Connect##";
                selectButtonText += (server.name + server.ip + server.map);

                if (ImGui::Button(selectButtonText.c_str()))
                {
                    ConnectToServer(server.ip, server.port, server.netchanEncryptionKey);
                }
            }

        }
        ImGui::EndTable();
    }
    ImGui::EndChild();

    ImGui::Separator();
    ImGui::PushItemWidth(ImGui::GetWindowContentRegionWidth() / 4);
    {
        ImGui::InputTextWithHint("##ServerBrowser_ServerConnString", "Enter IP address or \"localhost\"", m_chServerConnStringBuffer, IM_ARRAYSIZE(m_chServerConnStringBuffer));

        ImGui::SameLine();
        ImGui::InputTextWithHint("##ServerBrowser_ServerEncKey", "Enter encryption key", m_chServerEncKeyBuffer, IM_ARRAYSIZE(m_chServerEncKeyBuffer));

        ImGui::SameLine();
        if (ImGui::Button("Connect##ServerBrowser_ConnectByIp", ImVec2(ImGui::GetWindowContentRegionWidth() / 4, 18.5)))
        {
            ConnectToServer(m_chServerConnStringBuffer, m_chServerEncKeyBuffer);
        }

        ImGui::SameLine();
        if (ImGui::Button("Private Servers##ServerBrowser_HiddenServersButton", ImVec2(ImGui::GetWindowContentRegionWidth() / 4, 18.5)))
        {
            ImGui::OpenPopup("Connect to Private Server##HiddenServersConnectModal");
        }
        HiddenServersModal();
    }
    ImGui::PopItemWidth();
}

void IBrowser::HiddenServersModal()
{
    bool modalOpen = true;
    if (ImGui::BeginPopupModal("Connect to Private Server##HiddenServersConnectModal", &modalOpen))
    {
        ImGui::SetWindowSize(ImVec2(400.f, 200.f), ImGuiCond_Always);

        if (!m_idLockedIcon)
        {
            bool ret = LoadTextureBuffer((unsigned char*)m_vucLockedIconBlob, 0x1000 /*TODO [ AMOS ]: Calculate size dynamically*/, &m_idLockedIcon, &m_nLockedIconWidth, &m_nLockedIconHeight);
            IM_ASSERT(ret);
        }

        ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.00f, 0.00f, 0.00f, 0.00f)); // Override the style color for child bg.

        ImGui::BeginChild("##HiddenServersConnectModal_IconParent", ImVec2(m_nLockedIconWidth, m_nLockedIconHeight));
        ImGui::Image(m_idLockedIcon, ImVec2(m_nLockedIconWidth, m_nLockedIconHeight)); // Display texture.
        ImGui::EndChild();

        ImGui::PopStyleColor(); // Pop the override for the child bg.

        ImGui::SameLine();
        ImGui::Text("Enter the token to connect");

        ImGui::PushItemWidth(ImGui::GetWindowContentRegionWidth()); // Override item width.
        ImGui::InputTextWithHint("##HiddenServersConnectModal_TokenInput", "Token", &m_szHiddenServerToken);
        ImGui::PopItemWidth();

        ImGui::Dummy(ImVec2(ImGui::GetWindowContentRegionWidth(), 19.f)); // Place a dummy, basically making space inserting a blank element.

        ImGui::TextColored(m_ivHiddenServerMessageColor, m_szHiddenServerRequestMessage.c_str());
        ImGui::Separator();

        if (ImGui::Button("Connect##HiddenServersConnectModal_ConnectButton", ImVec2(ImGui::GetWindowContentRegionWidth() / 2, 24)))
        {
            m_szHiddenServerRequestMessage = "";
            ServerListing server;
            bool result = r5net->GetServerByToken(server, m_szHiddenServerRequestMessage, m_szHiddenServerToken); // Send token connect request.
            if (!server.name.empty())
            {
                ConnectToServer(server.ip, server.port, server.netchanEncryptionKey); // Connect to the server
                m_szHiddenServerRequestMessage = "Found Server: " + server.name;
                m_ivHiddenServerMessageColor = ImVec4(0.00f, 1.00f, 0.00f, 1.00f);
                ImGui::CloseCurrentPopup();
            }
            else
            {
                m_szHiddenServerRequestMessage = "Error: " + m_szHiddenServerRequestMessage;
                m_ivHiddenServerMessageColor = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
            }
        }

        ImGui::SameLine();
        if (ImGui::Button("Close##HiddenServersConnectModal_CloseButton", ImVec2(ImGui::GetWindowContentRegionWidth() / 2, 24)))
        {
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }
}

void IBrowser::HostServerSection()
{
    static std::string szServerNameErr = "";
    static std::string szServerMap = std::string();

    ImGui::InputTextWithHint("##ServerHost_ServerName", "Server Name (Required)", &m_Server.name);
    ImGui::Spacing();

    if (ImGui::BeginCombo("Playlist##ServerHost_PlaylistBox", m_Server.playlist.c_str()))
    {
        for (auto& item : g_szAllPlaylists)
        {
            if (ImGui::Selectable(item.c_str(), item == m_Server.playlist))
            {
                m_Server.playlist = item;
            }
        }
        ImGui::EndCombo();
    }

    if (ImGui::BeginCombo("Map##ServerHost_MapListBox", m_Server.map.c_str()))
    {
        for (auto& item : m_vszMapsList)
        {
            if (ImGui::Selectable(item.c_str(), item == m_Server.map))
            {
                m_Server.map = item;
                szServerMap = item;
                for (auto it = mapArray.begin(); it != mapArray.end(); ++it)
                {
                    if (it->second.compare(m_Server.map) == NULL)
                    {
                        szServerMap = it->first;
                    }
                }
            }
        }
        ImGui::EndCombo();
    }

    ImGui::Checkbox("Load Global Ban List##ServerHost_CheckCompBanDBCheckbox", &g_bCheckCompBanDB);
    ImGui::Spacing();

    ImGui::SameLine();
    ImGui::Text("Server Visiblity");

    if (ImGui::SameLine(); ImGui::RadioButton("Offline##ServerHost_ServerChoice1", eServerVisibility == EServerVisibility::OFFLINE))
    {
        eServerVisibility = EServerVisibility::OFFLINE;
    }
    if (ImGui::SameLine(); ImGui::RadioButton("Hidden##ServerHost_ServerChoice2", eServerVisibility == EServerVisibility::HIDDEN))
    {
        eServerVisibility = EServerVisibility::HIDDEN;
    }
    if (ImGui::SameLine(); ImGui::RadioButton("Public##ServerHost_ServerChoice2", eServerVisibility == EServerVisibility::PUBLIC))
    {
        eServerVisibility = EServerVisibility::PUBLIC;
    }

    ImGui::Spacing();
    ImGui::Separator();

    if (!g_pHostState->m_bActiveGame)
    {
        if (ImGui::Button("Start Server##ServerHost_StartServerButton", ImVec2(ImGui::GetWindowSize().x, 32)))
        {
            if (!m_Server.name.empty() && !m_Server.playlist.empty() && !szServerMap.empty())
            {
                Sys_Print(SYS_DLL::CLIENT, "Starting Server with name '%s', map '%s' and playlist '%s'\n", m_Server.name.c_str(), szServerMap.c_str(), m_Server.playlist.c_str());
                szServerNameErr = std::string();
                UpdateHostingStatus();

                /*
                * Playlist gets parsed in two instances, first in LoadPlaylist all the neccessary values.
                * Then when you would normally call launchplaylist which calls StartPlaylist it would cmd call mp_gamemode which parses the gamemode specific part of the playlist..
                */
                KeyValues_LoadPlaylist(m_Server.playlist.c_str());
                std::stringstream cgmd;
                cgmd << "mp_gamemode " << m_Server.playlist;
                ProcessCommand(cgmd.str().c_str());

                // This is to avoid a race condition.
                std::this_thread::sleep_for(std::chrono::milliseconds(100));

                std::stringstream cmd;
                cmd << "map " << szServerMap;
                ProcessCommand(cmd.str().c_str());
            }
            else
            {
                if (!m_Server.name.empty())
                {
                    szServerNameErr = "No Server Name assigned.";
                }
                else if (!m_Server.playlist.empty())
                {
                    szServerNameErr = "No Playlist assigned.";
                }
                else if (!szServerMap.empty())
                {
                    szServerNameErr = "Map didn't apply properly.";
                }
            }
        }
    }

    if (ImGui::Button("Force Start##ServerHost_ForceStart", ImVec2(ImGui::GetWindowSize().x, 32)))
    {
        szServerNameErr = std::string();
        if (!szServerMap.empty())
        {
            strncpy_s(g_pHostState->m_levelName, szServerMap.c_str(), 64); // Copy new map into hoststate levelname. 64 is size of m_levelname.
            g_pHostState->m_iNextState = HostStates_t::HS_NEW_GAME; // Force CHostState::FrameUpdate to start a server.
        }
        else
        {
            szServerNameErr = "Failed to force start: 'levelname' was empty.";
        }
    }

    ImGui::TextColored(ImVec4(1.00f, 0.00f, 0.00f, 1.00f), szServerNameErr.c_str());
    ImGui::TextColored(m_iv4HostRequestMessageColor, m_szHostRequestMessage.c_str());
    if (!m_szHostToken.empty())
    {
        ImGui::InputText("##ServerHost_HostToken", &m_szHostToken, ImGuiInputTextFlags_ReadOnly);
    }

    if (g_pHostState->m_bActiveGame)
    {
        if (ImGui::Button("Reload Scripts##ServerHost_ReloadServerButton", ImVec2(ImGui::GetWindowSize().x, 32)))
        {
            Sys_Print(SYS_DLL::ENGINE, "Recompiling scripts\n");
            ProcessCommand("reparse_weapons");
            ProcessCommand("reload");
        }

        if (ImGui::Button("Change Level##ServerHost_ChangeLevel", ImVec2(ImGui::GetWindowSize().x, 32)))
        {
            if (!szServerMap.empty())
            {
                strncpy_s(g_pHostState->m_levelName, szServerMap.c_str(), 64); // Copy new map into hoststate levelname. 64 is size of m_levelname.
                g_pHostState->m_iNextState = HostStates_t::HS_CHANGE_LEVEL_MP; // Force CHostState::FrameUpdate to change the level.
            }
            else
            {
                szServerNameErr = "Failed to change level: 'levelname' was empty.";
            }
        }

        if (ImGui::Button("Stop Server##ServerHost_StopServerButton", ImVec2(ImGui::GetWindowSize().x, 32)))
        {
            ProcessCommand("LeaveMatch"); // Use script callback instead.
            g_pHostState->m_iNextState = HostStates_t::HS_GAME_SHUTDOWN; // Force CHostState::FrameUpdate to shutdown the server for dedicated.
        }
    }
    else
    {
        if (ImGui::Button("Reload Playlist from Disk##ServerHost_ReloadPlaylist", ImVec2(ImGui::GetWindowSize().x, 32)))
        {
            DownloadPlaylists_Callback();
            CKeyValueSystem_InitPlaylist(); // Re-Init playlist.
        }
    }
}

void IBrowser::SettingsSection()
{
    ImGui::InputTextWithHint("##MatchmakingServerString", "Matchmaking Server String", &m_szMatchmakingServerString);
    if (ImGui::Button("Update Settings"))
    {
        if (r5net)
        {
            delete r5net;
            r5net = new R5Net::Client(m_szMatchmakingServerString);
        }
    }
    if (ImGui::Button("Regenerate Encryption Key##SettingsSection_RegenEncKey"))
    {
        RegenerateEncryptionKey();
    }
    ImGui::InputText("Encryption Key##SettingsSection_EncKey", (char*)g_szNetKey.c_str(), ImGuiInputTextFlags_ReadOnly);
}

void IBrowser::Draw(const char* title, bool* bDraw)
{
    if (!m_bThemeSet)
    {
        SetStyleVar();
        m_bThemeSet = true;
    }
    if (!ImGui::Begin(title, bDraw))
    {
        ImGui::End();
        return;
    }
    if (*bDraw == NULL)
    {
        g_bShowBrowser = false;
    }

    ImGui::SetNextWindowSize(ImVec2(840, 600), ImGuiCond_FirstUseEver);
    ImGui::SetWindowPos(ImVec2(-500, 50), ImGuiCond_FirstUseEver);

    ImGui::Begin(title, NULL, ImGuiWindowFlags_NoScrollbar);
    {
        CompMenu();

        switch (eCurrentSection)
        {
        case ESection::SERVER_BROWSER:
            ServerBrowserSection();
            break;
        case ESection::HOST_SERVER:
            HostServerSection();
            break;
        case ESection::SETTINGS:
            SettingsSection();
            break;
        default:
            break;
        }
    }
    ImGui::End();
}

void IBrowser::ProcessCommand(const char* command_line)
{
    std::thread t(&IBrowser::ExecCommand, this, command_line);
    t.detach();

    // This is to avoid a race condition.
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
}

void IBrowser::ExecCommand(const char* command_line)
{
    IVEngineClient_CommandExecute(NULL, command_line);
}

void IBrowser::ConnectToServer(const std::string ip, const std::string port, const std::string encKey)
{
    if (!encKey.empty())
    {
        ChangeEncryptionKeyTo(encKey);
    }

    std::stringstream cmd;
    cmd << "connect " << ip << ":" << port;
    ProcessCommand(cmd.str().c_str());
}

void IBrowser::ConnectToServer(const std::string connString, const std::string encKey)
{
    if (!encKey.empty())
    {
        ChangeEncryptionKeyTo(encKey);
    }

    std::stringstream cmd;
    cmd << "connect " << connString;
    ProcessCommand(cmd.str().c_str());
}

void IBrowser::RegenerateEncryptionKey()
{
    HNET_GenerateKey();
}

void IBrowser::ChangeEncryptionKeyTo(const std::string str)
{
    HNET_SetKey(str);
}

//#############################################################################
// ENTRYPOINT
//#############################################################################

IBrowser* g_pServerBrowser = nullptr;
bool g_bCheckCompBanDB = true;
void DrawBrowser(bool* bDraw)
{
    static IBrowser browser;
    static bool AssignPtr = []()
    {
        g_pServerBrowser = &browser;
        return true;
    } ();
    browser.Draw("Server Browser", bDraw);
}
