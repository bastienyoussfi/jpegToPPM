#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

/* On écrit dans le fichier .ppm ou .pgm en PPM ou PGM. */

static void concatenation(char chaine1[], const char chaine2[])
/* Concatène deux chaînes de caractères */
{
    uint8_t i = 0;
    while (chaine1[i] != '.') {
        i++;
    }
    uint8_t j = 0;
    uint8_t k = i;
    while (chaine2[j] != '\0') {
        chaine1[k] = chaine2[j];
        j++;
        k++;
    }
    chaine1[k] = '\0';
}


void ecriture_pgm(uint8_t blocs_idct[][8][8], uint16_t hauteur_image, uint16_t longueur_image, char *filename)
/* On écrit un fichier .pgm les images en niveaux de gris */
{
    FILE* fichier = NULL;
    concatenation(filename, ".pgm");
    fichier = fopen(filename, "wb"); //Ouverture du fichier binaire en écriture
    if (fichier != NULL) {
        fprintf(fichier, "P5\n%i %i\n255\n", longueur_image, hauteur_image);
        uint8_t blocs_incomplets_droite = (longueur_image%8 != 0); // Vaut 1 si on a une troncature à droite, 0 sinon
        uint8_t blocs_incomplets_bas = (hauteur_image%8 != 0); // Vaut 1 si on a une troncature en bas, 0 sinon
        uint8_t max_droite = longueur_image%8; // On calcule ce qui dépasse à droite
        uint8_t max_bas = hauteur_image%8; // On calcule ce qui dépasse en bas
        uint32_t longueur_en_nb_de_blocs = longueur_image/8; // Calcul du nombre de blocs sur la longueur
        uint32_t hauteur_en_nb_de_blocs = hauteur_image/8; // Calcul du nombre de blocs sur la hauteur
        for(uint32_t h = 0; h < hauteur_en_nb_de_blocs+blocs_incomplets_bas; h++) { // On boucle sur l'indice de ligne des blocs
            uint8_t imax = (h == hauteur_en_nb_de_blocs) ? max_bas : 8; //Si troncature en bas, on s'arrête à max_bas
            for(uint8_t i = 0; i < imax; i++) { // On boucle sur l'indice de ligne à l'intérieur des blocs, on écrit jusqu'à max_bas pour respecter la troncature
                for(uint16_t l = 0;  l < longueur_en_nb_de_blocs+blocs_incomplets_droite; l++) { // On boucle sur l'indice de colonne des blocs
                    uint8_t jmax = (l == longueur_en_nb_de_blocs) ? max_droite : 8; //Si troncature à droite, on s'arrête à max_droite
                    for (uint8_t j = 0; j < jmax; j++) { // On écrit jusqu'à max_droite pour respecter la troncature
                        fwrite(&(blocs_idct[(h*(longueur_en_nb_de_blocs+blocs_incomplets_droite))+l][i][j]), sizeof(uint8_t), 1, fichier);
                    }
                }
            }

        }
        fclose(fichier); // On ferme le fichier
    } else {
        // On affiche un message d'erreur si on ne peut pas ouvrir le fichier
        printf("Impossible d'ouvrir le fichier %s", filename);
    }
}



