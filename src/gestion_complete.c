#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>

#include "../include/quantif_inverse.h"
#include "../include/zig_zag_inverse.h"
#include "../include/iDCT.h"
#include "../include/YCbCr_to_RGB.h"
#include "../include/ecriture.h"
#include "../include/jpeg_reader.h"
#include "../include/matrices.h"
#include "../include/extraction_complete.h"
#include "../include/echantillonnage.h"


static uint32_t partie_entiere_superieure(uint16_t a, uint16_t b)
/* Calcule la partie entière supérieure de a/(8*b) */
{
    uint32_t partie_entiere_sup = a/(b*8);
    if (a%(8*b) != 0) {
        partie_entiere_sup = partie_entiere_sup + 1;
    }
    return partie_entiere_sup;
}


void decode_jpeg(char *filename)
// Decode entièrement une image .jpeg en niveaux de gris afin de retourner une image .pgm.
{
    // Extraction des blocs
    struct jpeg_desc *jpeg = jpeg_read(filename);
    uint32_t hauteur_image = jpeg_get_image_size(jpeg, V);
    uint32_t longueur_image = jpeg_get_image_size(jpeg, H);
    uint8_t *table_quantif = jpeg_get_quantization_table(jpeg, 0);
    uint32_t nb_blocs_largeur = partie_entiere_superieure(longueur_image, 1);
    uint32_t nb_blocs_hauteur = partie_entiere_superieure(hauteur_image, 1);
    uint32_t nb_blocs = nb_blocs_hauteur * nb_blocs_largeur;
    int32_t **blocs_AC_DC = alloc_matrice_int32(nb_blocs, 64);
    extraction(filename, blocs_AC_DC, nb_blocs); // On récupère les coeffs AC et DC dans une matrice appelée "blocs_AC_DC" qui contient nb_blocs tableaux.

    // On décode bloc par bloc en stockant le résultat dans "blocs_finaux".
    uint8_t blocs_finaux[nb_blocs][8][8];

    for(uint32_t num_bloc = 0; num_bloc < nb_blocs; num_bloc++) {
        // Quantification inverse
        int32_t vecteur_quantif[64];
        int32_t *bloc_courant = blocs_AC_DC[num_bloc];
        quantif_inv(table_quantif, bloc_courant, vecteur_quantif); // On récupère le vecteur après la quantification inverse dans "vecteur_quantif".

        // Zig_Zag
        int32_t matrice_zz[8][8];
        zig_zag_inv(vecteur_quantif, matrice_zz); // On récupère la matrice après le zig-zag inverse dans "matrice_zz".

        // iDCT
        iDCT_un_bloc(matrice_zz, blocs_finaux[num_bloc]); // On récupère la matrice après iDCT dans "blocs_finaux[num_blocs]".
    }
    // Ecriture pgm de l'ensemble de nos blocs décodés.
    ecriture_pgm(blocs_finaux, hauteur_image, longueur_image, filename);
    jpeg_close(jpeg);
    free_matrice_int32(blocs_AC_DC, nb_blocs);
}



void decode_jpeg_couleurs(char *filename)
// Decode entièrement une image .jpeg en couleurs afin de retourner une image .pgm ou .ppm.
{
    // Extraction des blocs
    struct jpeg_desc *jpeg = jpeg_read(filename);
    uint16_t hauteur_image = jpeg_get_image_size(jpeg, V);
    uint16_t longueur_image = jpeg_get_image_size(jpeg, H);

    //On récupère les tables de quantification
    uint8_t *tables_quantif[3]={jpeg_get_quantization_table(jpeg, 0), jpeg_get_quantization_table(jpeg, 1), jpeg_get_quantization_table(jpeg, 1)};
    uint32_t nb_mcu_largeur = partie_entiere_superieure(longueur_image, 1);
    uint32_t nb_mcu_hauteur = partie_entiere_superieure(hauteur_image, 1);
    uint32_t nb_mcu = nb_mcu_hauteur * nb_mcu_largeur;

    int32_t ***mcu_AC_DC = alloc_vecteur_de_matrices_int32(4, 64, nb_mcu); // Vecteur contenant une matrice de composantes Y Cb Cr de 64 élements.
    extraction_couleur(filename, mcu_AC_DC, nb_mcu); // On récupère les coeffs AC et DC dans une matrice appelée "mcu_AC_DC" qui contient nb_blocs tableaux.

    // On décode bloc par bloc en stockant le résultat dans "blocs_RGB_finaux".
    uint8_t blocs_RGB_finaux[nb_mcu][8][8][3];
    // On boucle sur les mcu plus les bloc
    for(uint32_t num_mcu = 0; num_mcu < nb_mcu; num_mcu++) {
        // Quantification inverse
        int32_t vecteurs_quantif[3][64];
        for (uint8_t composante_mcu = 0; composante_mcu < 3; composante_mcu++) {
            int32_t *bloc_courant = mcu_AC_DC[num_mcu][composante_mcu];
            // On récupère les vecteurs Y, Cb et Cr après la quantification inverse dans "vecteurs_quantif".
            quantif_inv(tables_quantif[composante_mcu], bloc_courant, vecteurs_quantif[composante_mcu]);
        }

        // Zig_Zag + iDCT
        int32_t matrices_zz[3][8][8];
        uint8_t matrices_idct[3][8][8];
        for (uint8_t composante_mcu = 0; composante_mcu < 3; composante_mcu++){
            // On récupère les matrice après le zig-zag inverse dans "matrices_zz".
            zig_zag_inv(vecteurs_quantif[composante_mcu], matrices_zz[composante_mcu]);
            // On récupère les matrices après iDCT dans "matrices_idct".
            iDCT_un_bloc(matrices_zz[composante_mcu], matrices_idct[composante_mcu]);
        }
        //uint8_t bloc_RGB[8][8][3];
        YCbCr_to_RGB(matrices_idct[0], matrices_idct[1], matrices_idct[2], blocs_RGB_finaux[num_mcu]);
    }
    // Ecriture ppm de l'ensemble de nos blocs décodés.
    ecriture_ppm(blocs_RGB_finaux, hauteur_image,  longueur_image, filename, 0);

    free_vecteur_de_matrices_int32(mcu_AC_DC, nb_mcu, 4);
    jpeg_close(jpeg);
}



