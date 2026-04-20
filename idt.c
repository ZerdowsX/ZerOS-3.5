#include <stdint.h>
#include "include/common.h"
#include "include/idt.h"
#include "include/mouse.h"

struct __attribute__((packed)) idt_entry {
    uint16_t base_low;
    uint16_t sel;
    uint8_t always0;
    uint8_t flags;
    uint16_t base_high;
};

struct __attribute__((packed)) idt_ptr {
    uint16_t limit;
    uint32_t base;
};

extern void idt_load(uint32_t idt_ptr_addr);
extern void* isr_stub_table[];

static struct idt_entry idt[256];
static struct idt_ptr idtp;

static void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags) {
    idt[num].base_low = base & 0xFFFF;
    idt[num].base_high = (base >> 16) & 0xFFFF;
    idt[num].sel = sel;
    idt[num].always0 = 0;
    idt[num].flags = flags;
}

static void pic_remap(void) {
    outb(0x20, 0x11);
    io_wait();
    outb(0xA0, 0x11);
    io_wait();

    outb(0x21, 0x20);
    io_wait();
    outb(0xA1, 0x28);
    io_wait();

    outb(0x21, 0x04);
    io_wait();
    outb(0xA1, 0x02);
    io_wait();

    outb(0x21, 0x01);
    io_wait();
    outb(0xA1, 0x01);
    io_wait();

    outb(0x21, 0xFF);
    outb(0xA1, 0xFF);
}

void idt_install(void) {
    idtp.limit = sizeof(idt) - 1;
    idtp.base = (uint32_t)&idt;

    for (int i = 0; i < 256; i++) {
        idt_set_gate((uint8_t)i, (uint32_t)isr_stub_table[i], 0x08, 0x8E);
    }

    idt_load((uint32_t)&idtp);
}

void irq_install(void) {
    pic_remap();

    uint8_t master_mask = 0xFB; // unmask IRQ2 only
    uint8_t slave_mask = 0xEF;  // unmask IRQ12 on slave

    outb(0x21, master_mask);
    outb(0xA1, slave_mask);
}

static void pic_send_eoi(uint32_t int_no) {
    if (int_no >= 40) {
        outb(0xA0, 0x20);
    }
    outb(0x20, 0x20);
}

void isr_handler_c(uint32_t int_no, uint32_t err_code) {
    (void)err_code;

    if (int_no == 44) {
        mouse_handle_irq();
    }

    if (int_no >= 32 && int_no <= 47) {
        pic_send_eoi(int_no);
    }
}
