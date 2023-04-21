#ifndef _CASUAL_LAYOUT_H
#define _CASUAL_LAYOUT_H

/*
*               Protect Mode Memoey layout
*
* 0A0000	+------------------------+
* 	        |  Reserved for BIOS	 |	Do not use.  Reserved for BIOS EBDA.
* 09A000	+------------------------+
* 	        |  Boot params		     |  Boot params.
* 099000	+------------------------+	
* 	        |  Stack     		     |	Kernel stack.
* 090000	+------------------------+
* 	        |  Protected-mode kernel |	The bulk of the kernel image.
* 010000	+------------------------+
* 	        |  Boot loader		     |	<- Boot sector entry point 0000:7C00
* 001000	+------------------------+
* 	        |  Reserved for MBR/BIOS |
* 000800	+------------------------+
* 	        |  Typically used by MBR |
* 000600	+------------------------+ 
* 	        |  BIOS use only	     |
* 000000	+------------------------+
*
*/

/* virtual address start at 3GB */
#define VIRTUAL_OFFSET 0xC0000000

/* kernel code range: 0x10000-0x90000 */
#define KERNEL_OFFSET (0x1000 + VIRTUAL_OFFSET)

/* stack */
#define KERNEL_STACK (0x99000 + VIRTUAL_OFFSET)

/* boot params */
#define BOOT_PARAMS_OFFSET (0x99000 + VIRTUAL_OFFSET)

#endif