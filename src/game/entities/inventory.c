#include "inventory.h"

Item inventory[INV_SLOTS];
int hotbar_selection = 0;

u8 inventory_add(u8 type_id, u8 count) {
    if (type_id == ITEM_NONE || count == 0) return 0;
    u8 remaining = count;

    /* Try to stack onto existing slots */
    for (int i = 0; i < INV_SLOTS && remaining > 0; i++) {
        if (inventory[i].type_id == type_id) {
            u8 can_add = item_max_stack(type_id) - inventory[i].count;
            if (can_add > 0) {
                u8 add = MIN(can_add, remaining);
                inventory[i].count += add;
                remaining -= add;
            }
        }
    }

    /* Try empty slots */
    for (int i = 0; i < INV_SLOTS && remaining > 0; i++) {
        if (inventory[i].count == 0) {
            u8 add = MIN(item_max_stack(type_id), remaining);
            inventory[i].type_id = type_id;
            inventory[i].count = add;
            remaining -= add;
        }
    }

    return remaining; /* 0 = all added */
}

u8 inventory_remove(int slot, u8 count) {
    if (slot < 0 || slot >= INV_SLOTS) return 0;
    u8 actual = MIN(count, inventory[slot].count);
    inventory[slot].count -= actual;
    if (inventory[slot].count == 0) inventory[slot].type_id = ITEM_NONE;
    return actual;
}

Item inventory_get(int slot) {
    if (slot < 0 || slot >= INV_SLOTS) {
        Item empty = {ITEM_NONE, 0};
        return empty;
    }
    return inventory[slot];
}

void inventory_set(int slot, Item item) {
    if (slot >= 0 && slot < INV_SLOTS) {
        inventory[slot] = item;
    }
}

u8 inventory_count(u8 type_id) {
    u8 total = 0;
    for (int i = 0; i < INV_SLOTS; i++) {
        if (inventory[i].type_id == type_id) {
            total += inventory[i].count;
        }
    }
    return total;
}

int inventory_remove_type(u8 type_id, u8 count) {
    if (inventory_count(type_id) < count) return 0;
    u8 remaining = count;
    for (int i = 0; i < INV_SLOTS && remaining > 0; i++) {
        if (inventory[i].type_id == type_id) {
            u8 take = MIN(inventory[i].count, remaining);
            inventory[i].count -= take;
            remaining -= take;
            if (inventory[i].count == 0) inventory[i].type_id = ITEM_NONE;
        }
    }
    return 1;
}

Item hotbar_get_selected(void) {
    return inventory_get(hotbar_selection);
}