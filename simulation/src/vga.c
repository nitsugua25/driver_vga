#include "vga.h"
#include "font.h"
#include "bmp.h"
#include <string.h>

#define CHAR_WIDTH 8
#define CHAR_HEIGHT 8

// VGA text buffer 25x80
static uint16_t vga_buffer[VGA_ROWS][VGA_COLS];
static vga_color_t default_color;

// Framebuffer 640x480 RGB
static uint8_t framebuffer[FB_HEIGHT][FB_WIDTH][3];

// Draw one character to framebuffer using font.c
static void draw_char_fb(int x, int y, char c) {
    const uint8_t* glyph = font_get(c);

    for (int row = 0; row < CHAR_HEIGHT; row++) {
        uint8_t bits = glyph[row];
        for (int col = 0; col < CHAR_WIDTH; col++) {
            int px = x + col;
            int py = y + row;
            if (px < 0 || px >= FB_WIDTH || py < 0 || py >= FB_HEIGHT)
                continue;

            if (bits & (1 << (7 - col))) {
                framebuffer[py][px][0] = 255; // R
                framebuffer[py][px][1] = 255; // G
                framebuffer[py][px][2] = 255; // B
            } else {
                framebuffer[py][px][0] = 0;
                framebuffer[py][px][1] = 0;
                framebuffer[py][px][2] = 0;
            }
        }
    }
}

// Initialize VGA buffer and framebuffer
void vga_init(vga_color_t color) {
    default_color = color;
    for (int r = 0; r < VGA_ROWS; r++)
        for (int c = 0; c < VGA_COLS; c++)
            vga_buffer[r][c] = (' ' | (default_color.fg << 12) | (default_color.bg << 8));

    memset(framebuffer, 0, sizeof(framebuffer));
}

// Clear screen
void vga_clear() {
    vga_init(default_color);
}

// Put one character at row/col
void vga_putc(int row, int col, char c) {
    if (row < 0 || row >= VGA_ROWS || col < 0 || col >= VGA_COLS)
        return;

    if (c != ' ' && !(c >= 'A' && c <= 'Z'))
        return;

    uint16_t entry = (uint16_t)c | (default_color.fg << 12) | (default_color.bg << 8);
    vga_buffer[row][col] = entry;

    // Draw in framebuffer
    draw_char_fb(col * CHAR_WIDTH, row * CHAR_HEIGHT, c);
}

// Write string starting at row/col
void vga_write(int row, int col, const char *txt) {
    for (int i = 0; txt[i] != '\0'; i++) {
        vga_putc(row, col + i, txt[i]);
    }
}

// Export framebuffer to BMP using bmp.c
int framebuffer_to_bmp(const char *filename) {
    return bmp_write(filename, FB_WIDTH, FB_HEIGHT, framebuffer);
}
