# Toolchain
CC = gcc
AS = as
LD = ld

# Flags
CFLAGS = -m32 -ffreestanding -O2 -Wall -Wextra -fno-pie -Iinclude
ASFLAGS = --32
LDFLAGS = -m elf_i386 -T linker.ld -nostdlib

# Source files
C_SOURCES = src/main.c src/vga.c src/pci.c
ASM_SOURCES = src/boot.S

# Object files
C_OBJECTS = $(C_SOURCES:.c=.o)
ASM_OBJECTS = $(ASM_SOURCES:.S=.o)
OBJECTS = $(ASM_OBJECTS) $(C_OBJECTS)

# Output
KERNEL = vga_kernel.bin
ISO = vga_kernel.iso

# Default target
all: $(ISO)

# Link kernel (keep as ELF, GRUB can boot it)
$(KERNEL): $(OBJECTS)
	$(LD) $(LDFLAGS) -o $(KERNEL) $(OBJECTS)

# Compile C sources
src/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Assemble ASM sources
src/%.o: src/%.S
	$(AS) $(ASFLAGS) $< -o $@

# Create bootable ISO
$(ISO): $(KERNEL)
	mkdir -p isodir/boot/grub
	cp $(KERNEL) isodir/boot/$(KERNEL)
	echo 'menuentry "VGA Kernel" {' > isodir/boot/grub/grub.cfg
	echo '    multiboot /boot/$(KERNEL)' >> isodir/boot/grub/grub.cfg
	echo '}' >> isodir/boot/grub/grub.cfg
	grub-mkrescue -o $(ISO) isodir

# Run in QEMU
run: $(ISO)
	qemu-system-i386 -cdrom $(ISO) -nographic -serial mon:stdio

# Clean
clean:
	rm -f $(OBJECTS) $(KERNEL) $(ISO) 
	rm -rf isodir

.PHONY: all run clean
