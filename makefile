# Tools
CC := gcc
LD := ld
NASM := nasm
OBJCOPY := objcopy
OBJDUMP := objdump
QEMU := qemu-system-i386
GDB := gdb
PYTHON := python3
export CC LD NASM OBJCOPY OBJDUMP QEMU GDB PYTHON

# Root directory
ROOT_DIR = $(CURDIR)
export ROOT_DIR

# Arch
ARCH = x86

# Do not print 'enter directory...'
MAKEFLAGS += --no-print-directory

# Standerd include directories
SRC_INCLUDE := -I$(ROOT_DIR)
KERNEL_INCLUDE := -I$(ROOT_DIR)/include

# Common build flags used by gcc
BUILD_FLAGS	:= -Wall -O0 -g
BUILD_FLAGS += -fno-strict-aliasing
BUILD_FLAGS += -fno-builtin
BUILD_FLAGS += -fno-pic
BUILD_FLAGS += -fno-omit-frame-pointer
BUILD_FLAGS += -fno-stack-protector
BUILD_FLAGS += $(KERNEL_INCLUDE)
BUILD_FLAGS += $(SRC_INCLUDE)

# Used by '.c'
CFLAGS := $(BUILD_FLAGS)

# Used by '.S'
ASFLAGS := $(BUILD_FLAGS)
ASFLAGS += -D__ASSEMBLY__

# Link options
LDFLAGS := 

export LDFLAGS BUILD_FLAGS CFLAGS ASFLAGS

# Kernel build in objects
BUILD_IN_Y :=
BUILD_IN_Y += arch/$(ARCH)
BUILD_IN_Y += lib
BUILD_IN_Y += kernel

CASUAL_OBJS := $(addsuffix /build-in.a, $(BUILD_IN_Y))
CASUAL_OBJS := $(addprefix $(ROOT_DIR)/, $(CASUAL_OBJS))

all: casual.bin
	@dd if=$< of=a.img seek=17 conv=notrunc

setup:
	@$(MAKE) -C $(ROOT_DIR)/arch/$(ARCH)/boot

casual:
	@$(foreach dir, $(BUILD_IN_Y), $(MAKE) -C $(ROOT_DIR)/$(dir);)
	@$(LD) -T $(ROOT_DIR)/scripts/casual.ld -o $@ $(CASUAL_OBJS)

casual.bin: casual
	$(OBJCOPY) -S -O binary $< $@

build:
	@-rm -f $(ROOT_DIR)/include/asm
	@ln -s $(ROOT_DIR)/arch/$(ARCH)/include/ $(ROOT_DIR)/include/asm

img: loader
	@$(PYTHON) $(ROOT_DIR)/scripts/mkimg.py $(ROOT_DIR)/arch/$(ARCH)/boot/

.PHONY: clean
clean:
	@cd arch/$(ARCH)/boot && $(MAKE) clean
	@$(foreach dir, $(BUILD_IN_Y), cd $(ROOT_DIR)/$(dir) && $(MAKE) clean;)
	@-rm -f casual casual.bin
