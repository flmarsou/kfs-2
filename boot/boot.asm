bits 32

; ============================================================================ ;
;    Multiboot Header (required by GRUB)                                       ;
; ============================================================================ ;

section .multiboot
	dd 0x1BADB002			; Magic Number
	dd 0x3					; Flags
	dd -(0x1BADB002 + 0x3)	; Checksum

; ============================================================================ ;
;    Bootloader                                                                ;
; ============================================================================ ;

section .text
	global	start
	extern	kmain

start:
	mov esp, stack_top	; Stack pointer
	call kmain			; Jump to C++ kernel

.hang:
	jmp .hang			; Infinite loop if kernel returns

section .bss
	resb 16384			; Stack size 16KiB
stack_top:
