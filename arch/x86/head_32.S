/*
 *  32bit kernel entry.
 *  the setup code will be overwrite, so we need to reset the gdt
 */

#include <asm/segment.h>
#include "layout.h"

    .code32
    .section .stext, "ax"
.global _start
_start:
    /* reset gdt and refresh %cs */
    movl %cs, %eax
    movl %eax, %ds
    lgdt gdt_descriptor
    ljmp $8, $start

start:
    /* close interrupt until idt was initialized */
    cli
    /* data segment */
    movl $(2 << 3), %eax
    movl %eax, %ds
    movl %eax, %es
    movl %eax, %ss
    movl %eax, %fs
    movl %eax, %gs

    /* stack */
    movl $KERNEL_STACK, %esp
    movl $KERNEL_STACK, %ebp

    jmp arch_start_kernel

    .section .data
    .align 16
gdt_start:
    .quad 0
    .quad GDT_ENTRY(0xc09b, 0, 0xfffff)
    .quad GDT_ENTRY(0xc093, 0, 0xfffff)
gdt_descriptor:
    .word gdt_descriptor - gdt_start - 1
    .long gdt_start
