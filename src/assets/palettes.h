#ifndef PALETTES_H
#define PALETTES_H

#include "engine/gba_types.h"

/* Initialize BG3 UI palette (sub-palette for HUD text) */
void palettes_init_ui(void);

/* Apply day/night brightness to all block palettes */
void palettes_apply_brightness(u8 brightness);

#endif