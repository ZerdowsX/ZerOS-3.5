# ZerOS-3.5

Минимальная 32-битная ОС для x86 (BIOS boot), написанная на NASM + C.

## Структура проекта

- `bootloader.asm` — 16-битный загрузчик BIOS, чтение kernel с диска, A20, переход в protected mode.
- `kernel.asm` — entry-point ядра.
- `kernel.c` — инициализация GDT/IDT/IRQ, запуск графики и мыши.
- `gdt.c` / `gdt.asm` — настройка и загрузка GDT.
- `idt.c` / `idt.asm` — таблица IDT, PIC remap, ISR/IRQ stubs.
- `mouse.c` — PS/2 mouse driver по IRQ12.
- `vga.c` — доступ к VGA framebuffer (mode 13h).
- `graphics.c` — `draw_pixel`, `draw_rect`, `clear_screen`, `draw_cursor`.
- `linker.ld` — линковка ядра на адрес загрузки `0x10000`.
- `Makefile` — сборка bootable image `os.img` и запуск в QEMU.

## Требования

- `nasm`
- `qemu-system-i386`
- GCC cross-toolchain: `i686-elf-gcc`, `i686-elf-ld`, `i686-elf-objcopy`

## Сборка

```bash
make
```

Итоговый образ: `os.img`.

## Запуск

```bash
make run
```

или напрямую:

```bash
qemu-system-i386 -drive format=raw,file=os.img
```

## Что происходит при загрузке

1. BIOS загружает `bootloader.asm` в `0x7C00`.
2. Bootloader ставит VGA mode `13h` (320x200x256), читает ядро с диска, включает A20.
3. Bootloader переключает CPU в protected mode и прыгает в kernel (`0x10000`).
4. Kernel поднимает GDT + IDT, ремапит PIC, включает IRQ12.
5. Экран очищается в зелёный цвет, рисуется курсор.
6. IRQ12 от PS/2 мыши двигает курсор в реальном времени без следов.
