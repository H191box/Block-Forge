#ifndef CRAFTING_H
#define CRAFTING_H

#include "recipes.h"
#include "../world/world.h"

/* Check if player has required items for recipe */
int crafting_can_craft(const Recipe* r);

/* Execute crafting (removes inputs, adds outputs) */
void crafting_execute(const Recipe* r);

/* Get the nearest crafting station type to player position */
CraftStation crafting_get_nearby_station(void);

/* Check if recipe's station requirement is met */
int crafting_station_available(const Recipe* r);

#endif