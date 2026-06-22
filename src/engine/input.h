#ifndef INPUT_H
#define INPUT_H

#include "gba_header.h"

/* Key state access */
extern u16 keys_pressed;
extern u16 keys_held;
extern u16 keys_just_pressed;
extern u16 keys_just_released;

/* Poll keypad, call once per frame */
void input_update(void);

/* D-pad repeat for menus (hold delay then repeat) */
u16 input_repeat(u16 key);

#endif /* INPUT_H */