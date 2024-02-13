#include "console.h"
#include "common.h"

// VGA buffer addr
static uint16_t *video_memory = (uint16_t *)0xB8000;

// cursor loc
// screen size is 80*25, each char use 2 Bytes
static uint8_t cursor_x = 0;
static uint8_t cursor_y = 0;
static uint8_t screen_x = 80;
static uint8_t screen_y = 25;

static uint8_t backspace = 0x08;
static uint8_t tab = 0x09;
static uint8_t space = 0x20;

#define MAX_INT_LEN 1024 

static void move_cursor() {
    uint16_t cursorLocation = cursor_y * 80 + cursor_x;
    // In VGA, 0x3D4 port used to set VGA internal register index
    // then, 0x3D5 port used to set VGA internal register value 
    // register 14 means cursor low 8 bit
    // register 15 means cursor high 8 bit
    outb(0x3D4, 14);    // tell VGA to set cursor's high bit
    outb(0x3D5, cursorLocation >> 8);
    outb(0x3D4, 15);
    outb(0x3D5, cursorLocation);
}


// clear console
void console_clear() {
    // use space with white backgroud and black font to full screen
    uint8_t color = (rc_black << 4) | (rc_white & 0x0F);
    uint16_t blank = space | (color << 8);

    for (int i = 0; i < screen_x * screen_y; i++) {
        video_memory[i] = blank;
    }

    cursor_x = 0;
    cursor_y = 0;
    move_cursor();
}

static void scroll() {
    // move back 24 line data upper 1 line and clear last line
    uint8_t color = (rc_black << 4) | (rc_white & 0x0F);
    uint16_t blank = space | (color << 8);

    if (cursor_y >= screen_y) {
        for (int i = 0 * screen_x; i < screen_x * (screen_y - 1); i++) {
            // move data upper 1 line
            video_memory[i] = video_memory[i+screen_x];
        }
        for (int i = (screen_y - 1)*screen_x; i < screen_x * screen_y; i++) {
            video_memory[i] = blank;
        }
        cursor_y = 24;
        move_cursor();
    }

}

// output a char with color
void console_putc_color(char c, real_color_t back, real_color_t fore) {
    uint8_t color = (back << 4) | (fore & 0x0F);
    uint16_t ch = c | (color << 8);

    if (c == backspace && cursor_x) {
        cursor_x--;
    } else if (c == tab) {
        cursor_x = (cursor_x+8) & (8 - 1);
    } else if (c == '\r') {
        cursor_x = 0;
    } else if (c == '\n') {
        cursor_y++;
        cursor_x = 0;
    } else if (c >=  ' ') {
        video_memory[cursor_x + screen_x * cursor_y] = ch;
        cursor_x++;
    }

    // switch line
    if (cursor_x >= screen_x) {
        cursor_x = 0;
        cursor_y++;
    }

    scroll();

    move_cursor();
}

// output a string end with \0, default black background and white font
void console_write(char *cstr) {
    console_write_color(cstr, rc_black, rc_white);
}

void int2str(uint32_t n, char *cstr, uint8_t radix) {
    int len, copy_n = n;
    for (len = 0; copy_n > 0; len++) {
        copy_n /= radix;
    }

    if (len == 0) {
        cstr[0] = '0';
        cstr[1] = '\0';
        return;
    }

    cstr[len] = '\0';
    while (len > 0) {
        int reminder = n % radix;
        char ch;
        if (reminder >= 10) {
            ch = 'F' - (15 - reminder);
        } else {
            ch = '0' + reminder;
        }
        cstr[len - 1] = ch;
        n /= radix;
        len--;
    }
}

void decimal2str(double n, char *cstr, uint8_t fiexed) {
    // integer part
    int integer = (int)n;
    int len, copy_n = n;
    for (len = 0; copy_n > 0; len++) {
        copy_n /= 10;
    }

    if (len == 0) {
        cstr[0] = '0';
        len += 1;
        cstr[len] = '.';
    } else {
        int2str(integer, cstr, 10);
        cstr[len] = '.';
    }

    // decimal part
    for (int idx = 1; idx <= fiexed; idx++) {
        n *= 10;
        cstr[len+idx] = '0' + ((int)n) % 10; 
    }

}

// output a string end with \0 with color
void console_write_color(char *cstr, real_color_t back, real_color_t fore) {
    while (*cstr) {
        console_putc_color(*cstr++, back, fore);
    }
}

// output a hex int
void console_write_hex(uint32_t n, real_color_t back, real_color_t fore) {
    char cstr[MAX_INT_LEN] = {0};
    int2str(n, cstr, 16);
    console_write_color(cstr, back, fore);
}

// output a dec int
void console_write_dec(uint32_t n, real_color_t back, real_color_t fore) {
    char cstr[MAX_INT_LEN] = {0};
    int2str(n, cstr, 10);
    console_write_color(cstr, back, fore);
}

void console_write_float(double n, real_color_t back, real_color_t fore, uint8_t fiexed) {
    char cstr[MAX_INT_LEN] = {0};
    decimal2str(n, cstr, fiexed);
    console_write_color(cstr, back, fore);
}
