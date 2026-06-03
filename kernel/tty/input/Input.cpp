#include "tty/input/Input.hpp"

# include "drivers/keyboard/Keyboard.hpp"

// ========================================================================== //
//    Methods                                                                 //
// ========================================================================== //

bool		Input::Poll()
{
	while (Keyboard::HasEvent())
	{
		const KeyboardEvent	EVENT = Keyboard::GetEvent();

		if (updateModifier(EVENT))
			continue ;

		if (!EVENT.pressed)
			continue ;

		lastInput_ = {InputKey::Null, 0};

		i8	c = keyToChar(EVENT.key);
		if (c)
		{
			lastInput_ = {InputKey::Character, c};
			return (true);
		}
		else if (keyToSpecial(EVENT.key))
		{
			return (true);
		}
	}

	return (false);
}

InputEvent	Input::GetInput()
{
	return (lastInput_);
}

// ========================================================================== //
//    Functions                                                               //
// ========================================================================== //

bool	Input::updateModifier(const KeyboardEvent &event)
{
	switch (event.key)
	{
		case (KeyboardKey::LeftShift):	shift_ = event.pressed; return (true);
		case (KeyboardKey::RightShift):	shift_ = event.pressed; return (true);

		case (KeyboardKey::LeftCtrl):	ctrl_ = event.pressed; return (true);
		case (KeyboardKey::RightCtrl):	ctrl_ = event.pressed; return (true);

		case (KeyboardKey::CapsLock):	caps_ = !caps_; return (true);

		case (KeyboardKey::LeftAlt):	alt_ = event.pressed; return (true);
		case (KeyboardKey::RightAlt):	alt_ = event.pressed; return (true);

		case (KeyboardKey::LeftSuper):	super_ = event.pressed; return (true);
		case (KeyboardKey::RightSuper):	super_ = event.pressed; return (true);

		default: return (false);
	}
}

i8		Input::keyToChar(KeyboardKey key)
{
	switch (key)
	{
		// Letters

		case (KeyboardKey::Q): return ((shift_ ^ caps_) ? 'A' : 'a');
		case (KeyboardKey::W): return ((shift_ ^ caps_) ? 'W' : 'w');
		case (KeyboardKey::E): return ((shift_ ^ caps_) ? 'E' : 'e');
		case (KeyboardKey::R): return ((shift_ ^ caps_) ? 'R' : 'r');
		case (KeyboardKey::T): return ((shift_ ^ caps_) ? 'T' : 't');
		case (KeyboardKey::Y): return ((shift_ ^ caps_) ? 'Y' : 'y');
		case (KeyboardKey::U): return ((shift_ ^ caps_) ? 'U' : 'u');
		case (KeyboardKey::I): return ((shift_ ^ caps_) ? 'I' : 'i');
		case (KeyboardKey::O): return ((shift_ ^ caps_) ? 'O' : 'o');
		case (KeyboardKey::P): return ((shift_ ^ caps_) ? 'P' : 'p');
		case (KeyboardKey::A): return ((shift_ ^ caps_) ? 'A' : 'a');
		case (KeyboardKey::S): return ((shift_ ^ caps_) ? 'S' : 's');
		case (KeyboardKey::D): return ((shift_ ^ caps_) ? 'D' : 'd');
		case (KeyboardKey::F): return ((shift_ ^ caps_) ? 'F' : 'f');
		case (KeyboardKey::G): return ((shift_ ^ caps_) ? 'G' : 'g');
		case (KeyboardKey::H): return ((shift_ ^ caps_) ? 'H' : 'h');
		case (KeyboardKey::J): return ((shift_ ^ caps_) ? 'J' : 'j');
		case (KeyboardKey::K): return ((shift_ ^ caps_) ? 'K' : 'k');
		case (KeyboardKey::L): return ((shift_ ^ caps_) ? 'L' : 'l');
		case (KeyboardKey::Z): return ((shift_ ^ caps_) ? 'Z' : 'z');
		case (KeyboardKey::X): return ((shift_ ^ caps_) ? 'X' : 'x');
		case (KeyboardKey::C): return ((shift_ ^ caps_) ? 'C' : 'c');
		case (KeyboardKey::V): return ((shift_ ^ caps_) ? 'V' : 'v');
		case (KeyboardKey::B): return ((shift_ ^ caps_) ? 'B' : 'b');
		case (KeyboardKey::N): return ((shift_ ^ caps_) ? 'N' : 'n');
		case (KeyboardKey::M): return ((shift_ ^ caps_) ? 'M' : 'm');

		// Digits

		case (KeyboardKey::Num1): return ((shift_) ? '!' : '1');
		case (KeyboardKey::Num2): return ((shift_) ? '@' : '2');
		case (KeyboardKey::Num3): return ((shift_) ? '#' : '3');
		case (KeyboardKey::Num4): return ((shift_) ? '$' : '4');
		case (KeyboardKey::Num5): return ((shift_) ? '%' : '5');
		case (KeyboardKey::Num6): return ((shift_) ? '^' : '6');
		case (KeyboardKey::Num7): return ((shift_) ? '&' : '7');
		case (KeyboardKey::Num8): return ((shift_) ? '*' : '8');
		case (KeyboardKey::Num9): return ((shift_) ? '(' : '9');
		case (KeyboardKey::Num0): return ((shift_) ? ')' : '0');

		// Symbols

		case (KeyboardKey::Grave):		return ((shift_) ? '~' : '`');
		case (KeyboardKey::Minus):		return ((shift_) ? '_' : '-');
		case (KeyboardKey::Equal):		return ((shift_) ? '+' : '=');
		case (KeyboardKey::BracketL):	return ((shift_) ? '{' : '[');
		case (KeyboardKey::BracketR):	return ((shift_) ? '}' : ']');
		case (KeyboardKey::Backslash):	return ((shift_) ? '|' : '\\');
		case (KeyboardKey::Semicolon):	return ((shift_) ? ':' : ';');
		case (KeyboardKey::Apostrophe):	return ((shift_) ? '"' : '\'');
		case (KeyboardKey::Comma):		return ((shift_) ? '<' : ',');
		case (KeyboardKey::Dot):		return ((shift_) ? '>' : '.');
		case (KeyboardKey::Slash):		return ((shift_) ? '?' : '/');

		case (KeyboardKey::Space):		return (' ');
		case (KeyboardKey::Tab):		return ('\t');

		default: return (0);
	}
}

