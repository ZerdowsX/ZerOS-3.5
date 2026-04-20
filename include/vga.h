#ifndef VGA_H
#define VGA_H

#include <stdint.h>

uint8_t* vga_get_framebuffer(void);
int vga_width(void);
int vga_height(void);

#endif
