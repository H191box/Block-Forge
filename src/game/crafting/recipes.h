#ifndef RECIPES_H
#define RECIPES_H

#include "gba_header.h"
#include "../entities/item.h"

/* Crafting station types */
typedef enum {
    STATION_HAND = 0,
    STATION_WORKBENCH,
    STATION_FURNACE
} CraftStation;

/* Recipe structure */
typedef struct {
    u8 input_type;
    u8 input_count;
    u8 output_type;
    u8 output_count;
    CraftStation station;
} Recipe;

#define MAX_RECIPES 14

/* Get recipe list */
const Recipe* recipes_get_all(void);

/* Get recipe count */
int recipes_get_count(void);

/* Get recipe name (output item name) */
const char* recipe_get_name(const Recipe* r);

#endif