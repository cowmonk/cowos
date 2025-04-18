#include <drivers/video/vga.h>
#include <string.h>

void
kernel_main(void)
{
        term_init();

        term_writestr("Hello World!\n");
}
