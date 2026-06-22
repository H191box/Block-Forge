#include "tiles.h"
#include "../engine/gba_header.h"
#include "../game/world/block_data.h"

/* BG0 charbase at VRAM 0x06000000 */
/* Each 8x8 4bpp tile = 32 bytes = 16 u16 */
/* We have 26 block types. Each needs one tile in BG0 charbase.
   All tiles use sub-palette 0 of BG_PALETTE (entries 0-15).
   We'll generate distinct pixel patterns per block and set
   sub-palette 0 colors to match block colors.
   
   Strategy: Each tile uses pixel values 0-3.
   Color 0 = transparent (background/sky)
   Color 1 = primary block color
   Color 2 = secondary/detail color
   Color 3 = highlight/shadow
*/

static void make_tile(u16* dst, const u8 pattern[8]) {
    /* pattern: 8 bytes, each byte has 2 bits per pixel (4 pixels per row)
       bit 0-1 = pixel 0, bit 2-3 = pixel 1, bit 4-5 = pixel 2, bit 6-7 = pixel 3
    */
    for (int row = 0; row < 8; row++) {
        u8 p = pattern[row];
        u16 lo = (p & 3) | ((p >> 2) & 3) << 4 | ((p >> 4) & 3) << 8 | ((p >> 6) & 3) << 12;
        dst[row * 2 + 0] = lo;
        dst[row * 2 + 1] = lo; /* Mirror: same pattern for right half */
    }
}

/* Solid fill tile */
static void make_solid(u16* dst, u8 color_idx) {
    u16 val = color_idx | (color_idx << 4) | (color_idx << 8) | (color_idx << 12);
    for (int i = 0; i < 16; i++) dst[i] = val;
}

/* Grass: green top 2 rows, brown bottom */
static const u8 grass_pat[8] = {
    0x55, 0x55, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11
};

/* Stone: gray with dark spots */
static const u8 stone_pat[8] = {
    0x55, 0xAA, 0x55, 0x5A, 0xA5, 0xAA, 0x55, 0xAA
};

/* Ore pattern: stone base with colored veins */
static const u8 ore_pat[8] = {
    0x55, 0x62, 0x45, 0x56, 0x65, 0xA4, 0x56, 0xA5
};

/* Wood: brown with grain */
static const u8 wood_pat[8] = {
    0x11, 0x11, 0x11, 0x22, 0x11, 0x11, 0x22, 0x11
};

/* Leaves: green with gaps */
static const u8 leaves_pat[8] = {
    0x10, 0x31, 0x12, 0x30, 0x01, 0x32, 0x01, 0x10
};

/* Water: blue with wave pattern */
static const u8 water_pat[8] = {
    0x11, 0x22, 0x11, 0x22, 0x11, 0x22, 0x11, 0x22
};

/* Lava: red/orange with bright spots */
static const u8 lava_pat[8] = {
    0x31, 0x12, 0x23, 0x31, 0x12, 0x23, 0x31, 0x12
};

/* Brick pattern */
static const u8 brick_pat[8] = {
    0x33, 0x33, 0x22, 0x22, 0x33, 0x33, 0x22, 0x22
};

/* Glass: transparent with edge */
static const u8 glass_pat[8] = {
    0xFF, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0xFF
};

/* Ladder: vertical rungs */
static const u8 ladder_pat[8] = {
    0x32, 0x10, 0x23, 0x10, 0x32, 0x10, 0x23, 0x10
};

/* Torch: small bright pattern */
static const u8 torch_pat[8] = {
    0x00, 0x10, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00
};

/* Chest: box shape */
static const u8 chest_pat[8] = {
    0x22, 0x22, 0x33, 0x33, 0x33, 0x33, 0x22, 0x22
};

/* Furnace: box with dark opening */
static const u8 furnace_pat[8] = {
    0x22, 0x22, 0x31, 0x31, 0x31, 0x31, 0x22, 0x22
};

/* Door: vertical planks */
static const u8 door_pat[8] = {
    0x22, 0x21, 0x22, 0x21, 0x22, 0x21, 0x22, 0x22
};

