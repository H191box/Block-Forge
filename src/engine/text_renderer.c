#include "text_renderer.h"
#include "gba_types.h"

/* BG3 charbase at CharBase 3 */
#define BG3_CHAR  VRAM_CHARBASE(3)

void text_init(void) {
    u16* cb3 = BG3_CHAR;

    /* Tile 0: blank (all transparent) */
    for (int i = 0; i < 16; i++) cb3[i] = 0;

    /* Tile 1: solid fill (all color 1) */
    for (int i = 0; i < 16; i++) cb3[16 + i] = 0x1111;

    /* Tile 2-9: border pieces using color 1 */
    /* We'll just use tile 1 for everything simple */
    for (int t = 2; t < 10; t++) {
        for (int i = 0; i < 16; i++) cb3[t * 16 + i] = 0x1111;
    }

    /* Tile 10: inventory slot (border) - outer pixels = color 1, inner = 0 */
    /* 8x8 tile, 4bpp. Row by row:
       11111111 = 0x1111
       10000001 = 0x1001  (wait, 4bpp: each pixel is 4 bits, 2 pixels per u16)
       Pixel [0]=1, [1]=0 => 0x0001
       Hmm, 4bpp is stored as: pixel0 in bits 0-3, pixel1 in bits 4-7, etc.
       Two pixels per u16: low nybble = left pixel, high nybble = right pixel.
    */
    /* Slot border: row 0 = all 1s, row 1 = 1,0,0,0,0,0,0,1, row 2-5 = 1,0,0,0,0,0,0,1, row 6 = 1,0,0,0,0,0,0,1, row 7 = all 1s */
    {
        u16* tile = &cb3[10 * 16];
        /* Row 0: all 1s */
        tile[0] = tile[1] = 0x1111;
        /* Row 1-6: 1,0,0,0,0,0,0,1 */
        tile[2] = 0x0001; tile[3] = 0x0000; /* 1,0,0,0 | 0,0,0,1 -> actually: */
        /* Two pixels per u16: pixel0=low nybble, pixel1=high nybble */
        /* Row "1,0,0,0,0,0,0,1": 
           u16[0] = pixel0=1, pixel1=0 => 0x0001
           u16[1] = pixel2=0, pixel3=0 => 0x0000
           u16[2] = pixel4=0, pixel5=0 => 0x0000
           u16[3] = pixel6=0, pixel7=1 => 0x1000
        */
        for (int r = 1; r <= 6; r++) {
            tile[r*2+0] = 0x0001;
            tile[r*2+1] = 0x1000;
        }
        /* Row 7: all 1s */
        tile[14] = tile[15] = 0x1111;
    }

    /* Tile 11: selected slot (same but color 2) */
    {
        u16* tile = &cb3[11 * 16];
        tile[0] = tile[1] = 0x2222;
        for (int r = 1; r <= 6; r++) {
            tile[r*2+0] = 0x0002;
            tile[r*2+1] = 0x2000;
        }
        tile[14] = tile[15] = 0x2222;
    }

    /* Font characters at tiles 16-111 (96 printable ASCII: 0x20-0x7F) */
    /* Font data defined in font.c, imported as extern */
    extern const u8 font_data[];
    for (int c = 0; c < 96; c++) {
        u16* tile = &cb3[(UI_TILE_FIRST_CHAR + c) * 16];
        for (int row = 0; row < 8; row++) {
            u8 bits = font_data[c * 8 + row];
            /* Expand 1bpp to 4bpp: bit=1 -> color 1, bit=0 -> color 0 */
            tile[row * 2 + 0] = 0; /* pixels 0-3 */
            tile[row * 2 + 1] = 0; /* pixels 4-7 */
            for (int px = 0; px < 4; px++) {
                if (bits & (0x80 >> px))
                    tile[row * 2 + 0] |= (1 << (px * 4));
            }
            for (int px = 4; px < 8; px++) {
                if (bits & (0x80 >> px))
                    tile[row * 2 + 1] |= (1 << ((px - 4) * 4));
            }
        }
    }

    text_clear();
}

void text_clear(void) {
    u16* sb = BG3_SCREEN;
    for (int i = 0; i < 32 * 32; i++) sb[i] = 0;
    REG_BG3HOFS = 0;
    REG_BG3VOFS = 0;
}

void text_fill_rect(u8 x, u8 y, u8 w, u8 h, u8 tile_id, u8 palette) {
    u16 entry = tile_id | (palette << 12);
    for (int ty = 0; ty < h; ty++) {
        for (int tx = 0; tx < w; tx++) {
            text_set_tile(x + tx, y + ty, entry);
        }
    }
}

void text_rect_outline(u8 x, u8 y, u8 w, u8 h, u8 tile_id, u8 palette) {
    u16 entry = tile_id | (palette << 12);
    for (int tx = 0; tx < w; tx++) {
        text_set_tile(x + tx, y, entry);
        text_set_tile(x + tx, y + h - 1, entry);
    }
    for (int ty = 1; ty < h - 1; ty++) {
        text_set_tile(x, y + ty, entry);
        text_set_tile(x + w - 1, y + ty, entry);
    }
}

int text_draw_char(char c, u8 x, u8 y, u8 palette) {
    u8 tile_x = x >> 3;
    u8 tile_y = y >> 3;
    u8 sub_x = x & 7;
    (void)sub_x; /* Assume byte-aligned for simplicity */

    int char_idx = c - 0x20;
    if (char_idx < 0 || char_idx >= 96) char_idx = 0;

    u16 entry = (UI_TILE_FIRST_CHAR + char_idx) | (palette << 12);
    BG3_SCREEN[(tile_y & 31) * 32 + (tile_x & 31)] = entry;
    return 8;
}

int text_draw(const char* str, u8 x, u8 y, u8 palette) {
    int cx = x;
    while (*str) {
        text_draw_char(*str, cx, y, palette);
        cx += 8;
        str++;
    }
    return cx - x;
}

int text_draw_centered(const char* str, u8 cx, u8 y, u8 width, u8 palette) {
    int len = 0;
    const char* s = str;
    while (*s) { len += 8; s++; }
    int start_x = cx + (width - len) / 2;
    if (start_x < 0) start_x = 0;
    return text_draw(str, start_x, y, palette);
}

int text_draw_num(int val, u8 x, u8 y, u8 palette) {
    char buf[12];
    int i = 0;
    bool neg = false;
    if (val < 0) { neg = true; val = -val; }
    if (val == 0) { buf[i++] = '0'; }
    while (val > 0) {
        buf[i++] = '0' + (val % 10);
        val /= 10;
    }
    if (neg) buf[i++] = '-';
    buf[i] = 0;
    /* Reverse */
    for (int a = 0, b = i - 1; a < b; a++, b--) {
        char tmp = buf[a]; buf[a] = buf[b]; buf[b] = tmp;
    }
    return text_draw(buf, x, y, palette);
}