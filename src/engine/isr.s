@ Block Forge - Interrupt Dispatcher (ARM mode, placed in IWRAM)
@ The GBA BIOS calls the address at 0x03007FFC when an IRQ fires.

    .section .iwram, "ax", %progbits
    .arm
    .global irq_handler

irq_handler:
    @ Save registers
    stmfd sp!, {r0-r3, r12, lr}

    @ Set up REG_BASE
    ldr r0, =0x04000000
    add r12, r0, #0x200     @ r12 = &REG_IE

    @ Read interrupt flags
    ldr r1, [r12, #0x00]    @ REG_IE
    ldr r2, [r12, #0x02]    @ REG_IF
    and r1, r1, r2          @ enabled & pending

    @ Check VBlank
    tst r1, #0x01
    beq skip_vblank
    bl vblank_isr
skip_vblank:

    @ Acknowledge all handled interrupts (REG_IF = flags)
    strh r1, [r12, #0x02]   @ REG_IF = r1

    @ Restore registers
    ldmfd sp!, {r0-r3, r12, lr}

    @ Return from IRQ
    subs pc, lr, #4

    .pool
