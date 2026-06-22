#ifndef WORLD_H
#define WORLD_H

#include "gba_header.h"

#define WORLD_W 128
#define WORLD_H 64

/* Get tile at world position */
u8 world_get_tile(int x, int y);

/* Set tile at world position */
void world_set_tile(int x, int y, u8 id);

/* Initialize world (allocate/zero) */
void world_init(void);

/* Generate world procedurally */
void world_generate(u32 seed);

/* Save world to SRAM */
void world_save(void);

/* Load world from SRAM, returns 1 if valid save found */
int world_load(void);

/* Apply gravity to blocks (sand, gravel) */
void world_update_gravity(void);

/* Expose world array for tilemap rendering */
extern u8 world_data[WORLD_W * WORLD_H];

#endif