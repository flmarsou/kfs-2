#pragma once

# include "utils/stdint.hpp"
# include "drivers/vga/vga.hpp"

class	TTY
{
	public:

		// ================================================================== //
		//    Getters                                                         //
		// ================================================================== //

		u8			GetCursorX() const;
		u8			GetCursorY() const;
		vga::Color	GetForegroundColor() const;
		vga::Color	GetBackgroundColor() const;

		// ================================================================== //
		//    Setters                                                         //
		// ================================================================== //

		void	SetCursor(u8 posX, u8 posY);
		void	SetForegroundColor(vga::Color color);
		void	SetBackgroundColor(vga::Color color);
		void	SetDefaultColor();

		// ================================================================== //
		//    Methods                                                         //
		// ================================================================== //

		//  Streaming                 //
		// ========================== //

		void	PutChar(u8 c);
		void	PutStr(const i8 *str);
		void	PutNbr(i32 nbr);
		void	PutHex(i32 nbr);
		void	PutPtr(const void *ptr);
		void	Print(const i8 *fmt, ...);

		//  Utils                     //
		// ========================== //

		void	Clear(u8 c);
		void	Backspace();
		void	EnableCursor(u8 size) const;
		void	DisableCursor() const;

		//  Absolute                  //
		// ========================== //

		void	PutCharAt(u8 posX, u8 posY, u8 c) const;
		void	PutStrAt(u8 posX, u8 posY, const i8 *str) const;
		void	PutNbrAt(u8 posX, u8 posY, i32 nbr) const;
		void	PutHexAt(u8 posX, u8 posY, i32 nbr) const;
		void	PutPtrAt(u8 posX, u8 posY, const void *ptr) const;
		void	PrintAt(u8 posX, u8 posY, const i8 *fmt, ...) const;

	private:

		// ================================================================== //
		//    Data                                                            //
		// ================================================================== //

		static inline u8	cursorX_ = 0;
		static inline u8	cursorY_ = 0;

		static inline vga::Color	foregroundColor_ = vga::Color::White;
		static inline vga::Color	backgroundColor_ = vga::Color::Black;
};
