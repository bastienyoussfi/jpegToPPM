#ifndef __YCbCr_to_RGB__
#define __YCbCr_to_RGB_H__

#include <stdint.h>
#include <stdbool.h>

extern void YCbCr_to_RGB(uint8_t Y[8][8], uint8_t Cb[8][8], uint8_t Cr[8][8], uint8_t bloc_RGB[8][8][3]);

extern void print_RGB(uint8_t bloc_RGB[8][8][3]);

#endif