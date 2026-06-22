#include "gba_types.h"

u32 rng_seed_val = 12345;

void rng_seed(u32 s) {
    rng_seed_val = s;
}

u32 rng_next(void) {
    rng_seed_val = rng_seed_val * 1103515245 + 12345;
    return (rng_seed_val >> 16) & 0x7FFF;
}