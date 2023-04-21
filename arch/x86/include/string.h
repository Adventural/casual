#ifndef _CASUAL_ASM_STRING_H
#define _CASUAL_ASM_STRING_H

#include <casual/types.h>

static inline void* memcpy(void* dest, void *src, size_t n) {
	asm volatile("cld; rep movsb"
	            : "=c"((int){0})
	            : "D"(dest), "S"(src), "c"(n)
	            : "flags", "memory");
	return dest;
}

static inline void memset(const void* src, char ch, size_t n)
{
    asm volatile(
        "cld; rep stosb"
        :
        :"a"(ch), "c"(n), "D"(src)
        :"memory","cc"
    );
}

#endif