#include "cga.h"
#include <asm/x86.h>
#include <casual/page.h>
#include <casual/types.h>

u32_t crt_pos;
u16_t *cga_buf;

void putc(char c) {
    switch (c) {
        case '\r':
            break;
        case '\n':
            crt_pos += CGA_COL;
            crt_pos -= crt_pos % CGA_COL;
            break;
        case '\b':
            if (crt_pos > 0) {
                cga_buf[crt_pos--] = 0x0700 | ' ';
            }
            break;
        case '\t':
            putc(' ');
            putc(' ');
            putc(' ');
            putc(' ');
            break;
        default:
            cga_buf[crt_pos++] = 0x0700 | c;
            break;
    }

    /* rollback one line */
    if (crt_pos >= CGA_SIZE) {
        u32_t i;
        for (i = 0; i < CGA_SIZE; i++) {
            if (i < crt_pos - CGA_COL) {
                cga_buf[i] = cga_buf[i + CGA_COL];
            } else {
                cga_buf[i] = 0x0700 | ' ';
            }
        }
        crt_pos -= CGA_COL;
    }
}

void cga_clear() {
    /* Clear the screen */
    int i = 0;
    for (i = 0; i < 25 * 80; i++) {
        cga_buf[i] = 0x0700 | ' ';
    }
    crt_pos = 0;
}

void cga_init() {
    crt_pos = 0;
    cga_buf = (u16_t *)__va(CGA_BASE);

    /* Hide the cursor */
    outb(14, CGA_REG_INDEX);
    outb(0xff, CGA_REG_DATA);
    outb(15, CGA_REG_INDEX);
    outb(0xff, CGA_REG_DATA);

    /* Disables blink? */
    inb(0x3DA);
    outb(0x30, 0x3C0);
    char b = inb(0x3C1);
    b &= ~8;
    outb(b, 0x3C0);

    /* Clear the screen */
    cga_clear();
}
