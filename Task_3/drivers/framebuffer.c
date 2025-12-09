#include <stdint.h>
/*  
 * Framebuffer Configuration
 *   */
#define FB_ADDRESS 0x000B8000
#define FB_ROWS    25
#define FB_COLS    80

void fb_scroll();

/* Pointer to VGA text-mode buffer */
volatile char *fb = (volatile char *)FB_ADDRESS;

/* Cursor position within the framebuffer grid */
uint16_t cursor_x = 0, cursor_y = 0;

/*  
 * fb_write_char
 * Writes a single character to the framebuffer using the specified colors.
 * Handles newline logic, boundary checks, and triggers scrolling when the
 * cursor exceeds the visible display area.
 *   */
void fb_write_char(char c, uint8_t fg, uint8_t bg) {
    if (c == '\n') {
        cursor_x = 0;
        cursor_y++;
    } else {
        uint16_t position = cursor_y * FB_COLS + cursor_x;
        fb[2 * position]     = c;             /* Character byte */
        fb[2 * position + 1] = (bg << 4) | fg;/* Attribute byte */
        cursor_x++;
    }

    /* Wrap horizontally */
    if (cursor_x >= FB_COLS) {
        cursor_x = 0;
        cursor_y++;
    }

    /* Trigger screen scroll on vertical overflow */
    if (cursor_y >= FB_ROWS) {
        fb_scroll();
        cursor_y = FB_ROWS - 1;
    }
}

/*  
 * fb_write_string
 * Writes a null-terminated string to the framebuffer by repeatedly calling
 * fb_write_char(), preserving color attributes.
 *   */
void fb_write_string(const char *str, uint8_t fg, uint8_t bg) {
    while (*str) {
        fb_write_char(*str++, fg, bg);
    }
}

/*  
 * fb_move_cursor
 * Relocates the cursor to coordinates (x, y), provided that they fall
 * within the framebuffer's valid bounds.
 *   */
void fb_move_cursor(uint16_t x, uint16_t y) {
    if (x < FB_COLS && y < FB_ROWS) {
        cursor_x = x;
        cursor_y = y;
    }
}

/*  
 * fb_clear
 * Clears the entire framebuffer using the specified background color.
 * Each cell is filled with a space character (' ') and the background
 * attribute. The cursor is reset to (0,0).
 *   */
void fb_clear(uint8_t bg) {
    uint16_t clear_value = (' ' | ((bg & 0x0F) << 12));
    volatile uint16_t *fb16 = (volatile uint16_t *)FB_ADDRESS;

    for (uint16_t i = 0; i < FB_ROWS * FB_COLS; i++) {
        fb16[i] = clear_value;
    }

    fb_move_cursor(0, 0);
}

/*  
 * fb_scroll
 * Scrolls the framebuffer upward by one row:
 *  - Moves rows 1..24 into rows 0..23
 *  - Clears the bottom row using default attributes (white on black)
 *   */
void fb_scroll() {
    volatile uint16_t *fb16 = (volatile uint16_t *)FB_ADDRESS;

    for (uint16_t y = 1; y < FB_ROWS; y++) {
        for (uint16_t x = 0; x < FB_COLS; x++) {
            fb16[(y - 1) * FB_COLS + x] = fb16[y * FB_COLS + x];
        }
    }

    for (uint16_t x = 0; x < FB_COLS; x++) {
        fb16[(FB_ROWS - 1) * FB_COLS + x] = (' ' | (0x0F << 8));
    }
}

/*  
 * fb_write_number
 * Converts a signed integer into its decimal string representation and
 * prints it to the framebuffer. Handles negative numbers and performs an
 * in-place string reversal after digit extraction.
 *   */
void fb_write_number(int number, uint8_t fg, uint8_t bg) {
    char buffer[12]; /* Enough for 32-bit signed int including minus sign */
    int i = 0;

    if (number == 0) {
        buffer[i++] = '0';
    } else {
        if (number < 0) {
            buffer[i++] = '-';
            number = -number;
        }
        while (number > 0) {
            buffer[i++] = (number % 10) + '0';
            number /= 10;
        }
    }
    buffer[i] = '\0';

    /* Reverse numeric portion (preserve leading '-') */
    for (int j = (buffer[0] == '-' ? 1 : 0); j < i / 2; j++) {
        char temp_char = buffer[j];
        buffer[j] = buffer[i - j - 1];
        buffer[i - j - 1] = temp_char;
    }

    fb_write_string(buffer, fg, bg);
}
