#ifndef VGA_H
#define VGA_H

#include <stdint.h>
#include "framebuffer.h"
#include "font.h"

#define VGA_TEXT_ROWS 25
#define VGA_TEXT_COLS 80
#define CHAR_WIDTH 8
#define CHAR_HEIGHT 8

// Text mode
void vga_init_text(framebuffer_t *fb);
void vga_putc_text(int row, int col, char c, uint8_t color, framebuffer_t *fb);
void vga_write_text(int row, int col, const char *txt, uint8_t color, framebuffer_t *fb);

// Graphics mode
void vga_draw_char(framebuffer_t *fb, int x, int y, char c); // draws using font.c
void vga_draw_string(framebuffer_t *fb, int x, int y, const char *txt);

#endif
