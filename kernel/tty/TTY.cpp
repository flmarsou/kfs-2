#include "tty/TTY.hpp"

# include <stdarg.h>

constexpr const i8	*BASE_16_DIGITS	= "0123456789ABCDEF";

// ========================================================================== //
//    Getters                                                                 //
// ========================================================================== //

u8			TTY::GetCursorX() const
{
	return (cursorX_);
}

u8			TTY::GetCursorY() const
{
	return (cursorY_);
}

vga::Color	TTY::GetForegroundColor() const
{
	return (foregroundColor_);
}

vga::Color	TTY::GetBackgroundColor() const
{
	return (backgroundColor_);
}

// ========================================================================== //
//    Setters                                                                 //
// ========================================================================== //

void	TTY::SetCursor(u8 posX, u8 posY)
{
	vga::setCursor(posX, posY);

	cursorX_ = posX;
	cursorY_ = posY;
}

void	TTY::SetForegroundColor(vga::Color color)
{
	foregroundColor_ = color;
}

void	TTY::SetBackgroundColor(vga::Color color)
{
	backgroundColor_ = color;
}

void	TTY::SetDefaultColor()
{
	foregroundColor_ = vga::Color::White;
	backgroundColor_ = vga::Color::Black;
}

// ========================================================================== //
//    Methods                                                                 //
// ========================================================================== //

//  Streaming                         //
// ================================== //

void	TTY::PutChar(u8 c)
{
	if (c == '\n')
	{
		++cursorY_;
		cursorX_ = 0;
	}
	else if (c == '\t')
	{
		PutStr("    ");
	}
	else
	{
		vga::setBuffer(c, cursorX_, cursorY_, foregroundColor_, backgroundColor_);
		++cursorX_;

		if (cursorX_ >= vga::WIDTH)
		{
			++cursorY_;
			cursorX_ = 0;
		}
	}

	if (cursorY_ >= vga::HEIGHT)
	{
		vga::scrollBuffer(foregroundColor_, backgroundColor_);
		cursorY_ = vga::HEIGHT - 1;
	}

	SetCursor(cursorX_, cursorY_);
}

void	TTY::PutStr(const i8 *str)
{
	while (*str)
	{
		PutChar(*str);
		++str;
	}
}

void	TTY::PutNbr(i32 nbr)
{
	i8		buffer[12];
	i32		i = 0;
	bool	negative = false;

	if (nbr == 0)
	{
		PutChar('0');
		return ;
	}

	if (nbr < 0)
	{
		negative = true;
		if (nbr == I32MIN)
			nbr = I32MAX;
		else
			nbr = -nbr;
	}

	while (nbr > 0)
	{
		buffer[i] = '0' + (nbr % 10);
		nbr /= 10;
		++i;
	}

	if (negative)
	{
		PutChar('-');
		if (i == 10 && buffer[0] == '7')
			buffer[0] = '8';
	}

	for (i32 j = i - 1; j >= 0; --j)
		PutChar(buffer[j]);
}

void	TTY::PutHex(i32 nbr)
{
	i8	buffer[8];
	i32	i = 0;

	if (nbr == 0)
	{
		PutChar('0');
		return ;
	}

	while (nbr > 0)
	{
		buffer[i] = BASE_16_DIGITS[nbr & 0xF];
		nbr >>= 4;
		++i;
	}

	for (i32 j = i - 1; j >= 0; --j)
		PutChar(buffer[j]);
}

void	TTY::PutPtr(const void *ptr)
{
	if (!ptr)
	{
		PutStr("(nil)");
		return ;
	}

	PutStr("0x");
	PutHex(reinterpret_cast<i32>(ptr));
}

void	TTY::Print(const i8 *fmt, ...)
{
	va_list	args;
	va_start(args, fmt);

	while (*fmt)
	{
		if (*fmt == '%')
		{
			++fmt;
			switch (*fmt)
			{
				case ('%'):	PutChar('%'); break ;
				case ('c'): PutChar(va_arg(args, i32)); break ;
				case ('s'): PutStr(va_arg(args, i8 *)); break ;
				case ('i'): PutNbr(va_arg(args, i32)); break ;
				case ('x'): PutHex(va_arg(args, i32)); break ;
				case ('p'): PutPtr(va_arg(args, void *)); break ;
			}
			++fmt;
		}
		else
		{
			PutChar(*fmt);
			++fmt;
		}
	}

	va_end(args);
}

//  Utils                             //
// ================================== //

void	TTY::Clear(u8 c)
{
	vga::clearBuffer(c, foregroundColor_, backgroundColor_);
}

void	TTY::Backspace()
{
	if (cursorX_ == 0)
	{
		if (cursorY_ == 0)
			return ;

		cursorX_ = vga::WIDTH - 1;

		vga::setBuffer(' ', cursorX_, --cursorY_, foregroundColor_, backgroundColor_);
		vga::setCursor(cursorX_, cursorY_);
	}
	else
	{
		vga::setBuffer(' ', --cursorX_, cursorY_, foregroundColor_, backgroundColor_);
		vga::setCursor(cursorX_, cursorY_);
	}
}

void	TTY::EnableCursor(u8 size) const
{
	vga::enableCursor(size);
}

void	TTY::DisableCursor() const
{
	vga::disableCursor();
}

