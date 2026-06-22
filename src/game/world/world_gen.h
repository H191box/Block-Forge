#ifndef WORLD_GEN_H
#define WORLD_GEN_H

#include "gba_header.h"
#include "block_data.h"

#define WORLD_W 128
#define WORLD_H 64

/* Generate terrain into the given world array */
void worldgen_generate(u8 world[WORLD_W * WORLD_H], u32 seed);

#endif