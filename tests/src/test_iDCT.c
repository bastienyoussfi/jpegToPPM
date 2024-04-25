#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include "../../include/matrices.h"
#include "../../include/iDCT.h"


/* La fonction main teste le module iDCT sur une matrice 8x8 quelconque. */

int main(void)
{
    uint8_t bloc_spatial[8][8];
    int32_t matrice_bloc[8][8];
    for(int16_t i = 0; i < 8; i++) {
        for(int16_t j = 0; j < 8; j++) {
            matrice_bloc[i][j] = i+j;
        }
    }
    iDCT_un_bloc(matrice_bloc, bloc_spatial);
    printf("Le bloc fréquentiel, avant transformée en cosinus inverse est :\n");
    print_matrix_int32_88(matrice_bloc, 8, 8);
    printf("\n");
    printf("Le bloc spatial, après transformée en cosinus inverse est :\n");
    print_matrix_uint8(bloc_spatial, 8, 8);
    return EXIT_SUCCESS;
}
