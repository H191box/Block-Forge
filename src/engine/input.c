#include "input.h"
#include "gba_header.h"

u16 keys_pressed = 0;
u16 keys_held = 0;
u16 keys_just_pressed = 0;
u16 keys_just_released = 0;

/* D-pad repeat: initial delay and repeat speed in frames */
#define REPEAT_DELAY  15
#define REPEAT_SPEED  5

static u16 repeat_timer[10]; /* Per-key repeat timer */
static u16 repeat_state[10]; /* 0=first, 1=waiting delay, 2=repeating */

void input_update(void) {
    u16 new_keys = ~REG_KEY & 0x3FF; /* Active low, only lower 10 bits */
    u16 changed = keys_held ^ new_keys;
    keys_just_pressed = changed & new_keys;
    keys_just_released = changed & keys_held;
    keys_held = new_keys;
    keys_pressed = keys_just_pressed;

    /* Update repeat state for all keys */
    for (int i = 0; i < 10; i++) {
        u16 bit = 1 << i;
        if (keys_just_pressed & bit) {
            repeat_timer[i] = 0;
            repeat_state[i] = 0;
        } else if (keys_held & bit) {
            repeat_timer[i]++;
        } else {
            repeat_timer[i] = 0;
            repeat_state[i] = 0;
        }
    }
}

u16 input_repeat(u16 key) {
    for (int i = 0; i < 10; i++) {
        u16 bit = 1 << i;
        if (key & bit) {
            if (repeat_state[i] == 0) {
                /* First press - trigger immediately */
                repeat_state[i] = 1;
                return bit;
            }
            if (repeat_state[i] == 1 && repeat_timer[i] >= REPEAT_DELAY) {
                /* Start repeating */
                repeat_state[i] = 2;
                repeat_timer[i] = 0;
                return bit;
            }
            if (repeat_state[i] == 2 && repeat_timer[i] >= REPEAT_SPEED) {
                /* Continue repeating */
                repeat_timer[i] = 0;
                return bit;
            }
            return 0;
        }
    }
    return 0;
}