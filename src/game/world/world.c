#include "world.h"
#include "world_gen.h"
#include "gba_types.h"
#include "block_data.h"

/* Flat world array, row-major [y * WORLD_W + x] */
u8 world_data[WORLD_W * WORLD_H];

u8 world_get_tile(int x, int y) {
    if (x < 0 || x >= WORLD_W || y < 0 || y >= WORLD_H) return BLOCK_BEDROCK;
    return world_data[y * WORLD_W + x];
}

void world_set_tile(int x, int y, u8 id) {
    if (x < 0 || x >= WORLD_W || y < 0 || y >= WORLD_H) return;
    world_data[y * WORLD_W + x] = id;
}

void world_init(void) {
    for (int i = 0; i < WORLD_W * WORLD_H; i++) {
        world_data[i] = 0;
    }
}

void world_generate(u32 seed) {
    worldgen_generate(world_data, seed);
}

void world_save(void) {
    /* Delegated to save.c */
}

int world_load(void) {
    /* Delegated to save.c */
    return 0;
}

/* Update gravity-affected blocks (bottom-up scan) */
void world_update_gravity(void) {
    for (int y = WORLD_H - 2; y >= 0; y--) {
        for (int x = 0; x < WORLD_W; x++) {
            u8 block = world_get_tile(x, y);
            if (block > 0 && block < BLOCK_COUNT && block_types[block].gravity) {
                u8 below = world_get_tile(x, y + 1);
                if (below == BLOCK_AIR || below == BLOCK_WATER) {
                    world_set_tile(x, y, below);
                    world_set_tile(x, y + 1, block);
                }
            }
        }
    }
}