#ifndef __MATRICES__
#define __MATRICES__

#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

extern void copie_vecteur_de_matrices(uint8_t matrice[4][8][8], uint8_t copie_matrice[4][8][8]);

extern void print_matrix(int16_t ** mat, uint32_t longueur, uint32_t hauteur);

extern void print_matrix_uint8(uint8_t mat[8][8], uint32_t longueur, uint32_t hauteur);

extern void print_matrix_int32(int32_t **mat, uint32_t longueur, uint32_t hauteur);

extern void print_matrix_int32_88(int32_t mat[8][8], uint32_t longueur, uint32_t hauteur);

extern void print_array(int16_t arr[], uint32_t size);

extern void print_array_int32(int32_t arr[], uint32_t size);

extern int16_t ** alloc_matrice(int16_t max_i, int16_t max_j);

extern int32_t ** alloc_matrice_int32(uint32_t max_i, uint32_t max_j);

extern void free_matrice(int16_t ** matrice, int16_t max_i);

extern void free_matrice_int32(int32_t ** matrice, uint32_t max_i);

extern int32_t *** alloc_vecteur_de_matrices_int32(int32_t max_i, int32_t max_j, int32_t nb_matrices);

extern void free_vecteur_de_matrices_int32(int32_t *** matrice, uint32_t nb_matrices, uint32_t max_i);

extern int32_t **** alloc_matrice_de_matrices_int32(uint32_t i_ext, uint32_t j_ext, uint32_t i_int, uint32_t j_int);

extern void free_matrice_de_matrices_int32(int32_t ****matrice, uint16_t i_ext, uint32_t j_ext, uint32_t i_int);

#endif
