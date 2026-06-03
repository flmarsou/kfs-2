#include "cpu/gdt/gdt.hpp"
#include "cpu/idt/idt.hpp"
#include "cpu/pic/pic.hpp"
#include "apps/Menu.hpp"

extern "C" void	kmain()
{
	gdt::init();
	idt::init();
	pic::init();

	TTY		tty;
	Menu	menu(tty);

	menu.Run();
}