void ecriture_ppm(uint8_t blocs_idct[][8][8][3], uint16_t hauteur_image, uint16_t longueur_image, char *filename, bool echantillonnage)
/* On écrit un fichier .ppm sans échantillonnage ou avec échantillonnage horizontal: le booléen échantillonnage indique s'il y a échantillonnage */
{
    FILE* fichier = NULL;
    concatenation(filename, ".ppm");
    fichier = fopen(filename, "wb"); //Ouverture du fichier binaire en écriture
    if (fichier != NULL) {
            fprintf(fichier, "P6\n%i %i\n255\n", longueur_image, hauteur_image);
            uint8_t blocs_incomplets_droite = (longueur_image%8 != 0)+echantillonnage; // Vaut 2 si on a une troncature à droite et de l'échantillonage, 1 avec une seule des deux conditions et 0 sinon
            uint8_t blocs_incomplets_bas = (hauteur_image%8 != 0); // Vaut 1 si on a une troncature en bas, 0 sinon
            uint8_t max_droite = longueur_image%8; // On calcule ce qui dépasse à droite
            uint8_t max_bas = hauteur_image%8; // On calcule ce qui dépasse en bas
            uint32_t longueur_en_nb_de_blocs = longueur_image/8; // Calcul du nombre de blocs sur la longueur
            uint32_t hauteur_en_nb_de_blocs = hauteur_image/8; // Calcul du nombre de blocs sur la hauteur
            for(uint32_t h = 0; h < hauteur_en_nb_de_blocs+blocs_incomplets_bas; h++) { // On boucle sur l'indice de ligne des blocs
                uint8_t imax = (h == hauteur_en_nb_de_blocs) ? max_bas : 8; //Si troncature en bas, on s'arrête à max_bas, sinon on va jusqu'à 8
                for(uint8_t i = 0; i < imax; i++) {
                    for(uint16_t l = 0;  l < longueur_en_nb_de_blocs+blocs_incomplets_droite; l++) {
                        uint8_t jmax = (l == longueur_en_nb_de_blocs) ? max_droite : 8; //Si troncature à droite, on s'arrête à max_droite, sinon on va jusqu'à 8
                        for (uint8_t j = 0; j < jmax; j++) {
                            uint8_t rgb[3] = {blocs_idct[(h*(longueur_en_nb_de_blocs+blocs_incomplets_droite))+l][i][j][0], blocs_idct[(h*(longueur_en_nb_de_blocs+blocs_incomplets_droite))+l][i][j][1], blocs_idct[(h*(longueur_en_nb_de_blocs+blocs_incomplets_droite))+l][i][j][2]};
                            fwrite(rgb, 3, 1, fichier);
                        }
                    }
                }
            }
            fclose(fichier); // On ferme le fichier
    } else {
        // On affiche un message d'erreur sinon
        printf("Impossible d'ouvrir le fichier %s", filename);
    }
}

void ecriture_echantillonnage(uint8_t blocs_idct[][8][8][3], uint16_t hauteur_image, uint16_t longueur_image, char *filename)
/* Ecrit dans le fichier .ppm avec échantillonnage vertical */
{
    FILE* fichier = NULL;
    concatenation(filename, ".ppm");
    fichier = fopen(filename, "wb"); //Ouverture du fichier binaire en écriture
    if (fichier != NULL) {
        fprintf(fichier, "P6\n%i %i\n255\n", longueur_image, hauteur_image);
        uint8_t blocs_incomplets_droite = (longueur_image%8 != 0);
        uint8_t blocs_incomplets_bas = (hauteur_image%8 != 0);
        uint8_t max_droite = (longueur_image%8);
        uint8_t max_bas = hauteur_image%8;
        uint32_t longueur_en_nb_de_blocs = longueur_image/8;
        uint32_t hauteur_en_nb_de_blocs = hauteur_image/8;
        for(uint32_t h = 0; h < hauteur_en_nb_de_blocs+blocs_incomplets_bas; h+=2) {
            bool b = !(h == hauteur_en_nb_de_blocs);
            uint8_t imax = (h == hauteur_en_nb_de_blocs) ? max_bas : 8; //Si troncature en bas, on s'arrête à max_bas
            for(uint8_t i = 0; i < imax; i++) {
                for(uint16_t l = 0;  l < 2*(longueur_en_nb_de_blocs+blocs_incomplets_droite)+b; l+=2) {
                    uint8_t jmax = (l == 2*longueur_en_nb_de_blocs) ? max_droite : 8; //Si troncature à droite, on s'arrête à max_droite
                    for (uint8_t j = 0; j < jmax; j++) {
                        uint8_t rgb[3] = {blocs_idct[(h*(longueur_en_nb_de_blocs+blocs_incomplets_droite))+l][i][j][0], blocs_idct[(h*(longueur_en_nb_de_blocs+blocs_incomplets_droite))+l][i][j][1], blocs_idct[(h*(longueur_en_nb_de_blocs+blocs_incomplets_droite))+l][i][j][2]};
                        fwrite(rgb, 3, 1, fichier);
                    }
                }
            }
            for(uint8_t i = 0; i < imax; i++) {
                for(uint16_t l = 1;  l < 2*(longueur_en_nb_de_blocs+blocs_incomplets_droite)+b; l+=2) {
                    uint8_t jmax = (l == 2*longueur_en_nb_de_blocs) ? max_droite : 8; //Si troncature à droite, on s'arrête à max_droite
                    for (uint8_t j = 0; j < jmax; j++) {
                        uint8_t rgb[3] = {blocs_idct[(h*(longueur_en_nb_de_blocs+blocs_incomplets_droite))+l][i][j][0], blocs_idct[(h*(longueur_en_nb_de_blocs+blocs_incomplets_droite))+l][i][j][1], blocs_idct[(h*(longueur_en_nb_de_blocs+blocs_incomplets_droite))+l][i][j][2]};
                        fwrite(rgb, 3, 1, fichier);

                    }
                }
            }
        }
        fclose(fichier); // On ferme le fichier
    } else {
        // On affiche un message d'erreur sinon
        printf("Impossible d'ouvrir le fichier %s", filename);
    }
}

