#include <casual/page.h>
#include <casual/kernel.h>
#include <casual/pgconvert.h>
#include <casual/kmalloc.h>
#include <asm/string.h>

struct sm_object *sm_objects[MAX_FIXED_ORDER+1];

#define KMALLOC_PAGES -1

static int get_order(size_t size)
{
    for(int i=0; i<64; i++){
        if((0x1U << i) >= size)
            return i;
    }
    return 0;
}

static struct sm_object* alloc_sm_object()
{
    struct page* page = calloc_pages(1, ALC_KERNEL);
    if (!page){
        WARN_ON("alloc_sm_object failed\n");
        return 0;
    }
    else
        return (struct sm_object*)page_address(page);
}

static int free_sm_object(struct sm_object* sm_obj)
{
    struct page* page = (struct page*)sm_obj;
    return free_pages(page);
}

void* __do_new_kmalloc(struct sm_object *sm_obj)
{
    /* new sm_object */
    struct sm_object* new_obj = alloc_sm_object();
    new_obj->next_obj = 0;
    new_obj->order = sm_obj->order;
    int data_pages = ((0x1U << sm_obj->order) * 1024) / PAGE_SIZE;
    new_obj->data_zone = alloc_pages(data_pages, ALC_KERNEL);
    if(!new_obj->data_zone){
        free_sm_object(new_obj);
        WARN_ON("__do_new_kmalloc failed\n");
        return 0;
    }

    /* add to list */
    sm_obj->next_obj = new_obj;

    new_obj->data_zone->sm_obj = new_obj;

    /* alloc the first object */
    new_obj->alloced = 1;
    new_obj->bitmap[0] = 1;
    void *addr = page_address(new_obj->data_zone);
    pr_info("kmalloc: %p pg_addr: %p page: %p\n", addr, page_address(new_obj->data_zone), new_obj->data_zone);
    return addr;
}

void* __do_kmalloc(struct sm_object *sm_obj)
{
    
    if (!sm_obj){
        WARN_ON("null sm_obj\n");
        return 0;
    }
    struct sm_object *obj = sm_obj;
    /* jump over the full datazone */
    while(obj && obj->alloced == DATAZONE_OBJECT_MAX){
        obj = obj->next_obj;
    }
    /* all datazones are full */
    if (!obj){
        return __do_new_kmalloc(sm_obj);
    }
    else{
        int i;
        for(i = 0; i < DATAZONE_OBJECT_MAX; i++){
            if(obj->bitmap[i] == 0)
                break;
        }
        obj->bitmap[i] = 1;
        obj->alloced += 1;
        void *pg_addr = page_address(obj->data_zone);
        pr_info("kmalloc: %p pg_addr: %p page: %p\n", pg_addr + (i << sm_obj->order), pg_addr, obj->data_zone);
        return pg_addr + (i << sm_obj->order);
    }
    
}

void* kmalloc(size_t size)
{
    int order = get_order(size);
    if (!order){
        WARN_ON("kmalloc: try to alloc 0 bytes\n");
        return 0;
    }
    if (order < MIN_FIXED_ORDER)
        order = MIN_FIXED_ORDER;
    /* kmalloc */
    if (order <= MAX_FIXED_ORDER){
        struct sm_object *sm_obj = sm_objects[order];
        return __do_kmalloc(sm_obj);
    }
    /* alloc_pages */
    int nums = (size + (PAGE_SIZE - 1)) / PAGE_SIZE;
    struct page* page = alloc_pages(nums, ALC_KERNEL);
    if(page){
        page->sm_obj = sm_objects[0];
        return (void*)page_address(page);
    }
    return 0;
}

int kfree(void *addr)
{
    pfn_t pfn = phys_pfn(__fa(addr));
    struct page* page = page_head(pfn_to_page(pfn));
    struct sm_object *obj = page->sm_obj;
    if(obj->order == KMALLOC_PAGES){
        /* addr from alloc_pages */
        return free_pages(page);
    }
    else{
        int obj_size = (0x1 << obj->order);
        if ((int)addr % obj_size == 0){
            int offset = (int)(addr - page_address(page)) / obj_size;
            obj->bitmap[offset] = 0;
            obj->alloced--;
            return 1;
        }
        else{
            WARN_ON("try to free object with wrong size\n");
        } 
    }
    return 0;
}

void kmalloc_init()
{
    for(int i = MIN_FIXED_ORDER; i <= MAX_FIXED_ORDER; i++){
        struct sm_object* sm_obj = alloc_sm_object();
        sm_obj->alloced = 0;
        sm_obj->next_obj = 0;
        
        /* object size */
        sm_obj->order = i;
        
        /* data_zone */
        int data_pages = ((0x1U << i) * 1024) / PAGE_SIZE;
        sm_obj->data_zone = alloc_pages(data_pages, ALC_KERNEL);
        
        /* kfree need it */
        sm_obj->data_zone->sm_obj = sm_obj;

        sm_objects[i] = sm_obj;
    }

    /* 
    * if kmalloc size was more than 2k, we directly call alloc pages
    * set order to KMALLOC_PAGES
    */
    struct sm_object* page_sm_obj = alloc_sm_object();
    page_sm_obj->order = KMALLOC_PAGES;
    sm_objects[0] = page_sm_obj;
}