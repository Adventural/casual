#ifndef _CASUAL_PGTABLE_H
#define _CASUAL_PGTABLE_H

#include <casual/types.h>
#include <casual/page.h>
#include <casual/pgtype.h>

#define KPGTABLE_START    0x1000000U
#define KPGTABLE_SIZE     0x400000U

#define _page_attribute(offset) ((page_attribute_t)(0x1U << offset))

#define _PAGE_BIT_PRESENT	0	/* is present */
#define _PAGE_BIT_RW		1	/* writeable */
#define _PAGE_BIT_USER		2	/* userspace addressable */
#define _PAGE_BIT_PWT		3	/* page write through */
#define _PAGE_BIT_PCD		4	/* page cache disabled */
#define _PAGE_BIT_ACCESSED	5	/* was accessed (raised by CPU) */
#define _PAGE_BIT_DIRTY		6	/* was written to (raised by CPU) */
#define _PAGE_BIT_PSE		7	/* 4 MB (or 2MB) page */
#define _PAGE_BIT_PAT		7	/* on 4KB pages */
#define _PAGE_BIT_GLOBAL	8	/* Global TLB entry PPro+ */
#define _PAGE_BIT_SOFTW1	9	/* available for programmer */
#define _PAGE_BIT_SOFTW2	10	/* " */
#define _PAGE_BIT_SOFTW3	11	/* " */

#define PAGE_PRESENT    _page_attribute(_PAGE_BIT_PRESENT)
#define PAGE_WRITE      _page_attribute(_PAGE_BIT_RW)         
#define PAGE_USER       _page_attribute(_PAGE_BIT_USER)
#define PAGE_WTHROUGH   _page_attribute(_PAGE_BIT_PWT)
#define PAGE_NOCACHE    _page_attribute(_PAGE_BIT_PCD)
#define PAGE_ACCESSED   _page_attribute(_PAGE_BIT_ACCESSED)
#define PAGE_DIRTY      _page_attribute(_PAGE_BIT_DIRTY)

#define _PAGE_BASE  (PAGE_PRESENT | PAGE_ACCESSED)

#define PAGE_KERNEL _PAGE_BASE

static int inline pde_val(pde_t x)
{
    return (x) >> 12;
}

static pde_t inline mk_pde(pfn_t pfn, page_attribute_t attr)
{
    return (pde_t)(pfn_phys(pfn) | attr);
}

static void inline set_pde(pfn_t pfn, int offset, pde_t pde)
{
    pde_t* pg_dir_table = (pde_t*)(pfn_phys(pfn));
    pg_dir_table[offset] = pde;
}

static int inline pte_val(pte_t x)
{
    return (x) >> 12;
}

static pte_t inline mk_pte(pfn_t pfn, page_attribute_t attr)
{
    return (pte_t)(pfn_phys(pfn) | attr);
}

static void inline set_pte(pfn_t pfn, int offset, pte_t pte)
{
    pte_t *pg_table = (pte_t*)(pfn_phys(pfn));
    pg_table[offset] = pte;
}

#endif