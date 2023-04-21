#include <casual/types.h>
#include <casual/kernel.h>
#include <casual/mm.h>
#include <casual/page.h>
#include <casual/pgtable.h>
#include <casual/pgconvert.h>
#include <asm/string.h>

extern struct zone_operations simple_operations;
static void paging_zone_kernel()
{
    int i;
    /* 
    * mem_map start at physics address 0x1400000(20MB).
    * pgtable start at physics 16MB with the maxsize 4MB.
    */
    struct page* map_base = (struct page*)__va(KPGTABLE_START + KPGTABLE_SIZE);
    for (i=0; i<zone_count; i++){
        if(mm_zones[i].type == ZONE_KERNEL){
            int pages = mm_zones[i].pg_totals;
            memset(map_base, 0, sizeof(struct page)*pages);
            mm_zones[i].mem_map = map_base;
            mm_zones[i].operations = &simple_operations;
            pr_info("start: %x pages: %d memmap: %p\n", \
                pfn_phys(mm_zones[i].start_pfn), pages, mm_zones[i].mem_map);
            map_base += pages;
        }
    }
    /* used pages: pgtable + memmap */
    pfn_t start_pfn = phys_pfn(KPGTABLE_START);
    pfn_t end_pfn = phys_pfn(__fa(map_base));
    for (int i=start_pfn; i<=end_pfn; i++){
        struct page* page = pfn_to_page(i);
        page->used = 1;
        page->compund = 1;
    }
}

static void paging_zone(int zone, struct zone_operations *ops)
{
    int i=0;
    for (i=0; i<zone_count; i++){
        if(mm_zones[i].type == zone){
            struct mm_zone *zone = &mm_zones[i];
            int pages_num = (zone->pg_totals * sizeof(struct page) \
                + PAGE_SIZE - 1) / PAGE_SIZE;
            struct page* zone_pages = alloc_pages(pages_num, ALC_KERNEL);
            void *pg_addr = page_address(zone_pages);
            memset(pg_addr, 0, PAGE_SIZE*pages_num);
            zone->mem_map = pg_addr;
            zone->operations = ops;
            pr_info("start: %x pages: %d memmap: %p\n", \
                pfn_phys(zone->start_pfn), zone->pg_totals, zone->mem_map);
        }
    }
}

void page_init()
{
    /* ALC_KERNEL */
    pr_info("zone kernel:\n");
    paging_zone_kernel();
    
    /* ALC_DMA */
    pr_info("zone dma:\n");
    paging_zone(ZONE_DMA, &simple_operations);

    /* ALC_HIGH */
    pr_info("zone high:\n");
    paging_zone(ZONE_HIGH, &simple_operations);
}