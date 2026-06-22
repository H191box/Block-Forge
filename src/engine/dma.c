#include "dma.h"
#include "gba_header.h"

void dma_memcpy16(const void* src, void* dst, u32 count) {
    REG_DMA3SAD = (u32)src;
    REG_DMA3DAD = (u32)dst;
    REG_DMA3CNT = DMA_ENABLE | DMA_16 | count;
    while (REG_DMA3CNT & DMA_ENABLE);
}

void dma_memcpy32(const void* src, void* dst, u32 count) {
    REG_DMA3SAD = (u32)src;
    REG_DMA3DAD = (u32)dst;
    REG_DMA3CNT = DMA_ENABLE | DMA_32 | count;
    while (REG_DMA3CNT & DMA_ENABLE);
}

void dma_memset16(u16 value, void* dst, u32 count) {
    REG_DMA3SAD = (u32)&value;
    REG_DMA3DAD = (u32)dst;
    REG_DMA3CNT = DMA_ENABLE | DMA_16 | DMA_SRC_FIX | count;
    while (REG_DMA3CNT & DMA_ENABLE);
}

void dma_copy_to_vram(const void* src, void* dst, u32 size) {
    REG_DMA3SAD = (u32)src;
    REG_DMA3DAD = (u32)dst;
    REG_DMA3CNT = DMA_ENABLE | DMA_32 | (size >> 2);
    while (REG_DMA3CNT & DMA_ENABLE);
}

void dma_copy_palette(const void* src, u16* dst, u32 count) {
    REG_DMA3SAD = (u32)src;
    REG_DMA3DAD = (u32)dst;
    REG_DMA3CNT = DMA_ENABLE | DMA_16 | count;
    while (REG_DMA3CNT & DMA_ENABLE);
}

void dma_copy_oam(const void* src) {
    REG_DMA3SAD = (u32)src;
    REG_DMA3DAD = (u32)OAM;
    REG_DMA3CNT = DMA_ENABLE | DMA_32 | (128 * 4);
    while (REG_DMA3CNT & DMA_ENABLE);
}