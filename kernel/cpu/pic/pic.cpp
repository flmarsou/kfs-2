#include "cpu/pic/pic.hpp"

# include "utils/sys/io.hpp"

namespace	port
{
	constexpr u16	PIC1_COMMAND	= 0x20;	// I/O port for sending commands to Master PIC
	constexpr u16	PIC1_DATA		= 0x21;	// I/O port for sending/receiving data to/from Master PIC
	constexpr u16	PIC2_COMMAND	= 0xA0;	// I/O port for sending commands to Slave PIC
	constexpr u16	PIC2_DATA		= 0xA1;	// I/O port for sending/receiving data to/from Slave PIC
}

namespace	value
{
	constexpr u8	ICW1_INIT		= 0x10;	// Initialize PIC sequence
	constexpr u8	ICW1_ICW4		= 0x01;	// ICW4 initialization
	constexpr u8	ICW4_8086		= 0x01;	// 8086/88 mode

	constexpr u8	IRQ0			= 0x20;	// Master PIC interrupt offset
	constexpr u8	IRQ8			= 0x28;	// Slave PIC interrupt offset

	constexpr u8	EOI				= 0x20;	// End Of Interrupt command
}

namespace	wiring
{
	constexpr u8	MASTER_ON_IRQ2	= 0x04;
	constexpr u8	SLAVE_ON_IRQ2	= 0x02;
}

namespace	mask
{
	// Master PIC masks (IRQ0-IRQ7)

	constexpr u8	MASTER_ALL		= 0b1111'1111;

	constexpr u8	TIMER			= 0b1111'1110;	// CPU Clock
	constexpr u8	KEYBOARD		= 0b1111'1101;	// Keyboard
	constexpr u8	CASCADE			= 0b1111'1011;	// Cascade (Slave PIC)
	constexpr u8	COM2			= 0b1111'0111;	// Communication
	constexpr u8	COM1			= 0b1110'1111;	// Communication
	constexpr u8	LPT2			= 0b1101'1111;	// Line Printer Terminal
	constexpr u8	FLOPPY			= 0b1011'1111;	// Floppy
	constexpr u8	LPT1			= 0b0111'1111;	// Line Printer Terminal

	// Slave PIC masks (IRQ8-IRQ-15)

	constexpr u8	SLAVE_ALL		= 0b1111'1111;

	constexpr u8	RTC				= 0b1111'1110;	// Real Time Clock
	constexpr u8	IRQ9			= 0b1111'1101;	// Free
	constexpr u8	IRQ10			= 0b1111'1011;	// Free
	constexpr u8	IRQ11			= 0b1111'0111;	// Free
	constexpr u8	PS2_MOUSE		= 0b1110'1111;	// Mouse
	constexpr u8	FPU_COPROCESSOR	= 0b1101'1111;	// Float Point Unit
	constexpr u8	IDE1			= 0b1011'1111;	// Integrated Drive Electronics
	constexpr u8	IDE2			= 0b0111'1111;	// Integrated Drive Electronics
}

namespace	pic
{
	void	send_eoi(u8 irq)
	{
		if (irq >= 8)
			outb(port::PIC2_COMMAND, value::EOI);
		outb(port::PIC1_COMMAND, value::EOI);
	}

	void	init()
	{
		// ICW1: Start initialization
		outb(port::PIC1_COMMAND, value::ICW1_INIT | value::ICW1_ICW4);	// Master PIC
		outb(port::PIC2_COMMAND, value::ICW1_INIT | value::ICW1_ICW4);	// Slave PIC

		// ICW2: Set interrupt vector offsets
		outb(port::PIC1_DATA, value::IRQ0);	// Master offset (INT 32-39)
		outb(port::PIC2_DATA, value::IRQ8);	// Slave offset (INT 40-47)

		// ICW3: Define Master/Slave wiring
		outb(port::PIC1_DATA, wiring::MASTER_ON_IRQ2);	// Master has a slave on IRQ2
		outb(port::PIC2_DATA, wiring::SLAVE_ON_IRQ2);	// Slave connected to Master's IRQ2

		// ICW4: Set x84 mode
		outb(port::PIC1_DATA, value::ICW4_8086);
		outb(port::PIC2_DATA, value::ICW4_8086);

		// OCW1: Set initial IRQ masks
		outb(port::PIC1_DATA, mask::TIMER & mask::KEYBOARD);
		outb(port::PIC2_DATA, mask::SLAVE_ALL);
	}
}
