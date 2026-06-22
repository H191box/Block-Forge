#ifndef GBA_HEADER_H
#define GBA_HEADER_H

#include "gba_types.h"

/* GBA Memory-Mapped I/O Registers */
#define REG_DISPCNT     (*(volatile u16*)0x04000000)
#define REG_DISPSTAT    (*(volatile u16*)0x04000004)
#define REG_VCOUNT      (*(volatile u16*)0x04000006)

/* BG Control Registers */
#define REG_BG0CNT      (*(volatile u16*)0x04000008)
#define REG_BG1CNT      (*(volatile u16*)0x0400000A)
#define REG_BG2CNT      (*(volatile u16*)0x0400000C)
#define REG_BG3CNT      (*(volatile u16*)0x0400000E)

/* BG Scroll Offsets */
#define REG_BG0HOFS     (*(volatile u16*)0x04000010)
#define REG_BG0VOFS     (*(volatile u16*)0x04000012)
#define REG_BG1HOFS     (*(volatile u16*)0x04000014)
#define REG_BG1VOFS     (*(volatile u16*)0x04000016)
#define REG_BG2HOFS     (*(volatile u16*)0x04000018)
#define REG_BG2VOFS     (*(volatile u16*)0x0400001A)
#define REG_BG3HOFS     (*(volatile u16*)0x0400001C)
#define REG_BG3VOFS     (*(volatile u16*)0x0400001E)

/* Window Registers */
#define REG_WIN0H       (*(volatile u16*)0x04000040)
#define REG_WIN1H       (*(volatile u16*)0x04000042)
#define REG_WIN0V       (*(volatile u16*)0x04000044)
#define REG_WIN1V       (*(volatile u16*)0x04000046)
#define REG_WININ       (*(volatile u16*)0x04000048)
#define REG_WINOUT      (*(volatile u16*)0x0400004A)

/* Blend Control */
#define REG_BLDMOD      (*(volatile u16*)0x04000050)
#define REG_COLEV       (*(volatile u16*)0x04000052)
#define REG_COLEY       (*(volatile u16*)0x04000054)

/* DMA Transfer Channels */
#define REG_DMA0SAD     (*(volatile u32*)0x040000B0)
#define REG_DMA0DAD     (*(volatile u32*)0x040000B4)
#define REG_DMA0CNT     (*(volatile u32*)0x040000B8)
#define REG_DMA1SAD     (*(volatile u32*)0x040000BC)
#define REG_DMA1DAD     (*(volatile u32*)0x040000C0)
#define REG_DMA1CNT     (*(volatile u32*)0x040000C4)
#define REG_DMA2SAD     (*(volatile u32*)0x040000C8)
#define REG_DMA2DAD     (*(volatile u32*)0x040000CC)
#define REG_DMA2CNT     (*(volatile u32*)0x040000D0)
#define REG_DMA3SAD     (*(volatile u32*)0x040000D4)
#define REG_DMA3DAD     (*(volatile u32*)0x040000D8)
#define REG_DMA3CNT     (*(volatile u32*)0x040000DC)

/* Timer Registers */
#define REG_TM0CNT      (*(volatile u16*)0x04000100)
#define REG_TM0D        (*(volatile u16*)0x04000102)
#define REG_TM1CNT      (*(volatile u16*)0x04000104)
#define REG_TM1D        (*(volatile u16*)0x04000106)
#define REG_TM2CNT      (*(volatile u16*)0x04000108)
#define REG_TM2D        (*(volatile u16*)0x0400010A)
#define REG_TM3CNT      (*(volatile u16*)0x0400010C)
#define REG_TM3D        (*(volatile u16*)0x0400010E)

/* Keypad Input */
#define REG_KEY         (*(volatile u16*)0x04000130)
#define REG_KEYCNT      (*(volatile u16*)0x04000132)

/* Sound Registers */
#define REG_SND1SWEEP   (*(volatile u16*)0x04000060)
#define REG_SND1CNT     (*(volatile u16*)0x04000062)
#define REG_SND1FREQ    (*(volatile u16*)0x04000064)
#define REG_SND2CNT     (*(volatile u16*)0x04000068)
#define REG_SND2FREQ    (*(volatile u16*)0x0400006C)
#define REG_SND3CNT     (*(volatile u16*)0x04000070)
#define REG_SND3WAV     (*(volatile u16*)0x04000072)
#define REG_SND3FREQ    (*(volatile u16*)0x04000074)
#define REG_SND4CNT     (*(volatile u16*)0x04000078)
#define REG_SNDDMGCNT   (*(volatile u16*)0x04000080)
#define REG_SNDDMGA     (*(volatile u16*)0x04000082)
#define REG_SNDDMGB     (*(volatile u16*)0x04000084)
#define REG_SNDCNT      (*(volatile u16*)0x04000084)
#define REG_SNDSTAT     (*(volatile u16*)0x04000088)
#define REG_SND_BIAS    (*(volatile u16*)0x04000090)

/* Interrupt Registers */
#define REG_IME         (*(volatile u16*)0x04000208)
#define REG_IE          (*(volatile u16*)0x04000200)
#define REG_IF          (*(volatile u16*)0x04000202)

/* Display Control Bits */
#define DISPCNT_MODE0       0x0000
#define DISPCNT_BG0         (1 << 0)
#define DISPCNT_BG1         (1 << 1)
#define DISPCNT_BG2         (1 << 2)
#define DISPCNT_BG3         (1 << 3)
#define DISPCNT_OBJ         (1 << 4)
#define DISPCNT_WIN0        (1 << 5)
#define DISPCNT_WIN1        (1 << 6)
#define DISPCNT_OBJWIN      (1 << 7)
#define DISPCNT_BLANK       (1 << 7)
#define DISPCNT_OBJ_1D      (1 << 6)

