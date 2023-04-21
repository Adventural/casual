#ifndef _CASUAL_KERNEL_H
#define _CASUAL_KERNEL_H
#include <stdarg.h>
#include <casual/compile.h>
#include <lib/string.h>
#include <lib/stdout.h>

/* kprintf */
#define KERNEL_INFO     0
#define KERNEL_WARNING  1
int kprintf(int type, const char* fmt, ...) __printf(2, 3);

#define pr_info(format, ...) kprintf(KERNEL_INFO, format, ## __VA_ARGS__)

#define WARN_ON(x)  kprintf(KERNEL_WARNING, x);
static void inline assert(int a)
{
    if(!a)
        kprintf(KERNEL_WARNING, "kernel error\n");
}

#endif