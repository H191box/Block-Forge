#ifndef HUD_H
#define HUD_H

#include "gba_header.h"

/* Render full HUD (health bar, depth indicator) */
void hud_render(void);

/* Render inventory screen overlay */
void hud_render_inventory(void);

/* Render crafting menu overlay */
void hud_render_crafting(int cursor);

#endif