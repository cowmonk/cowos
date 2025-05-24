#ifndef FB_H
#define FB_H

/* *pixel = vram + y*pitch + x*pixelwidth */

/*
  | width | how many pixels you have on a horizontal line                     |
  |===========================================================================|
  | height | how many horizontal lines of pixels are present                  |
  |===========================================================================|
  | pitch | how many bytes of VRAM you should skip to go one pixel down       |
  |===========================================================================|
  | depth | how many bits of color you have                                   |
  |===========================================================================|
  | pixelwidth | how many bytes of VRAM you should skip to go one pixel right |
*/

#include <klibc/types.h>

void putpixel(int pos_x, int pos_y, unsigned char VGA_COLOR);

#endif /* FB_H */
