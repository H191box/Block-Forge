# Block Forge - GBA Sandbox Game Makefile
# devkitARM toolchain

# Toolchain
ARMCC   = arm-none-eabi-gcc
ARMBIN  = arm-none-eabi-as
ARMOBJ  = arm-none-eabi-objcopy
ARMFIX  = gbafix

# Flags
ARCH    = -mthumb -mthumb-interwork
CPU     = -mcpu=arm7tdmi
CFLAGS  = $(ARCH) $(CPU) -O2 -Wall -ffreestanding -nostdlib -Iinclude -Isrc -Isrc/engine -Isrc/game
LDFLAGS = $(ARCH) $(CPU) -T linker.ld -nostdlib -lgcc
ASFLAGS = $(ARCH) $(CPU)

# Directories
SRCDIR  = src
BUILDDIR = build
OBJDIR  = $(BUILDDIR)/obj

# Source files
ENGINE_C = $(wildcard $(SRCDIR)/engine/*.c)
GAME_C   = $(wildcard $(SRCDIR)/game/*.c) \
           $(wildcard $(SRCDIR)/game/world/*.c) \
           $(wildcard $(SRCDIR)/game/crafting/*.c) \
           $(wildcard $(SRCDIR)/game/entities/*.c)
ASSET_C  = $(wildcard $(SRCDIR)/assets/*.c)
MAIN_C   = $(wildcard $(SRCDIR)/main/*.c)

# All C sources
CSRC = $(MAIN_C) $(ENGINE_C) $(GAME_C) $(ASSET_C)

# Object files
COBJ = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(CSRC))

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
	$(ARMCC) $(CFLAGS) -c $< -o $@

$(ELF): $(COBJ) linker.ld
	$(ARMCC) $(LDFLAGS) -o $@ $(COBJ) -Wl,-Map,$(MAP)

$(GBA): $(ELF)
	$(ARMOBJ) -O binary $< $@
	$(ARMFIX) $@ -t"BLOCK FORGE" -cBFOG -mLC

clean:
	rm -rf $(BUILDDIR)

.PHONY: all clean dirs