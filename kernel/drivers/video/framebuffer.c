#include <drivers/video/framebuffer.h>
#include <klibc/string.h>
#include <stddef.h>
#include <stdarg.h>
#include <limine_requests.h>
#include <drivers/video/font8x16.h>

static fb_console_t console;

void
fb_init(void)
{
        if (framebuffer_request.response == NULL || 
                framebuffer_request.response->framebuffer_count < 1) {
                return;
        }
        
        console.fb = framebuffer_request.response->framebuffers[0];
        console.fg_color = 0xFFFFFF; // White
        console.bg_color = 0x000000; // Black
        console.cursor_x = 0;
        console.cursor_y = 0;
        console.cols = console.fb->width / FB_CHAR_WIDTH;
        console.rows = console.fb->height / FB_CHAR_HEIGHT;
        
        fb_clear();
}

void
fb_clear(void)
{
        if (!console.fb) return;
        
        uint32_t *fb = (uint32_t *)console.fb->address;
        size_t pixels = (console.fb->pitch / 4) * console.fb->height;
        
        for (size_t i = 0; i < pixels; i++) {
                fb[i] = console.bg_color;
        }
        
        console.cursor_x = 0;
        console.cursor_y = 0;
}

void
fb_draw_rect(uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint32_t color)
{
        if (!console.fb) return;
        
        uint32_t *fb = (uint32_t *)console.fb->address;
        uint32_t pitch_pixels = console.fb->pitch / 4;
        
        for (uint32_t row = y; row < y + h && row < console.fb->height; row++) {
                for (uint32_t col = x; col < x + w && col < console.fb->width; col++) {
                        fb[row * pitch_pixels + col] = color;
                }
        }
}

void
fb_set_color(uint32_t fg, uint32_t bg)
{
        console.fg_color = fg;
        console.bg_color = bg;
}

static void
draw_char(uint32_t x, uint32_t y, char c, uint32_t fg_color, uint32_t bg_color)
{
        if (!console.fb) return;
        
        uint32_t *fb = (uint32_t *)console.fb->address;
        uint32_t pitch_pixels = console.fb->pitch / 4;
        
        // Get the font data for this character
        const uint8_t *char_data = &IBM_VGA_8x16[c * 16];
        
        // Draw each row of the character
        for (uint32_t row = 0; row < FB_CHAR_HEIGHT; row++) {
                uint8_t font_row = char_data[row];
                
                // Draw each pixel in the row
                for (uint32_t col = 0; col < FB_CHAR_WIDTH; col++) {
                        uint32_t px = x + col;
                        uint32_t py = y + row;
                    
                        // Check bounds
                        if (px >= console.fb->width || py >= console.fb->height) continue;
                        
                        // Check if this pixel should be set (bit test)
                        if (font_row & (0x80 >> col)) {
                                fb[py * pitch_pixels + px] = fg_color;
                        } else {
                                fb[py * pitch_pixels + px] = bg_color;
                        }
                }
        }
}

static void
scroll_up(void)
{
        if (!console.fb) return;
        
        uint32_t *fb = (uint32_t *)console.fb->address;
        uint32_t pitch_pixels = console.fb->pitch / 4;
        
        // Move all lines up by one character height
        for (uint32_t y = 0; y < console.fb->height - FB_CHAR_HEIGHT; y++) {
                for (uint32_t x = 0; x < console.fb->width; x++) {
                        fb[y * pitch_pixels + x] = fb[(y + FB_CHAR_HEIGHT) * pitch_pixels + x];
                }
        }
        
        // Clear the last line
        for (uint32_t y = console.fb->height - FB_CHAR_HEIGHT; y < console.fb->height; y++) {
                for (uint32_t x = 0; x < console.fb->width; x++) {
                        fb[y * pitch_pixels + x] = console.bg_color;
                }
        }
}

