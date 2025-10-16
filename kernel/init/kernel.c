#include <gdt.h>
#include <klibc/string.h>
#include <stddef.h>
#include <bootloader.h>
#include <drivers/video/framebuffer.h>
#include <mm/paging.h>

static void
hcf(void)
{
   	for (;;) {
        	__asm__("hlt");
    	}
}

void
kernel_main(void)
{
        /* Make sure the bootloader understands our base revision */
        if (LIMINE_BASE_REVISION_SUPPORTED == 0) hcf();
	
        /* Initialize framebuffer console */
        fb_init();
        map_vga_memory();
        
        /* Test print */
        fb_puts("Hello World\n");
        
        initGDT();

        /* done so now hang */
        hcf();
}
