#ifndef SAVE_H
#define SAVE_H

/* SRAM magic number */
#define SAVE_MAGIC 0x42464F47 /* "BOFG" = Block Forge Game */

/* Save all game data to SRAM */
void save_write(void);

/* Load all game data from SRAM. Returns 1 if valid save found. */
int save_read(void);

/* Reset SRAM (delete save) */
void save_reset(void);

#endif