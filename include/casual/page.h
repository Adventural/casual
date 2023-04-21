#ifndef _CASUAL_PAGE_H
#define _CASUAL_PAGE_H

#include <casual/types.h>
#include <casual/compile.h>
#include <casual/pgtype.h>

/* 3GB */
#define VIRTUAL_OFFSET 0xC0000000U

/* 4KB support only */
#define PAGE_SHIFT  12
#define PAGE_SIZE   (0x1U << PAGE_SHIFT)

/* get page frame number from physical address */
#define phys_pfn(x) ((pfn_t)(((phys_addr_t)(x)) >> PAGE_SHIFT))

/* convert page frame number to page address */
#define pfn_phys(x) ((phys_addr_t)(((pfn_t)(x)) << PAGE_SHIFT))

#define __va(x) (((void*)(x)) + VIRTUAL_OFFSET)
#define __fa(x) (((void*)(x)) - VIRTUAL_OFFSET)

struct page {
    /* used or not */
    int used;
    /* allocated pages. if not the first page, compund = -1 */
    int compund;
    /* kmalloc_object */
    struct sm_object *sm_obj;
    
} __packed;

#define ALC_KERNEL  0
#define ALC_DMA     1
#define ALC_VIRTUAL 2

struct page* alloc_pages(int num, int alloc_flags);
int free_pages(struct page* page);

struct page* calloc_pages(int num, int alloc_flags);

struct page* page_head(struct page* page);

#endif
