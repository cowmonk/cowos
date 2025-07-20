#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <stdint.h>
#include <stddef.h>
#include <limine.h>

#define FB_CHAR_WIDTH  8
#define FB_CHAR_HEIGHT 16

typedef struct {
    uint32_t fg_color;
    uint32_t bg_color;
    uint32_t cursor_x;
    uint32_t cursor_y;
    uint32_t cols;
    uint32_t rows;
    struct limine_framebuffer *fb;
} fb_console_t;

void fb_init(void);
void fb_putchar(char c);
void fb_puts(const char *str);
void fb_clear(void);
void fb_set_color(uint32_t fg, uint32_t bg);
void fb_printf(const char *fmt, ...);
void fb_draw_rect(uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint32_t color);

#endif /* FRAMEBUFFER_H */