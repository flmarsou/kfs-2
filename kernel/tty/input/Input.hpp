#pragma once

# include "utils/stdint.hpp"
# include "drivers/keyboard/Keyboard.hpp"

enum class	InputKey : u8
{
	Null = 0,

	Character,

	CapsLock,
	LeftShift,	RightShift,	LeftCtrl,	RightCtrl,
	LeftAlt,	RightAlt,	LeftSuper,	RightSuper,

	F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,

	Enter,		Escape,		Delete,		Backspace,
	ArrowUp,	ArrowLeft,	ArrowDown,	ArrowRight,
	Home,		PageUp,		End,		PageDown,
};

struct	InputEvent
{
	InputKey	type;
	i8			code;
};

class	Input
{
	public:

		// ================================================================== //
		//    Methods                                                         //
		// ================================================================== //

		static bool			Poll();
		static InputEvent	GetInput();

	private:

		// ================================================================== //
		//    Functions                                                       //
		// ================================================================== //

		static bool	updateModifier(const KeyboardEvent &event);
		static i8	keyToChar(KeyboardKey key);
		static bool	keyToSpecial(KeyboardKey key);

		// ================================================================== //
		//    Data                                                            //
		// ================================================================== //

		static inline bool	shift_ = false;
		static inline bool	ctrl_ = false;
		static inline bool	caps_ = false;
		static inline bool	alt_ = false;
		static inline bool	super_ = false;

		static inline InputEvent	lastInput_ = {InputKey::Null, 0};
};
