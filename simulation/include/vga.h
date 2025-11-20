#ifndef VGA_H
#define VGA_H

#include <stdint.h>

// VGA text mode size
#define VGA_ROWS 25
#define VGA_COLS 80

// Simulated framebuffer (640x480 RGB)
#define FB_WIDTH 640
#define FB_HEIGHT 480

// VGA color (4bit each)
typedef struct {
    uint8_t fg; // foreground
    uint8_t bg; // background
} vga_color_t;

// Initialization
void vga_init(vga_color_t default_color);

// Clear the whole screen
void vga_clear();

// Write a single character at row/col
void vga_putc(int row, int col, char c);

// Write a string starting at row/col
void vga_write(int row, int col, const char *txt);

// Export framebuffer to a 24-bit BMP file
int framebuffer_to_bmp(const char *filename);

#endif
