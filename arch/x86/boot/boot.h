
#ifndef _REAL_BOOT_H
#define _REAL_BOOT_H

#include "string.h"
#include <asm/cpuflags.h>
#include <casual/types.h>
#include <casual/compile.h>

/* Basic port I/O */
static inline void outb(u8_t v, u16_t port) {
    asm volatile("outb %0,%1" : : "a"(v), "dN"(port));
}
static inline u8_t inb(u16_t port) {
    u8_t v;
    asm volatile("inb %1,%0" : "=a"(v) : "dN"(port));
    return v;
}

static inline void io_delay(void) {
    const u16_t DELAY_PORT = 0x80;
    asm volatile("outb %%al,%0" : : "dN"(DELAY_PORT));
}

/* These functions are used to reference data in other segments. */

static inline u16_t ds(void) {
    u16_t seg;
    asm("movw %%ds,%0" : "=rm"(seg));
    return seg;
}

static inline void set_fs(u16_t seg) {
    asm volatile("movw %0,%%fs" : : "rm"(seg));
}
static inline u16_t fs(void) {
    u16_t seg;
    asm volatile("movw %%fs,%0" : "=rm"(seg));
    return seg;
}

static inline void set_gs(u16_t seg) {
    asm volatile("movw %0,%%gs" : : "rm"(seg));
}
static inline u16_t gs(void) {
    u16_t seg;
    asm volatile("movw %%gs,%0" : "=rm"(seg));
    return seg;
}

/* bioscall */
struct biosregs {
    union {
        struct {
            u32_t edi;
            u32_t esi;
            u32_t ebp;
            u32_t _esp;
            u32_t ebx;
            u32_t edx;
            u32_t ecx;
            u32_t eax;
            u32_t _fsgs;
            u32_t _dses;
            u32_t eflags;
        };
        struct {
            u16_t di, hdi;
            u16_t si, hsi;
            u16_t bp, hbp;
            u16_t _sp, _hsp;
            u16_t bx, hbx;
            u16_t dx, hdx;
            u16_t cx, hcx;
            u16_t ax, hax;
            u16_t gs, fs;
            u16_t es, ds;
            u16_t flags, hflags;
        };
        struct {
            u8_t dil, dih, edi2, edi3;
            u8_t sil, sih, esi2, esi3;
            u8_t bpl, bph, ebp2, ebp3;
            u8_t _spl, _sph, _esp2, _esp3;
            u8_t bl, bh, ebx2, ebx3;
            u8_t dl, dh, edx2, edx3;
            u8_t cl, ch, ecx2, ecx3;
            u8_t al, ah, eax2, eax3;
        };
    };
};
void intcall(u8_t int_no, const struct biosregs *ireg, struct biosregs *oreg) __regparm(3);

static inline void initregs(struct biosregs *reg) {
    memset(reg, 0, sizeof(struct biosregs));
    reg->eflags |= X86_EFLAGS_CF;
    reg->ds = ds();
    reg->es = ds();
    reg->fs = fs();
    reg->gs = gs();
}

/* memory.c */
#define E820MAX 128
#define E820_SIGNATURE 0x534d4150   /* SMAP */

struct e820_t {
    u64_t address;
    u64_t length;
    u32_t type;
} __packed;

void detect_memory(void);

/* put.c */
void puts_n(char *buf, u16_t n);
void puts(char *buf);
void puthex_8(u8_t num);
void puthex_16(u16_t num);
void puthex_32(u32_t num);
void puthex_64(u64_t num);
void die(char *s) __noreturn;

/* protect.c */
struct gdt_ptr {
	u16_t len;
	u32_t ptr;
} __packed;

void setup_kernel();

#endif
