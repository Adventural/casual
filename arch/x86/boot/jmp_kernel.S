#include <asm/cpuflags.h>
#include "config.h"

    .code16
.global jmp_to_kernel
jmp_to_kernel:
    /* protect mode */
    movl    %cr0, %eax
    orl     $X86_CR0_PE, %eax
    movl    %eax, %cr0

    ljmpl   $0x8, $entry_32+(SETUP_CODE_SEG << 4)

    .code32
entry_32:
    cli
    /* reset data segment */
    movl $(2 << 3), %eax
    movl %eax, %ds
    movl %eax, %es
    movl %eax, %ss
    movl %eax, %fs
    movl %eax, %gs

    call   setup_page

    mov     $0x0, %eax
    mov     %eax, %cr3

    movl    %cr0, %eax
    orl     $X86_CR0_PG, %eax
    movl    %eax, %cr0

    ljmp   $0x8,$0xc0010000

