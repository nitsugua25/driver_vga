#include "vga.h"
#include <stdio.h>

int main() {
    vga_color_t color = { .fg = 15, .bg = 0 }; // White on black
    vga_init(color);

    vga_write(0, 0, "HELLO");
    vga_write(1, 0, "WORLD");

    if (!framebuffer_to_bmp("output.bmp"))
        printf("BMP saved: output.bmp\n");
    else
        printf("Failed to save BMP\n");

    return 0;
}
