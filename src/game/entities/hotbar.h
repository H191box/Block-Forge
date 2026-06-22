#ifndef HOTBAR_H
#define HOTBAR_H

#include "gba_header.h"
#include "inventory.h"

/* Render hotbar on BG3 (bottom of screen) */
void hotbar_render(void);

/* Cycle selection left/right */
void hotbar_cycle_left(void);
void hotbar_cycle_right(void);

/* Set selection directly */
static inline void hotbar_set_selection(int idx) {
    hotbar_selection = idx & 7;
}

#endif