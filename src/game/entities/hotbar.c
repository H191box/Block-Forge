#include "hotbar.h"
#include "../world/block_data.h"
#include "../../engine/text_renderer.h"
#include "../../engine/gba_types.h"

void hotbar_render(void) {
    int bar_y = 18;
    int start_x = 3;

    for (int i = 0; i < HOTBAR_SLOTS; i++) {
        int tx = start_x + i * 3;
        u8 tile = (i == hotbar_selection) ? UI_TILE_SLOT_SEL : UI_TILE_SLOT;
        u8 pal = (i == hotbar_selection) ? 2 : 1;
        text_set_tile(tx, bar_y, tile | (pal << 12));
        text_set_tile(tx + 1, bar_y, tile | (pal << 12));
        text_set_tile(tx + 2, bar_y, tile | (pal << 12));
    }

    /* Draw selected item name below */
    Item sel = hotbar_get_selected();
    if (sel.count > 0 && sel.type_id != ITEM_NONE) {
        const char* name = item_get_name(sel.type_id);
        text_draw_centered(name, 0, bar_y * 8 + 8, 240, 0);
        
        char buf[8];
        buf[0] = 'x';
        buf[1] = '0' + (sel.count / 10);
        buf[2] = '0' + (sel.count % 10);
        buf[3] = 0;
        text_draw(buf, 208, bar_y * 8 + 8, 0);
    }
}

void hotbar_cycle_left(void) {
    hotbar_selection = (hotbar_selection - 1) & 7;
}

void hotbar_cycle_right(void) {
    hotbar_selection = (hotbar_selection + 1) & 7;
}