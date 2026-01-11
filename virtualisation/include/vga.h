#ifndef VGA_H
#define VGA_H

#include <stdint.h>
#include <stddef.h>

#define VGA_TEXT_ROWS 25
#define VGA_TEXT_COLS 80
#define VGA_MEMORY 0xB8000

/* VGA Colors */
enum vga_color {
    VGA_BLACK = 0,
    VGA_BLUE = 1,
    VGA_GREEN = 2,
    VGA_CYAN = 3,
    VGA_RED = 4,
    VGA_MAGENTA = 5,
    VGA_BROWN = 6,
    VGA_LIGHT_GREY = 7,
    VGA_DARK_GREY = 8,
    VGA_LIGHT_BLUE = 9,
    VGA_LIGHT_GREEN = 10,
    VGA_LIGHT_CYAN = 11,
    VGA_LIGHT_RED = 12,
    VGA_LIGHT_MAGENTA = 13,
    VGA_YELLOW = 14,
    VGA_WHITE = 15,
};

/* Macro to create color attribute (fg = text, bg = background) */
#define VGA_COLOR(fg, bg) ((bg << 4) | (fg & 0x0F))

/* Function prototypes */
void vga_init_text(void);
void vga_clear(void);
void vga_putc_text(int row, int col, char c, uint8_t color);
void vga_write_text(int row, int col, const char *txt, uint8_t color);

#endif
