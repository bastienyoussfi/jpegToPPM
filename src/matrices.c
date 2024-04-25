#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>


void copie_vecteur_de_matrices(uint8_t matrice[4][8][8], uint8_t copie_matrice[4][8][8])
// Permet de copier une matrice de vecteurs dans une nouvelle matrice de vecteurs
{
    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 8; j++) {
            for (int z = 0; z < 8; z++){
                copie_matrice[i][j][z] = matrice[i][j][z];
            }
        }
    }
}

void print_matrix(int16_t ** mat, uint32_t longueur, uint32_t hauteur)
// Affiche les éléments d'une matrice
{
    for(uint32_t i = 0; i < hauteur; i++){
        for(uint32_t j = 0; j < longueur; j++){
            printf("%x  ",mat[i][j]);
        }
        //printf("\n");
    }
}

void print_matrix_uint8(uint8_t mat[8][8], uint32_t longueur, uint32_t hauteur)
// Affiche les éléments d'une matrice
{
    for(uint32_t i = 0; i < hauteur; i++){
        for(uint32_t j = 0; j < longueur; j++){
            printf("%x  ",mat[i][j]);
        }
        printf("\n");
    }
}

void print_matrix_int32_88(int32_t mat[8][8], uint32_t longueur, uint32_t hauteur)
// Affiche les éléments d'une matrice
{
    for(uint32_t i = 0; i < hauteur; i++){
        for(uint32_t j = 0; j < longueur; j++){
            printf("%x  ",mat[i][j]);
        }
        printf("\n");
    }
}


void print_array(int16_t arr[], uint32_t size)
// Affiche les éléments d'un vecteur
{
    for(uint32_t i = 0; i < size; i++){
        printf("%x", arr[i]);
        printf(" ");
    }
    printf("\n");
}

void print_array_int32(int32_t arr[], uint32_t size)
// Affiche les éléments d'un vecteur
{
    for(uint32_t i = 0; i < size; i++){
        printf("%x", arr[i]);
        printf(" ");
    }
    printf("\n");
}


int16_t ** alloc_matrice(int16_t max_i, int16_t max_j)
{
    int16_t ** matrice = (int16_t **) malloc (sizeof(int16_t*)*max_i);
    assert(matrice);

    for (int16_t i = 0; i<max_i; i++) {
        matrice[i] = (int16_t *)malloc(max_j*sizeof(int16_t));
        assert(matrice[i]);
    }
    return matrice;
}

void free_matrice(int16_t ** matrice, int16_t max_i)
{
    for (int16_t i = 0; i<max_i; i++) {
        free(matrice[i]);
    }
    free(matrice);
}



int32_t ** alloc_matrice_int32(uint32_t max_i, uint32_t max_j)
{
    int32_t ** matrice = (int32_t **) malloc (sizeof(int32_t*)*max_i);
    assert(matrice);

    for (uint32_t i = 0; i<max_i; i++) {
        matrice[i] = (int32_t *)malloc(max_j*sizeof(int32_t));
        assert(matrice[i]);
    }
    return matrice;
}

void free_matrice_int32(int32_t ** matrice, uint32_t max_i)
{
    for (uint32_t i = 0; i<max_i; i++) {
        free(matrice[i]);
    }
    free(matrice);
}


int32_t *** alloc_vecteur_de_matrices_int32(uint32_t max_i, uint32_t max_j, uint32_t nb_matrices)
{
    int32_t *** matrice = (int32_t ***) malloc (sizeof(int32_t**)*nb_matrices);
    assert(matrice);

    for (uint32_t m = 0; m<nb_matrices; m++) {
        matrice[m] = (int32_t **)malloc(max_i*sizeof(int32_t *));
        assert(matrice[m]);
        for (uint32_t i = 0; i<max_i; i++) {
            matrice[m][i] = (int32_t *)malloc(max_j*sizeof(int32_t));
            assert(matrice[m][i]);
        }
    }
    return matrice;
}


void free_vecteur_de_matrices_int32(int32_t *** matrice, uint32_t nb_matrices, uint32_t max_i)
{
    for (uint32_t m = 0; m<nb_matrices; m++) {
        for (uint32_t i = 0; i<max_i; i++) {
            free(matrice[m][i]);
        }
        free(matrice[m]);
    }
    free(matrice);
}

int32_t **** alloc_matrice_de_matrices_int32(uint32_t i_ext, uint32_t j_ext, uint32_t i_int, uint32_t j_int)
{
    {
        int32_t **** matrice = (int32_t ****) malloc (sizeof(int32_t***)*i_ext);
        assert(matrice);

        for (uint32_t i = 0; i<i_ext; i++) {
            matrice[i] = (int32_t ***)malloc(j_ext*sizeof(int32_t **));
            assert(matrice[i]);
            for (uint32_t j=0; j<j_ext; j++) {
                matrice[i][j] = alloc_matrice_int32(i_int, j_int);
            }
        }
        return matrice;
    }
}


void free_matrice_de_matrices_int32(int32_t ****matrice, uint16_t i_ext, uint32_t j_ext, uint32_t i_int)
{
    for (uint32_t i = 0; i<i_ext; i++) {
        free_vecteur_de_matrices_int32(matrice[i], j_ext, i_int);
    }
    free(matrice);
}
