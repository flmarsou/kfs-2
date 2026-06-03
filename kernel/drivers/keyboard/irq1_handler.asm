bits 32

section .text
	global	irq1_handler
	extern	keyboard_handler

irq1_handler:
	cli
	pusha

	in al, 0x60				; Read Scancode
	push eax

	call keyboard_handler	; Send Scancode

	add esp, 4
	mov al, 0x20
	out 0x20, al

	popa
	iretd
