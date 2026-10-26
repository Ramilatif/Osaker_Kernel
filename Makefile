# Osaker_Kernel build: `make`, `make run`, `make debug`, `make clean`.

# Target board: selects platform/$(PLATFORM)/ (see docs/adr/0003).
PLATFORM ?= qemu-virt

BUILD := build
KERNEL := $(BUILD)/kernel.elf

CC := clang
LD := ld.lld
TARGET := --target=aarch64-none-elf

# C flags: cahier des charges section 7 and docs/adr/0002.
# -MMD -MP write .d files so that editing a header rebuilds its users.
CFLAGS := $(TARGET) -std=c17 -ffreestanding -mgeneral-regs-only \
          -mstrict-align -fno-pic -Wall -Wextra -Werror -g -Iinclude \
          -MMD -MP
ASFLAGS := $(TARGET) -g
LDFLAGS := -T linker.ld

SRCS := $(wildcard arch/aarch64/*.s) \
        $(wildcard core/*.c) \
        $(wildcard platform/$(PLATFORM)/*.c)

# core/main.c -> build/core/main.o, arch/aarch64/boot.s -> build/arch/aarch64/boot.o
OBJS := $(patsubst %,$(BUILD)/%.o,$(basename $(SRCS)))

QEMU := qemu-system-aarch64 -M virt -cpu cortex-a53 -nographic -kernel $(KERNEL)

.PHONY: all run debug clean

all: $(KERNEL)

$(KERNEL): $(OBJS) linker.ld
	$(LD) $(LDFLAGS) $(OBJS) -o $@

$(BUILD)/%.o: %.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/%.o: %.s
	@mkdir -p $(@D)
	$(CC) $(ASFLAGS) -c $< -o $@

# Quit QEMU with Ctrl-a x.
run: $(KERNEL)
	$(QEMU)

# Waits for GDB on localhost:1234 (gdb -nx build/kernel.elf, target remote :1234).
debug: $(KERNEL)
	$(QEMU) -s -S

clean:
	rm -rf $(BUILD)

-include $(OBJS:.o=.d)
