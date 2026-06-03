bits 32

section .text
	global	irq0_handler
	extern	timer_handler

irq0_handler:
	cli
	pusha

	call timer_handler

	popa
	iretd
