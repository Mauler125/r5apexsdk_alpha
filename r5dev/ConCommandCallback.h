#pragma once
#include "stdafx.h"
#include "ConCommand.h"
#include "IConVar.h"
#include "CClient.h"
#include "CNetChan.h"

///////////////////////////////////////////////////////////////////////////////
void CGameConsole_Callback(const CCommand& cmd);
void CCompanion_Callback(const CCommand& cmd);
void Kick_Callback(CCommand* cmd);
void KickID_Callback(CCommand* cmd);
void Ban_Callback(CCommand* cmd);
void BanID_Callback(CCommand* cmd);
void Unban_Callback(CCommand* cmd);
void ReloadBanList_Callback(CCommand* cmd);
void RTech_GenerateGUID_Callback(CCommand* cmd);
void RTech_Decompress_Callback(CCommand* cmd);
void NET_TraceNetChan_Callback(CCommand* cmd);
void NET_SetKey_Callback(CCommand* cmd);
void NET_GenerateKey_Callback(CCommand* cmd);

///////////////////////////////////////////////////////////////////////////////
