#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

static void sur_echantillonnage_vertical_un_bloc(uint8_t composante_chrominance[8][8], uint8_t nouvelle_composante1[8][8], uint8_t nouvelle_composante2[8][8])
{
    for (uint8_t ligne = 0; ligne < 4; ligne++) {
        for (uint8_t colonne = 0; colonne < 8; colonne++) {
            // Construit 2 blocs de chrominance à partir d'un seul en dupliquant les lignes de celui-ci.
            nouvelle_composante1[2 * ligne][colonne] = composante_chrominance[ligne][colonne];
            nouvelle_composante1[2 * ligne + 1][colonne] = composante_chrominance[ligne][colonne];
            nouvelle_composante2[2 * ligne][colonne] = composante_chrominance[ligne + 4][colonne];
            nouvelle_composante2[2 * ligne + 1][colonne] = composante_chrominance[ligne + 4][colonne];
        }
    }
}

static void sur_echantillonnage_horizontal_un_bloc(uint8_t composante_chrominance[8][8], uint8_t nouvelle_composante1[8][8], uint8_t nouvelle_composante2[8][8])
{
    for (uint8_t ligne = 0; ligne < 8; ligne++) {
        for (uint8_t colonne = 0; colonne < 4; colonne++) {
        // Construit 2 blocs de chrominance à partir d'un seul en dupliquant les colonnes de celui-ci.
        nouvelle_composante1[ligne][2 * colonne] = composante_chrominance[ligne][colonne];
        nouvelle_composante1[ligne][2 * colonne + 1] = composante_chrominance[ligne][colonne];
        nouvelle_composante2[ligne][2 * colonne] = composante_chrominance[ligne][colonne + 4];
        nouvelle_composante2[ligne][2 * colonne + 1] = composante_chrominance[ligne][colonne + 4];
        }
    }
}

void sur_echantillonnage_horizontal_mcu(uint8_t mcu[3][4][8][8], uint8_t Y[2][8][8], uint8_t Cb[2][8][8], uint8_t Cr[2][8][8], int nb_composantes_Y)
{
    // Construit une MCU YCbCr complète à partir d'une MCU sous-échantillonnée horizontalement.
    if (nb_composantes_Y == 2) {
        for(int i = 0; i < 8; i++) {
            for(int j = 0; j < 8; j++) {
                Y[0][i][j] = mcu[0][0][i][j]; // On récupère les composantes Y qui ne seront jamais sur-échantillonées
                Y[1][i][j] = mcu[0][1][i][j];
            }
        }
        // On sur-échantillonne nos Cb et Cr initiaux.
        sur_echantillonnage_horizontal_un_bloc(mcu[1][0], Cb[0], Cb[1]);
        sur_echantillonnage_horizontal_un_bloc(mcu[2][0], Cr[0], Cr[1]);
    }
    else if (nb_composantes_Y == 4) {
        for(int i = 0; i < 8; i++) {
            for(int j = 0; j < 8; j++) {
                Y[0][i][j] = mcu[0][0][i][j]; // On récupère les composantes Y qui se seront jamais sur-échantillonées.
                Y[1][i][j] = mcu[0][1][i][j];
                Y[2][i][j] = mcu[0][2][i][j];
                Y[3][i][j] = mcu[0][3][i][j];

            }
        }
        // On sur-échantillonne nos Cb et Cr initiaux.
        sur_echantillonnage_horizontal_un_bloc(mcu[1][0], Cb[0], Cb[1]);
        sur_echantillonnage_horizontal_un_bloc(mcu[2][0], Cr[0], Cr[1]);
    }
}

void sur_echantillonnage_vertical_mcu(uint8_t mcu[3][4][8][8], uint8_t Y[4][8][8], uint8_t Cb[4][8][8], uint8_t Cr[4][8][8], int nb_composantes_Y)
{
    // Construit une MCU YCbCr complète à partir d'une MCU sous-échantillonnée verticalement.
    if(nb_composantes_Y == 2) {
        for(int i = 0; i < 8; i++) {
            for(int j = 0; j < 8; j++) {
                Y[0][i][j] = mcu[0][0][i][j];// On récupère les composantes Y qui se seront jamais sur-échantillonées.
                Y[1][i][j] = mcu[0][1][i][j];
            }
        }
    sur_echantillonnage_vertical_un_bloc(mcu[1][0], Cb[0], Cb[1]);
    sur_echantillonnage_vertical_un_bloc(mcu[2][0], Cr[0], Cr[1]);
    }
    else if(nb_composantes_Y == 4) {
        for(int i = 0; i < 8; i++) {
            for(int j = 0; j < 8; j++) {
                Y[0][i][j] = mcu[0][0][i][j]; // On récupère les composantes Y qui se seront jamais sur-échantillonées.
                Y[1][i][j] = mcu[0][1][i][j];
                Y[2][i][j] = mcu[0][2][i][j];
                Y[3][i][j] = mcu[0][3][i][j];
            }
        }
    sur_echantillonnage_vertical_un_bloc(mcu[1][0], Cb[0], Cb[2]);
    sur_echantillonnage_vertical_un_bloc(mcu[1][1], Cb[1], Cb[3]);
    sur_echantillonnage_vertical_un_bloc(mcu[2][0], Cr[0], Cr[2]);
    sur_echantillonnage_vertical_un_bloc(mcu[2][1], Cr[1], Cr[3]);
    }
}
