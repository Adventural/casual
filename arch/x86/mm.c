#include "params.h"
#include <asm/string.h>
#include <casual/mm.h>
#include <casual/kernel.h>
#include <casual/page.h>

struct mm_zone mm_zones[MM_ZONE_MAX];
int zone_count;

#define E820_RAM_USABLE 1   /* usable ram */
#define E820_RAM_RESERV 2   /* reserve ram */
#define E820_RAM_ACPIREC 3  /* acpi table */
#define E820_RAM_ACPINVS 4  /* acpi nvs */
#define E820_RAM_AREACON 5  /* bad ram */

#define RESERVED_BOUNDRY   0x100000ULL     /* 1MB */
#define DMA_BOUNDRY        0x1000000ULL    /* 16MB */
#define KERNEL_BOUNDRY     0x38000000ULL   /* 896MB */
#define HIGH_BOUNDRY       0x100000000ULL  /* 4GB */

static void split_mm_zone(u64_t mm_start, ssize_t size);

static void _build_zone(u32_t start, size_t size, unsigned long long boundry, int type)
{
    mm_zones[zone_count].type = type;
    mm_zones[zone_count].start_pfn = (start >> PAGE_SHIFT);
    /* start + size > boundry? */
    if((unsigned long)start + size > boundry){
        size_t zone_size = (size_t)(boundry - start);
        mm_zones[zone_count++].pg_totals = zone_size / PAGE_SIZE;
        /* next zone */
        split_mm_zone(boundry, size - zone_size);
        return;
    }
    else{
        mm_zones[zone_count++].pg_totals = size / PAGE_SIZE;
    }
}

static void split_mm_zone(u64_t mm_start, ssize_t size)
{
    u32_t start = mm_start;
    size_t _size = (size_t)size;
    /* zone reserved */
    if(start >= 0 && start < RESERVED_BOUNDRY){
        _build_zone(start, _size, RESERVED_BOUNDRY, ZONE_RESERVED);
    }
    else if(start >= RESERVED_BOUNDRY && start < DMA_BOUNDRY){
        _build_zone(start, _size, DMA_BOUNDRY, ZONE_DMA);
    }
    else if(start >= DMA_BOUNDRY && start < KERNEL_BOUNDRY){
        _build_zone(start, _size, KERNEL_BOUNDRY, ZONE_KERNEL);
    }
    else if(start >= KERNEL_BOUNDRY){
        _build_zone(start, _size, HIGH_BOUNDRY, ZONE_HIGH);
    }
}

static void mm_zone_init(struct e820_entry* e820_entry)
{
    int i = 0;
    int e820_count = e820_entry->count;

    zone_count = 0;

    memset(mm_zones, 0, sizeof(struct mm_zone)*MM_ZONE_MAX);

    assert(e820_count != 0);
    kprintf(KERNEL_INFO, "bios-e820 count: %d\n", e820_count);
    for(i = 0; i < e820_count; i++){
        u64_t mm_start = e820_entry->e820_table[i].address;
        ssize_t size = e820_entry->e820_table[i].length;
        /* dropping memory start higher than 4g */
        if (mm_start >= PHYSICS_MAX){
            kprintf(KERNEL_WARNING, "bios-e820: the memory above 4G was dropped\n");
            continue;
        }
        /* usable memory */
        if(e820_entry->e820_table[i].type == E820_RAM_USABLE){
            /* split mm_zone */
            split_mm_zone(mm_start, size);
        }
        kprintf(KERNEL_INFO, "e820 start: %016llx length: %016llx\n", mm_start, size);
    }
}   

void arch_mm_init(struct e820_entry* e820_entry)
{
    mm_zone_init(e820_entry);
}