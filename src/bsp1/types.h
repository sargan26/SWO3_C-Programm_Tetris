#include <stdbool.h>

#ifndef TYPES_H
#define TYPES_H

typedef enum {
    color_black   = 0,
    color_red     = 255U <<  0U,
    color_green   = 255U <<  8U,
    color_blue    = 255U << 16U,
    color_yellow  = color_red   | color_green,
    color_magenta = color_red   | color_blue,
    color_cyan    = color_green | color_blue,
    color_white   = color_red   | color_green | color_blue,
} color;

typedef struct {
    int x, y;
} position;

typedef struct {
    position pos;
    bool matrix[4][4];
    color color;
} block;

#endif
