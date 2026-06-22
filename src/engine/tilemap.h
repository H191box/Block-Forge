#ifndef TILEMAP_H
#define TILEMAP_H

#include "gba_header.h"

/* World dimensions */
#define WORLD_W  128
#define WORLD_HEIGHT  64
#define TILE_SIZE 8
#define SCREEN_TILES_X  30
#define SCREEN_TILES_Y  20

/* Setup tilemap VRAM and screenblocks */
void tilemap_init(void);

/* Render visible world tiles into BG0 screenblock */
void tilemap_render(s16 cam_x, s16 cam_y);

/* Render sky gradient into BG1 */
void tilemap_render_sky(u8 brightness);

/* Set BG0 scroll offsets */
static inline void tilemap_scroll(s16 x, s16 y) {
    REG_BG0HOFS = x;
    REG_BG0VOFS = y;
}

#endif