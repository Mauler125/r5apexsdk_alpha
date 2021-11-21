#pragma once

class CCommand
{
private:
	enum
	{
		COMMAND_MAX_ARGC   = 64,
		COMMAND_MAX_LENGTH = 512,
	};

public:
	CCommand() = delete;

	inline int MaxCommandLength()
	{
		return COMMAND_MAX_LENGTH - 1;
	}

	inline std::int64_t ArgC() const
	{
		return m_nArgc;
	}

	inline const char** ArgV() const
	{
		return m_nArgc ? (const char**)m_ppArgv : NULL;
	}

	inline const char* ArgS() const
	{
		return m_nArgv0Size ? &m_pArgSBuffer[m_nArgv0Size] : "";
	}

	inline const char* GetCommandString() const
	{
		return m_nArgc ? m_pArgSBuffer : "";
	}

	inline const char* Arg(int nIndex) const
	{
		// FIXME: Many command handlers appear to not be particularly careful
		// about checking for valid argc range. For now, we're going to
		// do the extra check and return an empty string if it's out of range
		if (nIndex < 0 || nIndex >= m_nArgc)
		{
			return "";
		}
		return m_ppArgv[nIndex];
	}

	inline const char* operator[](int nIndex) const
	{
		return Arg(nIndex);
	}

private:
	std::int64_t m_nArgc;
	std::int64_t m_nArgv0Size;
	char         m_pArgSBuffer[COMMAND_MAX_LENGTH];
	char         m_pArgvBuffer[COMMAND_MAX_LENGTH];
	const char*  m_ppArgv[COMMAND_MAX_ARGC];
};

class ConCommandBase
{
public:
	void* m_pConCommandBaseVTable; //0x0000
	ConCommandBase* m_pNext;       //0x0008
	bool m_bRegistered;            //0x0010
private:
	char pad_0011[7];              //0x0011
public:
	const char* m_pszName;         //0x0018
	const char* m_pszHelpString;   //0x0020
private:
	char pad_0028[16];             //0x0028
public:
	int m_nFlags;                  //0x0038
private:
	char pad_003C[4];              //0x003C
}; //Size: 0x0038

namespace
{
	/* ==== CONCOMMAND ====================================================================================================================================================== */
	ADDRESS p_ConCommand_IsFlagSet = g_mGameDll.FindPatternSIMD((std::uint8_t*)"\x85\x51\x38\x0F\x95\xC0\xC3", "xxxxxxx");
	bool (*ConCommand_IsFlagSet)(ConCommandBase* cmd, int flag) = (bool (*)(ConCommandBase*, int))p_ConCommand_IsFlagSet.GetPtr(); /*85 51 38 0F 95 C0 C3*/
}

///////////////////////////////////////////////////////////////////////////////
bool HConCommand_IsFlagSet(ConCommandBase* cmd, int flag);
void ConCommand_InitConCommand();

void ConCommand_Attach();
void ConCommand_Detach();

///////////////////////////////////////////////////////////////////////////////
