#include "palettes.h"
#include "../engine/gba_header.h"

void palettes_init_ui(void) {
    /* BG_PALETTE[0] = transparent (color 0 of sub-palette 0) */
    /* BG_PALETTE[1] = sky color (set dynamically by tilemap_render_sky)
       Also used by BG3 text as "white" text color.
       We set it to white here as default; tilemap_render_sky overwrites it.
       Since BG3 (UI, priority 0) renders ON TOP of BG1 (sky, priority 3),
       text areas will show the UI color. But since they share the same
       palette entry, we need a compromise.
       
       Solution: BG3 text uses color index 1 = same as sky.
       During day: sky is light blue, text is light blue (readable on dark BG).
       During night: sky is dark blue, text is dark blue (hard to read!).
       
       Better: Use different color indices for text vs sky.
       BG1 sky tile uses color index 1.
       BG3 text tiles use color index 1 too (same sub-palette).
       
       We can't separate them with shared palettes.
       WORKAROUND: BG3 text areas are filled with solid tiles (color 1),
       which covers the sky behind. So text appears on a colored background.
       If the text itself is color 0 (transparent), it shows the solid background.
       No, text needs to be opaque.
       
       REAL WORKAROUND: Text uses color 1 for the glyph pixels.
       The solid tile fill behind text uses color 2.
       So we need:
       - Color 1 = text foreground (e.g., white)
       - Color 2 = text background (e.g., dark)
       - BG1 sky = color 1 (shared with text foreground)
       
       At night, color 1 gets darker (sky dims), so text dims too.
       The text background (color 2) stays dark, providing contrast.
       
       Actually, let's just not dim color 1 for the sky at night.
       The sky will always be the same shade (day sky blue).
       Night effect: just darken block palettes instead.
       This avoids all the palette conflict issues.
    */
    
    /* Sub-palette 0 colors (shared by BG1 sky and BG3 UI text):
       Color 0 = transparent
       Color 1 = white/light (sky + text)
       Color 2 = dark red (selected, damage)  
       Color 3 = green (health, craftable)
       Color 4 = dark blue (item slots)
       Color 5 = dark (text background panels)
    */
    BG_PALETTE[0] = 0x0000; /* transparent */
    BG_PALETTE[1] = RGB555(31, 31, 31); /* white - sky & text */
    BG_PALETTE[2] = RGB555(31, 5, 5);   /* red */
    BG_PALETTE[3] = RGB555(5, 31, 5);   /* green */
    BG_PALETTE[4] = RGB555(5, 8, 20);   /* dark blue */
    BG_PALETTE[5] = RGB555(8, 8, 8);    /* dark gray */
}

void palettes_apply_brightness(u8 brightness) {
    /* For night effect: darken sky color (sub-palette 0, color 1) */
    /* During day (brightness=100): white sky -> light blue
       During night (brightness=35): gray sky */
    int r = CLAMP(20 * brightness / 100 + 5, 0, 31);
    int g = CLAMP(24 * brightness / 100 + 5, 0, 31);
    int b = CLAMP(28 * brightness / 100 + 3, 0, 31);
    BG_PALETTE[1] = RGB555(r, g, b);
    
    /* Also dim the UI text slightly at night */
    /* Color 2 (red) dims */
    if (brightness < 80) {
        int factor = brightness * 31 / 100;
        BG_PALETTE[2] = RGB555(factor, factor * 5 / 100, factor * 5 / 100);
    }
}