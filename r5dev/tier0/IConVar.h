#pragma once
#include "basetypes.h"
#include "ConCommand.h"

//-----------------------------------------------------------------------------
// Command to ConVars and ConCommands
//-----------------------------------------------------------------------------
// ConVar systems
#define FCVAR_NONE                   0	// The default, no flags at all
#define FCVAR_UNREGISTERED      (1<<0)	// If this is set, don't add to linked list, etc.
#define FCVAR_DEVELOPMENTONLY   (1<<1)	// Hidden in released products. Flag is removed automatically if ALLOW_DEVELOPMENT_CVARS is defined.
#define FCVAR_GAMEDLL           (1<<2)	// defined by the game DLL
#define FCVAR_CLIENTDLL         (1<<3)	// defined by the client DLL
#define FCVAR_HIDDEN            (1<<4)	// Hidden. Doesn't appear in find or auto complete. Like DEVELOPMENTONLY, but can't be compiled out.

// ConVar only
#define FCVAR_PROTECTED	        (1<<5)	// It's a server cvar, but we don't send the data since it's a password, etc.  Sends 1 if it's not bland/zero, 0 otherwise as value
#define FCVAR_SPONLY            (1<<6)	// This cvar cannot be changed by clients connected to a multiplayer server.
#define	FCVAR_ARCHIVE           (1<<7)	// set to cause it to be saved to vars.rc
#define	FCVAR_NOTIFY            (1<<8)	// notifies players when changed
#define	FCVAR_USERINFO          (1<<9)	// changes the client's info string

#define FCVAR_PRINTABLEONLY     (1<<10)	// This cvar's string cannot contain unprintable characters ( e.g., used for player name etc ).

#define FCVAR_GAMEDLL_FOR_REMOTE_CLIENTS        (1<<10)  // When on concommands this allows remote clients to execute this cmd on the server. 
														 // We are changing the default behavior of concommands to disallow execution by remote clients without
														 // this flag due to the number existing concommands that can lag or crash the server when clients abuse them.

#define FCVAR_UNLOGGED          (1<<11)  // If this is a FCVAR_SERVER, don't log changes to the log file / console if we are creating a log
#define FCVAR_NEVER_AS_STRING   (1<<12)  // never try to print that cvar

// It's a ConVar that's shared between the client and the server.
// At signon, the values of all such ConVars are sent from the server to the client (skipped for local
// client, of course )
// If a change is requested it must come from the console (i.e., no remote client changes)
// If a value is changed while a server is active, it's replicated to all connected clients
#define FCVAR_REPLICATED        (1<<13)	// server setting enforced on clients, TODO rename to FCAR_SERVER at some time
#define FCVAR_CHEAT             (1<<14)	// Only useable in singleplayer / debug / multiplayer & sv_cheats
#define FCVAR_SS                (1<<15)	// causes varnameN where N == 2 through max splitscreen slots for mod to be autogenerated
#define FCVAR_DEMO              (1<<16)	// record this cvar when starting a demo file
#define FCVAR_DONTRECORD        (1<<17)	// don't record these command in demofiles
#define FCVAR_SS_ADDED          (1<<18)	// This is one of the "added" FCVAR_SS variables for the splitscreen players
#define FCVAR_RELEASE           (1<<19)	// Cvars tagged with this are the only cvars avaliable to customers
#define FCVAR_RELOAD_MATERIALS  (1<<20)	// If this cvar changes, it forces a material reload
#define FCVAR_RELOAD_TEXTURES   (1<<21)	// If this cvar changes, if forces a texture reload

#define FCVAR_NOT_CONNECTED          (1<<22)	// cvar cannot be changed by a client that is connected to a server
#define FCVAR_MATERIAL_SYSTEM_THREAD (1<<23)	// Indicates this cvar is read from the material system thread
#define FCVAR_ARCHIVE_GAMECONSOLE    (1<<24)	// cvar written to config.cfg on the Xbox

