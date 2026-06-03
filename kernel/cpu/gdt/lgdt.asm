bits 32

section .text
	global	load_gdt

load_gdt:
	mov eax, [esp + 4]
	lgdt [eax]

	; Load kernel data segments
	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax

	; Load kernel stack segment
	mov ax, 0x18
	mov ss, ax

	; Far jump to load kernel code segment
	jmp 0x08:.flush

.flush:
	ret
