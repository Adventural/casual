#ifndef _CASUAL_CGA_H
#define _CASUAL_CGA_H

#include <asm/x86.h>
#include <casual/types.h>

#define CGA_BASE 0xB8000

#define CGA_ROW 25
#define CGA_COL 80
#define CGA_SIZE 80 * 25

#define CGA_REG_MC 0x3D8    // mode control register
#define CGA_REG_COLOR 0x3D9 // color select register
#define CGA_REG_INDEX 0x3D4 // index register
#define CGA_REG_DATA 0x3D5  // data register

#endif
