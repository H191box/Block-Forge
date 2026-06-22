#include "tilemap.h"
#include "game/world/world.h"
#include "game/world/block_data.h"

#define BG0_SCREEN ((u16*)0x0600C000)
#define BG1_SCREEN ((u16*)0x0600C800)
#define BG0_CHAR  VRAM_CHARBASE(0)
#define BG1_CHAR  VRAM_CHARBASE(1)

/* Block ID -> sub-palette mapping (1-15, sub-palette 0 reserved for sky/UI) */
static const u8 block_palette[BLOCK_COUNT] = {
    0,  /* AIR */
    1,  /* DIRT */
    2,  /* GRASS */
    3,  /* STONE */
    1,  /* SAND (shares with dirt) */
    5,  /* WATER */
    6,  /* LAVA */
    7,  /* BEDROCK */
    8,  /* COPPER */
    9,  /* IRON */
    10, /* GOLD */
    11, /* CRYSTAL */
    12, /* WOOD */
    13, /* WOOD_PLANK */
    13, /* BRICK */
    3,  /* STONE_BRICK */
    1,  /* GLASS (shares with dirt) */
    14, /* TORCH */
    14, /* CHEST */
    14, /* WORKBENCH */
    9,  /* FURNACE */
    12, /* LADDER */
    12, /* DOOR */
    15, /* LEAVES */
    7,  /* COAL */
    1,  /* CLAY */
};

void tilemap_init(void) {
    for (int i = 0; i < 32*32; i++) BG0_SCREEN[i] = 0;
    for (int i = 0; i < 32*32; i++) BG1_SCREEN[i] = 0;
}

void tilemap_render(s16 cam_x, s16 cam_y) {
    s16 tile_cx = cam_x >> 3;
    s16 tile_cy = cam_y >> 3;
    s16 sub_x = cam_x & 7;
    s16 sub_y = cam_y & 7;
    if (sub_x < 0) { tile_cx--; sub_x += 8; }
    if (sub_y < 0) { tile_cy--; sub_y += 8; }

    REG_BG0HOFS = sub_x;
    REG_BG0VOFS = sub_y;

    for (int ty = 0; ty <= SCREEN_TILES_Y; ty++) {
        int wy = tile_cy + ty;
        int sb_y = ty & 31;
        for (int tx = 0; tx <= SCREEN_TILES_X; tx++) {
            int wx = tile_cx + tx;
            int sb_x = tx & 31;
            u8 block = 0;
            if (wx >= 0 && wx < WORLD_W && wy >= 0 && wy < WORLD_HEIGHT) {
                block = world_get_tile(wx, wy);
            }
            if (block == 0) {
                BG0_SCREEN[sb_y * 32 + sb_x] = 0;
            } else {
                u8 pal = block < BLOCK_COUNT ? block_palette[block] : 1;
                BG0_SCREEN[sb_y * 32 + sb_x] = block | (pal << 12);
            }
        }
    }
}

void tilemap_render_sky(u8 brightness) {
    REG_BG1HOFS = 0;
    REG_BG1VOFS = 0;

    /* Simple flat sky: one tile, all pixels = color 1, sub-palette 0 */
    u16* cb1 = BG1_CHAR;
    u16 pix = 0x1111;
    for (int i = 0; i < 16; i++) cb1[i] = pix;

    /* Fill screenblock */
    u16 sky_entry = 0; /* tile 0, sub-palette 0 */
    for (int i = 0; i < 32 * 32; i++) {
        BG1_SCREEN[i] = sky_entry;
    }

    /* Set sky color in sub-palette 0, color 1
       Brightness 100 = day (bright blue), 35 = night (dark blue) */
    int r = 8 * brightness / 100;
    int g = 12 * brightness / 100;
    int b = 25 * brightness / 100 + 5;
    if (r > 31) r = 31;
    if (g > 31) g = 31;
    if (b > 31) b = 31;
    BG_PALETTE[1] = RGB555(r, g, b);
}