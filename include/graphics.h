#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <stdint.h>

void draw_pixel(int x, int y, uint8_t color);
void draw_rect(int x, int y, int w, int h, uint8_t color);
void clear_screen(uint8_t color);
void draw_cursor(int x, int y);
void erase_cursor(void);

#endif
