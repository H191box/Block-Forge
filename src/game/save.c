#include "save.h"
#include "gba_header.h"
#include "gba_types.h"
#include "../world/world.h"
#include "../entities/player.h"
#include "../entities/inventory.h"
#include "../world/day_night.h"

/* Forward declarations from other modules */
extern Item inventory[];

typedef struct {
    u32 magic;
    u16 checksum;
    /* World data: 128*64 = 8192 bytes */
    /* Player: x, y, hp, max_hp, hunger, hotbar_selection */
    /* Inventory: 32 slots * 2 bytes = 64 bytes */
    /* Day/night counter: 4 bytes */
} SaveHeader;

/* SRAM layout:
   0x000: SaveHeader (8 bytes)
   0x008: world_data (8192 bytes)
   0x2008: player data (16 bytes)
   0x2018: inventory data (64 bytes)
   0x2058: daynight counter (4 bytes)
   0x205C: checksum stored in header
   Total: ~8.4KB, well within 64KB SRAM
*/

static u16 save_calc_checksum(void) {
    u16 sum = 0;
    u8* ptr;
    
    /* World data */
    ptr = (u8*)world_data;
    for (int i = 0; i < WORLD_W * WORLD_H; i++) {
        sum += ptr[i];
    }
    
    /* Player data */
    ptr = (u8*)&player;
    for (int i = 0; i < (int)sizeof(Player); i++) {
        sum += ptr[i];
    }
    
    /* Inventory */
    ptr = (u8*)inventory;
    for (int i = 0; i < INV_SLOTS * (int)sizeof(Item); i++) {
        sum += ptr[i];
    }
    
    return sum;
}

void save_write(void) {
    /* Wait for any pending operations */
    for (int i = 0; i < 10000; i++);
    
    u8* sram = (u8*)SRAM;
    
    /* Write header */
    SaveHeader hdr;
    hdr.magic = SAVE_MAGIC;
    
    /* Write world data at offset 8 */
    for (int i = 0; i < WORLD_W * WORLD_H; i++) {
        sram[8 + i] = world_data[i];
    }
    
    /* Write player data at offset 8200 */
    u8* pptr = (u8*)&player;
    for (int i = 0; i < (int)sizeof(Player); i++) {
        sram[8200 + i] = pptr[i];
    }
    
    /* Write inventory at offset 8216 */
    u8* iptr = (u8*)inventory;
    for (int i = 0; i < INV_SLOTS * (int)sizeof(Item); i++) {
        sram[8216 + i] = iptr[i];
    }
    
    /* Calculate and write checksum */
    hdr.checksum = save_calc_checksum();
    
    /* Write header to SRAM start */
    u8* hptr = (u8*)&hdr;
    for (int i = 0; i < (int)sizeof(SaveHeader); i++) {
        sram[i] = hptr[i];
    }
    
    /* Wait for SRAM write to complete */
    for (int i = 0; i < 10000; i++);
}

int save_read(void) {
    u8* sram = (u8*)SRAM;
    
    /* Check magic */
    SaveHeader hdr;
    u8* hptr = (u8*)&hdr;
    for (int i = 0; i < (int)sizeof(SaveHeader); i++) {
        hptr[i] = sram[i];
    }
    
    if (hdr.magic != SAVE_MAGIC) return 0;
    
    /* Read world */
    for (int i = 0; i < WORLD_W * WORLD_H; i++) {
        world_data[i] = sram[8 + i];
    }
    
    /* Read player */
    u8* pptr = (u8*)&player;
    for (int i = 0; i < (int)sizeof(Player); i++) {
        pptr[i] = sram[8200 + i];
    }
    
    /* Read inventory */
    u8* iptr = (u8*)inventory;
    for (int i = 0; i < INV_SLOTS * (int)sizeof(Item); i++) {
        iptr[i] = sram[8216 + i];
    }
    
    /* Verify checksum */
    u16 check = save_calc_checksum();
    if (check != hdr.checksum) return 0;
    
    return 1;
}

void save_reset(void) {
    u8* sram = (u8*)SRAM;
    for (int i = 0; i < 64; i++) {
        for (int j = 0; j < 1024; j++) {
            sram[i * 1024 + j] = 0xFF;
        }
    }
}