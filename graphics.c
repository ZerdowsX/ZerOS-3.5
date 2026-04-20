#include <stdint.h>
#include "include/graphics.h"
#include "include/vga.h"

#define CURSOR_W 8
#define CURSOR_H 12

static int last_x = -1;
static int last_y = -1;
static uint8_t cursor_bg[CURSOR_W * CURSOR_H];

static const uint8_t cursor_shape[CURSOR_H] = {
    0b10000000,
    0b11000000,
    0b11100000,
    0b11110000,
    0b11111000,
    0b11111100,
    0b11111110,
    0b11111100,
    0b11101100,
    0b11000110,
    0b10000011,
    0b00000001,
};

void draw_pixel(int x, int y, uint8_t color) {
    if (x < 0 || y < 0 || x >= vga_width() || y >= vga_height()) {
        return;
    }
    vga_get_framebuffer()[y * vga_width() + x] = color;
}

void draw_rect(int x, int y, int w, int h, uint8_t color) {
    for (int yy = 0; yy < h; yy++) {
        for (int xx = 0; xx < w; xx++) {
            draw_pixel(x + xx, y + yy, color);
        }
    }
}

void clear_screen(uint8_t color) {
    draw_rect(0, 0, vga_width(), vga_height(), color);
}

void erase_cursor(void) {
    if (last_x < 0 || last_y < 0) {
        return;
    }

    int idx = 0;
    for (int y = 0; y < CURSOR_H; y++) {
        for (int x = 0; x < CURSOR_W; x++) {
            int px = last_x + x;
            int py = last_y + y;
            if (px >= 0 && py >= 0 && px < vga_width() && py < vga_height()) {
                draw_pixel(px, py, cursor_bg[idx]);
            }
            idx++;
        }
    }

    last_x = -1;
    last_y = -1;
}

void draw_cursor(int x, int y) {
    erase_cursor();

    int idx = 0;
    for (int yy = 0; yy < CURSOR_H; yy++) {
        for (int xx = 0; xx < CURSOR_W; xx++) {
            int px = x + xx;
            int py = y + yy;
            if (px >= 0 && py >= 0 && px < vga_width() && py < vga_height()) {
                cursor_bg[idx] = vga_get_framebuffer()[py * vga_width() + px];
                if ((cursor_shape[yy] >> (7 - xx)) & 1) {
                    draw_pixel(px, py, 15);
                }
            }
            idx++;
        }
    }

    last_x = x;
    last_y = y;
}
