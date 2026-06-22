#ifndef PARTICLE_SYSTEM_H
#define PARTICLE_SYSTEM_H

#include "gba_header.h"

#define MAX_PARTICLES 32

typedef struct {
    s16 x, y;
    s16 vx, vy;
    u16 color;
    u8 life;
    u8 max_life;
} Particle;

void particles_init(void);
void particles_spawn(s16 x, s16 y, u16 color, int count);
void particles_update(void);
void particles_render(void);

#endif