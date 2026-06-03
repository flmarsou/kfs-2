#pragma once

# include "utils/stdint.hpp"

extern volatile u32	g_tick_count;

extern "C" void	timer_handler();
