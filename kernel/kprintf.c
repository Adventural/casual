#include <stdarg.h>
#include <casual/kernel.h>

/* cga */
extern void putc(char c);
/* serial */
extern void serial_putchar(char ch);

static void puts(char *s)
{
    int i=0;
    do {
        putc(s[i]);
        serial_putchar(s[i++]);
    }while(s[i] != '\0');
}

int __printf(2, 3) kprintf(int type, const char* fmt, ...)
{
    int l = 0;
    int i = 0;
    char buf[256];
    va_list args;

    switch (type)
    {
    case KERNEL_INFO:
        sprintf(buf, "[info] ");
        l += 7; /* except '\0' */
        break;
    case KERNEL_WARNING:
        sprintf(buf, "[warning] ");
        l += 10; /* except '\0' */
        break;
    default:
        break;
    }
    va_start(args, fmt);
    i = vsprintf(buf+l, fmt, args);
    va_end(args);

    puts(buf);
    return i;
}