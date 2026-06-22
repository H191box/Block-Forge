# ============================================================
#  Block Forge - GBA Sandbox Game
#  Makefile for devkitARM (devkitPro) — Docker build
#
#  PATRÓN basado en H191box/gems (igual que gravity-lab)
#  Usa custom block_forge.specs SIN libgba porque block-forge
#  tiene su propio engine (system.c, input.c, dma.c, etc.)
# ============================================================

# --- Toolchain (devkitPro inside Docker) ---
DEVKITPRO ?= /opt/devkitpro
DEVKITARM ?= $(DEVKITPRO)/devkitARM
LIBGBA    ?= $(DEVKITPRO)/libgba

CC      := $(DEVKITARM)/bin/arm-none-eabi-gcc
AS      := $(DEVKITARM)/bin/arm-none-eabi-as
OBJCOPY := $(DEVKITARM)/bin/arm-none-eabi-objcopy

# --- Project paths ---
SRCDIR   = src
BUILDDIR = build
TARGET   = block_forge

# --- Sources ---
MAIN_C   := $(wildcard $(SRCDIR)/main/*.c)
ENGINE_C := $(wildcard $(SRCDIR)/engine/*.c)
GAME_C   := $(wildcard $(SRCDIR)/game/*.c) \
           $(wildcard $(SRCDIR)/game/world/*.c) \
           $(wildcard $(SRCDIR)/game/crafting/*.c) \
           $(wildcard $(SRCDIR)/game/entities/*.c)
ASSET_C  := $(wildcard $(SRCDIR)/assets/*.c)

# isr.s only — crt0.s is NOT needed (gba.specs provides startup via devkitPro crt0.o)
ENGINE_S := $(wildcard $(SRCDIR)/engine/*.s)

CSRC := $(MAIN_C) $(ENGINE_C) $(GAME_C) $(ASSET_C)
SSRC := $(ENGINE_S)

COBJ := $(patsubst $(SRCDIR)/%.c,$(BUILDDIR)/%.o,$(CSRC))
SOBJ := $(patsubst $(SRCDIR)/%.s,$(BUILDDIR)/%.o,$(SSRC))

# --- Output ---
ROM = $(TARGET).gba

# -------------------------------------------------------
#  COMPILER FLAGS
#  NO -I$(LIBGBA)/include — block-forge has its own headers
# -------------------------------------------------------
CFLAGS := \
	-mthumb \
	-mthumb-interwork \
	-mcpu=arm7tdmi \
	-O2 \
	-ffunction-sections \
	-fdata-sections \
	-fno-common \
	-Iinclude \
	-I$(SRCDIR) \
	-I$(SRCDIR)/main \
	-I$(SRCDIR)/engine \
	-I$(SRCDIR)/game \
	-I$(SRCDIR)/game/world \
	-I$(SRCDIR)/game/crafting \
	-I$(SRCDIR)/game/entities \
	-I$(SRCDIR)/assets \
	-Wall -Wextra \
	-std=gnu99

ASFLAGS := -mcpu=arm7tdmi -mthumb-interwork

# -------------------------------------------------------
#  LINKER FLAGS
#  CRÍTICO: -specs=block_forge.specs (custom, sin -lgba)
#  Proporciona: ROM header, ARM→Thumb stub, .data copy, .bss zero
#  Usa nuestro gba.ld local (con sección .iwram para isr.s)
# -------------------------------------------------------
LDFLAGS := \
	-mthumb \
	-mthumb-interwork \
	-mcpu=arm7tdmi \
	-specs=block_forge.specs \
	-L$(LIBGBA)/lib \
	-L. \
	-Wl,--gc-sections \
	-Wl,-Map,$(BUILDDIR)/$(TARGET).map

# -------------------------------------------------------
#  BUILD RULES
# -------------------------------------------------------
all: $(BUILDDIR) $(ROM)

$(BUILDDIR):
	mkdir -p $(BUILDDIR)

$(ROM): $(SOBJ) $(COBJ)
	$(CC) -mthumb -mthumb-interwork -mcpu=arm7tdmi -specs=block_forge.specs -Wl,--gc-sections -Wl,-Map,$(BUILDDIR)/$(TARGET).map -o $(BUILDDIR)/$(TARGET).elf $(SOBJ) $(COBJ) -L$(LIBGBA)/lib -L. -lgcc -lc -lgcc
	$(OBJCOPY) -O binary $(BUILDDIR)/$(TARGET).elf $(ROM)
	@echo "=== ROM built: $(ROM) ==="
	@ls -la $(ROM)

$(BUILDDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c -o $@ $<

$(BUILDDIR)/%.o: $(SRCDIR)/%.s
	@mkdir -p $(dir $@)
	$(AS) $(ASFLAGS) -o $@ $<

# -------------------------------------------------------
#  UTILITIES
# -------------------------------------------------------
clean:
	rm -rf $(BUILDDIR) $(ROM)

rebuild: clean all

.PHONY: all clean rebuild
