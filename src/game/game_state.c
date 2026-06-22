#include "game_state.h"
#include "../engine/input.h"
#include "../engine/tilemap.h"
#include "../engine/sprite_manager.h"
#include "../engine/text_renderer.h"
#include "../engine/particle_system.h"
#include "../engine/timer.h"
#include "../engine/system.h"
#include "../engine/gba_types.h"
#include "../game/world/world.h"
#include "../game/world/world_gen.h"
#include "../game/world/block_data.h"
#include "../game/world/day_night.h"
#include "../game/entities/player.h"
#include "../game/entities/entity.h"
#include "../game/entities/inventory.h"
#include "../game/entities/hotbar.h"
#include "../game/mining.h"
#include "../game/hud.h"
#include "../game/save.h"
#include "../game/crafting/recipes.h"
#include "../game/crafting/crafting.h"

GameState game_state = STATE_TITLE;
s16 camera_x = 0, camera_y = 0;

/* Inventory/crafting cursor */
static int inv_cursor = 0;
static int craft_cursor = 0;

/* Entity spawn timer */
static u32 entity_timer = 0;

/* World seed */
static u32 world_seed = 12345;

/* Gravity update timer */
static u32 gravity_timer = 0;

/* Title blink */
static u16 title_blink = 0;

void gamestate_init(void) {
    /* Try to load save */
    if (save_read()) {
        game_state = STATE_PLAYING;
        return;
    }
    
    /* New game: generate world */
    world_init();
    world_generate(world_seed);
    
    /* Find spawn point (surface, near center) */
    int spawn_x = WORLD_W / 2;
    int spawn_y = 0;
    for (int y = 0; y < WORLD_H; y++) {
        if (world_get_tile(spawn_x, y) != BLOCK_AIR) {
            spawn_y = (y - 2) * 8;
            break;
        }
    }
    
    /* Clear spawn area */
    for (int dy = -2; dy <= 3; dy++) {
        for (int dx = -1; dx <= 1; dx++) {
            int wx = spawn_x + dx;
            int wy = spawn_y / 8 + dy;
            if (wy >= 0 && wy < WORLD_H && world_get_tile(wx, wy) != BLOCK_BEDROCK) {
                world_set_tile(wx, wy, BLOCK_AIR);
            }
        }
    }
    
    /* Init player */
    player_init(spawn_x * 8, spawn_y);
    inventory_set(0, item_create(ITEM_TORCH, 10));
    inventory_set(1, item_create(ITEM_WOOD_PLANK, 20));
    
    /* Init entities */
    entity_init();
    particles_init();
    
    /* Init day/night */
    daynight_init();
    
    game_state = STATE_PLAYING;
}

