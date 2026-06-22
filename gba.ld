/* Block Forge GBA Linker Script
   Based on devkitPro gba.ld, adds .iwram section for ISR */

OUTPUT_FORMAT("elf32-littlearm", "elf32-littlearm", "elf32-littlearm")
OUTPUT_ARCH(arm)
ENTRY(_start)

MEMORY
{
    ROM  (rx)  : ORIGIN = 0x08000000, LENGTH = 32M
    EWRAM(rwx) : ORIGIN = 0x02000000, LENGTH = 256K
    IWRAM(rwx) : ORIGIN = 0x03000000, LENGTH = 32K
}

__text_start = ORIGIN(ROM);
__text_end   = ORIGIN(ROM) + LENGTH(ROM);
__eheap_align = 8;

SECTIONS
{
    .text : {
        __text_start = .;
        *(.entry)
        *(.text)
        *(.text*)
        *(.glue_7)
        *(.glue_7t)
        *(.stub)
        *(.glue)
        *(.v4_bx)
        *(.vfp11_veneer)
        *(.ARM.extab)
        *(.ARM.exidx*)
        KEEP(*(.init))
        KEEP(*(.fini))
        . = ALIGN(4);
    } > ROM

    .rodata : {
        *(.rodata)
        *(.rodata*)
        . = ALIGN(4);
    } > ROM

    __rom_end = .;

    .data : {
        __data_start = .;
        *(.data)
        *(.data*)
        . = ALIGN(4);
        __data_end = .;
    } > EWRAM AT> ROM

    __data_lma = LOADADDR(.data);

    .bss : {
        __bss_start = .;
        *(.bss)
        *(.bss*)
        *(COMMON)
        . = ALIGN(4);
        __bss_end = .;
    } > EWRAM

    .iwram : {
        __iwram_start = .;
        *(.iwram)
        . = ALIGN(4);
        __iwram_end = .;
    } > IWRAM

    /* Discard everything not explicitly placed */
    /DISCARD/ : {
        *(.ARM.attributes)
        *(.comment)
        *(.note*)
        *(.ARM.exidx*)
    }
}
