#ifndef _CASUAL_MM_H
#define _CASUAL_MM_H

#include <casual/compile.h>
#include <casual/types.h>

/* 4G */
#define PHYSICS_MAX 0x100000000ULL

/* max zones */
#define MM_ZONE_MAX 32

/* zone type */
#define ZONE_RESERVED 0 /* 0mb - 1mb */
#define ZONE_DMA 1      /* 1mb - 16mb */
#define ZONE_KERNEL 2   /* 16mb - 896mb */
#define ZONE_HIGH 3     /* 896mb ~ */

struct zone_operations
{
    struct page* (*alloc)(int num, struct page* mem_map, int pg_totals);
    int (*free)(struct page*);
};

struct page;

struct mm_zone
{
    /* zone type */
    int type;

    /* zone start pfn */
    unsigned int start_pfn;

    /* how many pages in zone */
    int pg_totals;

    /* pages */
    struct page* mem_map;

    /* operations */
    struct zone_operations *operations;

} __packed;

extern struct mm_zone mm_zones[];
extern int zone_count;

#endif