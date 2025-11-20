#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <stdint.h>

typedef struct {
    uint32_t width;
    uint32_t height;
    uint8_t *buffer;  // pointer to framebuffer (8-bit or 24-bit)
} framebuffer_t;

// Initialize framebuffer with BAR and resolution
void framebuffer_init(framebuffer_t *fb, uint32_t bar, uint32_t width, uint32_t height);

// Set a pixel (8-bit indexed color)
void framebuffer_set_pixel(framebuffer_t *fb, uint32_t x, uint32_t y, uint8_t color);

// Optional: Set a pixel RGB (24-bit)
void framebuffer_set_pixel_rgb(framebuffer_t *fb, uint32_t x, uint32_t y, uint8_t r, uint8_t g, uint8_t b);

#endif