void decode_jpeg_couleurs_echantillonnage(char *filename)
// Decode entièrement une image .jpeg en couleurs échantillonnée afin de retourner une image .pgm ou .ppm.
{
    // Extraction des blocs
    struct jpeg_desc *jdesc = jpeg_read(filename);
    uint32_t hauteur_image = jpeg_get_image_size(jdesc, V);
    uint32_t longueur_image = jpeg_get_image_size(jdesc, H);

    //On récupère les tables de quantification
    uint8_t *tables_quantif[3]={jpeg_get_quantization_table(jdesc, 0), jpeg_get_quantization_table(jdesc, 1), jpeg_get_quantization_table(jdesc, 1)};

    /* Calcul du nombre de MCUs : */
    uint8_t largeur_MCU = jpeg_get_frame_component_sampling_factor(jdesc, H, 0);
    uint8_t hauteur_MCU = jpeg_get_frame_component_sampling_factor(jdesc, V, 0);
    uint32_t nb_mcu_largeur = partie_entiere_superieure(longueur_image, largeur_MCU);
    uint32_t nb_mcu_hauteur = partie_entiere_superieure(hauteur_image, hauteur_MCU);
    uint32_t nb_mcu = nb_mcu_hauteur * nb_mcu_largeur;
    int16_t nb_composantes_Y = largeur_MCU*hauteur_MCU;

    /* Calcul du nombre de blocs dans chaque MCU */
    uint8_t largeur_Cb = jpeg_get_frame_component_sampling_factor(jdesc, H, 1);
    uint8_t hauteur_Cb = jpeg_get_frame_component_sampling_factor(jdesc, V, 1);
    uint8_t largeur_Cr = jpeg_get_frame_component_sampling_factor(jdesc, H, 2);
    uint8_t hauteur_Cr = jpeg_get_frame_component_sampling_factor(jdesc, V, 2);
    uint8_t nb_compo = (largeur_MCU*hauteur_MCU) + (largeur_Cb*hauteur_Cb) + (largeur_Cr*hauteur_Cr);
    uint32_t nb_blocs = nb_composantes_Y*nb_mcu;

    /*On procède à l'extraction : */
    int32_t ****mcu_AC_DC = alloc_matrice_de_matrices_int32(nb_mcu, 3, nb_compo, 64); // Vecteur contenant des matrices de composantes Y Cb Cr de 64 élements.
    extraction_couleur_echantillonage(filename, mcu_AC_DC, nb_mcu); // On récupère les coeffs AC et DC dans une matrice appelée "mcu_AC_DC" qui contient nb_mcu tableaux.
    // On décode mcu par mcu en stockant les résultats dans "blocs_RGB_finaux".
    uint8_t blocs_RGB_finaux[nb_blocs][8][8][3];
    // On boucle sur les mcu plus les bloc
    uint16_t compteur = 0; //permet de savoir le num_du bloc courant
    for(uint32_t num_mcu = 0; num_mcu < nb_mcu; num_mcu++) {
        int32_t mcu_quantif[3][4][64];
        int32_t mcu_zz[3][4][8][8];
        uint8_t mcu_idct[3][4][8][8];
        for(uint8_t index = 0; index < 3; index++) {
            uint8_t largeur_index = jpeg_get_frame_component_sampling_factor(jdesc, H, index);
            uint8_t hauteur_index = jpeg_get_frame_component_sampling_factor(jdesc, V, index);
            uint8_t nb_composantes = largeur_index*hauteur_index;
            for (uint8_t composante_mcu = 0; composante_mcu < nb_composantes; composante_mcu++) {
                int32_t *bloc_courant = mcu_AC_DC[num_mcu][index][composante_mcu];
                // Quantification inverse:
                // On récupère les vecteurs Y, Cb et Cr après la quantification inverse dans "vecteurs_quantif".
                quantif_inv(tables_quantif[index], bloc_courant, mcu_quantif[index][composante_mcu]);
                // Zig_Zag:
                // On récupère les matrice après le zig-zag inverse dans "matrices_zz".
                zig_zag_inv(mcu_quantif[index][composante_mcu], mcu_zz[index][composante_mcu]);
                //iDCT:
                // On récupère les matrices après iDCT dans "matrices_idct".
                iDCT_un_bloc(mcu_zz[index][composante_mcu], mcu_idct[index][composante_mcu]);
                if (index == 0 && composante_mcu == 3) {
                }
            }
        }
        // Echantillonnage.
        uint8_t Y_echantillonnage[nb_composantes_Y][8][8];
        uint8_t Cb_echantillonnage[nb_composantes_Y][8][8];
        uint8_t Cr_echantillonnage[nb_composantes_Y][8][8];
        if (largeur_Cb < largeur_MCU) {
            sur_echantillonnage_horizontal_mcu(mcu_idct, Y_echantillonnage, Cb_echantillonnage, Cr_echantillonnage, nb_composantes_Y);
                if(hauteur_Cb < hauteur_MCU) {
                    uint8_t mcu_horizontal[3][nb_composantes_Y][8][8];
                    copie_vecteur_de_matrices(mcu_idct[0], mcu_horizontal[0]);
                    copie_vecteur_de_matrices(Cb_echantillonnage, mcu_horizontal[1]);
                    copie_vecteur_de_matrices(Cr_echantillonnage, mcu_horizontal[2]);
                    sur_echantillonnage_vertical_mcu(mcu_horizontal, Y_echantillonnage, Cb_echantillonnage, Cr_echantillonnage, nb_composantes_Y);
                }
        } else {
            sur_echantillonnage_vertical_mcu(mcu_idct, Y_echantillonnage, Cb_echantillonnage, Cr_echantillonnage, nb_composantes_Y);
        }

        // YCbCr to RGB
        for(uint8_t composante_mcu = 0; composante_mcu < nb_composantes_Y; composante_mcu++) {
            YCbCr_to_RGB(Y_echantillonnage[composante_mcu], Cb_echantillonnage[composante_mcu], Cr_echantillonnage[composante_mcu], blocs_RGB_finaux[compteur]);
            compteur++;
        }

    }
    // Ecriture de l'ensemble de nos bloc RGB décodés.
    if (largeur_Cb < largeur_MCU) {
        if(hauteur_Cb < hauteur_MCU) {
            ecriture_echantillonnage_H_et_V(blocs_RGB_finaux, hauteur_image,  longueur_image, filename);
        }else {
            ecriture_ppm(blocs_RGB_finaux, hauteur_image,  longueur_image, filename, 1);
        }
    } else {
        ecriture_echantillonnage(blocs_RGB_finaux, hauteur_image,  longueur_image, filename);
    }
    free_matrice_de_matrices_int32(mcu_AC_DC, nb_mcu, 3, nb_compo);
    jpeg_close(jdesc);
}



int main(int argc, char** argv)
// On crée notre décodeur jpeg.
{
    if (argc != 2) {
        fprintf(stderr, "Usage: %s fichier.jpeg\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    struct jpeg_desc *jdesc = jpeg_read(argv[1]);
    uint8_t nb_composantes = jpeg_get_nb_components(jdesc);
    uint8_t facteur_h_y = jpeg_get_frame_component_sampling_factor(jdesc, H, 0);
    uint8_t facteur_h_c = jpeg_get_frame_component_sampling_factor(jdesc, H, 1);
    uint8_t facteur_v_y = jpeg_get_frame_component_sampling_factor(jdesc, V, 0);
    uint8_t facteur_v_c = jpeg_get_frame_component_sampling_factor(jdesc, V, 1);
    jpeg_close(jdesc);
    if (nb_composantes == 1){
        decode_jpeg(argv[1]);
    } else if (facteur_h_c < facteur_h_y || facteur_v_c < facteur_v_y) {
        decode_jpeg_couleurs_echantillonnage(argv[1]);
    } else {
        decode_jpeg_couleurs(argv[1]);
    }

    return EXIT_SUCCESS;
}
