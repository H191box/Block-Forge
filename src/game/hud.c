#include "hud.h"
#include "../engine/text_renderer.h"
#include "../engine/gba_types.h"
#include "../entities/player.h"
#include "../entities/hotbar.h"
#include "../entities/inventory.h"
#include "../world/block_data.h"
#include "../world/day_night.h"
#include "../crafting/recipes.h"
#include "../crafting/crafting.h"

void hud_render(void) {
    /* Health bar at top-left */
    text_fill_rect(0, 0, 10, 1, UI_TILE_SOLID, 1);
    /* Red fill for HP */
    u8 hp_tiles = (player.hp * 8) / player.max_hp;
    text_fill_rect(1, 0, MIN(hp_tiles, 8), 1, UI_TILE_SOLID, 2); /* pal 2 = red */
    
    text_draw("HP", 0, 0, 0);
    
    /* Hunger bar next to HP */
    text_fill_rect(12, 0, 6, 1, UI_TILE_SOLID, 1);
    u8 hunger_tiles = (player.hunger * 4) / 100;
    text_fill_rect(13, 0, MIN(hunger_tiles, 4), 1, UI_TILE_SOLID, 3); /* pal 3 = green */
    text_draw("Fd", 12 * 8, 0, 0);
    
    /* Depth indicator */
    int depth = (player.y / 8);
    if (depth < 10) {
        text_draw("Surface", 200, 0, 0);
    } else if (depth < 30) {
        text_draw("Underground", 184, 0, 0);
    } else if (depth < 50) {
        text_draw("Deep", 224, 0, 0);
    } else {
        text_draw("Bedrock", 216, 0, 0);
    }

    /* Day/night indicator */
    DayNightPhase phase = daynight_get_phase();
    const char* time_str = "Day";
    if (phase == DN_NIGHT) time_str = "Night";
    else if (phase == DN_SUNSET) time_str = "Dusk";
    else if (phase == DN_SUNRISE) time_str = "Dawn";
    text_draw(time_str, 80, 0, 0);
    
    /* Render hotbar */
    hotbar_render();
}

void hud_render_inventory(void) {
    /* Dark overlay background */
    text_fill_rect(2, 2, 26, 16, UI_TILE_SOLID, 1);
    
    /* Title */
    text_draw_centered("INVENTORY", 0, 2 * 8, 240, 0);
    
    /* Inventory grid: 8 columns x 4 rows = 32 slots */
    int grid_x = 5;
    int grid_y = 5;
    
    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 8; col++) {
            int slot = row * 8 + col;
            int tx = grid_x + col * 3;
            int ty = grid_y + row * 3;
            
            /* Slot background */
            text_fill_rect(tx, ty, 2, 2, UI_TILE_SLOT, 1);
            
            /* Item count */
            Item item = inventory_get(slot);
            if (item.count > 0) {
                text_fill_rect(tx, ty, 2, 2, UI_TILE_SOLID, 4);
                if (item.count < 10) {
                    text_draw_num(item.count, (tx + 1) * 8, (ty + 1) * 8, 0);
                } else {
                    text_draw_num(item.count, tx * 8, (ty + 1) * 8, 0);
                }
                /* Item name on hover - just show first char for now */
                const char* name = item_get_name(item.type_id);
                text_draw_char(name[0], tx * 8 + 4, ty * 8, 0);
            }
        }
    }
    
    text_draw("START=Close L/R=Craft", 16, 17 * 8, 0);
}

void hud_render_crafting(int cursor) {
    /* Dark overlay */
    text_fill_rect(2, 2, 26, 16, UI_TILE_SOLID, 1);
    
    /* Title */
    text_draw_centered("CRAFTING", 0, 2 * 8, 240, 0);
    
    const Recipe* recipes = recipes_get_all();
    int count = recipes_get_count();
    
    /* Show station */
    CraftStation station = crafting_get_nearby_station();
    const char* station_str = "Hand";
    if (station == STATION_WORKBENCH) station_str = "Workbench";
    if (station == STATION_FURNACE) station_str = "Furnace";
    text_draw(station_str, 16, 3 * 8, 0);
    
    /* Recipe list */
    for (int i = 0; i < count && i < 12; i++) {
        int y = (4 + i) * 8;
        if (y > 16 * 8) break;
        
        const Recipe* r = &recipes[i];
        int can = crafting_can_craft(r) && crafting_station_available(r);
        u8 pal = can ? 3 : 1; /* Green if craftable */
        
        if (i == cursor) pal = 2; /* Red for selected */
        
        /* Arrow and name */
        text_draw(">", 16, y, pal);
        const char* name = recipe_get_name(r);
        text_draw(name, 24, y, pal);
        
        /* Input count */
        text_draw("x", 120, y, 0);
        text_draw_num(r->input_count, 128, y, 0);
        
        /* -> output count */
        text_draw("->x", 152, y, 0);
        text_draw_num(r->output_count, 176, y, 0);
    }
    
    text_draw("A=Craft B=Back", 48, 17 * 8, 0);
}