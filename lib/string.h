#ifndef _CASUAL_LIBSTRING_H
#define _CASUAL_LIBSTRING_H
#include <casual/types.h>

static inline int strcmp(const char * l, const char * r) {
	for (; *l == *r && *l; l++, r++);
	return *(unsigned char *)l - *(unsigned char *)r;
}

static inline int strncmp(const char * l, const char * r, int n) {
	for (int i=1; *l == *r && *l && i<n; l++, r++, i++);
	return *(unsigned char *)l - *(unsigned char *)r;
}

size_t strlen(const char *s);
size_t strnlen(const char * s, size_t count);

#endif