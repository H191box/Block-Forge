# Block Forge - GBA Sandbox Game Makefile
# Works with devkitARM or any arm-none-eabi toolchain in PATH

# Toolchain
ARMCC   = arm-none-eabi-gcc
ARMBIN  = arm-none-eabi-as
ARMOBJ  = arm-none-eabi-objcopy
ARMFIX  = gbafix

# Flags
ARCH    = -mthumb -mthumb-interwork
CPU     = -mcpu=arm7tdmi
CFLAGS  = $(ARCH) $(CPU) -O2 -Wall -ffreestanding -nostdlib \
          -Iinclude \
          -Isrc -Isrc/main -Isrc/engine -Isrc/game \
          -Isrc/game/world -Isrc/game/crafting -Isrc/game/entities \
          -Isrc/assets
LDFLAGS = $(ARCH) $(CPU) -T linker.ld -nostdlib -lgcc
ASFLAGS = -mcpu=arm7tdmi -mthumb-interwork

# Directories
SRCDIR  = src
BUILDDIR = build
OBJDIR  = $(BUILDDIR)/obj

# Source files
MAIN_C   = $(wildcard $(SRCDIR)/main/*.c)
ENGINE_C = $(wildcard $(SRCDIR)/engine/*.c)
GAME_C   = $(wildcard $(SRCDIR)/game/*.c) \
           $(wildcard $(SRCDIR)/game/world/*.c) \
           $(wildcard $(SRCDIR)/game/crafting/*.c) \
           $(wildcard $(SRCDIR)/game/entities/*.c)
ASSET_C  = $(wildcard $(SRCDIR)/assets/*.c)
MAIN_S   = $(wildcard $(SRCDIR)/main/*.s)
ENGINE_S = $(wildcard $(SRCDIR)/engine/*.s)

# All sources
CSRC = $(MAIN_C) $(ENGINE_C) $(GAME_C) $(ASSET_C)
SSRC = $(MAIN_S) $(ENGINE_S)

# Object files
COBJ = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(CSRC))
SOBJ = $(patsubst $(SRCDIR)/%.s, $(OBJDIR)/%.o, $(SSRC))

# Output
TARGET  = block_forge
ELF     = $(BUILDDIR)/$(TARGET).elf
GBA     = $(BUILDDIR)/$(TARGET).gba
MAP     = $(BUILDDIR)/$(TARGET).map

# Rules
all: dirs $(GBA)

dirs:
	@mkdir -p $(OBJDIR)/main $(OBJDIR)/engine $(OBJDIR)/game/world $(OBJDIR)/game/crafting $(OBJDIR)/game/entities $(OBJDIR)/assets

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(dir $@)
	$(ARMCC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.s
	@mkdir -p $(dir $@)
	$(ARMBIN) $(ASFLAGS) -o $@ $<

$(ELF): $(SOBJ) $(COBJ) linker.ld
	$(ARMCC) $(LDFLAGS) -o $@ $(SOBJ) $(COBJ) -Wl,-Map,$(MAP)

$(GBA): $(ELF)
	$(ARMOBJ) -O binary -j .gba_header -j .text -j .rodata -j .data $< $@
	$(ARMFIX) $@ -t"BLOCK FORGE" -cBFOG -mLC

clean:
	rm -rf $(BUILDDIR)

.PHONY: all clean dirs
