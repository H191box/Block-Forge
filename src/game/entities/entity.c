#include "entity.h"
#include "player.h"
#include "physics.h"
#include "../../engine/sprite_manager.h"
#include "../../engine/timer.h"
#include "../world/world.h"
#include "../world/block_data.h"
#include "inventory.h"
#include "../../engine/particle_system.h"

Entity entities[MAX_ENTITIES];

void entity_init(void) {
    for (int i = 0; i < MAX_ENTITIES; i++) {
        entities[i].type = ENT_NONE;
        spr_hide(SPR_SLIME0 + i);
    }
}

int entity_spawn(EntityType type, s16 x, s16 y) {
    for (int i = 0; i < MAX_ENTITIES; i++) {
        if (entities[i].type == ENT_NONE) {
            entities[i].type = type;
            entities[i].x = x;
            entities[i].y = y;
            entities[i].vx = 0;
            entities[i].vy = 0;
            entities[i].hp = (type == ENT_SLIME) ? 3 : 1;
            entities[i].state = 0;
            entities[i].timer = 0;
            entities[i].drop_item = (type == ENT_SLIME) ? ITEM_GEL : ITEM_NONE;
            entities[i].drop_count = (type == ENT_SLIME) ? 2 : 0;
            entities[i].on_ground = 0;
            return i;
        }
    }
    return -1;
}

void entity_update_all(void) {
    for (int i = 0; i < MAX_ENTITIES; i++) {
        if (entities[i].type == ENT_NONE) continue;

        Entity* e = &entities[i];

        if (e->type == ENT_SLIME) {
            e->timer++;
            if (e->timer >= 40) {
                e->timer = 0;
                e->vx = (rng_next() % 3) - 1;
                if (e->vx != 0) e->vx *= 2;
                if (e->on_ground) e->vy = -4;
            }
        } else if (e->type == ENT_BAT) {
            e->timer++;
            e->vx = (e->timer & 1) ? 1 : -1;
            e->vy = ((e->timer % 30) < 15) ? -1 : 1;
        }

        /* Gravity */
        e->vy += 1;
        if (e->vy > 4) e->vy = 4;

        /* Move X */
        s16 nx = e->x + e->vx;
        if (!physics_check_solid(nx, e->y, 16, 16)) {
            e->x = nx;
        } else {
            e->vx = -e->vx;
        }

        /* Move Y */
        s16 ny = e->y + e->vy;
        e->on_ground = 0;
        if (!physics_check_solid(e->x, ny, 16, 16)) {
            e->y = ny;
        } else {
            if (e->vy > 0) {
                e->y = ((ny + 16) / 8) * 8 - 16;
                e->on_ground = 1;
            }
            e->vy = 0;
        }

        /* Despawn if too far from player */
        int dx = ABS(e->x - player.x);
        int dy = ABS(e->y - player.y);
        if (dx > 30 * 8 || dy > 20 * 8 || e->y > WORLD_H * 8) {
            e->type = ENT_NONE;
        }

        /* Damage player on contact */
        if (e->x < player.x + PLAYER_W && e->x + 16 > player.x &&
            e->y < player.y + PLAYER_H && e->y + 16 > player.y) {
            if (game_tick % 30 == 0) {
                player.hp--;
            }
        }
    }
}

void entity_render_all(void) {
    for (int i = 0; i < MAX_ENTITIES; i++) {
        if (entities[i].type == ENT_NONE) {
            spr_hide(SPR_SLIME0 + i);
            continue;
        }
        Entity* e = &entities[i];
        if (e->type == ENT_SLIME) {
            int frame = (e->timer < 5) ? 1 : 0;
            spr_set(SPR_SLIME0 + i, e->x, e->y, 64 + frame * 32, 1, 1, 0x11);
        } else {
            int frame = (game_tick / 8) & 1;
            spr_set(SPR_SLIME0 + i, e->x, e->y, 128 + frame * 32, 2, 1, 0x11);
        }
    }
}

void entity_damage(int id, u8 amount) {
    if (id < 0 || id >= MAX_ENTITIES) return;
    Entity* e = &entities[id];
    if (e->type == ENT_NONE) return;
    
    e->hp -= amount;
    if (e->hp <= 0) {
        if (e->drop_item != ITEM_NONE) {
            inventory_add(e->drop_item, e->drop_count);
        }
        particles_spawn(e->x + 8, e->y + 8, RGB555(20, 20, 20), 6);
        e->type = ENT_NONE;
    }
}