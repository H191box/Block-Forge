#ifndef INVENTORY_H
#define INVENTORY_H

#include "item.h"
#include "block_data.h"

#define INV_SLOTS 32
#define HOTBAR_SLOTS 8
#define HOTBAR_SIZE 4 /* inventory slot width in tiles */

/* Add item to inventory. Returns amount that couldn't be added. */
u8 inventory_add(u8 type_id, u8 count);

/* Remove count items from slot. Returns actual amount removed. */
u8 inventory_remove(int slot, u8 count);

/* Get item at slot */
Item inventory_get(int slot);

/* Set item at slot directly */
void inventory_set(int slot, Item item);

/* Count total items of a given type in inventory */
u8 inventory_count(u8 type_id);

/* Remove items of a type (for crafting). Returns 1 if successful. */
int inventory_remove_type(u8 type_id, u8 count);

/* Get hotbar slot (slots 0-7) */
static inline Item hotbar_get(int idx) { return inventory_get(idx); }

/* Get currently selected hotbar item */
Item hotbar_get_selected(void);

/* Get selected hotbar index */
extern int hotbar_selection;

#endif