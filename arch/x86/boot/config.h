#ifndef _REAL_CONFIG_H
#define _REAL_CONFIG_H

/*
*               Memoey layout
*
* 0A0000	+------------------------+
* 	        |  Reserved for BIOS	 |	Do not use.  Reserved for BIOS EBDA.
* 09A000	+------------------------+
* 	        |  Boot params		     |  Boot params.
* 099000	+------------------------+	
* 	        |  Stack     		     |	Setup stack.
*      	    +------------------------+
* 	        |  Kernel setup code	 |	The kernel setup code.
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


/* setup code range: 0x90000-0x99000 */
#define SETUP_CODE_SEG 0x9000
#define SETUP_CODE_OFFSET 0x0
#define SETUP_DATA_SEG 0x9000
/* setup stack range: ~0x99000 */
#define SETUP_STACK_SEG 0x9000
#define SETUP_STACK_SS 0x9000

/* kernel code range: 0x10000-0x90000 */
#define KERNEL_CODE_SEG 0x1000
#define KERNEL_CODE_OFFSET 0x0

/* boot params */
#define BOOT_PARAMS_OFFSET 0x9000   /* Use in 16bit mode */

#endif