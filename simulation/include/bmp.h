#ifndef BMP_H
#define BMP_H

#include <stdint.h>

// Write a 24-bit RGB image to a BMP file
// pixels must be an array [height][width][3]
int bmp_write(
    const char *filename,
    int width,
    int height,
    uint8_t pixels[height][width][3]
);

#endif
