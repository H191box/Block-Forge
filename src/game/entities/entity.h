#ifndef ENTITY_H
#define ENTITY_H

#include "gba_header.h"
#include "gba_types.h"

#define MAX_ENTITIES 10

typedef enum {
    ENT_NONE = 0,
    ENT_SLIME,
    ENT_BAT
} EntityType;

typedef struct {
    EntityType type;
    s16 x, y;
    s16 vx, vy;
    u8 hp;
    u8 state;
    u8 timer;
    u8 drop_item;
    u8 drop_count;
    u8 on_ground;
} Entity;

extern Entity entities[MAX_ENTITIES];

void entity_init(void);
int entity_spawn(EntityType type, s16 x, s16 y);
void entity_update_all(void);
void entity_render_all(void);
void entity_damage(int id, u8 amount);

#endif