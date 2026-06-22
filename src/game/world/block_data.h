#ifndef BLOCK_DATA_H
#define BLOCK_DATA_H

#include "gba_header.h"

/* Block IDs */
typedef enum {
    BLOCK_AIR       = 0,
    BLOCK_DIRT      = 1,
    BLOCK_GRASS     = 2,
    BLOCK_STONE     = 3,
    BLOCK_SAND      = 4,
    BLOCK_WATER     = 5,
    BLOCK_LAVA      = 6,
    BLOCK_BEDROCK   = 7,
    BLOCK_COPPER    = 8,
    BLOCK_IRON      = 9,
    BLOCK_GOLD      = 10,
    BLOCK_CRYSTAL   = 11,
    BLOCK_WOOD      = 12,
    BLOCK_WOOD_PLANK= 13,
    BLOCK_BRICK     = 14,
    BLOCK_STONE_BRICK = 15,
    BLOCK_GLASS     = 16,
    BLOCK_TORCH     = 17,
    BLOCK_CHEST     = 18,
    BLOCK_WORKBENCH = 19,
    BLOCK_FURNACE   = 20,
    BLOCK_LADDER    = 21,
    BLOCK_DOOR      = 22,
    BLOCK_LEAVES    = 23,
    BLOCK_COAL      = 24,
    BLOCK_CLAY      = 25,
    BLOCK_COUNT     = 26
} BlockID;

/* Block properties */
typedef struct {
    u8 hardness;       /* 0=air/instant, 1-9=mineable, 10=unbreakable */
    u16 color;         /* RGB555 primary color */
    u16 color2;        /* RGB555 secondary/detail color */
    u8 solid;          /* 1 = blocks movement */
    u8 gravity;        /* 1 = falls if unsupported */
    u8 drop_item;      /* Item ID dropped when mined (0 = block itself) */
    u8 light;          /* Light level emitted (0=none, 1-15) */
} BlockType;

/* Item IDs (block IDs map to items, plus tool/food items) */
typedef enum {
    ITEM_NONE = 0,
    /* Block items = same as BlockID */
    ITEM_DIRT = 1,
    ITEM_GRASS = 2,
    ITEM_STONE = 3,
    ITEM_SAND = 4,
    ITEM_COPPER = 8,
    ITEM_IRON = 9,
    ITEM_GOLD = 10,
    ITEM_CRYSTAL = 11,
    ITEM_WOOD = 12,
    ITEM_WOOD_PLANK = 13,
    ITEM_BRICK = 14,
    ITEM_STONE_BRICK = 15,
    ITEM_GLASS = 16,
    ITEM_TORCH = 17,
    ITEM_CHEST = 18,
    ITEM_WORKBENCH = 19,
    ITEM_FURNACE = 20,
    ITEM_LADDER = 21,
    ITEM_DOOR = 22,
    ITEM_LEAVES = 23,
    ITEM_COAL = 24,
    ITEM_CLAY = 25,
    /* Non-block items */
    ITEM_IRON_SWORD = 100,
    ITEM_GOLD_SWORD = 101,
    ITEM_GEL = 102,
    ITEM_COUNT = 103
} ItemID;

/* Block properties table */
extern const BlockType block_types[BLOCK_COUNT];

/* Get block properties */
static inline const BlockType* block_get(u8 id) {
    if (id < BLOCK_COUNT) return &block_types[id];
    return &block_types[0]; /* air */
}

/* Check if block is solid */
static inline bool block_is_solid(u8 id) {
    return id > 0 && id < BLOCK_COUNT && block_types[id].solid;
}

/* Get item name (for display) */
const char* item_get_name(u8 item_id);

/* Get max stack size for item */
static inline u8 item_max_stack(u8 item_id) {
    /* Tools don't stack, everything else stacks to 99 */
    if (item_id == ITEM_IRON_SWORD || item_id == ITEM_GOLD_SWORD) return 1;
    return 99;
}

#endif