//  Absolute                          //
// ================================== //

void	TTY::PutCharAt(u8 posX, u8 posY, u8 c) const
{
	if (posX >= vga::WIDTH || posY >= vga::HEIGHT)
		return ;

	vga::setBuffer(c, posX, posY, foregroundColor_, backgroundColor_);
}

void	TTY::PutStrAt(u8 posX, u8 posY, const i8 *str) const
{
	const u8	OLD_X = posX;

	while (*str)
	{
		if (*str == '\n')
		{
			posX = OLD_X;
			++posY;
		}
		else
		{
			PutCharAt(posX, posY, *str);
			++posX;
		}
		++str;
	}
}

void	TTY::PutNbrAt(u8 posX, u8 posY, i32 nbr) const
{
	i8		buffer[12];
	i32		i = 0;
	bool	negative = false;

	if (nbr == 0)
	{
		PutCharAt(posX, posY, '0');
		return ;
	}

	if (nbr < 0)
	{
		negative = true;
		if (nbr == I32MIN)
			nbr = I32MAX;
		else
			nbr = -nbr;
	}

	while (nbr > 0)
	{
		buffer[i] = '0' + (nbr % 10);
		nbr /= 10;
		++i;
	}

	if (negative)
	{
		PutCharAt(posX, posY, '-');
		if (i == 10 && buffer[0] == '7')
			buffer[0] = '8';
	}

	for (i32 j = i - 1; j >= 0; --j)
	{
		PutCharAt(posX, posY, buffer[j]);
		++posX;
	}
}

void	TTY::PutHexAt(u8 posX, u8 posY, i32 nbr) const
{
	i8			buffer[8];
	i32			i = 0;

	if (nbr == 0)
	{
		PutCharAt(posX, posY, '0');
		return ;
	}

	while (nbr > 0)
	{
		buffer[i] = BASE_16_DIGITS[nbr & 0xF];
		nbr >>= 4;
		++i;
	}

	for (i32 j = i - 1; j >= 0; --j)
	{
		++posX;
		PutCharAt(posX, posY, buffer[j]);
	}
}

void	TTY::PutPtrAt(u8 posX, u8 posY, const void *ptr) const
{
	if (!ptr)
	{
		PutStrAt(posX, posY, "(nil)");
		return ;
	}

	PutStrAt(posX, posY, "0x");
	PutHexAt(posX + 1, posY, reinterpret_cast<i32>(ptr));
}

void	TTY::PrintAt(u8 posX, u8 posY, const i8 *fmt, ...) const
{
	const u8	OLD_X = posX;

	va_list	args;
	va_start(args, fmt);

	auto	lPutCharAt = [&](const i8 c)
	{
		if (c == '\n')
		{
			posX = OLD_X;
			++posY;
			return ;
		}

		if (posX >= vga::WIDTH || posY >= vga::HEIGHT)
			return ;

		vga::setBuffer(c, posX, posY, foregroundColor_, backgroundColor_);
		++posX;
	};

	auto	lPutStrAt = [&](const i8 *str)
	{
		while (*str)
		{
			lPutCharAt(*str);
			++str;
		}
	};

	auto	lPutHexAt = [&](u32 nbr)
	{
		i8			buffer[8];
		i32			i = 0;

		if (nbr == 0)
		{
			lPutCharAt('0');
			return ;
		}

		while (nbr > 0)
		{
			buffer[i] = BASE_16_DIGITS[nbr & 0xF];
			nbr >>= 4;
			++i;
		}

		for (i32 j = i - 1; j >= 0; --j)
			lPutCharAt(buffer[j]);
	};

	while (*fmt)
	{
		if (*fmt == '%')
		{
			++fmt;
			switch (*fmt)
			{
				case ('%'): lPutCharAt('%'); break ;
				case ('c'): lPutCharAt(va_arg(args, i32)); break ;
				case ('s'): lPutStrAt(va_arg(args, i8 *)); break ;
				case ('i'):
				{
					i32		nbr = va_arg(args, i32);
					i8		buffer[12];
					i32		i = 0;
					bool	negative = false;

					if (nbr == 0)
					{
						lPutCharAt('0');
						break  ;
					}

					if (nbr < 0)
					{
						negative = true;
						if (nbr == I32MIN)
							nbr = I32MAX;
						else
							nbr = -nbr;
					}

					while (nbr > 0)
					{
						buffer[i] = '0' + (nbr % 10);
						nbr /= 10;
						++i;
					}

					if (negative)
					{
						lPutCharAt('-');
						if (i == 10 && buffer[0] == '7')
							buffer[0] = '8';
					}

					for (i32 j = i - 1; j >= 0; --j)
						lPutCharAt(buffer[j]);

					break ;
				}
				case ('x'): lPutHexAt(va_arg(args, u32)); break ;
				case ('p'):
				{
					void	*ptr = va_arg(args, void *);

					if (!ptr)
					{
						lPutStrAt("(nil)");
						break ;
					}

					lPutStrAt("0x");
					lPutHexAt(reinterpret_cast<u32>(ptr));

					break ;
				}
			}
			++fmt;
		}
		else
		{
			lPutCharAt(*fmt);
			++fmt;
		}
	}

	va_end(args);
}
