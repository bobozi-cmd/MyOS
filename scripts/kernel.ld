/*
 *   kernel.ld -- link script for kernel format
 */

ENTRY(start)
SECTIONS
{
    /* section beginning address is 0x100000 */

    . = 0x100000;
    PROVIDE( kern_start = .);
    .text :
    {
        *(.text)
        . = ALIGN(4096);
    }

    .data :
    {
        *(.data)
        *(.rodata)  /* read-only data used for constant */
        . = ALIGN(4096);
    }

    .bss :
    {
        *(.bss)
        . = ALIGN(4096);
    }

    .stab :
    {
        *(.stab)
        . = ALIGN(4096);
    }

    .stabstr :
    {
        *(.stabstr)
        . = ALIGN(4096);
    }
    PROVIDE( kern_end = .);

    /DISCARD/ : { *(.comment) *(.eh_frame) }
}