/* BG Control Bits */
#define BGCNT_PRIORITY(n)   ((n) << 0)
#define BGCNT_CHARBASE(n)   ((n) << 2)
#define BGCNT_SCREENBASE(n) ((n) << 8)
#define BGCNT_256COLOR      (1 << 7)
#define BGCNT_MOSAIC        (1 << 6)
#define BGCNT_WRAP          (1 << 13)
#define BGCNT_SIZE_256x256  0x0000
#define BGCNT_SIZE_512x256  0x8000
#define BGCNT_SIZE_256x512  0x4000
#define BGCNT_SIZE_512x512  0xC000

/* Display Status (VBlank) */
#define DISPSTAT_VBLANK     (1 << 0)
#define DISPSTAT_HBLANK     (1 << 1)
#define DISPSTAT_VCOUNT     (1 << 2)
#define DISPSTAT_VBLANK_IRQ (1 << 3)
#define DISPSTAT_HBLANK_IRQ (1 << 4)
#define DISPSTAT_VCOUNT_IRQ (1 << 5)

/* Interrupt Flags */
#define IRQ_VBLANK      (1 << 0)
#define IRQ_HBLANK      (1 << 1)
#define IRQ_VCOUNT      (1 << 2)
#define IRQ_TIMER0      (1 << 3)
#define IRQ_TIMER1      (1 << 4)
#define IRQ_TIMER2      (1 << 5)
#define IRQ_TIMER3      (1 << 6)
#define IRQ_SERIAL      (1 << 7)
#define IRQ_DMA0        (1 << 8)
#define IRQ_DMA1        (1 << 9)
#define IRQ_DMA2        (1 << 10)
#define IRQ_DMA3        (1 << 11)
#define IRQ_KEYPAD      (1 << 12)
#define IRQ_GAMEPAK     (1 << 13)

/* Keypad Bits (active low) */
#define KEY_A           (1 << 0)
#define KEY_B           (1 << 1)
#define KEY_SELECT      (1 << 2)
#define KEY_START       (1 << 3)
#define KEY_RIGHT       (1 << 4)
#define KEY_LEFT        (1 << 5)
#define KEY_UP          (1 << 6)
#define KEY_DOWN        (1 << 7)
#define KEY_R           (1 << 8)
#define KEY_L           (1 << 9)

/* DMA Control Bits */
#define DMA_ENABLE      (1 << 31)
#define DMA_IRQ         (1 << 30)
#define DMA_16          (0 << 26)
#define DMA_32          (1 << 26)
#define DMA_REPEAT      (1 << 25)
#define DMA_NOW         (1 << 28)
#define DMA_AT_VBLANK   (1 << 27)
#define DMA_AT_HBLANK   (1 << 26)
#define DMA_SRC_INC     (0 << 23)
#define DMA_SRC_DEC     (1 << 23)
#define DMA_SRC_FIX     (2 << 23)
#define DMA_DST_INC     (0 << 21)
#define DMA_DST_DEC     (1 << 21)
#define DMA_DST_FIX     (2 << 21)

/* Timer Control Bits */
#define TIMER_ENABLE    (1 << 7)
#define TIMER_IRQ       (1 << 6)
#define TIMER_CASCADE   (1 << 2)
#define TIMER_FREQ_1    (0 << 0)
#define TIMER_FREQ_64   (1 << 0)
#define TIMER_FREQ_256  (2 << 0)
#define TIMER_FREQ_1024 (3 << 0)

/* VRAM Addresses */
#define VRAM            ((u16*)0x06000000)
#define VRAM_CHARBASE(n) ((u16*)(0x06000000 + ((n) * 0x4000)))
#define VRAM_SCREENBASE(n) ((u16*)(0x06000000 + ((n) * 0x800)))
#define OBJ_VRAM        ((u16*)0x06010000)

/* Palette */
#define BG_PALETTE      ((u16*)0x05000000)
#define OBJ_PALETTE     ((u16*)0x05000200)

/* OAM */
#define OAM             ((u16*)0x07000000)
#define OAM_ATTR0       ((u16*)0x07000000)
#define OAM_ATTR1       ((u16*)0x07000002)
#define OAM_ATTR2       ((u16*)0x07000004)

/* OAM Attribute Bits */
#define ATTR0_Y(y)          ((y) & 0xFF)
#define ATTR0_MODE_NORMAL   (0 << 8)
#define ATTR0_MODE_BLEND    (1 << 8)
#define ATTR0_MODE_WINDOW   (2 << 8)
#define ATTR0_MOSAIC        (1 << 12)
#define ATTR0_8BPP          (1 << 13)
#define ATTR0_SHAPE_SQUARE  (0 << 14)
#define ATTR0_SHAPE_WIDE    (1 << 14)
#define ATTR0_SHAPE_TALL    (2 << 14)

#define ATTR1_X(x)          ((x) & 0x1FF)
#define ATTR1_FLIP_H        (1 << 12)
#define ATTR1_FLIP_V        (1 << 13)
#define ATTR1_SIZE_8x8      (0 << 14)
#define ATTR1_SIZE_16x16    (1 << 14)
#define ATTR1_SIZE_32x32    (2 << 14)
#define ATTR1_SIZE_64x64    (3 << 14)

#define ATTR2_TILEID(n)     ((n) & 0x3FF)
#define ATTR2_PRIORITY(n)   ((n) << 10)
#define ATTR2_PAL(n)        ((n) << 12)

/* SRAM */
#define SRAM            ((u8*)0x0E000000)

/* RGB555 Macro */
#define RGB555(r, g, b) (((r) & 0x1F) | (((g) & 0x1F) << 5) | (((b) & 0x1F) << 10))

#endif /* GBA_HEADER_H */