#include "params.h"
#include "layout.h"
#include <casual/kernel.h>
#include <asm/string.h>
#include <asm/x86.h>

extern char* _bss_start[];
extern char* _bss_end[];

extern void cga_init();
extern void serial_init();
extern void arch_mm_init();
extern void start_kernel();

struct boot_params* boot_params = (struct boot_params*)BOOT_PARAMS_OFFSET;

extern void putc(char c);
void __noreturn arch_start_kernel()
{
    /* bss */
    memset(_bss_start, '0', _bss_end-_bss_start);

    /* init console, enable kprintf */
    cga_init();
    serial_init();
    kprintf(KERNEL_INFO, "casual start...\n");

    /* mmu */
    arch_mm_init(&boot_params->e820_entry);

    /* traps */

    /* kernel */
    start_kernel();

    unreachable();
}