
    .code16
_start:
    cli
    /* segment: 0x9000 */
    mov %cs, %ax
    mov %ax, %es
    mov %ax, %ss
    mov %ax, %ds
    mov %ax, %gs
    mov %ax, %fs

    /* set stack to 0x99000 */
    mov $0x9000, %ax
    mov %ax, %sp

    sti

    /* this function should never return */
    calll setup_main

    pushl $setup_failed_msg
    calll puts

.global reboot
reboot:
	# Allow the user to press a key, then reboot
	xorw	%ax, %ax
	int	$0x16
	int	$0x19

	# int 0x19 should never return.  In case it does anyway,
	# invoke the BIOS reset code...
	ljmp	$0xf000,$0xfff0

setup_failed_msg:
    .ascii "Press Any Key To Reboot.\r\n"
    .byte 0
    .byte 0