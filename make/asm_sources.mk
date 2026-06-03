# ============================================================================ #
#    Paths                                                                     #
# ============================================================================ #

ASM_BOOTLOADER	:=	\
					boot/boot.asm \

ASM_CPU			:=	\
					kernel/cpu/gdt/lgdt.asm \
					kernel/cpu/idt/lidt.asm \

ASM_DRIVERS		:=	\
					kernel/drivers/timer/irq0_handler.asm \
					kernel/drivers/keyboard/irq1_handler.asm \

# ============================================================================ #
#    Sources                                                                   #
# ============================================================================ #

ASM_SRCS	:=	\
				${ASM_BOOTLOADER} \
				${ASM_CPU} \
				${ASM_DRIVERS} \
