#ifndef __ECRITURE_H__
#define __ECRITURE_H__

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

/* On écrit dans le fichier .txt en PPM ou PGM. */

void ecriture_pgm(uint8_t blocs_idct[][8][8], uint16_t hauteur_image, uint16_t longueur_image, char *filename);

void ecriture_ppm(uint8_t blocs_idct[][8][8][3], uint16_t hauteur_image, uint16_t longueur_image, char *filename, bool echantillonnage);

void ecriture_echantillonnage(uint8_t blocs_idct[][8][8][3], uint16_t hauteur_image, uint16_t longueur_image, char *filename);

void ecriture_echantillonnage_H_et_V(uint8_t blocs_idct[][8][8][3], uint16_t hauteur_image, uint16_t longueur_image, char *filename);

#endif