static void update_playing(void) {
    /* Player movement */
    player_update();
    
    /* Camera follows player */
    s16 target_cx = player.x - 120 + PLAYER_W / 2;
    s16 target_cy = player.y - 80 + PLAYER_H / 2;
    camera_x += (target_cx - camera_x) / 4;
    camera_y += (target_cy - camera_y) / 4;
    
    /* Clamp camera */
    if (camera_x < 0) camera_x = 0;
    if (camera_x > WORLD_W * 8 - 240) camera_x = WORLD_W * 8 - 240;
    if (camera_y < 0) camera_y = 0;
    if (camera_y > WORLD_H * 8 - 160) camera_y = WORLD_H * 8 - 160;
    
    /* Hotbar cycling */
    if (keys_just_pressed & KEY_L) hotbar_cycle_left();
    if (keys_just_pressed & KEY_R) hotbar_cycle_right();
    
    /* Mining */
    if (keys_held & KEY_A) {
        if (!mining.active) {
            int tx, ty;
            mining_get_target(&tx, &ty);
            if (mining_in_range(tx, ty)) {
                mining_start(tx, ty);
            }
        }
        mining_update();
    } else {
        if (mining.active) mining_cancel();
    }
    
    /* Placing blocks */
    if (keys_just_pressed & KEY_B) {
        Item held = hotbar_get_selected();
        if (held.count > 0 && held.type_id < BLOCK_COUNT) {
            /* Can only place blocks (items < 26) */
            int tx, ty;
            mining_get_target(&tx, &ty);
            if (mining_in_range(tx, ty) && world_get_tile(tx, ty) == BLOCK_AIR) {
                /* Don't place on player */
                int bx1 = tx * 8, by1 = ty * 8;
                if (!(bx1 < player.x + PLAYER_W && bx1 + 8 > player.x &&
                      by1 < player.y + PLAYER_H && by1 + 8 > player.y)) {
                    world_set_tile(tx, ty, held.type_id);
                    inventory_remove(hotbar_selection, 1);
                }
            }
        }
    }
    
    /* Open inventory */
    if (keys_just_pressed & KEY_START) {
        game_state = STATE_INVENTORY;
        save_write();
        inv_cursor = 0;
        return;
    }
    
    /* Open crafting */
    if (keys_just_pressed & KEY_SELECT) {
        game_state = STATE_CRAFTING;
        craft_cursor = 0;
        return;
    }
    
    /* Attack nearby entities with A (non-mining) */
    if (keys_just_pressed & KEY_A && mining.active == 0) {
        int px = player.x + (player.facing ? -16 : PLAYER_W);
        int py = player.y;
        for (int i = 0; i < MAX_ENTITIES; i++) {
            if (entities[i].type == ENT_NONE) continue;
            if (entities[i].x < px + 16 && entities[i].x + 16 > px &&
                entities[i].y < py + 16 && entities[i].y + 16 > py) {
                entity_damage(i, 1);
            }
        }
    }
    
    /* Update entities */
    entity_update_all();
    
    /* Update particles */
    particles_update();
    
    /* Update day/night */
    daynight_update();
    
    /* Gravity (every 8 frames) */
    gravity_timer++;
    if (gravity_timer >= 8) {
        gravity_timer = 0;
        world_update_gravity();
    }
    
    /* Entity spawning (night only, every 300 frames) */
    entity_timer++;
    if (daynight_get_phase() == DN_NIGHT && entity_timer >= 300) {
        entity_timer = 0;
        if (rng_next() % 3 == 0) {
            int sx = camera_x / 8 + (rng_next() % 25) + 2;
            int sy = 5 + (rng_next() % 5);
            entity_spawn(ENT_SLIME, sx * 8, sy * 8);
        }
        if (rng_next() % 4 == 0) {
            int sx = camera_x / 8 + (rng_next() % 25) + 2;
            int sy = 15 + (rng_next() % 10);
            entity_spawn(ENT_BAT, sx * 8, sy * 8);
        }
    }
    
    /* Check death */
    if (player.hp <= 0) {
        game_state = STATE_DEAD;
    }
}

static void update_inventory(void) {
    if (keys_just_pressed & KEY_START) {
        game_state = STATE_PLAYING;
        return;
    }
    
    /* Navigate cursor with D-pad */
    if (keys_just_pressed & KEY_RIGHT) inv_cursor = (inv_cursor + 1) % INV_SLOTS;
    if (keys_just_pressed & KEY_LEFT) inv_cursor = (inv_cursor - 1 + INV_SLOTS) % INV_SLOTS;
    if (keys_just_pressed & KEY_DOWN) inv_cursor = (inv_cursor + 8) % INV_SLOTS;
    if (keys_just_pressed & KEY_UP) inv_cursor = (inv_cursor - 8 + INV_SLOTS) % INV_SLOTS;
    
    /* Move items with A */
    if (keys_just_pressed & KEY_A) {
        /* Simple: just select/deselect for now */
    }
}

