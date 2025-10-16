#include <drivers/video/vga.h>

// Helper functions for port IO
static inline void
outb(uint16_t port, uint8_t val)
{
        __asm__ volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
}

static inline uint8_t
inb(uint16_t port)
{
        uint8_t ret;
        __asm__ volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
        return ret;
}

// Write vga registers for mode change
static void
write_regs(uint8_t *regs)
{
        unsigned i;

        // Write misc output register
        outb(VGA_MISC_WRITE, *regs);
        regs++;
        
        // Write sequencer registers (5 registers)
        for (i = 0; i < 5; i++) {
                outb(VGA_SEQ_INDEX, i);
                outb(VGA_SEQ_DATA, *regs);
                regs++;
        }
        
        // Unlock CRTC registers 0-7 by clearing protect bit
        outb(VGA_CRTC_INDEX, 0x03);
        outb(VGA_CRTC_DATA, inb(VGA_CRTC_DATA) | 0x80);
        outb(VGA_CRTC_INDEX, 0x11);
        outb(VGA_CRTC_DATA, inb(VGA_CRTC_DATA) & ~0x80);
        
        // Update the register values to match
        regs[0x03] = regs[0x03] | 0x80;
        regs[0x11] = regs[0x11] & ~0x80;
        
        // Write CRTC registers (25 registers)
        for (i = 0; i < 25; i++) {
                outb(VGA_CRTC_INDEX, i);
                outb(VGA_CRTC_DATA, *regs);
                regs++;
        }
        
        // Write graphics controller registers (9 registers)
        for (i = 0; i < 9; i++) {
                outb(VGA_GC_INDEX, i);
                outb(VGA_GC_DATA, *regs);
                regs++;
        }
        
        // Write attribute controller registers (21 registers)
        for (i = 0; i < 21; i++) {
                inb(VGA_INSTAT_READ);  // Reset flip-flop
                outb(VGA_AC_INDEX, i);
                outb(VGA_AC_WRITE, *regs);
                regs++;
        }
        
        // Enable video display
        inb(VGA_INSTAT_READ);
        outb(VGA_AC_INDEX, 0x20);
}

void
vga_set_mode_13h(void)
{
    	// Register values for VGA mode 13h (320x200, 256 colors)
    	// Order: Misc, Seq[5], CRTC[25], GC[9], AC[21] 
    	static uint8_t mode_320x200x256[] = {
    	        // Misc output register
    	        0x63,
    	        // Sequencer registers
    	        0x03, 0x01, 0x0F, 0x00, 0x0E,
    	        // CRTC registers
    	        0x5F, 0x4F, 0x50, 0x82, 0x54, 0x80, 0xBF, 0x1F,
    	        0x00, 0x41, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    	        0x9C, 0x0E, 0x8F, 0x28, 0x40, 0x96, 0xB9, 0xA3,
    	        0xFF,
    	        // Graphics controller registers 
    	        0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x05, 0x0F,
    	        0xFF,
    	        // Attribute controller registers 
    	        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
    	        0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
    	        0x41, 0x00, 0x0F, 0x00, 0x00
    	};
    	
    	write_regs(mode_320x200x256);
}

void
vga_put_pixel(uint16_t x, uint16_t y, uint8_t color)
{
    	// check bounds 
    	if (x >= VGA_WIDTH || y >= VGA_HEIGHT) return;
    	
    	// Direct write to video memory: offset = y * 320 + x
    	uint8_t *vga = (uint8_t *)VGA_GRAPHICS_BUFFER;
    	vga[y * VGA_WIDTH + x] = color;
}

void
vga_clear_screen(uint8_t color)
{
    	uint8_t *vga = (uint8_t *)VGA_GRAPHICS_BUFFER;
    	// fill entire buffer (64k pixels)
    	for (int i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++) {
    	        vga[i] = color;
    	}
}
