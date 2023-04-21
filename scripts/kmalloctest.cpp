#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <map>
#include <stdlib.h>
#include <iostream>

using namespace std;

static int get_order(size_t size)
{
    for(int i=0; i<64; i++){
        if((0x1U << i) >= size)
            return i;
    }
    return 0;
}

struct page {
    /* used or not */
    int used;
    /* allocated pages. if not the first page, compund = -1 */
    int compund;
    /* small memory list */
    union {
        struct page* next_page;
        struct page* next_bitmap;
    };
} __attribute__((packed));

int main()
{
    struct page* page = (struct page*)malloc(sizeof(struct page));
    struct page* page2 = (struct page*)malloc(sizeof(struct page));
    page->next_bitmap = page2;
    cout << get_order(3);
    return 0;
}