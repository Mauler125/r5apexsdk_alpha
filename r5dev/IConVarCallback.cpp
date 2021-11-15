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

void FS_Warning_Level_Native()
{
	g_fs_warning_level_native = g_pCvar->FindVar("fs_warning_level_native")->m_iValue;
}

void SQ_Show_Rson_Loading_Callback()
{
	g_sq_show_rson_loading = g_pCvar->FindVar("sq_showrsonloading")->m_iValue;
}

void SQ_Show_Script_Loading_Callback()
{
	g_sq_show_script_loading = g_pCvar->FindVar("sq_showscriptloading")->m_iValue;
}

void SQ_Show_VM_Output_Callback()
{
	g_sq_show_vm_output = g_pCvar->FindVar("sq_showvmoutput")->m_iValue;
}

void SQ_Show_VM_Warning_Callback()
{
	g_sq_show_vm_warning = g_pCvar->FindVar("sq_showvmwarning")->m_iValue;
}

