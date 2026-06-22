#include "player.h"
#include "physics.h"
#include "../../engine/input.h"
#include "../../engine/sprite_manager.h"
#include "../../engine/timer.h"
#include "../world/world.h"
#include "../world/block_data.h"
#include "inventory.h"

Player player;

void player_init(s16 spawn_x, s16 spawn_y) {
    player.x = spawn_x;
    player.y = spawn_y;
    player.vx = 0;
    player.vy = 0;
    player.facing = 0;
    player.anim_frame = 0;
    player.anim_timer = 0;
    player.hp = 10;
    player.max_hp = 10;
    player.hunger = 100;
    player.mine_speed = 1;
    player.on_ground = 0;
    player.jump_pressed = 0;
}

void player_update(void) {
    /* Movement input */
    player.vx = 0;
    if (keys_held & KEY_LEFT) {
        player.vx = -PLAYER_SPEED;
        player.facing = 1;
    }
    if (keys_held & KEY_RIGHT) {
        player.vx = PLAYER_SPEED;
        player.facing = 0;
    }

    /* Jump */
    if (keys_held & KEY_UP) {
        if (player.on_ground && !player.jump_pressed) {
            player.vy = -PLAYER_JUMP;
            player.on_ground = 0;
            player.jump_pressed = 1;
        }
    } else {
        player.jump_pressed = 0;
    }

    /* Gravity */
    player.vy += PLAYER_GRAVITY;
    if (player.vy > MAX_FALL_SPEED) player.vy = MAX_FALL_SPEED;

    /* Move X with collision */
    s16 new_x = player.x + player.vx;
    if (!physics_check_solid(new_x, player.y, PLAYER_W, PLAYER_H)) {
        player.x = new_x;
    } else {
        /* Try to step up 1 tile */
        if (!physics_check_solid(new_x, player.y - 8, PLAYER_W, PLAYER_H) &&
            player.on_ground && player.vy >= 0) {
            player.x = new_x;
            player.y -= 8;
        }
    }

    /* Move Y with collision */
    s16 new_y = player.y + player.vy;
    player.on_ground = 0;
    if (!physics_check_solid(player.x, new_y, PLAYER_W, PLAYER_H)) {
        player.y = new_y;
    } else {
        if (player.vy > 0) {
            player.y = ((new_y + PLAYER_H) / 8) * 8 - PLAYER_H;
            player.on_ground = 1;
        } else {
            player.y = (new_y / 8 + 1) * 8;
        }
        player.vy = 0;
    }

    /* Clamp to world bounds */
    if (player.x < 0) player.x = 0;
    if (player.x + PLAYER_W > WORLD_W * 8) player.x = WORLD_W * 8 - PLAYER_W;
    if (player.y < 0) player.y = 0;

    /* Fall into void */
    if (player.y > WORLD_H * 8) {
        player.hp = 0;
    }

    /* Hunger slowly decreases */
    if (game_tick % 300 == 0 && player.hunger > 0) {
        player.hunger--;
    }
    if (player.hunger == 0 && game_tick % 120 == 0) {
        player.hp--;
    }

    /* Animation */
    if (player.vx != 0) {
        player.anim_timer++;
        if (player.anim_timer >= 8) {
            player.anim_timer = 0;
            player.anim_frame ^= 1;
        }
    } else {
        player.anim_frame = 0;
        player.anim_timer = 0;
    }

    /* Check mining speed from held item */
    player.mine_speed = 1;
    Item held = hotbar_get_selected();
    if (held.type_id == ITEM_IRON_SWORD) player.mine_speed = 2;
    if (held.type_id == ITEM_GOLD_SWORD) player.mine_speed = 3;
}

void player_render(void) {
    int tile = player.anim_frame * 32;
    spr_set(SPR_PLAYER, player.x - 2, player.y, tile, 0, 1, 0x11);
    if (player.facing) spr_set_hflip(SPR_PLAYER, true);
}