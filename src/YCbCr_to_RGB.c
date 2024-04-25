#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "../include/YCbCr_to_RGB.h"

void YCbCr_to_RGB(uint8_t Y[8][8], uint8_t Cb[8][8], uint8_t Cr[8][8], uint8_t bloc_RGB[8][8][3])
{
    // Renvoie des pixels RGB à partir des blocs Y, Cb et Cr.
    for (uint8_t x = 0; x < 8; x++) {
        for (uint8_t y = 0; y < 8; y++) {
            // Calcul des piexels
            float R = Y[x][y] - 0.0009267 * (Cb[x][y] - 128) + 1.4016868 * (Cr[x][y] - 128);
            float G = Y[x][y] - 0.3436954 * (Cb[x][y] - 128) - 0.7141690 * (Cr[x][y] - 128);
            float B = Y[x][y] + 1.7721604 * (Cb[x][y] - 128) + 0.0009902 * (Cr[x][y] - 128);
            // Saturation de R.
            if (R < 0) {
                R = 0;
            }else if(R >255) {
                R = 255;
            }
            // Conversion en entier non signé de R.
            uint8_t R_entier = R;
            bloc_RGB[x][y][0] = R_entier;

            // Saturation de G.
            if (G < 0) {
                G = 0;
            }else if(G >255) {
                G = 255;
            }
            // Conversion en entier non signé G.
            uint8_t G_entier = G;
            bloc_RGB[x][y][1] = G_entier;

            // Saturation de B.
            if (B < 0) {
                B = 0;
            }else if(B >255) {
                B = 255;
            }
            // Conversion en entier non signé B.
            uint8_t B_entier = B;
            bloc_RGB[x][y][2] = B_entier;
        }
    }
}

void print_RGB(uint8_t bloc_RGB[8][8][3])
{
    // Permet d'afficher le bloc_RGB.
    for(int i = 0; i < 8; i++){
        printf("\n");
        for(int j = 0; j < 8; j++) {
            printf("%d ", bloc_RGB[i][j][0]);
            printf("%d ", bloc_RGB[i][j][1]);
            printf("%d ", bloc_RGB[i][j][2]);
            printf("   ");
        }
    }
    printf("\n");
}
