/*
 * @Author: adventural 
 * @Date: 2022-05-18 00:53:03 
 *
 */

#include "config.h"

    .code16
mbr_start:
    ljmp $0, $_start

    .align 4
/* int 0x13, 0x42 function */
disk_address_packet:

buffer_packet_size: 
    .word 0x10
sectors:
    .word 0x10
buffer_address: /* segment:offset */
    .word SETUP_CODE_OFFSET
    .word SETUP_CODE_SEG
start_sectors:
    .long 0x1
start_sectors_high:
    .long 0x0

/* filled in by build.py */
kernel_sectors: 
    .word 0x50
kernel_start_sector: 
    .long 0x11
kernel_start_sector_high: 
    .long 0x0

_start:
    xor %ax, %ax
    mov %ax, %es
    mov %ax, %ss
    mov %ax, %ds
    mov %ax, %gs
    mov %ax, %fs
    
    /* init stack */
    mov $0x7000, %sp

    /* save drive reference first thing! */
    pushw %dx

    /* set %si to the disk address packet */
	movw $disk_address_packet, %si

    /* load the setup code from disk */
    call disk_read
    
    /* restore drive */
    popw %dx

    /* kernel range: 0x10000 - 0x90000 */
    movw $disk_address_packet, %si
    movw $KERNEL_CODE_OFFSET, 4(%si)
    movw $KERNEL_CODE_SEG,  6(%si)

    /* set kernel sectors */
    movw kernel_sectors, %bx
    movw %bx, 2(%si)
    movl kernel_start_sector, %ebx
    movl %ebx, 8(%si)
    movl kernel_start_sector_high, %ebx
    movl %ebx, 12(%si)

    /* load kernel code from disk */
    call disk_read
    
    /* jmp to setup code*/
    ljmp $SETUP_CODE_SEG, $SETUP_CODE_OFFSET

disk_read:
    mov $0x42, %ah
    int $0x13
    /* LBA read is not supported, print an error message.  */
	jc	lba_failed
    ret

lba_failed:
    movw $lba_failed_msg, %si
    jmp msg_loop

msg_loop:
	lodsb
	andb	%al, %al
	jz	bs_die
	movb	$0xe, %ah
	movw	$7, %bx
	int	$0x10
	jmp	msg_loop
    hlt

bs_die:
	# Allow the user to press a key, then reboot
	xorw	%ax, %ax
	int	$0x16
	int	$0x19

	# int 0x19 should never return.  In case it does anyway,
	# invoke the BIOS reset code...
	ljmp	$0xf000,$0xfff0

lba_failed_msg:
    .ascii "Disk Read Failed.\r\n"
    .ascii "Press Any Key To Reboot.\r\n"   
    .byte 0
    .byte 0

.org 510
	.byte 0x55
	.byte 0xaa