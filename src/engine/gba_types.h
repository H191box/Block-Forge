#ifndef GBA_TYPES_H
#define GBA_TYPES_H

/* Basic types */
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef signed char s8;
typedef signed short s16;
typedef signed int s32;
typedef int bool;
#define true  1
#define false 0
#define NULL  ((void*)0)

/* Utility macros */
#define MIN(a, b)   ((a) < (b) ? (a) : (b))
#define MAX(a, b)   ((a) > (b) ? (a) : (b))
#define CLAMP(v, lo, hi) MIN(MAX(v, lo), hi)
#define ABS(x)      ((x) < 0 ? -(x) : (x))

/* Simple seeded RNG (LCG) - global state */
extern u32 rng_seed_val;
void rng_seed(u32 s);
u32 rng_next(void);

/* Memory helpers */
static inline void* memcpy_fast(void* dst, const void* src, u32 n) {
    u8* d = (u8*)dst;
    const u8* s = (const u8*)src;
    for (u32 i = 0; i < n; i++) d[i] = s[i];
    return dst;
}

static inline void* memset_fast(void* dst, u8 val, u32 n) {
    u8* d = (u8*)dst;
    for (u32 i = 0; i < n; i++) d[i] = val;
    return dst;
}

static inline u32 strlen_fast(const char* s) {
    u32 len = 0;
    while (s[len]) len++;
    return len;
}

/* GBA ROM header struct */
typedef struct {
    u8  entry_point[4];
    u8  nintendo_logo[156];
    u8  game_title[12];
    u8  game_code[4];
    u8  maker_code[2];
    u8  fixed_96[96];
    u16 crc;
} GBAHeader;

#endif /* GBA_TYPES_H */