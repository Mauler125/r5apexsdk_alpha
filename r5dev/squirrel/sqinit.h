#pragma once

namespace
{
	ADDRESS p_Script_Remote_BeginRegisteringFunctions = g_mGameDll.FindPatternSIMD((std::uint8_t*)"\x48\x83\xEC\x28\x83\x3D\x00\x00\x00\x00\x00\x74\x10", "xxxxxx?????xx");
	void* Script_Remote_BeginRegisteringFunctions = (void*)p_Script_Remote_BeginRegisteringFunctions.GetPtr(); /*48 83 EC 28 83 3D ?? ?? ?? ?? ?? 74 10*/

	ADDRESS g_nRemoteFunctionCallsChecksum = p_Script_Remote_BeginRegisteringFunctions.FindPatternSelf("89 05", ADDRESS::Direction::DOWN, 150).ResolveRelativeAddressSelf(0x2, 0x6).GetPtr();
}
