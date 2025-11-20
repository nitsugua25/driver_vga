#ifndef FONT_H
#define FONT_H

#include <stdint.h>

// Return pointer to 8-byte bitmap for an ASCII character.
// Only A–Z and space are supported.
const uint8_t* font_get(char c);

#endif
