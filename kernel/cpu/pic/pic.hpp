#pragma once

# include "utils/stdint.hpp"
# include "tty/TTY.hpp"

namespace	pic
{
	void	init();
	void	send_eoi(u8 irq);
}
