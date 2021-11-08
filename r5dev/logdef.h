#pragma once
#include "stdafx.h"

//-------------------------------------------------------------------------
// NETCHAN                                                                |
inline auto g_spd_netchan_logger = spdlog::basic_logger_mt("netchan_logger", "platform\\logs\\NET_Trace.log");
static std::ostringstream g_spd_net_p_oss;
static auto g_spd_net_p_ostream_sink = std::make_shared<spdlog::sinks::ostream_sink_st>(g_spd_net_p_oss);
//-------------------------------------------------------------------------
// SQUIRREL PRINTF                                                        |
inline std::ostringstream g_spd_sqvm_p_oss;
inline auto g_spd_sqvm_p_ostream_sink = std::make_shared<spdlog::sinks::ostream_sink_st>(g_spd_sqvm_p_oss);
//-------------------------------------------------------------------------
// SQUIRREL WARNF                                                         |
inline std::ostringstream g_spd_sqvm_w_oss;
inline auto g_spd_sqvm_w_ostream_sink = std::make_shared<spdlog::sinks::ostream_sink_st>(g_spd_sqvm_w_oss);
