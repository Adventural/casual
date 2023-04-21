#include <asm/string.h>
#include <casual/kernel.h>
#include <casual/mm.h>
#include <casual/page.h>
#include <casual/pgconvert.h>
#include <casual/pgtable.h>

struct page* simple_alloc_pages(int num, struct page* mem_map, int pg_totals)
{
    int start;
    int end;
    int free_pages = 0;

    if(num <= 0 || num >= pg_totals)
        return 0;

    /* find consecutive pages equal to num  */
    for(start = 0, end = start; start < pg_totals && end < pg_totals;){
        struct page* page = mem_map + end;
        if(page->used == 1){
            end++;
            start = end;
            free_pages = 0;
        }
        else{
            end++;
            free_pages++;
            if (free_pages == num)
                break;
        }
    }
    
    if(start == pg_totals || end == pg_totals)
        /* fail */
        return 0;
    
    struct page* page;
    for (int i=end-1; i>=start; i--){
        page = mem_map + i;
        page->used = 1;
        page->compund = -1;
    }
    page->compund = num;

    return page;
}

int simple_free_pages(struct page* page)
{
    int num = page->compund;

    if (num <= 0)
        return 0;
    /* free */
    for(int i=0; i<num; i++){
        struct page *_page = page+i;
        _page->used = 0;
        _page->compund = 0;
    }
    return num;
}

struct zone_operations simple_operations = {
    .alloc = simple_alloc_pages,
    .free = simple_free_pages
};

struct page* alloc_pages_zone(int num, int zone)
{
    for (int i=0; i<zone_count; i++){
        if (mm_zones[i].type == zone){
            struct page* pages = mm_zones[i].operations->alloc( \
                num, mm_zones[i].mem_map, mm_zones[i].pg_totals);
            if (pages != 0)
                return pages;
        }
    }
    WARN_ON("alloc pages failed\n");
    return 0;
}

struct page* alloc_pages(int num, int alloc_flags)
{
    if (!num){
        WARN_ON("Try to alloc 0 pages\n");
        return 0;
    }
    switch(alloc_flags){
        /* 16-896MB */
        case ALC_KERNEL:
            return alloc_pages_zone(num, ZONE_KERNEL);
            break;
        /* 1-16MB */
        case ALC_DMA:
            return alloc_pages_zone(num, ZONE_DMA);
            break;
        /* 896MB~ */
        case ALC_VIRTUAL:
            return alloc_pages_zone(num, ZONE_HIGH);
            break;
        default:
            WARN_ON("alloc flags invalid\n");
    }
    return 0;
}

struct page* calloc_pages(int num, int alloc_flags)
{
    struct page* page = alloc_pages(num, alloc_flags);
    if(!page)
        return 0;
    memset(page_address(page), 0, PAGE_SIZE * num);
    return page;
}

int free_pages(struct page* page)
{
    if (!page){
        WARN_ON("Try to free NULL page\n");
        return 0;
    }
    struct mm_zone* zone = page_to_zone(page);
    zone->operations->free(page);
    return 1;
}