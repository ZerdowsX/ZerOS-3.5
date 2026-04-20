CROSS ?= i686-elf-
CC := $(CROSS)gcc
LD := $(CROSS)ld
AS := nasm
OBJCOPY := $(CROSS)objcopy

CFLAGS := -m32 -ffreestanding -fno-pic -fno-stack-protector -nostdlib -nostartfiles -Wall -Wextra -O2 -Iinclude
LDFLAGS := -m elf_i386 -T linker.ld

KERNEL_OBJS := kernel_entry.o kernel.o gdt.o gdt_asm.o idt.o idt_asm.o mouse.o vga.o graphics.o

all: os-image

bootloader.bin: bootloader.asm
	$(AS) -f bin $< -o $@

kernel_entry.o: kernel.asm
	$(AS) -f elf32 $< -o $@

gdt_asm.o: gdt.asm
	$(AS) -f elf32 $< -o $@

idt_asm.o: idt.asm
	$(AS) -f elf32 $< -o $@

kernel.o: kernel.c
	$(CC) $(CFLAGS) -c $< -o $@

gdt.o: gdt.c
	$(CC) $(CFLAGS) -c $< -o $@

idt.o: idt.c
	$(CC) $(CFLAGS) -c $< -o $@

mouse.o: mouse.c
	$(CC) $(CFLAGS) -c $< -o $@

vga.o: vga.c
	$(CC) $(CFLAGS) -c $< -o $@

graphics.o: graphics.c
	$(CC) $(CFLAGS) -c $< -o $@

kernel.elf: $(KERNEL_OBJS) linker.ld
	$(LD) $(LDFLAGS) -o $@ $(KERNEL_OBJS)

kernel.bin: kernel.elf
	$(OBJCOPY) -O binary $< $@

os-image: bootloader.bin kernel.bin
	dd if=/dev/zero of=os.img bs=512 count=2880 status=none
	dd if=bootloader.bin of=os.img conv=notrunc status=none
	dd if=kernel.bin of=os.img bs=512 seek=1 conv=notrunc status=none

run: os-image
	qemu-system-i386 -drive format=raw,file=os.img

clean:
	rm -f *.o *.bin *.elf os.img

.PHONY: all os-image run clean
