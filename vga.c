#include "include/vga.h"

#define VGA_FB ((uint8_t*)0xA0000)
#define VGA_W 320
#define VGA_H 200

uint8_t* vga_get_framebuffer(void) {
    return VGA_FB;
}

int vga_width(void) {
    return VGA_W;
}

int vga_height(void) {
    return VGA_H;
}