void
fb_putchar(char c)
{
        if (!console.fb) return;
        
        // Handle special characters
        switch (c) {
                case '\n':
                        console.cursor_x = 0;
                        console.cursor_y++;
                        break;
                case '\r':
                        console.cursor_x = 0;
                        break;
                case '\t':
                        console.cursor_x = (console.cursor_x + 8) & ~7;
                        break;
                case '\b':
                        if (console.cursor_x > 0) {
                                console.cursor_x--;
                                draw_char(console.cursor_x * FB_CHAR_WIDTH, 
                                          console.cursor_y * FB_CHAR_HEIGHT, 
                                          ' ', console.fg_color, console.bg_color);
                        }
                        break;
                default:
                        // Draw the character
                        draw_char(console.cursor_x * FB_CHAR_WIDTH, 
                                  console.cursor_y * FB_CHAR_HEIGHT, 
                                  c, console.fg_color, console.bg_color);
                        console.cursor_x++;
                        break;
        }
        
        // Handle line wrapping
        if (console.cursor_x >= console.cols) {
                console.cursor_x = 0;
                console.cursor_y++;
        }
        
        // Handle scrolling
        if (console.cursor_y >= console.rows) {
                scroll_up();
                console.cursor_y = console.rows - 1;
        }
}

void
fb_puts(const char *str)
{
        while (*str) {
                fb_putchar(*str++);
        }
}

static void
print_number(unsigned long num, int base, int width, char pad)
{
        char digits[] = "0123456789ABCDEF";
        char buffer[32];
        int pos = 0;
        
        // Convert number to string (reversed)
        if (num == 0) {
                buffer[pos++] = '0';
        } else {
                while (num > 0) {
                        buffer[pos++] = digits[num % base];
                        num /= base;
                }
        }
        
        // Add padding if needed
        while (pos < width) {
                buffer[pos++] = pad;
        }
        
        // Print reversed
        while (pos > 0) {
                fb_putchar(buffer[--pos]);
        }
}

static void
print_signed(long num, int width, char pad)
{
        if (num < 0) {
                fb_putchar('-');
                print_number(-num, 10, width, pad);
        } else {
                print_number(num, 10, width, pad);
        }
}

void
fb_printf(const char *fmt, ...)
{
        va_list args;
        va_start(args, fmt);
        
        while (*fmt) {
                if (*fmt == '%') {
                        fmt++;
                        
                        // Handle width and padding
                        char pad = ' ';
                        int width = 0;
                        
                        if (*fmt == '0') {
                                pad = '0';
                                fmt++;
                        }
                        
                        while (*fmt >= '0' && *fmt <= '9') {
                                width = width * 10 + (*fmt - '0');
                                fmt++;
                        }
                        
                        // Handle format specifiers
                        switch (*fmt) {
                                case 'd':
                                case 'i':
                                        print_signed(va_arg(args, int), width, pad);
                                        break;
                                    
                                case 'u':
                                        print_number(va_arg(args, unsigned int), 10, width, pad);
                                        break;
                                    
                                case 'x':
                                        print_number(va_arg(args, unsigned int), 16, width, pad);
                                        break;
                                    
                                case 'X':
                                        print_number(va_arg(args, unsigned int), 16, width, pad);
                                        break;
                                    
                                case 'p':
                                        fb_puts("0x");
                                        print_number((unsigned long)va_arg(args, void*), 16, 16, '0');
                                        break;
                                    
                                case 'c':
                                        fb_putchar(va_arg(args, int));
                                        break;
                                    
                                case 's':
                                        {
                                                const char *str = va_arg(args, const char*);
                                                if (str) {
                                                        fb_puts(str);
                                                } else {
                                                        fb_puts("(null)");
                                                }
                                        }
                                        break;
                                    
                                case '%':
                                        fb_putchar('%');
                                        break;
                                    
                                default:
                                        fb_putchar('%');
                                        fb_putchar(*fmt);
                                        break;
                        }
                } else {
                        fb_putchar(*fmt);
                }
                fmt++;
        }
        
        va_end(args);
}
