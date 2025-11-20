#include "bmp.h"
#include <stdio.h>

static void write_u32(FILE *f, uint32_t v) {
    uint8_t b[4] = { v, v >> 8, v >> 16, v >> 24 };
    fwrite(b, 1, 4, f);
}

static void write_u16(FILE *f, uint16_t v) {
    uint8_t b[2] = { v, v >> 8 };
    fwrite(b, 1, 2, f);
}

int bmp_write(
    const char *filename,
    int width,
    int height,
    uint8_t pixels[height][width][3]
) {
    FILE *f = fopen(filename, "wb");
    if (!f) return -1;

    int row_size = width * 3;
    int pixel_data_size = row_size * height;

    // --- BMP HEADER ---
    fwrite("BM", 1, 2, f);
    write_u32(f, 54 + pixel_data_size);
    write_u16(f, 0);
    write_u16(f, 0);
    write_u32(f, 54);

    // --- DIB HEADER ---
    write_u32(f, 40);
    write_u32(f, width);
    write_u32(f, height);
    write_u16(f, 1);
    write_u16(f, 24);
    write_u32(f, 0);
    write_u32(f, pixel_data_size);
    write_u32(f, 2835);
    write_u32(f, 2835);
    write_u32(f, 0);
    write_u32(f, 0);

    // --- PIXELS (bottom → top) ---
    for (int y = height - 1; y >= 0; y--) {
        fwrite(pixels[y], 1, row_size, f);
    }

    fclose(f);
    return 0;
}
