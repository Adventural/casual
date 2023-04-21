/*
 * @Author: jiaxgong jiaxgong@163.com
 * @Date: 2022-04-16 21:06:10
 * @Description:
 */
#include "boot.h"
#include "config.h"
#include "boot_params.h"
#include <casual/compile.h>
#include <casual/types.h>

struct boot_params *boot_params = (struct boot_params *)BOOT_PARAMS_OFFSET;


void setup_main(void) {

    puts("setup kernel...\r\n");

    detect_memory();
    
    setup_kernel();
}