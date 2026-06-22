#ifndef MINING_H
#define MINING_H

#include "gba_header.h"
#include "gba_types.h"
#include "../entities/player.h"
#include "../world/block_data.h"

typedef struct {
    u8 active;
    int tile_x, tile_y;
    u8 crack_stage;    /* 0-3 */
    u16 progress;      /* frames elapsed */
    u16 threshold;     /* frames needed */
} MiningState;

extern MiningState mining;

/* Start mining a tile */
void mining_start(int tx, int ty);

/* Cancel mining */
void mining_cancel(void);

/* Update mining (call when A held) */
void mining_update(void);

/* Check if a tile is within mining range of the player */
int mining_in_range(int tx, int ty);

/* Get the tile the player is facing */
void mining_get_target(int* tx, int* ty);

#endif