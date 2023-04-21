#include "boot.h"
#include "config.h"
#include <casual/compile.h>
#include <asm/segment.h>

static u64_t gdt_table[3] __attribute__((aligned(16)));
static struct gdt_ptr gdtr __attribute__((aligned(16)));

static void enable_a20()
{
    /* ref: https://github.com/mit-pdos/xv6-public/blob/master/bootasm.S */
    while((inb(0x64) & 0x2U) != 0){
        ;
    };
    outb(0xd1, 0x64);
    while((inb(0x64) & 0x2U) != 0){
        ;
    };
    outb(0xdf, 0x60);
}


void __noreturn setup_kernel()
{
    /* flat mode */
    gdt_table[0] = GDT_ENTRY(0, 0, 0);
    gdt_table[1] = GDT_ENTRY(0xc09b, 0, 0xfffff);
    gdt_table[2] = GDT_ENTRY(0xc093, 0, 0xfffff);
    gdtr.len = sizeof(u64_t)*3 - 1;
    gdtr.ptr = (u32_t)gdt_table + (ds() << 4);

    enable_a20();

    /* load gdt */
    __asm__ volatile(
        "lgdtl %0\n\t"
        "jmp jmp_to_kernel"
        :
        : "m" (gdtr)
    );

    unreachable();
}