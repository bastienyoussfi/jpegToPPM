#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include "../include/matrices.h"


float coefficient_C(float x)
//Calcule le coefficient C pour un réel donné.
{
    if (x == 0) {
        return 1/sqrt(2);
    }else {
        return 1;
    }
}

void iDCT_un_bloc(int32_t bloc_frequentiel[8][8], uint8_t bloc_spatial[8][8])
// Calcule S(x,y) le bloc spatial, pour un bloc fréquentiel donné.
// avec x et y coordonnées spatiales et lambda et mu coordonnées fréquentielles.
{
    for (int8_t y = 0; y < 8; y++) {
        for (int8_t x = 0; x < 8; x++) {
            float S_xy = 0;
            for (int8_t lambda = 0; lambda < 8; lambda++){
                float alpha = cosf(((2 * x + 1) * lambda * M_PI)/16);
                for (int8_t mu = 0; mu < 8; mu++) {
                    float beta = cosf(((2 * y + 1) * mu * M_PI)/16);
                    S_xy += coefficient_C(lambda) * coefficient_C(mu) * alpha * beta *(bloc_frequentiel[lambda][mu]);
                }
            }
            S_xy = S_xy/(sqrt(16));
        // Rajout du offset
        S_xy += 128;
        // Saturation
        if (S_xy < 0) {
            S_xy = 0;
        }else if(S_xy > 255) {
            S_xy = 255;
        }
        // Conversion en entier non signé
        bloc_spatial[x][y] = (uint8_t) S_xy;
        }
    }
}

static void print_bloc_spatial(uint8_t bloc[8][8])
// Affiche le bloc spatial.
{
    for(int i = 0; i < 8; i++) {
        printf("\n");
        for(int j = 0; j < 8; j++) {
            printf("%d ", bloc[i][j]);
        }
    }
    printf("\n");
}


static void print_bloc_frequentiel(int16_t bloc[8][8])
// Affiche le bloc fréquentiel.
{
    for(int i = 0; i < 8; i++) {
        printf("\n");
        for(int j = 0; j < 8; j++) {
            printf("%d ", bloc[i][j]);
        }
    }
    printf("\n");
}
