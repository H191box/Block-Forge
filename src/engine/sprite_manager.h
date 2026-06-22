#ifndef SPRITE_MANAGER_H
#define SPRITE_MANAGER_H

#include "gba_header.h"

#define MAX_SPRITES 128
#define OAM_ATTR_SIZE 4 /* bytes per OAM entry: attr0, attr1, attr2, padding */

/* Sprite slot IDs */
#define SPR_PLAYER         0
#define SPR_SLIME0         1
#define SPR_SLIME1         2
#define SPR_BAT0           3
#define SPR_BAT1           4
#define SPR_CRACK0         5
#define SPR_CRACK1         6
#define SPR_CRACK2         7
#define SPR_PARTICLE_BASE  8   /* 8..39 = 32 particles */
#define SPR_FIRST_FREE     40

/* OAM entry in EWRAM (matches GBA hardware OAM layout) */
typedef struct {
    u16 attr0;
    u16 attr1;
    u16 attr2;
    u16 attr3; /* padding / affine */
} OAMEntry;

extern OAMEntry oam_buffer[MAX_SPRITES];

/* Clear all sprites */
void spr_init(void);

/* Hide a sprite */
static inline void spr_hide(int id) {
    oam_buffer[id].attr0 = ATTR0_Y(160) | ATTR0_SHAPE_SQUARE;
    oam_buffer[id].attr1 = 0;
    oam_buffer[id].attr2 = 0;
}

/* Set sprite position and tile */
void spr_set(int id, s16 x, s16 y, u16 tile_id, u8 palette, u8 priority, u8 shape_size);

/* Flip sprite horizontally */
void spr_set_hflip(int id, bool flip);

/* Move sprite (just update position) */
static inline void spr_move(int id, s16 x, s16 y) {
    oam_buffer[id].attr1 = (oam_buffer[id].attr1 & 0xF000) | ATTR1_X(x);
    oam_buffer[id].attr0 = (oam_buffer[id].attr0 & 0xFF00) | ATTR0_Y(y);
}

/* Copy OAM buffer to hardware */
void spr_update(void);

#endif