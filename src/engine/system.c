#include "system.h"
#include "gba_header.h"
#include "gba_types.h"

volatile u32 frame_count = 0;
volatile u32 vblank_flag = 0;

/* VBlank interrupt handler (called from isr.s in ARM mode) */
void vblank_isr(void) {
    frame_count++;
    vblank_flag = 1;
}

/* Register interrupt handler with BIOS */
static void register_irq_handler(void) {
    /* BIOS interrupt vector table at 0x03007FFC */
    volatile u32* irq_vector = (volatile u32*)0x03007FFC;
    extern void irq_handler(void);  /* defined in isr.s */
    *irq_vector = (u32)irq_handler;
}

/* Initialize GBA for Mode 0: 4 text BG layers, OBJ enabled */
void system_init(void) {
    /* Disable display during setup */
    REG_DISPCNT = 0;

    /* BG1: Sky - CharBase 1, ScreenBase 25, 16-color, Priority 3 (behind all) */
    REG_BG1CNT = BGCNT_CHARBASE(1) | BGCNT_SCREENBASE(25) | BGCNT_PRIORITY(3);

    /* BG2: Entities layer (unused, entities use sprites) */
    REG_BG2CNT = BGCNT_CHARBASE(2) | BGCNT_SCREENBASE(26) | BGCNT_PRIORITY(2);

    /* BG0: World terrain - CharBase 0, ScreenBase 24, 16-color, Priority 1 */
    REG_BG0CNT = BGCNT_CHARBASE(0) | BGCNT_SCREENBASE(24) | BGCNT_PRIORITY(1);

    /* BG3: UI/HUD - CharBase 3, ScreenBase 27, 16-color, Priority 0 (in front) */
    REG_BG3CNT = BGCNT_CHARBASE(3) | BGCNT_SCREENBASE(27) | BGCNT_PRIORITY(0);

    /* Enable BG0, BG1, BG3, OBJ in Mode 0, 1D sprite mapping */
    REG_DISPCNT = DISPCNT_MODE0 | DISPCNT_BG0 | DISPCNT_BG1 | DISPCNT_BG3 | DISPCNT_OBJ | DISPCNT_OBJ_1D;

    /* Set up VBlank interrupt */
    register_irq_handler();
    REG_IE = IRQ_VBLANK;
    REG_DISPSTAT = DISPSTAT_VBLANK_IRQ;
    REG_IME = 1;

    /* Clear OAM */
    volatile u16* oam = (volatile u16*)OAM;
    for (int i = 0; i < 128 * 4; i++) {
        oam[i] = 0;
    }

    /* Clear BG palettes to black */
    for (int i = 0; i < 256; i++) {
        BG_PALETTE[i] = 0;
    }

    /* Clear OBJ palettes to black */
    for (int i = 0; i < 256; i++) {
        OBJ_PALETTE[i] = 0;
    }
}

/* Wait for next VBlank */
void sys_wait_vblank(void) {
    vblank_flag = 0;
    while (!vblank_flag);
}