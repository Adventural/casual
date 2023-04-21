#ifndef _CASUAL_KMALLOC_H
#define _CASUAL_KMALLOC_H

/* min size = 2^3 */
#define MIN_FIXED_ORDER 3

/* max size = 2^11 = 2k */
#define MAX_FIXED_ORDER 11

#define DATAZONE_OBJECT_MAX 1024

/* small memory object */
struct sm_object{
    /* memory size = 2^order */
    int order;
    int alloced;
    struct page* data_zone;
    struct sm_object* next_obj;
    /* 1024 memory object per data_zone */
    char bitmap[DATAZONE_OBJECT_MAX];
};

#endif