void tiles_init(void) {
    u16* cb0 = VRAM_CHARBASE(0);

    /* Tile 0: Air (transparent) */
    make_solid(&cb0[0 * 16], 0);

    /* Tile 1: Dirt - solid brown */
    make_solid(&cb0[1 * 16], 1);

    /* Tile 2: Grass - green top, brown bottom */
    make_tile(&cb0[2 * 16], grass_pat);

    /* Tile 3: Stone */
    make_tile(&cb0[3 * 16], stone_pat);

    /* Tile 4: Sand - solid sandy */
    make_solid(&cb0[4 * 16], 1);

    /* Tile 5: Water */
    make_tile(&cb0[5 * 16], water_pat);

    /* Tile 6: Lava */
    make_tile(&cb0[6 * 16], lava_pat);

    /* Tile 7: Bedrock - dark solid */
    make_solid(&cb0[7 * 16], 2);

    /* Tile 8: Copper ore */
    make_tile(&cb0[8 * 16], ore_pat);

    /* Tile 9: Iron ore */
    make_tile(&cb0[9 * 16], ore_pat);

    /* Tile 10: Gold ore */
    make_tile(&cb0[10 * 16], ore_pat);

    /* Tile 11: Crystal ore */
    make_tile(&cb0[11 * 16], ore_pat);

    /* Tile 12: Wood */
    make_tile(&cb0[12 * 16], wood_pat);

    /* Tile 13: Wood Plank */
    make_solid(&cb0[13 * 16], 1);

    /* Tile 14: Brick */
    make_tile(&cb0[14 * 16], brick_pat);

    /* Tile 15: Stone Brick */
    make_tile(&cb0[15 * 16], stone_pat);

    /* Tile 16: Glass */
    make_tile(&cb0[16 * 16], glass_pat);

    /* Tile 17: Torch */
    make_tile(&cb0[17 * 16], torch_pat);

    /* Tile 18: Chest */
    make_tile(&cb0[18 * 16], chest_pat);

    /* Tile 19: Workbench */
    make_tile(&cb0[19 * 16], chest_pat);

    /* Tile 20: Furnace */
    make_tile(&cb0[20 * 16], furnace_pat);

    /* Tile 21: Ladder */
    make_tile(&cb0[21 * 16], ladder_pat);

    /* Tile 22: Door */
    make_tile(&cb0[22 * 16], door_pat);

    /* Tile 23: Leaves */
    make_tile(&cb0[23 * 16], leaves_pat);

    /* Tile 24: Coal ore */
    make_tile(&cb0[24 * 16], ore_pat);

    /* Tile 25: Clay */
    make_solid(&cb0[25 * 16], 1);

    /* Now set up BG sub-palette 0 (entries 0-15) for block colors.
       Each block type needs colors for indices 1, 2, 3.
       Since different blocks use the same tile indices, we need
       per-block palettes. But we only have sub-palette 0...
       
       Solution: Use different sub-palettes for different block types.
       Map block ID to sub-palette = block_id (capped at 15).
       Update tilemap_render to set palette based on block ID.
       Sub-palette N uses BG_PALETTE[N*16 .. N*16+15].
       
       So: set up 16 sub-palettes, one for each "color group" of blocks.
       Block -> palette mapping:
       0 (air): any
       1 (dirt): palette 1
       2 (grass): palette 2
       3 (stone): palette 3
       4 (sand): palette 4
       5 (water): palette 5
       6 (lava): palette 6
       7 (bedrock): palette 7
       8-11 (ores): palette 8-11
       12 (wood): palette 12
       13-16 (building): palette 13
       17-22 (utility): palette 14
       23-25 (misc): palette 15
    */

    int p;

    /* Palette 0: reserved (transparent) */
    BG_PALETTE[0] = 0; /* transparent */

    /* Palette 1: Dirt */
    p = 1 * 16;
    BG_PALETTE[p + 0] = 0;
    BG_PALETTE[p + 1] = RGB555(20, 12, 5);
    BG_PALETTE[p + 2] = RGB555(15, 9, 3);
    BG_PALETTE[p + 3] = RGB555(25, 16, 8);

    /* Palette 2: Grass */
    p = 2 * 16;
    BG_PALETTE[p + 0] = 0;
    BG_PALETTE[p + 1] = RGB555(8, 20, 5);
    BG_PALETTE[p + 2] = RGB555(20, 12, 5);
    BG_PALETTE[p + 3] = RGB555(12, 25, 8);

    /* Palette 3: Stone */
    p = 3 * 16;
    BG_PALETTE[p + 0] = 0;
    BG_PALETTE[p + 1] = RGB555(18, 18, 18);
    BG_PALETTE[p + 2] = RGB555(12, 12, 12);
    BG_PALETTE[p + 3] = RGB555(24, 24, 24);

    /* Palette 4: Sand */
    p = 4 * 16;
    BG_PALETTE[p + 0] = 0;
    BG_PALETTE[p + 1] = RGB555(28, 24, 14);
    BG_PALETTE[p + 2] = RGB555(24, 20, 10);
    BG_PALETTE[p + 3] = RGB555(31, 28, 18);

    /* Palette 5: Water */
    p = 5 * 16;
    BG_PALETTE[p + 0] = 0;
    BG_PALETTE[p + 1] = RGB555(5, 10, 25);
    BG_PALETTE[p + 2] = RGB555(8, 14, 28);
    BG_PALETTE[p + 3] = RGB555(12, 18, 31);

    /* Palette 6: Lava */
    p = 6 * 16;
    BG_PALETTE[p + 0] = 0;
    BG_PALETTE[p + 1] = RGB555(28, 10, 0);
    BG_PALETTE[p + 2] = RGB555(31, 20, 0);
    BG_PALETTE[p + 3] = RGB555(31, 28, 10);

    /* Palette 7: Bedrock */
    p = 7 * 16;
    BG_PALETTE[p + 0] = 0;
    BG_PALETTE[p + 1] = RGB555(8, 8, 8);
    BG_PALETTE[p + 2] = RGB555(5, 5, 5);
    BG_PALETTE[p + 3] = RGB555(12, 12, 12);

    /* Palette 8: Copper ore */
    p = 8 * 16;
    BG_PALETTE[p + 0] = 0;
    BG_PALETTE[p + 1] = RGB555(18, 18, 18);
    BG_PALETTE[p + 2] = RGB555(28, 14, 8);
    BG_PALETTE[p + 3] = RGB555(22, 10, 5);

    /* Palette 9: Iron ore */
    p = 9 * 16;
    BG_PALETTE[p + 0] = 0;
    BG_PALETTE[p + 1] = RGB555(18, 18, 18);
    BG_PALETTE[p + 2] = RGB555(22, 20, 20);
    BG_PALETTE[p + 3] = RGB555(14, 12, 12);

    /* Palette 10: Gold ore */
    p = 10 * 16;
    BG_PALETTE[p + 0] = 0;
    BG_PALETTE[p + 1] = RGB555(18, 18, 18);
    BG_PALETTE[p + 2] = RGB555(31, 26, 5);
    BG_PALETTE[p + 3] = RGB555(24, 20, 3);

    /* Palette 11: Crystal ore */
    p = 11 * 16;
    BG_PALETTE[p + 0] = 0;
    BG_PALETTE[p + 1] = RGB555(18, 18, 18);
    BG_PALETTE[p + 2] = RGB555(15, 5, 25);
    BG_PALETTE[p + 3] = RGB555(20, 10, 31);

    /* Palette 12: Wood */
    p = 12 * 16;
    BG_PALETTE[p + 0] = 0;
    BG_PALETTE[p + 1] = RGB555(22, 14, 5);
    BG_PALETTE[p + 2] = RGB555(18, 10, 3);
    BG_PALETTE[p + 3] = RGB555(26, 18, 8);

    /* Palette 13: Building blocks (plank, brick, stone brick, glass) */
    p = 13 * 16;
    BG_PALETTE[p + 0] = 0;
    BG_PALETTE[p + 1] = RGB555(24, 18, 10);
    BG_PALETTE[p + 2] = RGB555(20, 14, 6);
    BG_PALETTE[p + 3] = RGB555(28, 22, 14);

    /* Palette 14: Utility (torch, chest, workbench, furnace, ladder, door) */
    p = 14 * 16;
    BG_PALETTE[p + 0] = 0;
    BG_PALETTE[p + 1] = RGB555(22, 14, 5);
    BG_PALETTE[p + 2] = RGB555(16, 8, 3);
    BG_PALETTE[p + 3] = RGB555(28, 20, 10);

    /* Palette 15: Leaves, coal, clay */
    p = 15 * 16;
    BG_PALETTE[p + 0] = 0;
    BG_PALETTE[p + 1] = RGB555(5, 18, 8);
    BG_PALETTE[p + 2] = RGB555(3, 14, 5);
    BG_PALETTE[p + 3] = RGB555(8, 22, 12);
}