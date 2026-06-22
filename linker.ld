/* Block Forge Linker Script - ARM7TDMI GBA Memory Map */
/* Updated: added .gba_header section for proper ROM header */

ENTRY(_start)

MEMORY
{
    ROM  (rx)  : ORIGIN = 0x08000000, LENGTH = 32M
    EWRAM(rw)  : ORIGIN = 0x02000000, LENGTH = 256K
    IWRAM(rw)  : ORIGIN = 0x03000000, LENGTH = 32K
    SRAM(rw)   : ORIGIN = 0x0E000000, LENGTH = 64K
}

SECTIONS
{
    /* GBA ROM Header - must be first in ROM */
    .gba_header 0x08000000 : {
        KEEP(*(.gba_header))
    } > ROM

    /* ROM section - code and read-only data */
    .text : {
        *(.text.entry)
        *(.text*)
        *(.glue_7)
        *(.glue_7t)
        *(.rodata*)
        _etext = .;
    } > ROM

    /* Read-write data in EWRAM */
    .data : AT(_etext) {
        _sdata = .;
        *(.data*)
        _edata = .;
    } > EWRAM

    /* Copy source address */
    __data_lma = LOADADDR(.data);

    /* BSS in EWRAM */
    .bss : {
        _sbss = .;
        *(.bss*)
        *(COMMON)
        _ebss = .;
    } > EWRAM

    /* Stack in IWRAM */
    .stack (NOLOAD) : {
        . = ALIGN(4);
        _stack_top = . + 8192;
    } > IWRAM

    /* IWRAM section for ISRs (ARM mode code placed in IWRAM for speed) */
    .iwram : {
        *(.iwram)
        . = ALIGN(4);
    } > IWRAM

    /* SRAM region for save data */
    .sram (NOLOAD) : {
        _sram_start = .;
        . += 64K;
        _sram_end = .;
    } > SRAM

    /DISCARD/ : {
        *(.ARM.attributes)
        *(.comment)
        *(.ARM.exidx*)
        *(.note*)
    }
}