bool	Input::keyToSpecial(KeyboardKey key)
{
	switch (key)
	{
		case (KeyboardKey::F1):			lastInput_.type = InputKey::F1; return (true);
		case (KeyboardKey::F2):			lastInput_.type = InputKey::F2; return (true);
		case (KeyboardKey::F3):			lastInput_.type = InputKey::F3; return (true);
		case (KeyboardKey::F4):			lastInput_.type = InputKey::F4; return (true);
		case (KeyboardKey::F5):			lastInput_.type = InputKey::F5; return (true);
		case (KeyboardKey::F6):			lastInput_.type = InputKey::F6; return (true);
		case (KeyboardKey::F7):			lastInput_.type = InputKey::F7; return (true);
		case (KeyboardKey::F8):			lastInput_.type = InputKey::F8; return (true);
		case (KeyboardKey::F9):			lastInput_.type = InputKey::F9; return (true);
		case (KeyboardKey::F10):		lastInput_.type = InputKey::F10; return (true);
		case (KeyboardKey::F11):		lastInput_.type = InputKey::F11; return (true);
		case (KeyboardKey::F12):		lastInput_.type = InputKey::F12; return (true);

		case (KeyboardKey::Enter):		lastInput_.type = InputKey::Enter; return (true);
		case (KeyboardKey::Escape):		lastInput_.type = InputKey::Escape; return (true);
		case (KeyboardKey::Delete):		lastInput_.type = InputKey::Delete; return (true);
		case (KeyboardKey::Backspace):	lastInput_.type = InputKey::Backspace; return (true);
		case (KeyboardKey::ArrowUp):	lastInput_.type = InputKey::ArrowUp; return (true);
		case (KeyboardKey::ArrowLeft):	lastInput_.type = InputKey::ArrowLeft; return (true);
		case (KeyboardKey::ArrowDown):	lastInput_.type = InputKey::ArrowDown; return (true);
		case (KeyboardKey::ArrowRight):	lastInput_.type = InputKey::ArrowRight; return (true);
		case (KeyboardKey::Home):		lastInput_.type = InputKey::Home; return (true);
		case (KeyboardKey::PageUp):		lastInput_.type = InputKey::PageUp; return (true);
		case (KeyboardKey::End):		lastInput_.type = InputKey::End; return (true);
		case (KeyboardKey::PageDown):	lastInput_.type = InputKey::PageDown; return (true);

		default: return (false);
	}
}
