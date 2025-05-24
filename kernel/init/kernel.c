#include <klibc/string.h>
/* #include <drivers/video/vga.h> */
#include <stddef.h>
#include <bootloader.h>

static void hcf(void) {
    for (;;) {
        __asm__("hlt");
    }
}

void
kernel_main(void)
{
        /* Make sure the bootloader understands our base revision */
        if (LIMINE_BASE_REVISION_SUPPORTED == 0) hcf();

        /* ensure a framebuffer */
        if (framebuffer_request.response == NULL
                || framebuffer_request.response->framebuffer_count < 1) hcf();

        /* fetch the first framebuffer */
        struct limine_framebuffer *framebuffer = framebuffer_request.response->framebuffers[0];

        /* assume framebuffer model is RGB w/32-bit pixels */
        for (size_t i = 0; i < 100; i++) {
                volatile uint32_t *fb_ptr = framebuffer->address;
                fb_ptr[i * (framebuffer->pitch / 4) + i] = 0xffffff;
        }

        /* need to get an actual framebuffer driver working before we can use vga */

        /*
        term_init();

        term_writestr("Hello World!");
        */

        /* done so now hang */
        hcf();
}
