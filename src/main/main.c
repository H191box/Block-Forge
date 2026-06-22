#include "block_forge.h"

/* Note: _start is in src/main/crt0.s (ARM mode startup code) */

int main(void) {
    /* Initialize hardware */
    system_init();
    input_update();
    timer_init();

    /* Initialize assets */
    tiles_init();
    sprites_init();
    sprites_init_palettes();
    palettes_init_ui();
    text_init();

    /* Initialize tilemap screenblocks */
    tilemap_init();

    /* Initialize game (tries load, otherwise generates) */
    gamestate_init();

    /* Main game loop */
    while (1) {
        /* Update */
        gamestate_update();

        /* Render */
        gamestate_render();

        /* Apply day/night palette effects */
        if (game_state == STATE_PLAYING ||
            game_state == STATE_INVENTORY ||
            game_state == STATE_CRAFTING) {
            palettes_apply_brightness(daynight_get_brightness());
        }

        /* Wait for VBlank */
        sys_wait_vblank();
    }

    return 0;
}