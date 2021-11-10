#include "stdafx.h"
#include "IConVarCallback.h"
#include "IConVar.h"
#include "ConCommand.h"

void CM_Debug_Cmdquery_Callback()
{
	g_cm_debug_cmdquery = g_pCvar->FindVar("cm_debug_cmdquery")->m_iValue;
}

void CM_Return_False_Cmdquery_All_Callback()
{
	g_cm_return_false_cmdquery_all = g_pCvar->FindVar("cm_return_false_cmdquery_all")->m_iValue;
}

void CM_Return_False_Cmdquery_Dev_Cheat_Callback()
{
	g_cm_return_false_cmdquery_dev_cheat = g_pCvar->FindVar("cm_return_false_cmdquery_dev_cheat")->m_iValue;
}
