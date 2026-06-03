#include "drivers/timer/timer.hpp"

# include "cpu/pic/pic.hpp"

volatile u32	g_tick_count = 0;

extern "C" void	timer_handler()
{
	++g_tick_count;
	pic::send_eoi(0);
}
