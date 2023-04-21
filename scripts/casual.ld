OUTPUT_FORMAT(elf32-i386)
OUTPUT_ARCH(i386)
ENTRY(_start)
SECTIONS
{
    . = 0Xc0010000;
    .stext : 
    {
        *(.stext)
    }
    .text : 
    {
        *(.text)
    }
    .data : 
    {
        *(.data)
        *(.rodata)
    }
    .bss :
    {
        PROVIDE(_bss_start = .);
        *(.bss)
        PROVIDE(_bss_end = .);
    }
}