#ifndef BLOCK_FORGE_H
#define BLOCK_FORGE_H

/* Block Forge - GBA 2D Sandbox Construction Game
   Task ID: 7
   Version: 1.0 */

/* Game Constants */
#define SCREEN_W  240
#define SCREEN_H  160
#define WORLD_W   128
#define WORLD_H   64
#define TILE_SIZE 8

/* Engine includes */
#include "engine/gba_header.h"
#include "engine/gba_types.h"
#include "engine/system.h"
#include "engine/input.h"
#include "engine/dma.h"
#include "engine/timer.h"
#include "engine/tilemap.h"
#include "engine/sprite_manager.h"
#include "engine/text_renderer.h"
#include "engine/particle_system.h"

/* Game includes */
#include "game/world/block_data.h"
#include "game/world/world.h"
#include "game/world/world_gen.h"
#include "game/world/day_night.h"
#include "game/entities/item.h"
#include "game/entities/inventory.h"
#include "game/entities/hotbar.h"
#include "game/entities/player.h"
#include "game/entities/entity.h"
#include "game/crafting/recipes.h"
#include "game/crafting/crafting.h"
#include "game/mining.h"
#include "game/physics.h"
#include "game/hud.h"
#include "game/save.h"
#include "game/game_state.h"

/* Asset includes */
#include "assets/font.h"
#include "assets/tiles.h"
#include "assets/sprites.h"
#include "assets/palettes.h"

#endif /* BLOCK_FORGE_H */