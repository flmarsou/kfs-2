#include "apps/applications/Terminal.hpp"

# include "tty/input/Input.hpp"
# include "utils/string.hpp"
# include "cpu/gdt/gdt.hpp"
# include "cpu/idt/idt.hpp"
# include "utils/sys/io.hpp"

// ========================================================================== //
//    Constructors & Destructor                                               //
// ========================================================================== //

Terminal::Terminal(TTY &tty)
	: tty_(tty)
{
	tty_.EnableCursor(2);
	tty_.SetCursor(0, 0);
	tty_.SetBackgroundColor(vga::Color::Black);
	tty_.SetForegroundColor(vga::Color::White);
	tty_.Clear(' ');
	tty_.PutStr(" > ");

	while (true)
	{
		if (!Input::Poll())
		{
			asm volatile ("hlt");
			continue ;
		}

		const InputEvent	input = Input::GetInput();

		switch (input.type)
		{
			case (InputKey::Escape): return ;

			case (InputKey::Character): tty_.PutChar(input.code); break ;
			case (InputKey::Backspace): tty_.Backspace(); break ;

			case (InputKey::Enter):
			{
				ParseCommand();
				tty_.PutStr(" > ");
				break ;
			}

			default: break ;
		}
	}
}

Terminal::~Terminal()
{
	tty_.DisableCursor();
}

// ========================================================================== //
//    Methods                                                                 //
// ========================================================================== //

void	Terminal::ParseCommand()
{
	i8	buffer[vga::WIDTH];

	for (i32 i = 3; i < vga::WIDTH - 1; ++i)
	{
		buffer[i - 3] = vga::getBuffer(i, tty_.GetCursorY());
		if (!(buffer[i - 3] >= 'a' && buffer[i - 3] <= 'z'))
		{
			buffer[i - 3] = '\0';
			break ;
		}
	}

	tty_.PutChar('\n');

	if (!strcmp(buffer, "gdt"))
		gdt::print(tty_);
	else if (!strcmp(buffer, "idt"))
		idt::print(tty_);
	else if (!strcmp(buffer, "help"))
		tty_.Print("Commands: gdt, idt, help, reboot");
	else if (!strcmp(buffer, "reboot"))
	{
		u8	good = 0x2;

		while (good & 0x2)
			good = inb(0x64);

		outb(0x64, 0xFE);
	}
	else
		tty_.Print("Unknown command, type \"help\"\n");

	tty_.PutChar('\n');
}
