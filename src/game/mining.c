#include "mining.h"
#include "../world/world.h"
#include "../entities/inventory.h"
#include "../entities/player.h"
#include "../engine/input.h"
#include "../engine/sprite_manager.h"
#include "../engine/particle_system.h"
#include "../engine/timer.h"

MiningState mining = {0};

#define MINE_RANGE 2

void mining_get_target(int* tx, int* ty) {
    int px = (player.x + PLAYER_W / 2) / 8;
    int py = (player.y + PLAYER_H / 2) / 8;
    
    int dx = player.facing ? -1 : 1;
    *tx = px + dx;
    *ty = py;
    
    if (keys_held & KEY_UP) {
        *ty = py - 1;
        *tx = px;
    } else if (keys_held & KEY_DOWN) {
        *ty = py + 1;
        *tx = px;
    }
}

int mining_in_range(int tx, int ty) {
    int px = (player.x + PLAYER_W / 2) / 8;
    int py = (player.y + PLAYER_H / 2) / 8;
    int dx = ABS(tx - px);
    int dy = ABS(ty - py);
    return dx <= MINE_RANGE && dy <= MINE_RANGE;
}

void mining_start(int tx, int ty) {
    u8 block = world_get_tile(tx, ty);
    if (block == BLOCK_AIR || block >= BLOCK_COUNT) return;
    
    const BlockType* bt = block_get(block);
    if (bt->hardness >= 10) return;
    if (bt->hardness == 0) return;
    
    mining.active = 1;
    mining.tile_x = tx;
    mining.tile_y = ty;
    mining.crack_stage = 0;
    mining.progress = 0;
    mining.threshold = (bt->hardness * 15) / player.mine_speed;
}

void mining_cancel(void) {
    mining.active = 0;
    mining.crack_stage = 0;
    for (int i = 0; i < 3; i++) {
        spr_hide(SPR_CRACK0 + i);
    }
}

void mining_update(void) {
    if (!mining.active) return;
    
    if (!(keys_held & KEY_A)) {
        mining_cancel();
        return;
    }
    
    int tx, ty;
    mining_get_target(&tx, &ty);
    if (tx != mining.tile_x || ty != mining.tile_y) {
        mining_cancel();
        return;
    }
    
    mining.progress++;
    
    u8 new_stage = MIN(3, (u8)((mining.progress * 4) / mining.threshold));
    if (new_stage != mining.crack_stage) {
        mining.crack_stage = new_stage;
    }
    
    /* Show crack overlay sprite */
    if (mining.crack_stage > 0) {
        int cam_x = player.x - 120 + PLAYER_W / 2;
        int cam_y = player.y - 80 + PLAYER_H / 2;
        s16 sx = mining.tile_x * 8 - cam_x;
        s16 sy = mining.tile_y * 8 - cam_y;
        int spr_id = SPR_CRACK0 + mining.crack_stage - 1;
        spr_set(spr_id, sx, sy, 192 + (mining.crack_stage - 1) * 16, 3, 0, 0x00);
    }
    
    /* Mining complete */
    if (mining.progress >= mining.threshold) {
        u8 block = world_get_tile(mining.tile_x, mining.tile_y);
        const BlockType* bt = block_get(block);
        
        u8 drop = bt->drop_item ? bt->drop_item : block;
        if (drop != ITEM_NONE) {
            inventory_add(drop, 1);
        }
        
        int cam_x = player.x - 120 + PLAYER_W / 2;
        int cam_y = player.y - 80 + PLAYER_H / 2;
        particles_spawn(mining.tile_x * 8 + 4 - cam_x, mining.tile_y * 8 + 4 - cam_y, bt->color, 6);
        
        world_set_tile(mining.tile_x, mining.tile_y, BLOCK_AIR);
        
        mining_cancel();
    }
}