#ifndef _CASUAL_PGCONVERT_H
#define _CASUAL_PGCONVERT_H

#include <casual/pgtype.h>
#include <casual/page.h>
#include <casual/mm.h>

struct mm_zone* page_to_zone(struct page* page);

static inline pfn_t page_to_pfn(struct page* page)
{
    struct mm_zone *zone = page_to_zone(page);
    assert(zone != 0);
    return zone->start_pfn + ((unsigned int)(page - zone->mem_map));
}

struct mm_zone* pfn_to_zone(pfn_t pfn);

static inline struct page* pfn_to_page(pfn_t pfn)
{
    struct mm_zone *zone = pfn_to_zone(pfn);
    assert(zone != 0);
    unsigned int offset = pfn - zone->start_pfn;
    return (struct page*)(zone->mem_map + offset);
}

static inline void* page_address(struct page* page) {
    if(!page){
        WARN_ON("try to use page 0\n");
        return 0;
    }
    pfn_t pfn = page_to_pfn(page);
    return (void*)__va(pfn_phys(pfn));
}

#endif