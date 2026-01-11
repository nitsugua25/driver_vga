#include "vga.h"

static uint16_t *vga_buffer = (uint16_t *)VGA_MEMORY;
static uint8_t default_color = VGA_COLOR(VGA_LIGHT_GREY, VGA_BLACK);

/* Helper to create VGA entry (character + color) */
static inline uint16_t vga_entry(char c, uint8_t color) {
    return (uint16_t)c | ((uint16_t)color << 8);
}

/* Initialize VGA text mode */
void vga_init_text(void) {
    vga_buffer = (uint16_t *)VGA_MEMORY;
    default_color = VGA_COLOR(VGA_LIGHT_GREY, VGA_BLACK);
}

/* Clear the screen */
void vga_clear(void) {
    for (int y = 0; y < VGA_TEXT_ROWS; y++) {
        for (int x = 0; x < VGA_TEXT_COLS; x++) {
            int index = y * VGA_TEXT_COLS + x;
            vga_buffer[index] = vga_entry(' ', default_color);
        }
    }
}

/* Put a single character at (row, col) */
void vga_putc_text(int row, int col, char c, uint8_t color) {
    if (row < 0 || row >= VGA_TEXT_ROWS || col < 0 || col >= VGA_TEXT_COLS)
        return;
    
    /* Only support A-Z and space as per requirements */
    if (c != ' ' && !(c >= 'A' && c <= 'Z'))
        return;
    
    int index = row * VGA_TEXT_COLS + col;
    vga_buffer[index] = vga_entry(c, color);
}

/* Write a string at (row, col) */
void vga_write_text(int row, int col, const char *txt, uint8_t color) {
    int x = col;
    for (size_t i = 0; txt[i] != '\0'; i++) {
        if (x >= VGA_TEXT_COLS)
            break;
        vga_putc_text(row, x, txt[i], color);
        x++;
    }
}
