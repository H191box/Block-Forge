#include "sprite_manager.h"
#include "gba_types.h"

OAMEntry oam_buffer[MAX_SPRITES];

void spr_init(void) {
    for (int i = 0; i < MAX_SPRITES; i++) {
        spr_hide(i);
    }
    spr_update();
}

void spr_set(int id, s16 x, s16 y, u16 tile_id, u8 palette, u8 priority, u8 shape_size) {
    u8 shape = (shape_size >> 4) & 3;
    u8 size  = shape_size & 3;
    oam_buffer[id].attr0 = ATTR0_Y(y) | (shape << 14);
    oam_buffer[id].attr1 = ATTR1_X(x) | (size << 14);
    oam_buffer[id].attr2 = ATTR2_TILEID(tile_id) | ATTR2_PRIORITY(priority) | ATTR2_PAL(palette);
}

void spr_set_hflip(int id, bool flip) {
    if (flip) {
        oam_buffer[id].attr1 |= ATTR1_FLIP_H;
    } else {
        oam_buffer[id].attr1 &= ~ATTR1_FLIP_H;
    }
}

void spr_update(void) {
    /* Copy buffer to OAM via DMA */
    REG_DMA3SAD = (u32)oam_buffer;
    REG_DMA3DAD = (u32)OAM;
    REG_DMA3CNT = DMA_ENABLE | DMA_32 | (MAX_SPRITES * 4);
    while (REG_DMA3CNT & DMA_ENABLE);
}