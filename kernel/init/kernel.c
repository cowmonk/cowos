#include <gdt.h>
#include <klibc/string.h>
#include <drivers/video/vga.h>
#include <stddef.h>
#include <bootloader.h>

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
	
	vga_set_mode_13h();

	/* initGDT(); */

        /* done so now hang */
        hcf();
}
