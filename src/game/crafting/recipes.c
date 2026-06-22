#include "recipes.h"
#include "../world/block_data.h"

static const Recipe recipe_list[MAX_RECIPES] = {
    /* Hand recipes */
    { ITEM_WOOD,      1,  ITEM_WOOD_PLANK,   4,  STATION_HAND },
    { ITEM_STONE,     4,  ITEM_STONE_BRICK,  4,  STATION_HAND },
    { ITEM_SAND,      2,  ITEM_GLASS,        1,  STATION_HAND }, /* needs furnace but simplified */
    
    /* Workbench recipes */
    { ITEM_WOOD_PLANK,10, ITEM_WORKBENCH,    1,  STATION_WORKBENCH },
    { ITEM_WOOD_PLANK,8,  ITEM_CHEST,        1,  STATION_WORKBENCH },
    { ITEM_WOOD_PLANK,5,  ITEM_LADDER,       3,  STATION_WORKBENCH },
    { ITEM_WOOD_PLANK,6,  ITEM_DOOR,         1,  STATION_WORKBENCH },
    { ITEM_WOOD,      1,  ITEM_TORCH,        4,  STATION_WORKBENCH },
    { ITEM_IRON,      3,  ITEM_IRON_SWORD,   1,  STATION_WORKBENCH },
    { ITEM_GOLD,      3,  ITEM_GOLD_SWORD,   1,  STATION_WORKBENCH },
    
    /* Furnace recipes */
    { ITEM_STONE,     20, ITEM_FURNACE,      1,  STATION_FURNACE },
    { ITEM_CLAY,      2,  ITEM_BRICK,        4,  STATION_FURNACE },
    { ITEM_COAL,      1,  ITEM_TORCH,        4,  STATION_FURNACE },
    { ITEM_SAND,      2,  ITEM_GLASS,        1,  STATION_FURNACE },
};

const Recipe* recipes_get_all(void) {
    return recipe_list;
}

int recipes_get_count(void) {
    return MAX_RECIPES;
}

const char* recipe_get_name(const Recipe* r) {
    return item_get_name(r->output_type);
}