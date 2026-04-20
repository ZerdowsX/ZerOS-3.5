#include <stdint.h>
#include "include/common.h"
#include "include/mouse.h"
#include "include/graphics.h"
#include "include/vga.h"

static int mouse_x = 160;
static int mouse_y = 100;

static uint8_t packet[3];
static uint8_t packet_index = 0;

static void mouse_wait_write(void) {
    for (int i = 0; i < 100000; i++) {
        if ((inb(0x64) & 0x02) == 0) {
            return;
        }
    }
}

static void mouse_wait_read(void) {
    for (int i = 0; i < 100000; i++) {
        if (inb(0x64) & 0x01) {
            return;
        }
    }
}

static void mouse_write(uint8_t value) {
    mouse_wait_write();
    outb(0x64, 0xD4);
    mouse_wait_write();
    outb(0x60, value);
}

static uint8_t mouse_read(void) {
    mouse_wait_read();
    return inb(0x60);
}

void mouse_install(void) {
    mouse_wait_write();
    outb(0x64, 0xA8);

    mouse_wait_write();
    outb(0x64, 0x20);
    mouse_wait_read();
    uint8_t status = inb(0x60);

    status |= 0x02;
    status &= ~0x20;

    mouse_wait_write();
    outb(0x64, 0x60);
    mouse_wait_write();
    outb(0x60, status);

    mouse_write(0xF6);
    (void)mouse_read();

    mouse_write(0xF4);
    (void)mouse_read();

    draw_cursor(mouse_x, mouse_y);
}

void mouse_handle_irq(void) {
    uint8_t status = inb(0x64);
    if ((status & 0x01) == 0 || (status & 0x20) == 0) {
        return;
    }

    packet[packet_index++] = inb(0x60);

    if (packet_index < 3) {
        return;
    }
    packet_index = 0;

    if ((packet[0] & 0x08) == 0) {
        return;
    }

    int dx = (int8_t)packet[1];
    int dy = (int8_t)packet[2];

    mouse_x += dx;
    mouse_y -= dy;

    if (mouse_x < 0) mouse_x = 0;
    if (mouse_y < 0) mouse_y = 0;
    if (mouse_x > vga_width() - 8) mouse_x = vga_width() - 8;
    if (mouse_y > vga_height() - 12) mouse_y = vga_height() - 12;

    draw_cursor(mouse_x, mouse_y);
}

int mouse_get_x(void) {
    return mouse_x;
}

int mouse_get_y(void) {
    return mouse_y;
}
