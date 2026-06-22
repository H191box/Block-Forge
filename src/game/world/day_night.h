#ifndef DAY_NIGHT_H
#define DAY_NIGHT_H

#include "gba_header.h"

/* Day/night phases */
typedef enum {
    DN_DAY,
    DN_SUNSET,
    DN_NIGHT,
    DN_SUNRISE
} DayNightPhase;

/* Day = 3000 frames, Night = 2000 frames */
#define DN_DAY_LENGTH    3000
#define DN_SUNSET_LENGTH 300
#define DN_NIGHT_LENGTH  2000
#define DN_SUNRISE_LENGTH 300
#define DN_FULL_CYCLE    (DN_DAY_LENGTH + DN_SUNSET_LENGTH + DN_NIGHT_LENGTH + DN_SUNRISE_LENGTH)

void daynight_init(void);
void daynight_update(void);
DayNightPhase daynight_get_phase(void);
u8 daynight_get_brightness(void);

#endif