#define FCVAR_SERVER_CAN_EXECUTE    (1<<28)	// the server is allowed to execute this command on clients via ClientCommand/NET_StringCmd/CBaseClientState::ProcessStringCmd.
#define FCVAR_SERVER_CANNOT_QUERY   (1<<29)	// If this is set, then the server is not allowed to query this cvar's value (via IServerPluginHelpers::StartQueryCvarValue).
#define FCVAR_CLIENTCMD_CAN_EXECUTE (1<<30)	// IVEngineClient::ClientCmd is allowed to execute this command.

class ConVar
{
public:
	ConCommandBase m_ConCommandBase;  //0x0000
	void*          m_pConVarVTable;   //0x0040
	ConVar*        m_pParent;         //0x0048
	const char*    n_pszDefaultValue; //0x0050
	const char*    m_pzsCurrentValue; //0x0058
	std::int64_t   m_iStringLength;   //0x0060
	float          m_flValue;         //0x0068
	int            m_iValue;          //0x006C
	bool           m_bHasMin;         //0x0070
	float          m_flMinValue;      //0x0074
	bool           m_bHasMax;         //0x0078
	float          m_flMaxValue;      //0x007C
	char           pad_0080[32];      //0x0080
}; //Size: 0x00A0

class CCVarIteratorInternal // Fully reversed table, just look at the virtual function table and rename the function.
{
public:
	virtual void            SetFirst(void) = 0; //0
	virtual void            Next(void)     = 0; //1
	virtual	bool            IsValid(void)  = 0; //2
	virtual ConCommandBase* Get(void)      = 0; //3
};

class CCVar
{
public:
	ConCommandBase* FindCommandBase(const char* szCommandName) // @0x1405983A0 in R5pc_r5launch_N1094_CL456479_2019_10_30_05_20_PM
	{
		using OriginalFn = ConCommandBase * (__thiscall*)(CCVar*, const char*);
		return (*reinterpret_cast<OriginalFn**>(this))[14](this, szCommandName);
	}

	ConVar* FindVar(const char* szVarName) // @0x1405983B0 in R5pc_r5launch_N1094_CL456479_2019_10_30_05_20_PM
	{
		using OriginalFn = ConVar * (__thiscall*)(CCVar*, const char*);
		return (*reinterpret_cast<OriginalFn**>(this))[16](this, szVarName);
	}

	void* /*Implement ConCommand class.*/ FindCommand(const char* szCommandName) // @0x1405983F0 in R5pc_r5launch_N1094_CL456479_2019_10_30_05_20_PM
	{
		using OriginalFn = void* (__thiscall*)(CCVar*, const char*);
		return (*reinterpret_cast<OriginalFn**>(this))[18](this, szCommandName);
	}

	CCVarIteratorInternal* FactoryInternalIterator() // @0x140597C10 in R5pc_r5launch_N1094_CL456479_2019_10_30_05_20_PM
	{
		using OriginalFn = CCVarIteratorInternal * (__thiscall*)(CCVar*);
		return (*reinterpret_cast<OriginalFn**>(this))[41](this);
	}

	std::unordered_map<std::string, ConCommandBase*> DumpToMap()
	{
		std::stringstream ss;
		CCVarIteratorInternal* itint = FactoryInternalIterator(); // Allocatd new InternalIterator.

		std::unordered_map<std::string, ConCommandBase*> allConVars;

		for (itint->SetFirst(); itint->IsValid(); itint->Next()) // Loop through all instances.
		{
			ConCommandBase* pCommand = itint->Get();
			const char* commandName = pCommand->m_pszName;
			allConVars[commandName] = pCommand;
		}

		return allConVars;
	}
};

namespace
{
	/* ==== ICONVAR ========================================================================================================================================================= */
	ADDRESS p_IConVar_IsFlagSet = g_mGameDll.FindPatternSIMD((std::uint8_t*)"\x48\x8B\x41\x48\x85\x50\x38", "xxxxxxx");
	bool (*IConVar_IsFlagSet)(ConVar* cvar, int flag) = (bool (*)(ConVar*, int))p_IConVar_IsFlagSet.GetPtr(); /*48 8B 41 48 85 50 38*/

