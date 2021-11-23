#include "core/stdafx.h"
#include "vpc/keyvalues.h"
#include "rtech/stryder.h"

///////////////////////////////////////////////////////////////////////////////
std::vector<std::string> g_szAllPlaylists = { "none" };
CKeyValuesSystem* g_pKeyValuesSystem = reinterpret_cast<CKeyValuesSystem*>(p_KeyValues_Init.FindPatternSelf("48 8D ?? ?? ?? ?? 01", ADDRESS::Direction::DOWN, 100).ResolveRelativeAddressSelf(0x3, 0x7).GetPtr());;

#if defined (GAMEDLL_S0) || defined (GAMEDLL_S1)
KeyValues** g_pPlaylistKeyValues = reinterpret_cast<KeyValues**>(p_Stryder_StitchRequest.FindPatternSelf("48 8B 2D", ADDRESS::Direction::DOWN, 100).ResolveRelativeAddressSelf(0x3, 0x7).GetPtr()); // Get the KeyValue for the playlist file.
#elif defined (GAMEDLL_S2) || defined (GAMEDLL_S3)
KeyValues** g_pPlaylistKeyValues = reinterpret_cast<KeyValues**>(p_Stryder_StitchRequest.FindPatternSelf("48 8B 0D", ADDRESS::Direction::DOWN, 100).ResolveRelativeAddressSelf(0x3, 0x7).GetPtr()); // Get the KeyValue for the playlist file.
#endif

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
void CKeyValueSystem_InitPlaylist()
{
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
	std::thread t1(CKeyValueSystem_InitPlaylist); // Start thread to grab playlists.
	t1.detach(); // Detach thread from current one.
}
