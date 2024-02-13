#ifndef INCLUDE_CONSOLE_H_
#define INCLUDE_CONSOLE_H_

#include "types.h"

typedef
enum real_color {
    rc_black = 0,
    rc_blue = 1,
    rc_green = 2,
    rc_cyan = 3,
    rc_red = 4,
    rc_magenta = 5,
    rc_brown = 6,
    rc_light_grey = 7,
    rc_dark_grey = 8,
    rc_light_blue = 9,
    rc_light_green = 10,
    rc_light_cyan = 11,
    rc_light_red = 12,
    rc_light_magenta = 13,
    rc_light_brown = 14,
    rc_white = 15
} real_color_t;

// clear console
void console_clear();

// output a char with color
void console_putc_color(char c, real_color_t back, real_color_t fore);

// output a string end with \0, default black background and white font
void console_write(char *cstr);

// output a string end with \0 with color
void console_write_color(char *cstr, real_color_t back, real_color_t fore);

// output a hex int
void console_write_hex(uint32_t n, real_color_t back, real_color_t fore);

// output a dec int
void console_write_dec(uint32_t n, real_color_t back, real_color_t fore);

// output a decimal
void console_write_float(double n, real_color_t back, real_color_t fore, uint8_t fiexed);

#endif // INCLUDE_CONSOLE_H_