#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "gba_header.h"

typedef enum {
    STATE_TITLE,
    STATE_PLAYING,
    STATE_INVENTORY,
    STATE_CRAFTING,
    STATE_PAUSED,
    STATE_DEAD
} GameState;

extern GameState game_state;
extern s16 camera_x, camera_y;

/* Initialize game state machine */
void gamestate_init(void);

/* Update current state */
void gamestate_update(void);

/* Render current state */
void gamestate_render(void);

#endif