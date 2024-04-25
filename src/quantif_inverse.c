#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>


/* Multiplie chaque coefficient du bloc par le coefficient de la table de
quantification correspondante. La table de quantification est en paramètre car elle dépend
de la composante traitée (Y, Cb, Cr). On la choisit avec la fonction jpeg_get_quantization_table
qui dépend de l'indice i de la composante. */

void quantif_inv(uint8_t table_quantif[64], int32_t bloc_frequentiel[64], int32_t bloc_apres_quantif[64])
{
    for(uint8_t i = 0; i < 64; i++){
            bloc_apres_quantif[i] = bloc_frequentiel[i]*table_quantif[i];
        }
}
