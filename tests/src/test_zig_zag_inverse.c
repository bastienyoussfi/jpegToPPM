#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "../../include/matrices.h"
#include "../../include/zig_zag_inverse.h"


/* La fonction main teste le module zig-zag inverse sur une matrice quelconque. */

int main(void)
{
    int32_t test[64];
    for(int i = 1; i < 65; i++){
        test[i-1] = i;
    }
    int32_t sortie[8][8];
    zig_zag_inv(test, sortie);
    printf("Le vecteur en entrée est :\n");
    print_array_int32(test, 64);
    printf("\nLa matrice après zig-zag inverse est :\n");
    print_matrix_int32_88(sortie, 8, 8);
    return EXIT_SUCCESS;
}
