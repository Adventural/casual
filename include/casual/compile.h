#ifndef _CASUAL_COMPILE_H
#define _CASUAL_COMPILE_H

#define __regparm(x)    __attribute__((regparm(x)))
#define __noreturn	    __attribute__((noreturn))
#define __packed		__attribute__((packed))
#define __printf(a, b)	__attribute__((format(printf, a, b)))

#define unreachable() \
	do {					\
		__builtin_unreachable();	\
	} while (0)

#endif