static void update_crafting(void) {
    if (keys_just_pressed & KEY_B) {
        game_state = STATE_PLAYING;
        return;
    }
    
    int count = recipes_get_count();
    
    if (keys_just_pressed & KEY_DOWN) craft_cursor = (craft_cursor + 1) % count;
    if (keys_just_pressed & KEY_UP) craft_cursor = (craft_cursor - 1 + count) % count;
    
    if (keys_just_pressed & KEY_A) {
        const Recipe* recipes = recipes_get_all();
        const Recipe* r = &recipes[craft_cursor];
        if (crafting_can_craft(r) && crafting_station_available(r)) {
            crafting_execute(r);
        }
    }
}

static void update_dead(void) {
    if (keys_just_pressed & KEY_START) {
        /* Respawn */
        int spawn_x = WORLD_W / 2;
        int spawn_y = 0;
        for (int y = 0; y < WORLD_H; y++) {
            if (world_get_tile(spawn_x, y) != BLOCK_AIR) {
                spawn_y = (y - 2) * 8;
                break;
            }
        }
        player.hp = player.max_hp;
        player.hunger = 100;
        player.x = spawn_x * 8;
        player.y = spawn_y;
        player.vy = 0;
        /* Lose half inventory */
        for (int i = 0; i < INV_SLOTS; i++) {
            Item it = inventory_get(i);
            if (it.count > 1) {
                it.count /= 2;
                inventory_set(i, it);
            }
        }
        game_state = STATE_PLAYING;
    }
}

void gamestate_update(void) {
    game_tick++;
    input_update();
    
    switch (game_state) {
        case STATE_TITLE:
            title_blink++;
            if (keys_just_pressed & KEY_A) {
                gamestate_init();
            }
            break;
        case STATE_PLAYING:
            update_playing();
            break;
        case STATE_INVENTORY:
            update_inventory();
            break;
        case STATE_CRAFTING:
            update_crafting();
            break;
        case STATE_PAUSED:
            if (keys_just_pressed & KEY_START) game_state = STATE_PLAYING;
            break;
        case STATE_DEAD:
            update_dead();
            break;
    }
}

static void render_title(void) {
    text_clear();
    text_draw_centered("BLOCK FORGE", 0, 48, 240, 0);
    text_draw_centered("A 2D Sandbox", 0, 64, 240, 0);
    
    if ((title_blink / 30) % 2 == 0) {
        text_draw_centered("Press A to Start", 0, 110, 240, 0);
    }
    text_draw_centered("v1.0", 0, 140, 240, 0);
}

static void render_playing(void) {
    /* Render sky */
    u8 brightness = daynight_get_brightness();
    tilemap_render_sky(brightness);
    
    /* Render world tiles */
    tilemap_render(camera_x, camera_y);
    
    /* Render entities */
    entity_render_all();
    
    /* Render player */
    player_render();
    
    /* Render particles */
    particles_render();
    
    /* Render HUD on BG3 */
    text_clear();
    hud_render();
    
    /* Update sprites to OAM */
    spr_update();
}

void gamestate_render(void) {
    switch (game_state) {
        case STATE_TITLE:
            render_title();
            spr_update();
            break;
        case STATE_PLAYING:
            render_playing();
            break;
        case STATE_INVENTORY:
            render_playing(); /* render world behind */
            text_clear();
            hud_render_inventory();
            spr_update();
            break;
        case STATE_CRAFTING:
            render_playing();
            text_clear();
            hud_render_crafting(craft_cursor);
            spr_update();
            break;
        case STATE_PAUSED:
            text_clear();
            text_draw_centered("PAUSED", 0, 72, 240, 0);
            text_draw_centered("Press START", 0, 88, 240, 0);
            spr_update();
            break;
        case STATE_DEAD:
            text_clear();
            text_draw_centered("YOU DIED", 0, 64, 240, 0);
            text_draw_centered("Press START to Respawn", 0, 96, 240, 0);
            spr_update();
            break;
    }
}