	ADDRESS p_ConVar_SetInfo = g_mGameDll.FindPatternSIMD((std::uint8_t*)"\x40\x53\x48\x83\xEC\x60\x48\x8B\xD9\xC6\x41\x10\x00\x33\xC9\x48\x8D\x05\x00\x00\x00\x00\x48\x89\x4C\x24\x00\x0F\x57\xC0\x48\x89\x4C\x24\x00\x48\x89\x03\x48\x8D\x05\x00\x00\x00\x00\x48\x89\x43\x40", "xxxxxxxxxxxxxxxxxx????xxxx?xxxxxxx?xxxxxx????xxxx");
	void* (*ConVar_SetInfo)(void* a1, int a2, int a3, int a4, void* a5) = (void* (*)(void* a1, int a2, int a3, int a4, void* a5))p_ConVar_SetInfo.GetPtr(); /*40 53 48 83 EC 60 48 8B D9 C6 41 10 00 33 C9 48 8D 05 ? ? ? ? 48 89 4C 24 ? 0F 57 C0 48 89 4C 24 ? 48 89 03 48 8D 05 ? ? ? ? 48 89 43 40*/
#if defined (GAMEDLL_S0) || defined (GAMEDLL_S1)
	ADDRESS p_ConVar_Register = g_mGameDll.FindPatternSIMD((std::uint8_t*)"\x48\x89\x5C\x24\x00\x48\x89\x6C\x24\x00\x48\x89\x74\x24\x00\x48\x89\x7C\x24\x00\x41\x56\x48\x83\xEC\x30\xF3\x0F\x10\x44\x24\x00", "xxxx?xxxx?xxxx?xxxx?xxxxxxxxxxx?"); /*48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 48 89 7C 24 ? 41 56 48 83 EC 30 F3 0F 10 44 24 ?*/
	void* (*ConVar_Register)(ConVar* allocatedConvar, const char* name, const char* defaultValue, int flags, const char* helpString, bool bMin, float fMin, bool bMax, float fMax, void* callback, void* unk) = (void* (*)(ConVar*, const char*, const char*, int, const char*, bool, float, bool, float, void*, void*))p_ConVar_Register.GetPtr();
#elif defined (GAMEDLL_S2) || defined (GAMEDLL_S3)
	ADDRESS p_ConVar_Register = g_mGameDll.FindPatternSIMD((std::uint8_t*)"\x48\x89\x5C\x24\x00\x48\x89\x6C\x24\x00\x48\x89\x74\x24\x00\x57\x48\x83\xEC\x40\xF3\x0F\x10\x84\x24\x00\x00\x00\x00", "xxxx?xxxx?xxxx?xxxxxxxxxx????"); /*48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 57 48 83 EC 40 F3 0F 10 84 24 ? ? ? ?*/
	void* (*ConVar_Register)(ConVar* allocatedConvar, const char* name, const char* defaultValue, int flags, const char* helpString, bool bMin, float fMin, bool bMax, float fMax, void* callback, void* unk) = (void* (*)(ConVar*, const char*, const char*, int, const char*, bool, float, bool, float, void*, void*))p_ConVar_Register.GetPtr();
#endif
}

namespace
{
	ADDRESS g_pConVarVtable  = p_ConVar_SetInfo.Offset(0x00).FindPatternSelf("48 8D 05", ADDRESS::Direction::DOWN, 100).ResolveRelativeAddressSelf(0x3, 0x7).GetPtr(); // Get vtable ptr for ConVar table.
	ADDRESS g_pIConVarVtable = p_ConVar_SetInfo.Offset(0x16).FindPatternSelf("48 8D 05", ADDRESS::Direction::DOWN, 100).ResolveRelativeAddressSelf(0x3, 0x7).GetPtr(); // Get vtable ptr for ICvar table.
}

///////////////////////////////////////////////////////////////////////////////
bool HIConVar_IsFlagSet(ConVar* cvar, int flag);
void IConVar_InitConVar();
void IConVar_ClearHostNames();

void IConVar_Attach();
void IConVar_Detach();

///////////////////////////////////////////////////////////////////////////////
extern CCVar* g_pCvar;
