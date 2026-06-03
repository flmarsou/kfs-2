#include "tty/draw/shapes.hpp"

namespace	shapes
{
	void	quad(TTY &tty, u8 startX, u8 startY, u8 width, u8 height, u8 c)
	{
		const u8	END_Y = startY + height;
		const u8	END_X = startX + width;

		for (i32 y = startY; y < END_Y; ++y)
			for (i32 x = startX; x < END_X; ++x)
				tty.PutCharAt(x, y, c);
	}
}
