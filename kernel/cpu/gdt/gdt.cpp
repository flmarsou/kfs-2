#include "cpu/gdt/gdt.hpp"

# include "utils/stdint.hpp"

# define PACKED	__attribute__((packed))

namespace	type
{
	constexpr u8	DATA_R		= 0x00;	// Read-Only
	constexpr u8	DATA_RA		= 0x01;	// Read-Only, Accessed
	constexpr u8	DATA_RW		= 0x02;	// Read/Write
	constexpr u8	DATA_RWA	= 0x03;	// Read/Write, Accessed
	constexpr u8	DATA_RE		= 0x04;	// Read-Only, Expand
	constexpr u8	DATA_REA	= 0x05;	// Read-Only, Expand, Accessed
	constexpr u8	DATA_RWE	= 0x06;	// Read/Write, Expand
	constexpr u8	DATA_RWEA	= 0x07;	// Read/Write, Expand, Accessed

	constexpr u8	CODE_E		= 0x08;	// Execute-Only
	constexpr u8	CODE_EA		= 0x09;	// Execute-Only, Accessed
	constexpr u8	CODE_ER		= 0x0A;	// Execute/Read
	constexpr u8	CODE_ERA	= 0x0B;	// Execute/Read, Accessed
	constexpr u8	CODE_EC		= 0x0C;	// Execute-Only, Conforming
	constexpr u8	CODE_ECA	= 0x0D;	// Execute-Only, Conforming, Accessed
	constexpr u8	CODE_ERC	= 0x0E;	// Execute/Read, Conforming
	constexpr u8	CODE_ERCA	= 0x0F;	// Execute/Read, Conforming, Accessed
}

namespace	dpl
{
	constexpr u8	RING0		= 0x00;	// Privilege level 0 (Kernel)
	constexpr u8	RING1		= 0x20;	// Privilege level 1
	constexpr u8	RING2		= 0x40;	// Privilege level 2
	constexpr u8	RING3		= 0x60;	// Privilege level 3 (User)
}

namespace	access
{
	constexpr u8	PRESENT		= 0x80;	// Segment is present in memory
	constexpr u8	CODEDATA	= 0x10;	// Segment is code or data
	constexpr u8	SYSTEM		= 0x00;	// Segment is system
}

namespace	flags
{
	constexpr u8	BYTE		= 0x00;	// Limit is in bytes
	constexpr u8	PAGE		= 0x80;	// Limit is in 4 KiB pages
	constexpr u8	SIZE_16		= 0x00;	// 16-bit protected mode segment
	constexpr u8	SIZE_32		= 0x40;	// 32-bit protected mode segment
}

/**
 *                                        Segment Descriptor
 * |===============|===============|===============|=======================|=======================|
 * | 63         56 | 55         52 | 51         48 | 47                 40 | 39                 32 |
 * | Base          | Flags         | Limit         | Access Byte           | Base                  |
 * |===============|===============|===============|=======================|=======================|
 * | 31                                         16 | 15                                          0 |
 * | Base                                          | Limit                                         |
 * |===============================================|===============================================|
 * 
 * - Base:  A 32-bit value containing the linear address where the segment begins.
 * - Limit: A 20-bit value telling the maximum addressable unit (granularity).
 * 
 *                    Access Byte
 * |=====|===========|=====|=====|=====|=====|=====|
 * |  7  | 6       5 |  4  |  3  |  2  |  1  |  0  |
 * | P   | DPL       | S   | E   | DC  | RW  | A   |
 * |=====|===========|=====|=====|=====|=====|=====|
 * 
 * - P:   Present bit. Allows an entry to refer to a valid segment.
 * - DPL: Descriptor Privilege Level
 * - S:   Descriptor type bit. Defines system or code/date segment.
 * - E:   Executable bit. Defines a code or data segment.
 * - DC:  Conforming bit for code segment, expand-down for data segment.
 * - RW:  Read/Write bit.
 * - A:   Accessed bit.
 * 
 *           Flags
 * |=====|=====|=====|=====|
 * |  3  |  2  |  1  |  0  |
 * | G   | DB  | L   | N/A |
 * |=====|=====|=====|=====|
 * 
 * - G:  Granularity bit. Sets limit to 1 Byte Blocks or 4 KiB pages.
 * - DB: Size bit. Sets to 16-bit or 32-bit.
 * - L:  Long-Mode bit. Sets to 64-bit.
 */
struct	Entry
{
	u16	limitLow;
	u16	baseLow;

	u8	baseMid;
	u8	access;
	u8	flags;
	u8	baseHigh;
}	PACKED;

struct	Pointer
{
	u16	size;
	u32	data;
}	PACKED;

namespace
{
	extern "C" void	load_gdt(const Pointer *ptr);

	void	create_descriptor(Entry &entry, u32 base, u32 limit, u8 access, u8 flags)
	{
		entry.limitLow = limit & 0xFFFF;
		entry.baseLow = base & 0xFFFF;

		entry.baseMid = (base >> 16) & 0xFF;
		entry.access = access;
		entry.flags = ((limit >> 16) & 0x0F) | (flags & 0xF0);
		entry.baseHigh = (base >> 24) & 0xFF;
	}
}

namespace	gdt
{
	static Entry	entry[4];
	static Pointer	ptr;

	void	init()
	{
		// Null
		create_descriptor(entry[0], 0, 0, 0, 0);

		// Kernel Code
		create_descriptor(entry[1], 0, 0xFFFFF,
			access::PRESENT | dpl::RING0 | access::CODEDATA | type::CODE_ER,
			flags::PAGE | flags::SIZE_32
		);

		// Kernel Data
		create_descriptor(entry[2], 0, 0xFFFFF,
			access::PRESENT | dpl::RING0 | access::CODEDATA | type::DATA_RW,
			flags::PAGE | flags::SIZE_32
		);

		// Kernel Stack
		create_descriptor(entry[3], 0, 0xFFFFF,
			access::PRESENT | dpl::RING0 | access::CODEDATA | type::DATA_RW,
			flags::PAGE | flags::SIZE_32
		);

		ptr.size = sizeof(entry) - 1;
		ptr.data = reinterpret_cast<u32>(&entry);

		load_gdt(&ptr);
	}

	void	print(TTY &tty)
	{
		tty.PutStr("GDT Dump:\n\n");

		for (i32 i = 1; i <= 3; ++i)
		{
			const Entry	&E = entry[i];

			const i8	*RING = ((E.access & 0x60) >> 5) ? "User" : "Kernel";
			const i8	*SEG = (E.access & 0x08) ? "Code" : "Data";

			tty.Print("Entry[%i]: %s %s Segment -> %p\n", i, RING, SEG, &E);
		}
	}
}
