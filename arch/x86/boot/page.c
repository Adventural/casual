#include "boot.h"
#include "boot_params.h"
#include <casual/pgtable.h>
#include <casual/page.h>

extern struct boot_params *boot_params;

static void check_protect()
{
    u32_t *a = (u32_t*)0;
    u32_t *b = (u32_t*)0x100000;
    *a = 0xCACBCACB;
    if (*a == *b){
        /* reboot */
        __asm__ volatile("jmp 0xffff0");
    }
}

void setup_page()
{
    check_protect();

    /* setup page table, virtual: 3G-4G --> physical: 0-1G*/
    phys_addr_t vstart = 0xC0000000;

    /* page directory entry: 0-4K */
    boot_params->pde_start = 0;
    phys_addr_t pde_start = (phys_addr_t)boot_params->pde_start;

    /* page table entry: 16MB~ */
    boot_params->pte_start = 0x1000000;
    phys_addr_t pte_start = (phys_addr_t)boot_params->pte_start;

    /* setup pde */
    pfn_t page_table_pfn = phys_pfn(pte_start);
    for (; vstart != 0;){
        int offset = vstart >> 22;
        pde_t pde = mk_pde(page_table_pfn++, PAGE_KERNEL);
        set_pde(phys_pfn(pde_start), offset, pde);
        /* next page directory entry */
        vstart += (0x1U << 22);
    }

    /* page directory, virtual: 0-1MB --> physical: 0-1MB */
    pde_t pde = mk_pde(page_table_pfn, PAGE_KERNEL);
    set_pde(phys_pfn(pde_start), 0, pde);

    /* setup pte, linear */
    vstart = 0xC0000000U;
    int offset = 0;
    for (; vstart != 0;){
        pte_t pte = mk_pte(phys_pfn(vstart - 0xC0000000U), PAGE_KERNEL);
        set_pte(phys_pfn(pte_start), offset++, pte);
        /* next page */
        vstart += PAGE_SIZE;
    }

    /* page table, virtual: 0-1MB --> physical: 0-1MB */
    offset = 0;
    for(vstart = 0; vstart < 0x100000; vstart += PAGE_SIZE){
        pte_t pte = mk_pte(phys_pfn(vstart), PAGE_KERNEL);
        set_pte(page_table_pfn, offset++, pte);
    }

}