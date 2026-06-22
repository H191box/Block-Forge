#ifndef DMA_H
#define DMA_H

#include "gba_header.h"

/* DMA3 16-bit copy */
void dma_memcpy16(const void* src, void* dst, u32 count);

/* DMA3 32-bit copy */
void dma_memcpy32(const void* src, void* dst, u32 count);

/* DMA3 fill 16-bit */
void dma_memset16(u16 value, void* dst, u32 count);

/* Fast VRAM copy */
void dma_copy_to_vram(const void* src, void* dst, u32 size);

/* Fast palette copy */
void dma_copy_palette(const void* src, u16* dst, u32 count);

/* Fast OAM copy */
void dma_copy_oam(const void* src);

#endif /* DMA_H */