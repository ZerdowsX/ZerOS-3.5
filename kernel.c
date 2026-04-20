#include <stdint.h>
#include "include/gdt.h"
#include "include/idt.h"
#include "include/graphics.h"
#include "include/mouse.h"

void kmain(void) {
    gdt_install();
    idt_install();
    irq_install();

    clear_screen(2);
    mouse_install();

    __asm__ volatile ("sti");

    while (1) {
        __asm__ volatile ("hlt");
    }
}
