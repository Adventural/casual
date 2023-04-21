#ifndef ASM_PORT
#define ASM_PORT

#include <casual/types.h>

/* port I/O */
static void inline outb(u8_t v, u16_t port) {
    __asm__ volatile("outb %0,%1" : : "a"(v), "dN"(port));
}
static u8_t inline inb(u16_t port) {
    u8_t v;
    asm volatile("inb %1,%0" : "=a"(v) : "dN"(port));
    return v;
}

static void inline io_delay(void) {
    const u16_t DELAY_PORT = 0x80;
    __asm__ volatile("outb %%al,%0" : : "dN"(DELAY_PORT));
}

/* instructions */
static void inline hlt()
{
    __asm__ volatile("hlt");
}

static void inline cli()
{
    __asm__ volatile("cli");
}

static void inline sti()
{
    __asm__ volatile("sti");
}

/* routines */
static int inline cmos_seconds(void) {
	outb(0, 0x70);
	return inb(0x71);
}

#endif