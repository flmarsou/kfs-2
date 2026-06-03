#include "drivers/keyboard/Keyboard.hpp"

# include "cpu/pic/pic.hpp"

constexpr u8	KEY_RELEASE		= 0x80;
constexpr u8	KEY_EXTENDED	= 0xE0;

namespace
{
	extern "C" void	keyboard_handler(u8 scancode)
	{
		Keyboard::onIRQ(scancode);
		pic::send_eoi(1);
	}
}

// ========================================================================== //
//    Methods                                                                 //
// ========================================================================== //

void			Keyboard::onIRQ(u8 scancode)
{
	// Handle two-bytes extended scancodes
	if (scancode == KEY_EXTENDED)
	{
		extended_ = true;
		return ;
	}

	// Handle release action
	const bool	PRESSED = !(scancode & KEY_RELEASE);
	scancode &= ~KEY_RELEASE;

	// Extract extended byte
	const u16	FULLCODE = extended_ ? (0xE000 | scancode) : scancode;
	extended_ = false;

	//
	const KeyboardKey	KEY = static_cast<KeyboardKey>(FULLCODE);
	const u8			NEXT = (head_ + 1) % bufferSize_;

	if (NEXT == tail_)
		return ;

	buffer_[head_] = {KEY, PRESSED};
	head_ = NEXT;
}

bool			Keyboard::HasEvent()
{
	return (head_ != tail_);
}

KeyboardEvent	Keyboard::GetEvent()
{
	if (!HasEvent())
		return {KeyboardKey::Null, false};

	const KeyboardEvent	EVENT = buffer_[tail_];
	tail_ = (tail_ + 1) % bufferSize_;

	return (EVENT);
}
