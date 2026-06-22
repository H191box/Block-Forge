#include "block_data.h"

const BlockType block_types[BLOCK_COUNT] = {
    /* AIR */        {0,  0x0000,          0x0000,              0, 0, 0, 0},
    /* DIRT */       {2,  RGB555(20,12,5),  RGB555(15,9,3),     1, 0, 1, 0},
    /* GRASS */      {2,  RGB555(8,20,5),   RGB555(20,12,5),    1, 0, 2, 0},
    /* STONE */      {5,  RGB555(18,18,18), RGB555(12,12,12),   1, 0, 3, 0},
    /* SAND */       {1,  RGB555(28,24,14), RGB555(24,20,10),   1, 1, 4, 0},
    /* WATER */      {0,  RGB555(5,10,25),  RGB555(8,14,28),    0, 0, 0, 0},
    /* LAVA */       {10, RGB555(28,10,0),  RGB555(31,20,0),    1, 0, 0, 3},
    /* BEDROCK */    {10, RGB555(8,8,8),    RGB555(5,5,5),      1, 0, 0, 0},
    /* COPPER */     {4,  RGB555(18,18,18), RGB555(28,14,8),    1, 0, 8, 0},
    /* IRON */       {5,  RGB555(18,18,18), RGB555(22,20,20),   1, 0, 9, 0},
    /* GOLD */       {6,  RGB555(18,18,18), RGB555(31,26,5),    1, 0, 10,0},
    /* CRYSTAL */    {7,  RGB555(18,18,18), RGB555(15,5,25),    1, 0, 11,1},
    /* WOOD */       {3,  RGB555(22,14,5),  RGB555(18,10,3),    1, 0, 12,0},
    /* WOOD_PLANK */ {2,  RGB555(24,18,10), RGB555(20,14,6),    1, 0, 13,0},
    /* BRICK */      {5,  RGB555(24,10,8),  RGB555(18,7,5),     1, 0, 14,0},
    /* STONE_BRICK */{6,  RGB555(20,20,20), RGB555(14,14,14),   1, 0, 15,0},
    /* GLASS */      {1,  RGB555(20,22,25), RGB555(25,28,31),   0, 0, 16,0},
    /* TORCH */      {0,  RGB555(28,20,5),  RGB555(31,28,10),   0, 0, 17,5},
    /* CHEST */      {3,  RGB555(22,14,5),  RGB555(16,8,3),     1, 0, 18,0},
    /* WORKBENCH */  {3,  RGB555(22,14,5),  RGB555(16,8,3),     1, 0, 19,0},
    /* FURNACE */    {5,  RGB555(20,18,18), RGB555(28,14,8),    1, 0, 20,1},
    /* LADDER */     {0,  RGB555(22,14,5),  RGB555(18,10,3),    0, 0, 21,0},
    /* DOOR */       {2,  RGB555(22,14,5),  RGB555(18,10,3),    1, 0, 22,0},
    /* LEAVES */     {1,  RGB555(5,18,8),   RGB555(3,14,5),     0, 0, 23,0},
    /* COAL */       {4,  RGB555(18,18,18), RGB555(5,5,5),      1, 0, 24,0},
    /* CLAY */       {2,  RGB555(22,20,18), RGB555(18,16,14),   1, 0, 25,0},
};

const char* item_names[] = {
    "Nothing",     /* 0 */
    "Dirt",        /* 1 */
    "Grass",       /* 2 */
    "Stone",       /* 3 */
    "Sand",        /* 4 */
    "Water",       /* 5 */
    "Lava",        /* 6 */
    "Bedrock",     /* 7 */
    "Copper Ore",  /* 8 */
    "Iron Ore",    /* 9 */
    "Gold Ore",    /* 10 */
    "Crystal",     /* 11 */
    "Wood",        /* 12 */
    "Wood Plank",  /* 13 */
    "Brick",       /* 14 */
    "Stn Brick",   /* 15 */
    "Glass",       /* 16 */
    "Torch",       /* 17 */
    "Chest",       /* 18 */
    "Workbench",   /* 19 */
    "Furnace",     /* 20 */
    "Ladder",      /* 21 */
    "Door",        /* 22 */
    "Leaves",      /* 23 */
    "Coal",        /* 24 */
    "Clay",        /* 25 */
};

const char* extra_names[] = {
    "Iron Sword",
    "Gold Sword",
    "Gel",
};

const char* item_get_name(u8 item_id) {
    if (item_id >= 100 && item_id - 100 < 3) return extra_names[item_id - 100];
    if (item_id < 26) return item_names[item_id];
    return "Unknown";
}