#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

/* Prend un vecteur de 64 valeurs et renvoie la matrice 8x8 obtenue en faisant
la méthode de zig-zag inverse */

void zig_zag_inv(int32_t in[64], int32_t out[8][8])
{
    uint8_t ligne = 0; // Représente le numéro de ligne courant
    uint8_t colonne = 0; // Représente le numéro de colonne courant
    uint8_t indice  =  0; // Représente l'indice courant (va de 0 à 64)
    int32_t *p = NULL;
    while(indice <= 63){
        p = &in[indice];
      	if (ligne == 0 && ((ligne+colonne)%2) == 0 && colonne != 7) { // Cas où on doit bouger à droite quand on est en haut
    		out[ligne][colonne] = *p;
    		colonne++;
    		indice++;
        } else if (ligne == 7 && ((ligne+colonne)%2) != 0 && colonne != 7) { // Cas où on doit bouger à droite quand on est en bas
    		out[ligne][colonne] = *p;
    		colonne++;
    		indice++;
        } else if (colonne == 0 && ((ligne+colonne)%2) != 0 && ligne != 7) { // Cas où on doit bouger en bas quand on est à gauche
    		out[ligne][colonne] = *p;
    		ligne++;
    		indice++;
        } else if (colonne == 7 && ((ligne+colonne)%2) == 0 && ligne != 7) { // Cas où on doit bouger en bas quand on est à droite
    		out[ligne][colonne] = *p;
    		ligne++;
    		indice++;
        } else if (colonne != 0 && ligne != 7 && ((ligne+colonne)%2) !=  0) { // Cas où on doit bouger en diagonale en bas à gauche
    		out[ligne][colonne] = *p;
    		ligne++;
            colonne--;
    		indice++;
        } else if (ligne != 0 && colonne != 7 && ((ligne + colonne)%2)  ==  0) { // Cas où on doit bouger en diagonale en haut à droite
    		out[ligne][colonne] = *p;
    		ligne--;
            colonne++;
    		indice++;
        } else if (indice  ==  63){ // Dernier élément
            p = &in[63];
            out[7][7] = *p;
            indice++;
        }
    }
}
