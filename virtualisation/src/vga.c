#include "vga.h"
#include <string.h>

// TEXT MODE -------------------------------------------------------
void vga_init_text(framebuffer_t *fb) {
    for (int y = 0; y < VGA_TEXT_ROWS; y++) {
        for (int x = 0; x < VGA_TEXT_COLS; x++) {
            uint16_t *ptr = (uint16_t *)fb->buffer;
            ptr[y * VGA_TEXT_COLS + x] = 0x0720; // space, light grey on black
        }
    }
}

void vga_putc_text(int row, int col, char c, uint8_t color, framebuffer_t *fb) {
    if (row < 0 || row >= VGA_TEXT_ROWS || col < 0 || col >= VGA_TEXT_COLS) return;
    uint16_t *ptr = (uint16_t *)fb->buffer;
    ptr[row * VGA_TEXT_COLS + col] = (color << 8) | c;
}

void vga_write_text(int row, int col, const char *txt, uint8_t color, framebuffer_t *fb) {
    while (*txt) {
        vga_putc_text(row, col, *txt, color, fb);
        col++;
        if (col >= VGA_TEXT_COLS) break;
        txt++;
    }
}

// GRAPHICS MODE ---------------------------------------------------
static void draw_char_pixel(framebuffer_t *fb, int px, int py, uint8_t on) {
    if (px >= fb->width || py >= fb->height || px < 0 || py < 0) return;
    uint8_t *p = &fb->buffer[(py * fb->width + px) * 3]; // 24-bit RGB
    if (on) {
        p[0] = 255; p[1] = 255; p[2] = 255;
    } else {
        p[0] = 0; p[1] = 0; p[2] = 0;
    }
}

void vga_draw_char(framebuffer_t *fb, int x, int y, char c) {
    const uint8_t *glyph = font_get(c);
    for (int row = 0; row < CHAR_HEIGHT; row++) {
        uint8_t bits = glyph[row];
        for (int col = 0; col < CHAR_WIDTH; col++) {
            draw_char_pixel(fb, x + col, y + row, (bits & (1 << (7 - col))) != 0);
        }
    }
}

void vga_draw_string(framebuffer_t *fb, int x, int y, const char *txt) {
    while (*txt) {
        vga_draw_char(fb, x, y, *txt);
        x += CHAR_WIDTH;
        txt++;
    }
}
