#ifndef ITEM_H
#define ITEM_H

#include "gba_header.h"
#include "block_data.h"

/* Item stack */
typedef struct {
    u8 type_id;  /* ItemID */
    u8 count;
} Item;

/* Create an item */
static inline Item item_create(u8 type, u8 count) {
    Item i = {type, count};
    return i;
}

/* Check if item slot is empty */
static inline bool item_empty(Item* slot) {
    return slot->type_id == ITEM_NONE || slot->count == 0;
}

#endif