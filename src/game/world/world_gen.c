#include "world_gen.h"
#include "gba_types.h"

#define W(a, x, y) ((a)[(y) * WORLD_W + (x)])

/* Simple height interpolation for terrain */
static s8 heightmap[WORLD_W];
static u8 cave_map[WORLD_W * WORLD_H];

static void gen_heightmap(u32 seed) {
    rng_seed(seed);
    
    /* Generate control points every 8-16 columns */
    s8 heights[20];
    int num_pts = 0;
    heights[0] = 8 + (rng_next() % 4); /* Surface around y=8-11 */
    int x = 0;
    num_pts = 1;
    
    while (x < WORLD_W) {
        int step = 8 + (rng_next() % 9);
        x += step;
        if (x >= WORLD_W) x = WORLD_W - 1;
        heights[num_pts] = 6 + (rng_next() % 8); /* y=6-13 */
        num_pts++;
        if (num_pts >= 20) break;
    }
    heights[num_pts - 1] = 8 + (rng_next() % 4);
    
    /* Interpolate */
    int seg = 0;
    for (int i = 0; i < WORLD_W; i++) {
        while (seg < num_pts - 2 && i >= ((seg + 1) * WORLD_W / num_pts)) {
            seg++;
        }
        int seg_start = seg * WORLD_W / num_pts;
        int seg_end = (seg + 1) * WORLD_W / num_pts;
        int seg_len = seg_end - seg_start;
        if (seg_len < 1) seg_len = 1;
        int t = i - seg_start;
        int h0 = heights[seg];
        int h1 = heights[seg + 1];
        heightmap[i] = h0 + (s8)((h1 - h0) * t / seg_len);
    }
}

/* Cellular automata caves */
static void gen_caves(u32 seed) {
    rng_seed(seed + 999);
    
    /* Initial random fill for underground (y >= 10) */
    for (int y = 10; y < WORLD_H - 4; y++) {
        for (int x = 0; x < WORLD_W; x++) {
            cave_map[y * WORLD_W + x] = (rng_next() < 11000) ? 1 : 0;
            /* 11000/32768 ~ 33% fill */
        }
    }
    
    /* Run 4 iterations of cellular automata */
    for (int iter = 0; iter < 4; iter++) {
        u8 new_caves[WORLD_W * WORLD_H];
        for (int y = 10; y < WORLD_H - 4; y++) {
            for (int x = 0; x < WORLD_W; x++) {
                int neighbors = 0;
                for (int dy = -1; dy <= 1; dy++) {
                    for (int dx = -1; dx <= 1; dx++) {
                        if (dx == 0 && dy == 0) continue;
                        int nx = x + dx;
                        int ny = y + dy;
                        if (nx < 0 || nx >= WORLD_W || ny < 10 || ny >= WORLD_H - 4) {
                            neighbors++;
                        } else {
                            neighbors += cave_map[ny * WORLD_W + nx];
                        }
                    }
                }
                /* Rule: if neighbors >= 5, become wall (1); else cave (0) */
                new_caves[y * WORLD_W + x] = (neighbors >= 5) ? 1 : 0;
            }
        }
        for (int y = 10; y < WORLD_H - 4; y++) {
            for (int x = 0; x < WORLD_W; x++) {
                cave_map[y * WORLD_W + x] = new_caves[y * WORLD_W + x];
            }
        }
    }
}

/* Place ore veins */
static void place_ore(u8 world[], u8 ore_id, u8 min_y, u8 max_y, int vein_count, u32 seed) {
    rng_seed(seed);
    for (int v = 0; v < vein_count; v++) {
        int cx = rng_next() % WORLD_W;
        int cy = min_y + (rng_next() % (max_y - min_y + 1));
        int size = 3 + (rng_next() % 6); /* 3-8 ore per vein */
        for (int i = 0; i < size; i++) {
            int ox = cx + (rng_next() % 3) - 1;
            int oy = cy + (rng_next() % 3) - 1;
            if (ox >= 0 && ox < WORLD_W && oy >= min_y && oy <= max_y) {
                if (W(world, ox, oy) == BLOCK_STONE) {
                    W(world, ox, oy) = ore_id;
                }
            }
        }
    }
}

