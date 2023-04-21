#ifndef _CASUAL_ARCH_PARAMS_H
#define _CASUAL_ARCH_PARAMS_H

#include <casual/types.h>
#include <casual/compile.h>

struct e820_t {
    u64_t address;
    u64_t length;
    u32_t type;
} __packed;

struct e820_entry {
    u32_t count;
    struct e820_t e820_table[0];
} __packed;

struct boot_params {
    u32_t longmode_supoport;
    u32_t pde_start;
    u32_t pte_start;
    u32_t pte_end;
    struct e820_entry e820_entry;
} __packed;

#endif