#ifndef __ECHANTILLONNAGE_H__
#define __ECHANTILLONNAGE_H__

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

extern void sur_echantillonnage_horizontal_mcu(uint8_t mcu[3][4][8][8], uint8_t Y[2][8][8], uint8_t Cb[2][8][8], uint8_t Cr[2][8][8], int nb_composantes_Y);

extern void sur_echantillonnage_vertical_mcu(uint8_t mcu[3][4][8][8], uint8_t Y[2][8][8], uint8_t Cb[2][8][8], uint8_t Cr[2][8][8], int nb_composantes_Y);

#endif
