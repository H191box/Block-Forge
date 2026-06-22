#include "day_night.h"
#include "timer.h"

static u32 dn_counter = 0;

void daynight_init(void) {
    dn_counter = 0;
}

void daynight_update(void) {
    dn_counter++;
    if (dn_counter >= DN_FULL_CYCLE) {
        dn_counter -= DN_FULL_CYCLE;
    }
}

DayNightPhase daynight_get_phase(void) {
    u32 t = dn_counter;
    if (t < DN_DAY_LENGTH) return DN_DAY;
    t -= DN_DAY_LENGTH;
    if (t < DN_SUNSET_LENGTH) return DN_SUNSET;
    t -= DN_SUNSET_LENGTH;
    if (t < DN_NIGHT_LENGTH) return DN_NIGHT;
    return DN_SUNRISE;
}

u8 daynight_get_brightness(void) {
    u32 t = dn_counter;
    
    if (t < DN_DAY_LENGTH) {
        return 100; /* Full brightness */
    }
    t -= DN_DAY_LENGTH;
    
    if (t < DN_SUNSET_LENGTH) {
        /* Fade from 100 to 35 */
        return 100 - (65 * t / DN_SUNSET_LENGTH);
    }
    t -= DN_SUNSET_LENGTH;
    
    if (t < DN_NIGHT_LENGTH) {
        return 35; /* Dark */
    }
    t -= DN_NIGHT_LENGTH;
    
    /* Sunrise: 35 to 100 */
    return 35 + (65 * t / DN_SUNRISE_LENGTH);
}