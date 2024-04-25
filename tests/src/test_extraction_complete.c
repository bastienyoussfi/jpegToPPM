#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "../../include/matrices.h"
#include "../../include/extraction_complete.h"


static uint32_t partie_entiere_superieure(uint16_t a, uint16_t b)
/* Calcule la partie entière supérieure de a/(8*b) */
{
    uint32_t partie_entiere_sup = a/(b*8);
    if (a%(8*b) != 0) {
        partie_entiere_sup = partie_entiere_sup + 1;
    }
    return partie_entiere_sup;
}

/* La fonction main teste le module extraction_complete sur une image,
elle renvoie ses coefficients AC et DC. */

int main(int argc, char** argv)
{

    if (argc != 2) {
        fprintf(stderr, "Usage: %s fichier.jpeg\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    struct jpeg_desc *jpeg = jpeg_read(argv[1]);
    uint32_t hauteur_image = jpeg_get_image_size(jpeg, V);
    uint32_t longueur_image = jpeg_get_image_size(jpeg, H);
    uint8_t nb_composantes = jpeg_get_nb_components(jpeg);
    uint32_t nb_blocs_largeur = partie_entiere_superieure(longueur_image, 1);
    uint32_t nb_blocs_hauteur = partie_entiere_superieure(hauteur_image, 1);
    uint32_t nb_blocs = nb_blocs_hauteur * nb_blocs_largeur;
    uint8_t largeur_MCU = jpeg_get_frame_component_sampling_factor(jpeg, H, 0);
    uint8_t largeur_Cb = jpeg_get_frame_component_sampling_factor(jpeg, H, 1);
    uint8_t largeur_Cr = jpeg_get_frame_component_sampling_factor(jpeg, H, 2);
    uint8_t hauteur_MCU = jpeg_get_frame_component_sampling_factor(jpeg, V, 0);
    uint8_t hauteur_Cb = jpeg_get_frame_component_sampling_factor(jpeg, V, 1);
    uint8_t hauteur_Cr = jpeg_get_frame_component_sampling_factor(jpeg, V, 2);
    uint32_t nb_mcu_largeur = partie_entiere_superieure(longueur_image, largeur_MCU);
    uint32_t nb_mcu_hauteur = partie_entiere_superieure(hauteur_image, hauteur_MCU);
    uint32_t nb_mcu = nb_mcu_hauteur * nb_mcu_largeur;
    uint8_t nb_compo = (largeur_MCU*hauteur_MCU) + (largeur_Cb*hauteur_Cb) + (largeur_Cr*hauteur_Cr);

    if (nb_composantes == 1){
        int32_t **blocs_AC_DC = alloc_matrice_int32(nb_blocs, 64);
        extraction(argv[1], blocs_AC_DC, nb_blocs);
            for (uint32_t i = 0; i < nb_blocs; i++) {
                printf("\nBloc %i :\n", i);
                print_array_int32(blocs_AC_DC[i], 64);
            }
        free_matrice_int32(blocs_AC_DC, nb_blocs);
    } else if (largeur_Cb < largeur_MCU || hauteur_Cb < hauteur_MCU) {
        int32_t ****mcu_AC_DC = alloc_matrice_de_matrices_int32(nb_mcu, 3, nb_compo, 64);
        extraction_couleur_echantillonage(argv[1], mcu_AC_DC, nb_mcu);
        for (uint32_t num_mcu = 0; num_mcu < nb_mcu; num_mcu++) {
            printf("\n\nMCU : %i\n", num_mcu);
            for (uint32_t index = 0; index < 3; index++) {
                printf("Composante : %i\n", index);
                uint8_t largeur_index = jpeg_get_frame_component_sampling_factor(jpeg, H, index);
                uint8_t hauteur_index = jpeg_get_frame_component_sampling_factor(jpeg, V, index);
                uint8_t nb_composantes = largeur_index*hauteur_index;
                for (uint32_t composante_mcu = 0; composante_mcu < nb_composantes; composante_mcu++) {
                    printf("Bloc : %i\n", composante_mcu);
                    print_array_int32(mcu_AC_DC[num_mcu][index][composante_mcu], 64);
                    printf("\n");
                }
            }
        }
        free_matrice_de_matrices_int32(mcu_AC_DC, nb_mcu, 3, nb_compo);
    } else {
        int32_t ***mcu_AC_DC = alloc_vecteur_de_matrices_int32(4, 64, nb_mcu);
        extraction_couleur(argv[1], mcu_AC_DC, nb_mcu);
        for (uint32_t mcu_courant = 0; mcu_courant < nb_mcu; mcu_courant++) {
            printf("\n\nMCU : %i\n", mcu_courant);
            for (uint8_t bloc_courant = 0; bloc_courant < 3; bloc_courant++) {
                printf("Composante : %i\n", bloc_courant);
                print_array_int32(mcu_AC_DC[mcu_courant][bloc_courant], 64);
            }
        }
        free_vecteur_de_matrices_int32(mcu_AC_DC, nb_mcu, 4);
    }
    jpeg_close(jpeg);
    return EXIT_SUCCESS;
}
