#ifndef _REAL_PARAMS_H
#define _REAL_PARAMS_H

#include "boot.h"
#include <casual/compile.h>
#include <casual/types.h>

struct e820_entry {
    u32_t count;
    struct e820_t e820_table[0];
}__packed;

struct boot_params {
    u32_t longmode_supoport;
    u32_t pde_start;
    u32_t pte_start;
    u32_t pte_end;
    struct e820_entry e820_entry;
}__packed;

#endif