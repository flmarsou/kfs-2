#pragma once

# include "utils/stdint.hpp"

namespace	vga
{
	enum class	Color : u8
	{
		Black,
		Blue,
		Green,
		Cyan,
		Red,
		Purple,
		Brown,
		Gray,
		DarkGray,
		LightBlue,
		LightGreen,
		LightCyan,
		LightRed,
		LightPurple,
		Yellow,
		White,
	};

	constexpr u8	WIDTH	= 80;
	constexpr u8	HEIGHT	= 25;

	u8		getBuffer(u8 posX, u8 posY);
	void	setBuffer(u8 c, u8 posX, u8 posY, Color fg, Color bg);
	void	clearBuffer(u8 c, Color fg, Color bg);
	void	scrollBuffer(Color fg, Color bg);

	void	getCursor(u8 &posX, u8 &posY);
	void	setCursor(u8 posX, u8 posY);
	void	enableCursor(u8 size);
	void	disableCursor();
}
