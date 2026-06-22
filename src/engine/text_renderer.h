#ifndef TEXT_RENDERER_H
#define TEXT_RENDERER_H

#include "gba_header.h"

/* Initialize font: expand 1bpp to 4bpp in BG3 charbase */
void text_init(void);

/* Draw a character at pixel (x,y) on BG3. Returns width (8). */
int text_draw_char(char c, u8 x, u8 y, u8 palette);

/* Draw string on BG3. Returns total width. */
int text_draw(const char* str, u8 x, u8 y, u8 palette);

/* Draw string on BG3, centered in given width. */
int text_draw_centered(const char* str, u8 cx, u8 y, u8 width, u8 palette);

/* Fill BG3 with transparent tiles */
void text_clear(void);

/* Draw a filled rectangle on BG3 (UI panel) */
void text_fill_rect(u8 x, u8 y, u8 w, u8 h, u8 tile_id, u8 palette);

/* Draw a rect outline on BG3 */
void text_rect_outline(u8 x, u8 y, u8 w, u8 h, u8 tile_id, u8 palette);

/* Write a single number */
int text_draw_num(int val, u8 x, u8 y, u8 palette);

/* BG3 screenblock */
#define BG3_SCREEN ((u16*)0x0600D800)

/* Draw a tile at BG3 position (in tiles) */
static inline void text_set_tile(u8 tx, u8 ty, u16 tile_entry) {
    BG3_SCREEN[(ty & 31) * 32 + (tx & 31)] = tile_entry;
}

/* UI tile IDs in BG3 charbase */
#define UI_TILE_BLANK    0
#define UI_TILE_SOLID    1
#define UI_TILE_BORDER_T 2
#define UI_TILE_BORDER_B 3
#define UI_TILE_BORDER_L 4
#define UI_TILE_BORDER_R 5
#define UI_TILE_BORDER_TL 6
#define UI_TILE_BORDER_TR 7
#define UI_TILE_BORDER_BL 8
#define UI_TILE_BORDER_BR 9
#define UI_TILE_SLOT     10
#define UI_TILE_SLOT_SEL 11
#define UI_TILE_FIRST_CHAR 16 /* Font characters start at tile 16 */

#endif