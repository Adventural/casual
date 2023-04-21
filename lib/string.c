#include <lib/string.h>

size_t strlen(const char *s) {
	size_t out = 0;
	while (s[out++]) {
		/* nothing */;
	}
	return out;
}

size_t strnlen(const char * s, size_t count)
{
	const char *sc;

	for (sc = s; count-- && *sc != '\0'; ++sc)
		/* nothing */;
	return sc - s;
}