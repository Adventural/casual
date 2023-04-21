#ifndef _REAL_STRING_H
#define _REAL_STRING_H

#include <casual/types.h>

static inline void *memcpy(void *dest, const void *src, long n) {
    asm volatile("cld; rep movsb"
                 : "=c"((int){0})
                 : "D"(dest), "S"(src), "c"(n)
                 : "flags", "memory");
    return dest;
}

static inline void memset(void *src, u8_t ch, u32_t n) {
    asm volatile("cld; rep stosb"
                 :
                 : "a"(ch), "c"(n), "D"(src)
                 : "memory", "cc");
}

static inline unsigned long strlen(const char *s) {
    unsigned long out = 0;
    while (*s) {
        out++;
        s++;
    }
    return out;
}

#endif