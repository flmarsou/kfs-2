#pragma once

# include "utils/stdint.hpp"

enum class	KeyboardKey : u16
{
	Null = 0,

	// Letters

	Q			= 0x10,		W			= 0x11,		E			= 0x12,		R			= 0x13,
	T			= 0x14,		Y			= 0x15,		U			= 0x16,		I			= 0x17,
	O			= 0x18,		P			= 0x19,		A			= 0x1E,		S			= 0x1F,
	D			= 0x20,		F			= 0x21,		G			= 0x22,		H			= 0x23,
	J			= 0x24,		K			= 0x25,		L			= 0x26,		Z			= 0x2C,
	X			= 0x2D,		C			= 0x2E,		V			= 0x2F,		B			= 0x30,
	N			= 0x31,		M			= 0x32,

	// Digits

	Num1		= 0x02,		Num2		= 0x03,		Num3		= 0x04,		Num4		= 0x05,
	Num5		= 0x06,		Num6		= 0x07,		Num7		= 0x08,		Num8		= 0x09,
	Num9		= 0x0A,		Num0		= 0x0B,

	// Symbols

	Grave		= 0x29,		Minus		= 0x0C,		Equal		= 0x0D,		BracketL	= 0x1A,
	BracketR	= 0x1B,		Backslash	= 0x2B,		Semicolon	= 0x27,		Apostrophe	= 0x28,
	Comma		= 0x33,		Dot			= 0x34,		Slash		= 0x35,

	// Function

	F1			= 0x3B,		F2			= 0x3C,		F3			= 0x3D,		F4			= 0x3E,
	F5			= 0x3F,		F6			= 0x40,		F7			= 0x41,		F8			= 0x42,
	F9			= 0x43,		F10			= 0x44,		F11			= 0x57,		F12			= 0x58,

	// Controls

	Escape		= 0x01,		Delete		= 0xE053,	Tab			= 0x0F,		Backspace	= 0x0E,
	CapsLock	= 0x3A,		Enter		= 0x1C,		LeftShift	= 0x2A,		RightShift	= 0x36,
	LeftCtrl	= 0x1D,		LeftSuper	= 0xE05B,	LeftAlt		= 0x38,		Space		= 0x39,
	RightAlt	= 0xE038,	RightSuper	= 0xE05D,	RightCtrl	= 0xE01D,

	// Navigation

	ArrowUp		= 0xE048,	ArrowLeft	= 0xE04B,	ArrowDown	= 0xE050,	ArrowRight	= 0xE04D,
	Home		= 0xE047,	PageUp		= 0xE049,	End			= 0xE04F,	PageDown	= 0xE051,
};

struct	KeyboardEvent
{
	KeyboardKey	key;
	bool		pressed;
};

class	Keyboard
{
	public:

		// ================================================================== //
		//    Methods                                                         //
		// ================================================================== //

		static void				onIRQ(u8 scancode);
		static bool				HasEvent();
		static KeyboardEvent	GetEvent();

	private:

		// ================================================================== //
		//    Data                                                            //
		// ================================================================== //

		static constexpr u8			bufferSize_ = 128;
		static inline KeyboardEvent	buffer_[bufferSize_] = {KeyboardKey::Null, false};

		static inline u8	tail_ = 0;
		static inline u8	head_ = 0;

		static inline bool	extended_ = false;
};
