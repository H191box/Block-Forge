#include "crafting.h"
#include "../entities/inventory.h"
#include "../entities/player.h"
#include "../world/block_data.h"

int crafting_can_craft(const Recipe* r) {
    if (!r) return 0;
    return inventory_count(r->input_type) >= r->input_count;
}

void crafting_execute(const Recipe* r) {
    if (!r || !crafting_can_craft(r)) return;
    inventory_remove_type(r->input_type, r->input_count);
    inventory_add(r->output_type, r->output_count);
}

CraftStation crafting_get_nearby_station(void) {
    /* Check 5x5 area around player for workbench/furnace */
    int px = (player.x + PLAYER_W / 2) / 8;
    int py = (player.y + PLAYER_H / 2) / 8;
    
    for (int dy = -2; dy <= 2; dy++) {
        for (int dx = -2; dx <= 2; dx++) {
            u8 block = world_get_tile(px + dx, py + dy);
            if (block == BLOCK_WORKBENCH) return STATION_WORKBENCH;
            if (block == BLOCK_FURNACE) return STATION_FURNACE;
        }
    }
    return STATION_HAND;
}

int crafting_station_available(const Recipe* r) {
    CraftStation nearby = crafting_get_nearby_station();
    if (r->station == STATION_HAND) return 1;
    return nearby == r->station;
}