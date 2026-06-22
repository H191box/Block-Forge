@ Block Forge - GBA CRT0 Startup Code (ARM mode)
@ Sets up stack, copies .data, zeroes .bss, calls main

    .section .text.entry, "ax", %progbits
    .arm                    @ Must start in ARM mode
    .global _start

_start:
    @ Disable interrupts
    ldr r0, =0x04000208
    mov r1, #0
    str r1, [r0]

    @ Set stack pointer to top of IWRAM
    ldr sp, =0x03007800

    @ Copy .data from ROM to EWRAM
    ldr r0, =_sdata
    ldr r1, =_edata
    ldr r2, =__data_lma
    cmp r0, r1
    beq 2f
1:
    ldr r3, [r2], #4
    str r3, [r0], #4
    cmp r0, r1
    bne 1b
2:

    @ Zero .bss
    ldr r0, =_sbss
    ldr r1, =_ebss
    mov r2, #0
    cmp r0, r1
    beq 4f
3:
    str r2, [r0], #4
    cmp r0, r1
    bne 3b
4:

    @ Switch to Thumb and call main
    ldr r0, =main
    bx  r0

    @ Infinite loop
5:
    b 5b

    .pool
