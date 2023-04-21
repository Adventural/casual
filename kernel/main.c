#include <casual/kernel.h>
#include <casual/page.h>
#include <casual/pgconvert.h>
#include <asm/x86.h>
#include <asm/string.h>

extern void page_init();
extern void kmalloc_init();

void start_kernel()
{
    /* alloc_pages */
    page_init();

    /* kmalloc */
    kmalloc_init();

    /* traps */
    traps_init();
    pr_info("sdasdasd\n");
    hlt();
}