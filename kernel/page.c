#include <casual/types.h>
#include <casual/kernel.h>
#include <casual/mm.h>
#include <casual/page.h>
#include <casual/pgtable.h>
#include <casual/pgconvert.h>
#include <asm/string.h>

struct mm_zone* page_to_zone(struct page* page)
{
    for(int i=0; i<zone_count; i++){
        struct page* first_page = mm_zones[i].mem_map;
        struct page* last_page = mm_zones[i].mem_map + mm_zones[i].pg_totals;
        if(page >= first_page && page <= last_page)
            return &mm_zones[i];
    }
    return 0;
}

struct mm_zone *pfn_to_zone(pfn_t pfn)
{
    for(int i=0; i<zone_count; i++){
        pfn_t first_pfn = mm_zones[i].start_pfn;
        pfn_t last_pfn = mm_zones[i].start_pfn + mm_zones[i].pg_totals;
        if(pfn >= first_pfn && pfn < last_pfn)
            return &mm_zones[i];
    }
    return 0;
}

/* find the first page */
struct page* page_head(struct page* page)
{
    while(page->compund == -1){
        page = page - 1;
    }
    return page;
}
