#ifndef VGA_H
#define VGA_H

#include <stdint.h>

// VGA mem addr
#define VGA_TEXT_BUFFER 0xB8000      // Text mode video memory
#define VGA_GRAPHICS_BUFFER 0xA0000  // Graphics mode video memory

// VGA mode 13h dimensions
#define VGA_WIDTH 320
#define VGA_HEIGHT 200

// VGA register IO ports 
#define VGA_AC_INDEX 0x3C0        // attribute controller index
#define VGA_AC_WRITE 0x3C0        // attribute controller write
#define VGA_AC_READ 0x3C1         // attribute controller read
#define VGA_MISC_WRITE 0x3C2      // miscellaneous output register
#define VGA_SEQ_INDEX 0x3C4       // sequencer index
#define VGA_SEQ_DATA 0x3C5        // sequencer data
#define VGA_GC_INDEX 0x3CE        // graphics controller index
#define VGA_GC_DATA 0x3CF         // graphics controller data
#define VGA_CRTC_INDEX 0x3D4      // CRT controller index
#define VGA_CRTC_DATA 0x3D5       // CRT controller data
#define VGA_INSTAT_READ 0x3DA     // input status read

void vga_set_mode_13h(void);
void vga_put_pixel(uint16_t x, uint16_t y, uint8_t color);
void vga_clear_screen(uint8_t color);

#endif