void ecriture_echantillonnage_H_et_V(uint8_t blocs_idct[][8][8][3], uint16_t hauteur_image, uint16_t longueur_image, char *filename)
{
    //On écrit un fichier .ppm pour une image avec échantillonnage horizontal et vertical
    FILE* fichier = NULL;
    concatenation(filename, ".ppm");
    fichier = fopen(filename, "w");
    if (fichier != NULL)
    {
        fprintf(fichier, "P6\n%i %i\n255\n", longueur_image, hauteur_image);
        uint8_t blocs_incomplets_droite = (longueur_image%8 != 0);
        uint8_t blocs_incomplets_bas = (hauteur_image%8 != 0);
        uint8_t max_droite = (longueur_image%8);
        uint8_t max_bas = hauteur_image%8;
        uint32_t longueur_en_nb_de_blocs = longueur_image/8;
        uint32_t hauteur_en_nb_de_blocs = hauteur_image/8;
        for(uint32_t h = 0; h < hauteur_en_nb_de_blocs+blocs_incomplets_bas; h+=2) {
            /* Troncature en bas */
            if (h == hauteur_en_nb_de_blocs) { // Condition où l'on a troncature en bas
                for(uint8_t i = 0; i < max_bas; i++) { // Si troncature en bas, on s'arrête à max_bas
                    for(uint16_t l = 0;  l < 2*(longueur_en_nb_de_blocs)+blocs_incomplets_droite; l+=4) { // On écrit les blocs 2 par 2 et on en saute 2 à chaque fois, ici on écrit la première ligne
                        uint8_t jmax1 = (l == 2*longueur_en_nb_de_blocs-1) ? max_droite : 8; // Si troncature à droite, on s'arrête à max_droite, sinon on va jusqu'à 8
                        for (uint8_t j = 0; j < jmax1; j++) {
                            uint8_t rgb[3] = {blocs_idct[(h*(longueur_en_nb_de_blocs+blocs_incomplets_droite))+l][i][j][0], blocs_idct[(h*(longueur_en_nb_de_blocs+blocs_incomplets_droite))+l][i][j][1], blocs_idct[(h*(longueur_en_nb_de_blocs+blocs_incomplets_droite))+l][i][j][2]};
                            fwrite(rgb, 3, 1, fichier);
                        }
                        uint16_t u = l+1; // On vérifie que le second bloc n'est pas le bloc tout à droite de l'image
                        uint8_t jmax2 = (u == 2*longueur_en_nb_de_blocs-1) ? max_droite : 8; // Si troncature à droite, on s'arrête à max_droite, sinon on va jusqu'à 8
                        for(uint8_t j = 0; j < jmax2; j++) {
                            uint8_t rgb[3] = {blocs_idct[(h*(longueur_en_nb_de_blocs+blocs_incomplets_droite))+l+1][i][j][0], blocs_idct[(h*(longueur_en_nb_de_blocs+blocs_incomplets_droite))+l+1][i][j][1], blocs_idct[(h*(longueur_en_nb_de_blocs+blocs_incomplets_droite))+l+1][i][j][2]};
                            fwrite(rgb, 3, 1, fichier);
                        }
                    }
                }
                for(uint8_t i = 0; i < max_bas; i++) { // Ici on écrit la deuxième ligne
                    for(uint16_t l = 2;  l < 2*(longueur_en_nb_de_blocs)+blocs_incomplets_droite; l+=4) { // On écrit les blocs 2 par 2 et on en saute 2 à chaque fois, ici on écrit la deuxième ligne
                        if (l == 2*(longueur_en_nb_de_blocs)+blocs_incomplets_droite) { // Condition où l'on a troncature à droite
                            for (uint8_t j = 0; j < max_droite; j++) {
                                uint8_t rgb[3] = {blocs_idct[(h*(longueur_en_nb_de_blocs+blocs_incomplets_droite))+l][i][j][0], blocs_idct[(h*(longueur_en_nb_de_blocs+blocs_incomplets_droite))+l][i][j][1], blocs_idct[(h*(longueur_en_nb_de_blocs+blocs_incomplets_droite))+l][i][j][2]};
                                fwrite(rgb, 3, 1, fichier);
                            }
                        } else { // Sinon, on va jusqu'à 8
                            for(uint8_t j = 0; j < 8; j++) {
                                uint8_t rgb[3] = {blocs_idct[(h*(longueur_en_nb_de_blocs+blocs_incomplets_droite))+l+1][i][j][0], blocs_idct[(h*(longueur_en_nb_de_blocs+blocs_incomplets_droite))+l+1][i][j][1], blocs_idct[(h*(longueur_en_nb_de_blocs+blocs_incomplets_droite))+l+1][i][j][2]};
                                fwrite(rgb, 3, 1, fichier);
                            }
                            uint16_t u = l+1; // On regarde si le second bloc est le bloc tronqué à droite
                            uint8_t jmax2 = (u == 2*(longueur_en_nb_de_blocs)+blocs_incomplets_droite) ? max_droite : 8;
                            for(uint8_t j = 0; j < jmax2; j++) { // Si troncature à droite, on s'arrête à max_droite, sinon on va jusqu'à 8
                                uint8_t rgb[3] = {blocs_idct[(h*(longueur_en_nb_de_blocs+blocs_incomplets_droite))+l+1][i][j][0], blocs_idct[(h*(longueur_en_nb_de_blocs+blocs_incomplets_droite))+l+1][i][j][1], blocs_idct[(h*(longueur_en_nb_de_blocs+blocs_incomplets_droite))+l+1][i][j][2]};
                                fwrite(rgb, 3, 1, fichier);
                            }
                        }
                    }
                }
            } else {
                for(uint8_t i = 0; i< 8; i++) { // Sinon, si on n'a pas de troncature en bas
                    for(uint16_t l = 0;  l < 2*(longueur_en_nb_de_blocs)+blocs_incomplets_droite; l+=4) { // On écrit les blocs 2 par 2 et on en saute 2 à chaque fois, ici on écrit la première ligne
                        if (l == 2*longueur_en_nb_de_blocs-1) { // Condition ou l'on à troncature à droite
                            for (uint8_t j = 0; j < max_droite; j++) { // On va jusqu'à max_droite
                                uint8_t rgb[3] = {blocs_idct[(h*(longueur_en_nb_de_blocs+blocs_incomplets_droite))+l][i][j][0], blocs_idct[(h*(longueur_en_nb_de_blocs+blocs_incomplets_droite))+l][i][j][1], blocs_idct[(h*(longueur_en_nb_de_blocs+blocs_incomplets_droite))+l][i][j][2]};
                                fwrite(rgb, 3, 1, fichier);
                            }
                        } else {
                            for(uint8_t j = 0; j < 8; j++) { // Sinon, on va jusqu'à 8
                                uint8_t rgb[3] = {blocs_idct[(h*(longueur_en_nb_de_blocs+blocs_incomplets_droite))+l][i][j][0], blocs_idct[(h*(longueur_en_nb_de_blocs+blocs_incomplets_droite))+l][i][j][1], blocs_idct[(h*(longueur_en_nb_de_blocs+blocs_incomplets_droite))+l][i][j][2]};
                                fwrite(rgb, 3, 1, fichier);
                            }
                            uint16_t u = l+1; // On regarde si le second bloc est le bloc tronqué à droite
                            uint8_t jmax2 = (u == 2*longueur_en_nb_de_blocs-1) ? max_droite : 8;  // Si troncature à droite, on s'arrête à max_droite, sinon on va jusqu'à 8
                            for(uint8_t j = 0; j < jmax2; j++) {
                                uint8_t rgb[3] = {blocs_idct[(h*(longueur_en_nb_de_blocs+blocs_incomplets_droite))+l+1][i][j][0], blocs_idct[(h*(longueur_en_nb_de_blocs+blocs_incomplets_droite))+l+1][i][j][1], blocs_idct[(h*(longueur_en_nb_de_blocs+blocs_incomplets_droite))+l+1][i][j][2]};
                                fwrite(rgb, 3, 1, fichier);
                            }
                        }
                    }
                }
                for(uint8_t i = 0; i< 8; i++) { // Ici on écrit la deuxième ligne
                    for(uint16_t l = 2;  l < 2*(longueur_en_nb_de_blocs+blocs_incomplets_droite); l+=4) { // On écrit les blocs 2 par 2 et on en saute 2 à chaque fois, ici on écrit la deuxième ligne
                        if (l == 2*(longueur_en_nb_de_blocs)+blocs_incomplets_droite) { // Condition où l'on a troncature à droite
                            for (uint8_t j = 0; j < max_droite; j++) { // On va jusqu'à max_droite
                                uint8_t rgb[3] = {blocs_idct[(h*(longueur_en_nb_de_blocs+blocs_incomplets_droite))+l][i][j][0], blocs_idct[(h*(longueur_en_nb_de_blocs+blocs_incomplets_droite))+l][i][j][1], blocs_idct[(h*(longueur_en_nb_de_blocs+blocs_incomplets_droite))+l][i][j][2]};
                                fwrite(rgb, 3, 1, fichier);
                            }
                        } else {
                            for(uint8_t j = 0; j < 8; j++) { // Sinon, on va jusqu'à 8
                                uint8_t rgb[3] = {blocs_idct[(h*(longueur_en_nb_de_blocs+blocs_incomplets_droite))+l][i][j][0], blocs_idct[(h*(longueur_en_nb_de_blocs+blocs_incomplets_droite))+l][i][j][1], blocs_idct[(h*(longueur_en_nb_de_blocs+blocs_incomplets_droite))+l][i][j][2]};
                                fwrite(rgb, 3, 1, fichier);
                            }
                            uint16_t u = l+1; // On regarde si le second bloc est le bloc tronqué à droite
                            uint8_t jmax2 = (u == 2*(longueur_en_nb_de_blocs)+blocs_incomplets_droite) ? max_droite : 8;
                            for(uint8_t j = 0; j < jmax2; j++) { // Si troncature à droite, on s'arrête à max_droite, sinon on va jusqu'à 8
                                uint8_t rgb[3] = {blocs_idct[(h*(longueur_en_nb_de_blocs+blocs_incomplets_droite))+l+1][i][j][0], blocs_idct[(h*(longueur_en_nb_de_blocs+blocs_incomplets_droite))+l+1][i][j][1], blocs_idct[(h*(longueur_en_nb_de_blocs+blocs_incomplets_droite))+l+1][i][j][2]};
                                fwrite(rgb, 3, 1, fichier);
                            }
                        }
                    }
                }
            }
        }
        fclose(fichier); // On ferme le fichier
    } else {
        // On affiche un message d'erreur sinon
        printf("Impossible d'ouvrir le fichier %s", filename);
    }
}
