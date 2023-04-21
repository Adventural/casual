CFLAGS += -m32

KERNEL_SRCS = $(wildcard *.c)

KERNEL_OBJS = $(addsuffix .o, $(basename $(KERNEL_SRCS)))

all: build-in.a

# Partial link
build-in.a: $(KERNEL_OBJS)
	@$(LD) -T $(ROOT_DIR)/scripts/casual.ld -r $^ -o $@

include $(ROOT_DIR)/scripts/compile.mk

clean:
	-rm -f *.o build-in.a