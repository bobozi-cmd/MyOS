#include "console.h"
#include "vargs.h"

#define MAX_BUFSIZE 1024
#define FIXED 3

void vprintk(real_color_t back, real_color_t fore, const char * fmt, va_list args);

// kernel print func
void printk(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    vprintk(rc_black, rc_white, fmt, args);
    va_end(args);
}

// kernel print func with color
void printk_color(real_color_t back, real_color_t fore, const char * fmt, ...) {
    va_list args;
    va_start(args, fmt);
    vprintk(back, fore, fmt, args);
    va_end(args);
}

void vprintk(real_color_t back, real_color_t fore, const char * fmt, va_list args) {
    while (*fmt != '\0') {
        if (*fmt == '%') {
            fmt++;
            if (*fmt == 'd') {
                int i = va_arg(args, int);
                if (i < 0) {
                    i = -i;
                    console_putc_color('-', back, fore);
                }
                console_write_dec(i, back, fore);
            } else if (*fmt == 'c') {
                int c = va_arg(args, int);
                console_putc_color((char) c, back, fore);
            } else if (*fmt == 's') {
                char * s = va_arg(args, char *);
                console_write_color(s, back, fore);
            } else if (*fmt == 'f') {
                double d = va_arg(args, double);
                console_write_float(d, back, fore, FIXED);
            } else if (*fmt == '%') {
                console_putc_color('%', back, fore);
            } else if (*fmt == 'x') {
                int i = va_arg(args, int);
                console_write_hex(i, back, fore);
            }
        } else {
            console_putc_color(*fmt, back, fore);
        }
        fmt++;
    }
}

