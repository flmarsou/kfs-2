# ============================================================================ #
#    Paths                                                                     #
# ============================================================================ #

CXX_KERNEL	:=	\
				kernel/kmain.cpp \

CXX_CPU		:=	\
				kernel/cpu/gdt/gdt.cpp \
				kernel/cpu/idt/idt.cpp \
				kernel/cpu/pic/pic.cpp \

CXX_DRIVERS	:=	\
				kernel/drivers/vga/vga.cpp \
				kernel/drivers/timer/timer.cpp \
				kernel/drivers/keyboard/Keyboard.cpp \

CXX_TTY		:=	\
				kernel/tty/TTY.cpp \
				kernel/tty/input/Input.cpp \
				kernel/tty/draw/Button.cpp \
				kernel/tty/draw/shapes.cpp \

CXX_APPS	:=	\
				kernel/apps/Menu.cpp \

# ============================================================================ #
#    Sources                                                                   #
# ============================================================================ #

CXX_SRCS	:=	\
				${CXX_KERNEL} \
				${CXX_CPU} \
				${CXX_DRIVERS} \
				${CXX_TTY} \
				${CXX_APPS} \
