#ifndef SYSTEM_H
#define SYSTEM_H

#include "gba_header.h"

/* Global frame counter, incremented in VBlank ISR */
extern volatile u32 frame_count;

/* Initialize GBA hardware for Mode 0 */
void system_init(void);

/* Wait for VBlank (polling) */
void sys_wait_vblank(void);

/* Global VBlank flag */
extern volatile u32 vblank_flag;

#endif /* SYSTEM_H */