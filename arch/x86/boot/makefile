CFLAGS += -m16
ASFLAGS += -m32

all: mbr setup.bin

SETUP_SRCS := \
	head.S \
	puts.c \
	bioscall.S \
	main.c \
	memory.c \
	protect.c \
	jmp_kernel.S

SETUP_OBJS := $(addsuffix .o, $(basename $(SETUP_SRCS)))

mbr: mbr.o
	@$(LD) -T $(ROOT_DIR)/scripts/mbr.ld -o mbr.bin $^

setup.bin: setup.elf
	$(OBJCOPY) -S -O binary $< $@ 

setup.elf: $(SETUP_OBJS) page.o
	@$(LD) -T $(ROOT_DIR)/scripts/setup.ld -o $@ $^

page.o:	page.c
	@echo + cc $(CURDIR)/$<
	@$(CC) $(BUILD_FLAGS) -m32 -c -o $@ $<

include $(ROOT_DIR)/scripts/compile.mk

clean:
	-rm -f *.o loader mbr.bin setup.bin setup.elf