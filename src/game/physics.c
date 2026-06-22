#include "physics.h"
#include "game/world/world.h"
#include "game/world/block_data.h"

int physics_check_solid(s16 x, s16 y, u8 w, u8 h) {
    int tiles_x1 = x / 8;
    int tiles_y1 = y / 8;
    int tiles_x2 = (x + w - 1) / 8;
    int tiles_y2 = (y + h - 1) / 8;
    
    for (int ty = tiles_y1; ty <= tiles_y2; ty++) {
        for (int tx = tiles_x1; tx <= tiles_x2; tx++) {
            u8 block = world_get_tile(tx, ty);
            if (block_is_solid(block)) return 1;
        }
    }
    return 0;
}