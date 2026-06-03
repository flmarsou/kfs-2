#include "cpu/idt/idt.hpp"

# include "utils/stdint.hpp"

# define PACKED	__attribute__((packed))

namespace	ivt
{
	constexpr u8	TIMER		= 0x20;
	constexpr u8	KEYBOARD	= 0x21;
}

namespace	segment
{
	constexpr u8	KERNEL_CODE		= 0x08;
	constexpr u8	KERNEL_DATA		= 0x10;
}

namespace	gate
{
	constexpr u8	INTERRUPT_16	= 0x06;
	constexpr u8	GATE_16			= 0x07;
	constexpr u8	INTERRUPT_32	= 0x8E;
	constexpr u8	GATE_32			= 0x8F;
}

/**
 *                                          Gate Descriptor
 * |===============================================|====|========|====|============|===============|
 * | 63                                         48 | 47 | 46  45 | 44 | 43      40 | 39         32 |
 * | Offset                                        | P  | DPL    | 0  | Gate Type  | Reserved      |
 * |===============================================|====|========|====|============|===============|
 * | 31                                         16 | 15                                          0 |
 * | Segment Selector                              | Offset                                        |
 * |===============================================|===============================================|
 * 
 * - Offset:    A 32-bit value representing the address of the entry point.
 * - Selector:  A Segment Selector with multiple fields which must point to a valid GDT code segment.
 * - Gate Type: A 4-bit value which defines the type of gate this Interrupt Descriptor represents.
 *     - 0x5: Task Gate
 *     - 0x6: 16-bit Interrupt Gate
 *     - 0x7: 16-bit Trap Gate
 *     - 0xE: 32-bit Interrupt Gate
 *     - 0xF: 32-bit Trap Gate
 * - DPL:       Descriptor Privilege Level
 * - P:         Present bit. Allows an entry to refer to a valid segment.
 */
struct	Entry
{
	u16	offsetLow;
	u16	selector;

	u8	zero;
	u8	gate;
	u16	offsetHigh;
}	PACKED;

struct	Pointer
{
	u16	size;
	u32	data;
}	PACKED;

namespace
{
	extern "C" void	load_idt(const Pointer *ptr);

	extern "C" void	irq0_handler();
	extern "C" void	irq1_handler();

	void	create_descriptor(Entry &entry, u32 offset, u16 selector, u8 gate)
	{
		entry.offsetLow = offset & 0xFFFF;
		entry.selector = selector;

		entry.zero = 0;
		entry.gate = gate;
		entry.offsetHigh = (offset >> 16) & 0xFFFF;
	}
}

namespace	idt
{
	static Entry	entry[256];
	static Pointer	ptr;

	void	init()
	{
		for (u32 i = 0; i < 256; ++i)
			create_descriptor(entry[i], 0, 0 ,0);

		// Timer
		create_descriptor(
			entry[ivt::TIMER],
			reinterpret_cast<u32>(irq0_handler),
			segment::KERNEL_CODE,
			gate::INTERRUPT_32
		);

		// Keyboard
		create_descriptor(
			entry[ivt::KEYBOARD],
			reinterpret_cast<u32>(irq1_handler),
			segment::KERNEL_CODE,
			gate::INTERRUPT_32
		);

		ptr.size = sizeof(entry) - 1;
		ptr.data = reinterpret_cast<u32>(&entry);

		load_idt(&ptr);

		asm volatile("sti");
	}

	void	print(TTY &tty)
	{
		tty.PutStr("IDT Dump:\n\n");

		for (i32 i = 0; i < 256; ++i)
		{
			const Entry	&E = entry[i];

			if (E.offsetLow == 0 && E.offsetHigh == 0)
				continue ;

			const i8	*type = "Unknown";

			switch (i)
			{
				case (ivt::TIMER):		type = "Timer"; break ;
				case (ivt::KEYBOARD):	type = "Keyboard"; break ;

				default: break ;
			}

			tty.Print("Entry[%i]: %s -> %p\n", i, type, &E);
		}
	}
}
