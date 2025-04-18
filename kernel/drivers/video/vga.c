#include <drivers/video/vga.h>
#include <string.h>

size_t term_row;
size_t term_col;
uint8_t term_color;
uint16_t* term_buf = (uint16_t*)VGA_MEMORY;

void
term_init(void)
{
        term_row = 0;
        term_col = 0;
        term_color = vga_entry_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK);

        for (size_t y = 0; y < VGA_HEIGHT; y++) {
                for (size_t x = 0; x < VGA_WIDTH; x++) {
                        const size_t index = y * VGA_WIDTH + x;
                        term_buf[index] = vga_entry(' ', term_color);
                }
        }
}

void
term_setcolor(uint8_t color)
{
        term_color = color;
}

void
term_putentryat(char c, uint8_t color, size_t x, size_t y)
{
         const size_t index = y * VGA_HEIGHT + x;
         term_buf[index] = vga_entry(c, color);
}

void
term_putchar(char c)
{
        term_putentryat(c, term_color, term_row);
        if (++term_col == VGA_WIDTH) {
                term_col = 0;
                if (++term_row == VGA_HEIGHT) {
                        term_row = 0;
                }
        }
}

void
term_writestr(const char* data)
{
        for (size_t i = 0; i < strlen(data); i++) {
                term_putchar(data[i]);
        }
}
