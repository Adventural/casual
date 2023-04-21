CFLAGS += -m32
ASFLAGS += -m32

all: build-in.a

ARCH_SRCS := 
ARCH_SRCS += $(wildcard *.c)
ARCH_SRCS += $(wildcard *.S)
ARCH_OBJS := $(addsuffix .o, $(basename $(ARCH_SRCS)))

# Partial link
build-in.a: $(ARCH_OBJS)
	@$(LD) -r -T $(ROOT_DIR)/scripts/casual.ld -o $@ $^

include $(ROOT_DIR)/scripts/compile.mk

clean:
	-rm -f *.o build-in.a