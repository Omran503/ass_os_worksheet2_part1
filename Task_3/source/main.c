#include <stdint.h>  // Provides standard fixed-width integer definitions

/*  
 * External Framebuffer API
 *   */
extern void fb_write_string(const char *str, uint8_t fg, uint8_t bg);
extern void fb_clear(uint8_t bg);
extern void fb_move_cursor(uint16_t x, uint16_t y);
extern void fb_write_number(int number, uint8_t fg, uint8_t bg);

/*  
 * Demonstrates the framebuffer interface exported by the kernel. This
 * routine exercises string output, cursor relocation, and integer printing
 * in VGA text mode. Once output is complete, the CPU enters an infinite
 * loop to prevent returning from the kernel entry point.
 *   */
void main() {
    /* Clear the screen using a blue background attribute (0x1). */
    fb_clear(0x1);

    /* Write the initial greeting at the current cursor position.
     * Foreground: white (0xF), Background: blue (0x1).
     */
    fb_write_string("Welcome to my Tiny OS!", 0xF, 0x1);

    /* Reposition the cursor to (10, 5) and print a message.
     * Foreground: yellow (0xE), Background: blue (0x1).
     */
    fb_move_cursor(10, 5);
    fb_write_string("Cursor moved here!", 0xE, 0x1);

    /* Move the cursor to (15, 7) and render a numeric value.
     * Foreground: red (0xC), Background: blue (0x1).
     */
    fb_move_cursor(15, 7);
    fb_write_number(12345, 0xC, 0x1);

    /* Write an additional message on the next line.
     * Foreground: green (0xA), Background: blue (0x1).
     */
    fb_write_string("\nEnjoy your new framebuffer features!", 0xA, 0x1);

    /* Prevent exit from the kernel by entering an infinite loop. */
    while (1);
}
