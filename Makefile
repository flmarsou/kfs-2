COLOR_GREEN		:=	\033[32m
COLOR_BLUE		:=	\033[36m
COLOR_YELLOW	:=	\033[33m
COLOR_RESET		:=	\033[0m
MUTE_OUTPUT		:=	2>/dev/null

# ============================================================================ #
#     Compilers / Tools                                                        #
# ============================================================================ #

CXX			:=	gcc
ASM			:=	nasm
LD			:=	ld

CXXFLAGS	:=	-m32 \
				-fno-builtin -fno-exceptions -fno-stack-protector -fno-rtti -fno-pic \
				-nostdlib -nodefaultlibs \
				-Wall -Werror -Wextra \
				-O2
ASMFLAGS	:=	-f elf32
LDFLAGS		:=	-m elf_i386 -T

# ============================================================================ #
#     Files                                                                    #
# ============================================================================ #

# Source Files
include make/cxx_sources.mk	# Import CXX_SRCS
include make/asm_sources.mk	# Import ASM_SRCS

# Object Files
CXX_OBJS	:=	${patsubst %.cpp, build/%.o, ${CXX_SRCS}}
ASM_OBJS	:=	${patsubst %.asm, build/%.o, ${ASM_SRCS}}

# Configuration Files
LINKER_CFG	:=	boot/linker.ld
GRUB_CFG	:=	boot/grub.cfg

# Binary Files
ELF			:=	build/iso/kfs
ISO			:=	build/iso/kfs.iso

# ============================================================================ #
#     Rules                                                                    #
# ============================================================================ #

#    Kernel Compilation                #
# ------------------------------------ #

build/%.o:	%.asm
	@mkdir -p ${dir $@}
	@echo "${COLOR_BLUE}Compiling \"$<\"...${COLOR_RESET}"
	@${ASM} ${ASMFLAGS} $< -o $@

build/%.o:	%.cpp
	@mkdir -p ${dir $@}
	@echo "${COLOR_BLUE}Compiling \"$<\"...${COLOR_RESET}"
	@${CXX} ${CXXFLAGS} -Ikernel -c $< -o $@

#    Kernel Linking                    #
# ------------------------------------ #

${ELF}:	${ASM_OBJS} ${CXX_OBJS} ${LINKER_CFG}
	@echo "${COLOR_GREEN}Compilation successful!${COLOR_RESET}"
	@mkdir -p build/iso
	@echo "${COLOR_BLUE}Linking object files...${COLOR_RESET}"
	@${LD} ${LDFLAGS} ${LINKER_CFG} -o $@ ${ASM_OBJS} ${CXX_OBJS}
	@echo "${COLOR_GREEN}Linking successful!${COLOR_RESET}"

#    Build ISO                         #
# ------------------------------------ #

${ISO}:	${ELF}
	@mkdir -p build/iso/boot/grub
	@cp ${GRUB_CFG} build/iso/boot/grub/grub.cfg
	@echo "${COLOR_BLUE}Building ISO...${COLOR_RESET}"
	@grub-mkrescue -o ${ISO} build/iso ${MUTE_OUTPUT}
# 	@grub-mkrescue -o ${ISO} build/iso --compress=xz ${MUTE_OUTPUT}
	@echo "${COLOR_GREEN}Building successful!${COLOR_RESET}"

#    Commands                          #
# ------------------------------------ #

all:	${ASM_OBJS} ${CXX_OBJS} ${ELF} ${ISO}

clean:
	@echo "${COLOR_YELLOW}Removing \"build\" directory${COLOR_RESET}"
	@rm -rf build

re:		clean all

run:
	@echo "${COLOR_GREEN}Running KFS...${COLOR_RESET}"
	@qemu-system-i386 -cdrom ${ISO}
