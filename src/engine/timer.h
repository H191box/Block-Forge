#ifndef TIMER_H
#define TIMER_H

#include "gba_header.h"

/* Game tick: increments every frame via software counter */
extern u32 game_tick;

/* Initialize timer 0 for general timing */
void timer_init(void);

/* Simple frame-based timer helper */
typedef struct {
    u32 start_tick;
    u32 duration;
} Timer;

static inline void timer_start(Timer* t, u32 duration) {
    t->start_tick = game_tick;
    t->duration = duration;
}

static inline u32 timer_elapsed(Timer* t) {
    return game_tick - t->start_tick;
}

static inline bool timer_done(Timer* t) {
    return timer_elapsed(t) >= t->duration;
}

static inline u32 timer_remaining(Timer* t) {
    u32 e = timer_elapsed(t);
    return (e >= t->duration) ? 0 : t->duration - e;
}

static inline float timer_progress(Timer* t) {
    if (t->duration == 0) return 1.0f;
    float p = (float)timer_elapsed(t) / (float)t->duration;
    return p > 1.0f ? 1.0f : p;
}

#endif /* TIMER_H */