#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <map>
#include <stdlib.h>

using namespace std;

int alloced;
map<struct page*, int> alloc_table;
map<struct page*, int>::iterator alloc_t;

struct page {
    /* used or not */
    int used;
    /* allocated pages. if not the first page, compund = -1 */
    int compund;
} __packed;


struct page* simple_alloc_pages(int num, struct page* mem_map, int pg_totals)
{
    int start;
    int end;
    int free_pages = 0;

    if(num <= 0 || num >= pg_totals)
        return NULL;

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
        return NULL;
    
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
    if (num <= 0){
        printf("free error\n");
        exit(0);
    }
    for(int i=0; i<num; i++){
        struct page *_page = page+i;
        _page->used = 0;
        _page->compund = 0;
    }
    return 0;
}

int check(struct page *mem_map, int pg_totals)
{
    int used = 0;
    for (int i=0; i<pg_totals; i++){
        struct page *page = mem_map+i;
        if(page->used != 0)
            used++;
    }
    if(used == alloced){
        return 0;
    }
    else{
        printf("check error used: %d\n", used);
        exit(0);
    }
    return 0;
}

int main()
{
    struct page *mem_map = (struct page *)malloc(1000000*sizeof(struct page));
    memset(mem_map, 0, sizeof(struct page)*1000000);
    alloced = 0;
    int i = 100000;
    while(i--)
    {
        if (alloc_table.size() != 0){
            int p = rand() % 5;
            if (p >= 3){
                int position = rand() % alloc_table.size();
                int j = 0;
                for (alloc_t = alloc_table.begin(); alloc_t != alloc_table.end(); alloc_t++, j++){
                    if(j == position){
                        alloced -= alloc_t->second;
                        simple_free_pages(alloc_t->first);
                        alloc_table.erase(alloc_t);
                        printf("freed: %d page: %p\n", alloc_t->second, (void*)alloc_t->first);
                        break;
                    }
                }
                check(mem_map, 1000000);
                continue;
            }
        }
        int size = rand() % 1000;
        struct page* page = simple_alloc_pages(size, mem_map, 1000000);
        if(page != NULL){
            printf("alloced: %d page: %p\n", size, (void*)page);
            alloc_table[page] =  size;
            alloced += size;
        }
        else{
            printf("alloc failed!\n");
            //break;
        }
        check(mem_map, 1000000);
        printf("alloced:%d\n", alloced);
    }
    return 0;
}