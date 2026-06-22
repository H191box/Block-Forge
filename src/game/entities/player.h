#ifndef PLAYER_H
#define PLAYER_H

#include "gba_header.h"
#include "item.h"
#include "inventory.h"

/* Player state */
typedef struct {
    /* Position in pixel coordinates */
    s16 x, y;
    /* Velocity */
    s16 vx, vy;
    /* Facing: 0=right, 1=left */
    u8 facing;
    /* Animation */
    u8 anim_frame;
    u8 anim_timer;
    /* Stats */
    u8 hp;
    u8 max_hp;
    u8 hunger;
    /* Mining speed bonus (from tools) */
    u8 mine_speed;
    /* On ground flag */
    u8 on_ground;
    /* Jump buffer */
    u8 jump_pressed;
} Player;

extern Player player;

void player_init(s16 spawn_x, s16 spawn_y);
void player_update(void);
void player_render(void);

/* Player dimensions: 16x16 sprite, hitbox is 12x16 centered */
#define PLAYER_W 12
#define PLAYER_H 16

/* Movement constants */
#define PLAYER_SPEED     2
#define PLAYER_JUMP      7
#define PLAYER_GRAVITY   1
#define MAX_FALL_SPEED   6

#endif