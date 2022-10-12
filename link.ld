OUTPUT_FORMAT(elf32-i386)
ENTRY(kernel_entry_point)
SECTIONS
{
    . = 0x1000;
    .text : { *(.text) }
    .data : { *(.data) }
    .bss  : { *(.bss)  }
}
