#ifndef _CASUAL_LIBSTDOUT_H
#define _CASUAL_LIBSTDOUT_H
#include <stdarg.h>
#include <casual/compile.h>

int vsprintf(char *buf, const char *fmt, va_list args);
int __printf(2, 3) sprintf(char *buf, const char *fmt, ...);

#endif