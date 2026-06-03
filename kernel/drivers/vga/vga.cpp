#include "drivers/vga/vga.hpp"

# include "utils/sys/io.hpp"

namespace	port
{
	constexpr u16	INDEX	= 0x3D4;
	constexpr u16	DATA	= 0x3D5;
}

namespace	value
{
	constexpr u8	CURSOR_HIGH		= 0x0E;
	constexpr u8	CURSOR_LOW		= 0x0F;

	constexpr u8	CURSOR_START	= 0x0A;
	constexpr u8	CURSOR_END		= 0x0B;

	constexpr u8	CURSOR_OFF		= 0x20;
}

namespace
{
	inline u16	make_entry(u8 c, vga::Color fg, vga::Color bg)
	{
		return ((static_cast<u16>(bg) << 12) | (static_cast<u16>(fg) << 8) | c);
	}
}

namespace	vga
{
	/**
	 *                                      VGA Memory
	 * |=======|==================|==================|====================================|
	 * | 15    | 14            12 | 11             8 | 7                                0 |
	 * | Blink | Background Color | Foreground Color | Character                          |
	 * |=======|==================|==================|====================================|
	 */
	volatile u16	*buffer = reinterpret_cast<u16 *>(0xB8000);

	u8		getBuffer(u8 posX, u8 posY)
	{
		return (buffer[posY * WIDTH + posX] & 0xFF);
	}

	void	setBuffer(u8 c, u8 posX, u8 posY, Color fg, Color bg)
	{
		buffer[posY * WIDTH + posX] = make_entry(c, fg, bg);
	}

	void	clearBuffer(u8 c, Color fg, Color bg)
	{
		for (i32 y = 0; y < HEIGHT; ++y)
			for (i32 x = 0; x < WIDTH; ++x)
				setBuffer(c, x, y, fg, bg);
	}

	void	scrollBuffer(Color fg, Color bg)
	{
		// Move lines by one
		for (i32 y = 0; y < HEIGHT - 1; ++y)
		for (i32 x = 0; x < WIDTH; ++x)
			buffer[y * WIDTH + x] = buffer[(y + 1) * WIDTH + x];

		// Clear bottom line
		for (i32 x = 0; x < WIDTH; ++x)
			buffer[(HEIGHT - 1) * WIDTH + x] = make_entry(' ', fg, bg);
	}

	void	getCursor(u8 &posX, u8 &posY)
	{
		u16	pos;

		// Read high byte
		outb(port::INDEX, value::CURSOR_HIGH);
		pos = static_cast<u16>(inb(port::DATA)) << 8;

		// Read low byte
		outb(port::INDEX, value::CURSOR_LOW);
		pos |= static_cast<u16>(inb(port::DATA));

		posX = pos % WIDTH;
		posY = pos / WIDTH;
	}

	void	setCursor(u8 posX, u8 posY)
	{
		const u16	POS = posY * WIDTH + posX;

		// Write high byte
		outb(port::INDEX, value::CURSOR_HIGH);
		outb(port::DATA, static_cast<u8>(POS >> 8));

		// Write low byte
		outb(port::INDEX, value::CURSOR_LOW);
		outb(port::DATA, static_cast<u8>(POS & 0xFF));
	}

	void	enableCursor(u8 size)
	{
		const u8	START = 16 - size;

		outb(port::INDEX, value::CURSOR_START);
		outb(port::DATA, START & 0x1F);

		outb(port::INDEX, value::CURSOR_END);
		outb(port::DATA, 15 & 0x1F);
	}

	void	disableCursor()
	{
		outb(port::INDEX, value::CURSOR_START);
		outb(port::DATA, value::CURSOR_OFF);
	}
}
