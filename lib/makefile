CFLAGS += -m32

LIB_SRCS = $(wildcard *.c)

LIB_OBJS = $(addsuffix .o, $(basename $(LIB_SRCS)))

all: build-in.a

# Partial link
build-in.a: $(LIB_OBJS)
	@$(LD) -T $(ROOT_DIR)/scripts/casual.ld -r $^ -o $@

include $(ROOT_DIR)/scripts/compile.mk

clean:
	-rm -f *.o build-in.a