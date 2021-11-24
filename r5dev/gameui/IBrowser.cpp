#include "core/stdafx.h"
#include "core/init.h"
#include "tier0/IConVar.h"
#include "windows/id3dx.h"
#include "windows/console.h"
#include "engine/net_chan.h"
#include "engine/sys_utils.h"
#include "engine/host_state.h"
#include "client/IVEngineClient.h"
#include "networksystem/serverlisting.h"
#include "vpc/keyvalues.h"
#include "tier0/ConCommandCallback.h"
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

std::map<std::string, std::string> mapArray =
{
    { "mp_rr_canyonlands_64k_x_64k", "King's Canyon Season 0" },
    { "mp_rr_desertlands_64k_x_64k", "World's Edge Season 3" },
    { "mp_rr_canyonlands_mu1", "King's Canyon Season 2" },
    { "mp_rr_canyonlands_mu1_night", "King's Canyon Season 2 After Dark" },
    { "mp_rr_desertlands_64k_x_64k_nx", "World's Edge Season 3 After Dark" },
    { "mp_lobby", "Lobby Season 3" },
    { "mp_rr_canyonlands_staging", "King's Canyon Firing Range" }
};

IBrowser::IBrowser() : MatchmakingServerStringBuffer("r5a-comp-sv.herokuapp.com"), r5net(new R5Net::Client("r5a-comp-sv.herokuapp.com"))
{
    memset(ServerConnStringBuffer, 0, sizeof(ServerConnStringBuffer));

    std::string path = "stbsp";
    for (const auto& entry : std::filesystem::directory_iterator(path))
    {
        std::string filename = entry.path().string();
        int slashPos = filename.rfind("\\", std::string::npos);
        filename = filename.substr((INT8)slashPos + 1, std::string::npos);
        filename = filename.substr(0, filename.size() - 6);

        auto it = mapArray.find(filename); // Find MapName in mapArray.
        if (it != mapArray.end())
        {
            MapsList.push_back(it->second);
        }
        else
        {
            MapsList.push_back(filename);
        }
    }

    MyServer.map = MapsList[0];
    static std::thread HostingServerRequestThread([this]()
        {
            while (true)
            {
                UpdateHostingStatus();
                std::this_thread::sleep_for(std::chrono::milliseconds(5000));
            }
        });

    HostingServerRequestThread.detach();
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


    HostingStatus = g_pHostState->m_bActiveGame ? EHostStatus::HOSTING : EHostStatus::NOT_HOSTING; // Are we hosting a server?

    switch (HostingStatus)
    {
    case EHostStatus::NOT_HOSTING:
    {
        HostRequestMessage = "";
        HostRequestMessageColor = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
        break;
    }
    case EHostStatus::HOSTING:
    {
        if (ServerVisibility == EServerVisibility::OFFLINE)
        {
            break;
        }

        if (*reinterpret_cast<std::int32_t*>(0x1656057E0) == NULL) // Check if script checksum is valid yet.
        {
            break;
        }

        switch (ServerVisibility)
        {

        case EServerVisibility::HIDDEN:
            MyServer.hidden = true;
            break;
        case EServerVisibility::PUBLIC:
            MyServer.hidden = false;
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
    ServerList.clear();

    static bool bThreadLocked = false;

    if (!bThreadLocked)
    {
        std::thread t([this]()
            {
                Sys_Print(SYS_DLL::CLIENT, "Refreshing server list with string '%s'\n", MatchmakingServerStringBuffer.c_str());
                bThreadLocked = true;
                ServerList = r5net->GetServersList(ServerListMessage);
                bThreadLocked = false;
            });

        t.detach();
    }
}

void IBrowser::SendHostingPostRequest()
{
    HostToken = std::string();
    Sys_Print(SYS_DLL::CLIENT, "Sending PostServerHost request\n");
    bool result = r5net->PostServerHost(HostRequestMessage, HostToken,
        ServerListing{
            MyServer.name,
            std::string(g_pHostState->m_levelName),
            "",
            g_pCvar->FindVar("hostport")->m_pzsCurrentValue,
            g_pCvar->FindVar("mp_gamemode")->m_pzsCurrentValue,
            MyServer.hidden,
            std::to_string(*reinterpret_cast<std::int32_t*>(0x1656057E0)),
            std::string(),
            g_szNetKey.c_str()
        }
    );

    if (result)
    {
        HostRequestMessageColor = ImVec4(0.00f, 1.00f, 0.00f, 1.00f);
        std::stringstream msg;
        msg << "Broadcasting! ";
        if (!HostToken.empty())
        {
            msg << "Share the following token for people to connect: ";
        }
        HostRequestMessage = msg.str().c_str();
    }
    else
    {
        HostRequestMessageColor = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
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
    ServerBrowserFilter.Draw();
    ImGui::SameLine();
    if (ImGui::Button("Refresh List"))
    {
        RefreshServerList();
    }
    ImGui::EndGroup();
    //ImGui::TextColored(ImVec4(1.00f, 0.00f, 0.00f, 1.00f), ServerListMessage.c_str());
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

        for (ServerListing& server : ServerList)
        {
            const char* name = server.name.c_str();
            const char* map = server.map.c_str();
            const char* port = server.port.c_str();
            const char* playlist = server.playlist.c_str();

            if (ServerBrowserFilter.PassFilter(name)
                || ServerBrowserFilter.PassFilter(map)
                || ServerBrowserFilter.PassFilter(port))
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
        ImGui::InputTextWithHint("##ServerBrowser_ServerConnString", "Enter IP address or \"localhost\"", ServerConnStringBuffer, IM_ARRAYSIZE(ServerConnStringBuffer));

        ImGui::SameLine();
        ImGui::InputTextWithHint("##ServerBrowser_ServerEncKey", "Enter encryption key", ServerEncKeyBuffer, IM_ARRAYSIZE(ServerEncKeyBuffer));
        ImGui::SameLine();

        if (ImGui::Button("Connect##ServerBrowser_ConnectByIp", ImVec2(ImGui::GetWindowContentRegionWidth() / 4, 18.5)))
        {
            ConnectToServer(ServerConnStringBuffer, ServerEncKeyBuffer);
        }

        ImGui::SameLine();

        if (ImGui::Button("Private Servers##ServerBrowser_HiddenServersButton", ImVec2(ImGui::GetWindowContentRegionWidth() / 4, 18.5)))
        {
            ImGui::OpenPopup("Connect to Private Server##HiddenServersConnectModal");
        }
        HiddenServersModal();
        // TODO: Still not in a seperate class...

    }
    ImGui::PopItemWidth();
}

void IBrowser::HiddenServersModal()
{
    bool modalOpen = true;
    if (ImGui::BeginPopupModal("Connect to Private Server##HiddenServersConnectModal", &modalOpen))
    {
        // I *WILL* move this in a separate class

        ImGui::SetWindowSize(ImVec2(400.f, 200.f), ImGuiCond_Always);

        int imgWidth = 48;
        int imgHeight = 48;
        static ID3D11ShaderResourceView* apex_private_servers_icon = NULL;

        if (!ApexLockIcon)
        {
            bool ret = LoadTextureBuffer("lockedserver.png", &apex_private_servers_icon, &imgWidth, &imgHeight);
            IM_ASSERT(ret);
            printf("lockedserver.png\n");
            ApexLockIcon = apex_private_servers_icon;
        }

        ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.00f, 0.00f, 0.00f, 0.00f)); // Override the style color for child bg.
        ImGui::BeginChild("##HiddenServersConnectModal_IconParent", ImVec2(m_nLockedIconWidth, m_nLockedIconHeight));
        {
            ImGui::Image(ApexLockIcon, ImVec2(m_nLockedIconWidth, m_nLockedIconHeight)); // Display texture.
            printf("%p\n", ApexLockIcon);
        }
        ImGui::EndChild();
        ImGui::PopStyleColor(); // Pop the override for the child bg.

        ImGui::SameLine();

        ImGui::Text("Enter the following details to continue");

        ImGui::PushItemWidth(ImGui::GetWindowContentRegionWidth()); // Override item width.
        ImGui::InputTextWithHint("##HiddenServersConnectModal_TokenInput", "Token", &m_szHiddenServerToken);
        ImGui::PopItemWidth(); // Pop item width.

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
    static std::string ServerNameErr = "";
    static std::string ServerMap = std::string();

    ImGui::InputTextWithHint("##ServerHost_ServerName", "Server Name (Required)", &MyServer.name);
    ImGui::Spacing();

    if (ImGui::BeginCombo("Playlist##ServerHost_PlaylistBox", MyServer.playlist.c_str()))
    {
        for (auto& item : g_szAllPlaylists)
        {
            if (ImGui::Selectable(item.c_str(), item == MyServer.playlist))
            {
                MyServer.playlist = item;
            }
        }
        ImGui::EndCombo();
    }

    if (ImGui::BeginCombo("Map##ServerHost_MapListBox", MyServer.map.c_str()))
    {
        for (auto& item : MapsList)
        {
            if (ImGui::Selectable(item.c_str(), item == MyServer.map))
            {
                MyServer.map = item;
                ServerMap = item;
                for (auto it = mapArray.begin(); it != mapArray.end(); ++it)
                {
                    if (it->second.compare(MyServer.map) == NULL)
                    {
                        ServerMap = it->first;
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

    if (ImGui::SameLine(); ImGui::RadioButton("Offline##ServerHost_ServerChoice1", ServerVisibility == EServerVisibility::OFFLINE))
    {
        ServerVisibility = EServerVisibility::OFFLINE;
    }
    if (ImGui::SameLine(); ImGui::RadioButton("Hidden##ServerHost_ServerChoice2", ServerVisibility == EServerVisibility::HIDDEN))
    {
        ServerVisibility = EServerVisibility::HIDDEN;
    }
    if (ImGui::SameLine(); ImGui::RadioButton("Public##ServerHost_ServerChoice2", ServerVisibility == EServerVisibility::PUBLIC))
    {
        ServerVisibility = EServerVisibility::PUBLIC;
    }

    ImGui::Spacing();
    ImGui::Separator();

    if (!g_pHostState->m_bActiveGame)
    {
        if (ImGui::Button("Start Server##ServerHost_StartServerButton", ImVec2(ImGui::GetWindowSize().x, 32)))
        {
            if (!MyServer.name.empty() && !MyServer.playlist.empty() && !ServerMap.empty())
            {
                Sys_Print(SYS_DLL::CLIENT, "Starting Server with name '%s', map '%s' and playlist '%s'\n", MyServer.name.c_str(), ServerMap.c_str(), MyServer.playlist.c_str());
                ServerNameErr = std::string();
                UpdateHostingStatus();

                /*
                * Playlist gets parsed in two instances, first in LoadPlaylist all the neccessary values.
                * Then when you would normally call launchplaylist which calls StartPlaylist it would cmd call mp_gamemode which parses the gamemode specific part of the playlist..
                */
                KeyValues_LoadPlaylist(MyServer.playlist.c_str());
                std::stringstream cgmd;
                cgmd << "mp_gamemode " << MyServer.playlist;
                ProcessCommand(cgmd.str().c_str());

                // This is to avoid a race condition.
                std::this_thread::sleep_for(std::chrono::milliseconds(100));

                std::stringstream cmd;
                cmd << "map " << ServerMap;
                ProcessCommand(cmd.str().c_str());
            }
            else
            {
                if (!MyServer.name.empty())
                {
                    ServerNameErr = "No Server Name assigned.";
                }
                else if (!MyServer.playlist.empty())
                {
                    ServerNameErr = "No Playlist assigned.";
                }
                else if (!ServerMap.empty())
                {
                    ServerNameErr = "Map didn't apply properly.";
                }
            }
        }
    }

    if (ImGui::Button("Force Start##ServerHost_ForceStart", ImVec2(ImGui::GetWindowSize().x, 32)))
    {
        ServerNameErr = std::string();
        if (!ServerMap.empty())
        {
            strncpy_s(g_pHostState->m_levelName, ServerMap.c_str(), 64); // Copy new map into hoststate levelname. 64 is size of m_levelname.
            g_pHostState->m_iNextState = HostStates_t::HS_NEW_GAME; // Force CHostState::FrameUpdate to start a server.
        }
        else
        {
            ServerNameErr = "Failed to force start. Map didn't apply properly.";
        }
    }

    ImGui::TextColored(ImVec4(1.00f, 0.00f, 0.00f, 1.00f), ServerNameErr.c_str());
    ImGui::TextColored(HostRequestMessageColor, HostRequestMessage.c_str());
    if (!HostToken.empty())
    {
        ImGui::InputText("##ServerHost_HostToken", &HostToken, ImGuiInputTextFlags_ReadOnly);
    }

    if (g_pHostState->m_bActiveGame)
    {
        if (ImGui::Button("Reload Scripts##ServerHost_ReloadServerButton", ImVec2(ImGui::GetWindowSize().x, 32)))
        {
            spdlog::debug("[+CCompanion+] Reloading scripts..\n");
            ProcessCommand("reparse_weapons");
            ProcessCommand("reload");
        }

        if (ImGui::Button("Change Level##ServerHost_ChangeLevel", ImVec2(ImGui::GetWindowSize().x, 32)))
        {
            if (!ServerMap.empty())
            {
                spdlog::debug("[+CCompanion+] Changing level to {}..\n", ServerMap);
                strncpy_s(g_pHostState->m_levelName, ServerMap.c_str(), 64); // Copy new map into hoststate levelname. 64 is size of m_levelname.
                g_pHostState->m_iNextState = HostStates_t::HS_CHANGE_LEVEL_MP; // Force CHostState::FrameUpdate to change the level.
            }
            else
            {
                ServerNameErr = "Failed to change level. Map didn't apply properly.";
            }
        }

        if (ImGui::Button("Stop Server##ServerHost_StopServerButton", ImVec2(ImGui::GetWindowSize().x, 32)))
        {
            spdlog::debug("[+CCompanion+] Stopping server..\n");
            ProcessCommand("LeaveMatch"); // Use script callback instead.
            g_pHostState->m_iNextState = HostStates_t::HS_GAME_SHUTDOWN; // Force CHostState::FrameUpdate to shutdown the server for dedicated.
        }
    }
    else
    {
        if (ImGui::Button("Reload Playlist from Disk##ServerHost_ReloadPlaylist", ImVec2(ImGui::GetWindowSize().x, 32)))
        {
            spdlog::debug("[+CCompanion+] Reloading playlist..\n");
            DownloadPlaylists_Callback();
            CKeyValueSystem_InitPlaylist(); // Re-Init playlist.
        }
    }
}

void IBrowser::SettingsSection()
{
    // Matchmaking Server String
    ImGui::InputTextWithHint("##MatchmakingServerString", "Matchmaking Server String", &MatchmakingServerStringBuffer);
    if (ImGui::Button("Update Settings"))
    {
        if (r5net)
        {
            delete r5net;
            r5net = new R5Net::Client(MatchmakingServerStringBuffer);
        }
    }
    // Encryption Key
    if (ImGui::Button("Regenerate Encryption Key##SettingsSection_RegenEncKey"))
    {
        RegenerateEncryptionKey();
    }
    ImGui::InputText("Encryption Key##SettingsSection_EncKey", (char*)g_szNetKey.c_str(), ImGuiInputTextFlags_ReadOnly);
}

void IBrowser::Draw(const char* title, bool* bDraw)
{
    if (!ThemeSet)
    {
        SetStyleVar();
        ThemeSet = true;
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

        switch (CurrentSection)
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
    spdlog::debug("[+CCompanion+] Processing command: {} creating thread now.\n", command_line);
    std::thread t(&IBrowser::ExecCommand, this, command_line);
    spdlog::debug("[+CCompanion+] Thread created.\n");
    t.detach();
    spdlog::debug("[+CCompanion+] Detached from Thread.\n");

    // This is to avoid a race condition.
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
}

void IBrowser::ExecCommand(const char* command_line)
{
    // TODO: separate thread to avoid race condition.
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
