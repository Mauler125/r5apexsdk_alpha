#include "stdafx.h"
#include "CKeyValuesSystem.h"

///////////////////////////////////////////////////////////////////////////////
std::vector<std::string> g_szAllPlaylists = { "none" };
CKeyValuesSystem* g_pKeyValuesSystem = nullptr;
KeyValues** g_pPlaylistKeyValues = nullptr;

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
void InitPlaylist()
{
	spdlog::debug("Native(E): Parsing Playlist..\n");
	while (true)
	{
		if ((*g_pPlaylistKeyValues))
		{
			KeyValues* playlists = (*g_pPlaylistKeyValues)->FindKey("Playlists", false); // Find playlists key.
			if (playlists)
			{
				g_szAllPlaylists.clear();
				for (KeyValues* dat = playlists->m_pSub; dat != nullptr; dat = dat->m_pPeer) // Parse through all sub keys.
				{
					g_szAllPlaylists.push_back(dat->GetName()); // Get all playlist names.
				}

				break; // Break if playlist got filled.
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(50));
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
	}
}

const char* KeyValues::GetName()
{
	return g_pKeyValuesSystem->GetStringForSymbol(MAKE_3_BYTES_FROM_1_AND_2(m_iKeyNameCaseSensitive, m_iKeyNameCaseSensitive2));
}

void CKeyValueSystem_Init()
{
	std::thread t1(InitPlaylist); // Start thread to grab playlists.
	t1.detach(); // Detach thread from current one.
}
