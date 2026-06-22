#include "particle_system.h"
#include "sprite_manager.h"
#include "gba_types.h"
#include "timer.h"

Particle particles[MAX_PARTICLES];
static u16 particle_tile; /* small 8x8 tile in OBJ VRAM for particle */

void particles_init(void) {
    for (int i = 0; i < MAX_PARTICLES; i++) {
        particles[i].life = 0;
        spr_hide(SPR_PARTICLE_BASE + i);
    }

    /* Create a 4x4 particle sprite tile in OBJ VRAM at tile 512 */
    particle_tile = 512;
    /* 4bpp 16x16 sprite occupies 8 tiles (2x2), but 8x8 = 2 tiles */
    /* For 8x8 sprite in 1D mapping: tile occupies 2 halfwords * 8 rows = 16 halfwords */
    u16* obj_tile = (u16*)(0x06010000 + particle_tile * 32);
    /* 4x4 particle centered in 8x8 tile: rows 2-5, cols 2-5 */
    for (int r = 0; r < 8; r++) {
        u16 val = 0;
        if (r >= 2 && r <= 5) val = 0x0010; /* pixel at col 2 */
        obj_tile[r * 2 + 0] = val;
        obj_tile[r * 2 + 1] = val;
    }
}

void particles_spawn(s16 x, s16 y, u16 color, int count) {
    int spawned = 0;
    for (int i = 0; i < MAX_PARTICLES && spawned < count; i++) {
        if (particles[i].life == 0) {
            particles[i].x = x + (rng_next() % 8) - 4;
            particles[i].y = y + (rng_next() % 8) - 4;
            particles[i].vx = (rng_next() % 5) - 2;
            particles[i].vy = -(rng_next() % 3) - 1;
            particles[i].color = color;
            particles[i].life = 20 + (rng_next() % 15);
            particles[i].max_life = particles[i].life;
            spawned++;
        }
    }
}

void particles_update(void) {
    for (int i = 0; i < MAX_PARTICLES; i++) {
        if (particles[i].life > 0) {
            particles[i].vy += 1; /* gravity */
            particles[i].x += particles[i].vx;
            particles[i].y += particles[i].vy;
            if (particles[i].y > 160) particles[i].life = 0;
            particles[i].life--;
        }
    }
}

void particles_render(void) {
    for (int i = 0; i < MAX_PARTICLES; i++) {
        int id = SPR_PARTICLE_BASE + i;
        if (particles[i].life > 0) {
            /* Use palette based on particle color */
            u8 pal = (i & 3) + 4; /* palettes 4-7 for particle colors */
            /* Set palette color 1 to particle color */
            OBJ_PALETTE[pal * 16 + 1] = particles[i].color;
            /* Fade: make semi-transparent by dimming */
            u8 alpha = particles[i].life * 100 / particles[i].max_life;
            if (alpha < 30) {
                spr_hide(id);
            } else {
                spr_set(id, particles[i].x, particles[i].y, particle_tile, pal, 1, 0x00);
            }
        } else {
            spr_hide(id);
        }
    }
}