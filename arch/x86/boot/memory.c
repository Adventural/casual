#include "boot.h"
#include "config.h"
#include "boot_params.h"

extern struct boot_params *boot_params;

void detect_memory(void)
{
    struct biosregs iregs, oregs;

    boot_params->e820_entry.count = 0;
    
    initregs(&iregs);
    iregs.eax = 0xe820;
    iregs.ebx = 0;
    iregs.es = SETUP_DATA_SEG;
    iregs.edi = (u32_t)boot_params->e820_entry.e820_table;
    iregs.ecx = sizeof(struct e820_t);
    iregs.edx = E820_SIGNATURE;

    do {
        intcall(0x15, &iregs, &oregs);
        if (oregs.eflags & X86_EFLAGS_CF)
            break;

        /* error? */
        if (oregs.eax != E820_SIGNATURE)
            die("Detect Memory Failed.\r\n");
        if (oregs.ecx != sizeof(struct e820_t))
            die("Detect Memory Failed.\r\n");
        
        /* next read */
        iregs.ebx = oregs.ebx;
        iregs.edi += sizeof(struct e820_t);
        boot_params->e820_entry.count++;

    }while(iregs.ebx && boot_params->e820_entry.count < E820MAX);
}   