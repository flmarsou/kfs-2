#pragma once

# include "utils/stdint.hpp"

/**
 * Write a byte to an I/O port.
 * 
 * @param port	The I/O port number (e.g. 0x60 for keyboard, 0x20 for PIC).
 * @param value	The byte value to send to the port.
 */
static inline void	outb(u16 port, u8 value)
{
	asm volatile ("outb %0, %1" : : "a"(value), "Nd"(port));
}

/**
 * Read a byte from an I/O port.
 * 
 * @param port	The I/O port number (e.g. 0x60 for keyboard, 0x20 for PIC).
 * @return		The byte value read from the port.
 */
static inline u8	inb(u16 port)
{
	u8	ret;

	asm volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));

	return (ret);
}
