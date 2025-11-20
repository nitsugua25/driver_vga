#include "framebuffer.h"

void framebuffer_init(framebuffer_t *fb, uint32_t bar, uint32_t width, uint32_t height) {
    fb->width = width;
    fb->height = height;
    fb->buffer = (uint8_t *)bar; // direct mapping in VM
}

void framebuffer_set_pixel(framebuffer_t *fb, uint32_t x, uint32_t y, uint8_t color) {
    if (x >= fb->width || y >= fb->height) return;
    fb->buffer[y * fb->width + x] = color;
}

void framebuffer_set_pixel_rgb(framebuffer_t *fb, uint32_t x, uint32_t y, uint8_t r, uint8_t g, uint8_t b) {
    if (x >= fb->width || y >= fb->height) return;
    uint8_t *p = &fb->buffer[(y * fb->width + x) * 3];
    p[0] = r;
    p[1] = g;
    p[2] = b;
}
