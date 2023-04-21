#include "boot.h"
#include "string.h"
#include "config.h"

char hexcode[] = "0123456789abcdef";

/* output 'n' bytes */
void puts_n(char *buf, u16_t n) {
    struct biosregs ireg, oreg;
    initregs(&ireg);
	initregs(&oreg);

	/* cursor position */
	ireg.ah = 0x3;
	ireg.bh = 0;
	intcall(0x10, &ireg, &oreg);

    ireg.ah = 0x13;
    ireg.al = 0x1;
	ireg.bl = 0x07;
	ireg.dx = oreg.dx;
    ireg.cx = n;
	ireg.es = SETUP_DATA_SEG;
    ireg.ebp = (u32_t)buf;

    intcall(0x10, &ireg, 0);
}

/* output a string ended by \0 */
void puts(char *buf) {
	puts_n(buf, strlen(buf));
}

void char2hex(char *buf, u8_t ch)
{
	unsigned int high = (unsigned int)(ch >> 4);
	unsigned int low = (unsigned int)(ch & 0x0F);
	buf[0] = hexcode[high];
	buf[1] = hexcode[low];
}

void puthex_8(u8_t num)
{
	char buf[2] = {0};
	char2hex(buf, num);
	puts("0x");
	puts_n(buf, 2);
}

void puthex_16(u16_t num)
{
	char buf[4] = {0};
	char *s = (char*)&num;
	char2hex(buf, s[1]);
	char2hex(buf+2, s[0]);
	puts("0x");
	puts_n(buf, 4);
}

void puthex_32(u32_t num)
{
	char buf[8] = {0};
	char *s = (char*)&num;
	char2hex(buf, s[3]);
	char2hex(buf+2, s[2]);
	char2hex(buf+4, s[1]);
	char2hex(buf+6, s[0]);
	puts("0x");
	puts_n(buf, 8);
}

void puthex_64(u64_t num)
{
	char buf[16] = {0};
	char *s = (char*)&num;
	char2hex(buf, s[7]);
	char2hex(buf+2, s[6]);
	char2hex(buf+4, s[5]);
	char2hex(buf+6, s[4]);
	char2hex(buf+8, s[3]);
	char2hex(buf+10, s[2]);
	char2hex(buf+12, s[1]);
	char2hex(buf+14, s[0]);
	puts("0x");
	puts_n(buf, 16);
}

void __noreturn die(char *err_msg)
{
	puts(err_msg);
	puts("Press Any Key To Reboot.\r\n");
	__asm__ volatile (
		"jmp reboot"
	);
	unreachable();
}