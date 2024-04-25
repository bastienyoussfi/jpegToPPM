#ifndef __ZIG_ZAG_INVERSE_H__
#define __ZIG_ZAG_INVERSE_H__

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

/* Prend un vecteur de 64 valeurs et renvoie la matrice 8x8 obtenu en faisant
la méthode de zig-zag inverse.*/

extern void zig_zag_inv(int32_t in[64], int32_t out[8][8]);

#endif