/* Place trees on surface */
static void place_trees(u8 world[]) {
    for (int x = 3; x < WORLD_W - 3; x++) {
        int surface_y = heightmap[x];
        if (W(world, x, surface_y) != BLOCK_GRASS) continue;
        if (rng_next() % 10 < 3) { /* 30% chance */
            /* Trunk: 3-5 blocks tall */
            int trunk_h = 3 + (rng_next() % 3);
            for (int ty = 1; ty <= trunk_h; ty++) {
                if (surface_y - ty >= 0) {
                    W(world, x, surface_y - ty) = BLOCK_WOOD;
                }
            }
            /* Leaves: 3x3 canopy at top */
            int top = surface_y - trunk_h;
            for (int dy = -2; dy <= 0; dy++) {
                for (int dx = -1; dx <= 1; dx++) {
                    int lx = x + dx;
                    int ly = top + dy;
                    if (lx >= 0 && lx < WORLD_W && ly >= 0 && W(world, lx, ly) == BLOCK_AIR) {
                        W(world, lx, ly) = BLOCK_LEAVES;
                    }
                }
            }
            /* Top leaf */
            if (top - 1 >= 0 && W(world, x, top - 1) == BLOCK_AIR) {
                W(world, x, top - 1) = BLOCK_LEAVES;
            }
            x += 3; /* Spacing */
        }
    }
}

void worldgen_generate(u8 world[WORLD_W * WORLD_H], u32 seed) {
    /* Step 1: Generate heightmap */
    gen_heightmap(seed);
    
    /* Step 2: Generate caves */
    gen_caves(seed);
    
    /* Step 3: Fill world */
    for (int x = 0; x < WORLD_W; x++) {
        int surface = heightmap[x];
        
        for (int y = 0; y < WORLD_H; y++) {
            if (y < surface) {
                /* Above surface: air */
                W(world, x, y) = BLOCK_AIR;
            } else if (y == surface) {
                /* Surface: grass */
                W(world, x, y) = BLOCK_GRASS;
            } else if (y < surface + 4) {
                /* Top soil: dirt */
                W(world, x, y) = BLOCK_DIRT;
            } else if (y >= WORLD_H - 2) {
                /* Bottom 2 rows: bedrock */
                W(world, x, y) = BLOCK_BEDROCK;
            } else if (y >= 10 && y < WORLD_H - 4 && cave_map[y * WORLD_W + x] == 0) {
                /* Cave */
                W(world, x, y) = BLOCK_AIR;
            } else if (y < 30) {
                /* Underground: stone */
                W(world, x, y) = BLOCK_STONE;
            } else if (y < 50) {
                /* Deep underground: stone */
                W(world, x, y) = BLOCK_STONE;
            } else {
                /* Near bedrock: stone */
                W(world, x, y) = BLOCK_STONE;
            }
        }
    }
    
    /* Step 4: Place ores */
    place_ore(world, BLOCK_COAL,    12, 50, 20, seed + 1);
    place_ore(world, BLOCK_COPPER,  10, 35, 15, seed + 2);
    place_ore(world, BLOCK_IRON,    15, 45, 12, seed + 3);
    place_ore(world, BLOCK_GOLD,    30, 55, 8,  seed + 4);
    place_ore(world, BLOCK_CRYSTAL, 40, 58, 5,  seed + 5);
    place_ore(world, BLOCK_CLAY,    8,  15, 8,  seed + 6);
    
    /* Step 5: Place trees */
    rng_seed(seed + 777);
    place_trees(world);
    
    /* Step 6: Place a few lava pools in deep underground */
    rng_seed(seed + 888);
    for (int i = 0; i < 5; i++) {
        int lx = rng_next() % WORLD_W;
        int ly = 45 + (rng_next() % 10);
        int size = 3 + (rng_next() % 4);
        for (int dy = 0; dy < 2; dy++) {
            for (int dx = 0; dx < size; dx++) {
                int px = lx + dx;
                int py = ly + dy;
                if (px < WORLD_W && py < WORLD_H - 2 && W(world, px, py) == BLOCK_AIR) {
                    W(world, px, py) = BLOCK_LAVA;
                }
            }
        }
    }
}