#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "../../include/quantif_inverse.h"


/* La fonction main teste le module de quantification inverse sur un vecteur
quelconque et avec une table de quantification créée à la main. */

int main(void){
    int32_t testbloc[64];
    uint8_t testquantif[64];
    for(int i = 0; i < 64; i++) {
        testquantif[i] = 2;
    }
    for(int i = 0; i < 64; i++) {
        testbloc[i] = i;
    }

    printf("\nLe vecteur en entrée est :\n");
    for(int i = 0; i < 64; i++) {
        printf("%d", testbloc[i]);
        printf(" ");
    }

    printf("\n\nLa fausse table de quantification est :\n");
    for(int i = 0; i < 64; i++) {
        printf("%d", testquantif[i]);
        printf(" ");
    }

    int32_t test[64];

    quantif_inv(testquantif, testbloc, test);

    printf("\n\nOn obtient en sortie : :\n");

    for(int i = 0; i < 64; i++) {
        printf("%d", test[i]);
        printf(" ");
    }

    printf("\n");
}
