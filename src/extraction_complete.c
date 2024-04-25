#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "../include/huffman.h"
#include "../include/bitstream.h"
#include "../include/jpeg_reader.h"
#include "../include/matrices.h"


void extraction(const char *filename, int32_t **res, uint32_t nb_blocs)
{
    // Permet d'extraire et décoder les coefficients AC/DC d'une image JPEG en niveau de gris.
    uint32_t predicateur = 0;
    struct jpeg_desc *jdesc = jpeg_read(filename);
    struct bitstream *stream = jpeg_get_bitstream(jdesc);

    for (uint32_t bloc_courant = 0; bloc_courant < nb_blocs; bloc_courant++) {
        /* DC */
        struct huff_table *huffman_dc = jpeg_get_huffman_table(jdesc, DC, 0);
        int8_t magnitude_dc = huffman_next_value(huffman_dc, stream);
        uint32_t dc = 0;
        uint16_t nb_lus_dc = bitstream_read(stream, magnitude_dc, &dc, true);
        if (nb_lus_dc != magnitude_dc) {
            fprintf(stderr, "Erreur de lecture ! ");
        }
        dc = ((dc < (uint32_t) (1 << (magnitude_dc - 1))) ? (dc - ((1 << magnitude_dc) - 1)) : dc); // On décode DC
        res[bloc_courant][0] = dc + predicateur; // On ajoute à la valeur courante de DC la valeur précédente
        predicateur = res[bloc_courant][0]; // On met à jour le prédicateur

        /* AC */
        uint8_t indice = 1;
        struct huff_table *huffman_ac = jpeg_get_huffman_table(jdesc, AC, 0);
        while (indice < 64) {
            int8_t rle_s = huffman_next_value(huffman_ac, stream);
            uint8_t rle = (uint8_t) rle_s; // Le symbole rle est non signé
            uint8_t nb_zeros = rle >> 4; // On récupère les 4 bits de poids fort représentant le nombre de zéros
            uint8_t magnitude_ac = rle & 0x0f; // On récupère les 4 bits de poids faible représentant la magnitude
            /* Cas 0x00 (code EOB)*/
            if (nb_zeros == 0 && magnitude_ac == 0) {
                while (indice < 64) {
                    res[bloc_courant][indice] = 0;
                    indice++;
                }
            /* Cas 0xF0 (code ZRL) */
            } else if (nb_zeros == 0xf && magnitude_ac == 0) {
                for (uint8_t i=0; i<16; i++) {
                    res[bloc_courant][indice] = 0;
                    indice++;
                }
            /* Cas général*/
            } else {
                for (uint8_t i=0; i<nb_zeros; i++) {
                    res[bloc_courant][indice] = 0;
                    indice++;
                }
                uint32_t ac = 0;
                uint16_t nb_lus_ac = bitstream_read(stream, magnitude_ac, &ac, true);
                if (nb_lus_ac != magnitude_ac){
                    fprintf(stderr, "Erreur de lecture ! ");
                }
                res[bloc_courant][indice] = ((ac < (uint32_t) (1 << (magnitude_ac - 1)))? (ac - ((1 << magnitude_ac) - 1)) : ac);
                indice++;
            }
        }
    }
    jpeg_close(jdesc);
}

void extraction_couleur(const char *filename, int32_t ***res, uint32_t nb_mcu)
{
    // Permet d'extraire et décoder les coefficients AC/DC d'une image JPEG en couleurs sans échantillonnage.
    uint32_t predicateurs[] = {0, 0, 0};
    struct jpeg_desc *jdesc = jpeg_read(filename);
    struct bitstream *stream = jpeg_get_bitstream(jdesc);
    uint8_t huff_indice;
    for (uint32_t mcu_courant = 0; mcu_courant < nb_mcu; mcu_courant++) {
        for (uint8_t bloc_courant = 0; bloc_courant < 3; bloc_courant++) {
            /* DC */
            huff_indice = (bloc_courant != 0); // 0 si c'est Y, 1 sinon

            struct huff_table *huffman_dc = jpeg_get_huffman_table(jdesc, DC, huff_indice);
            int8_t magnitude_dc = huffman_next_value(huffman_dc, stream);
            uint32_t dc = 0;
            uint16_t nb_lus_dc = bitstream_read(stream, magnitude_dc, &dc, true);
            if (nb_lus_dc != magnitude_dc) {
                fprintf(stderr, "Erreur de lecture ! ");
            }
            dc = ((dc < (uint32_t) (1 << (magnitude_dc - 1))) ? (dc - ((1 << magnitude_dc) - 1)) : dc); // On décode DC
            res[mcu_courant][bloc_courant][0] = dc + predicateurs[bloc_courant]; // On ajoute à la valeur courante de DC la valeur précédente
            predicateurs[bloc_courant] = res[mcu_courant][bloc_courant][0]; // On met à jour le prédicateur

            /* AC */
            uint8_t indice = 1;
            struct huff_table *huffman_ac = jpeg_get_huffman_table(jdesc, AC, huff_indice);
            while (indice < 64) {
                int8_t rle_s = huffman_next_value(huffman_ac, stream);
                uint8_t rle = (uint8_t) rle_s;
                uint8_t nb_zeros = rle >> 4;
                uint8_t magnitude_ac = rle & 0x0f;
                /* Cas 0x00*/
                if (nb_zeros == 0 && magnitude_ac == 0) {
                    while (indice < 64) {
                        res[mcu_courant][bloc_courant][indice] = 0;
                        indice++;
                    }
                /* Cas 0xF0 */
                } else if (nb_zeros == 0xf && magnitude_ac == 0) {
                    for (uint8_t i=0; i<16; i++) {
                        res[mcu_courant][bloc_courant][indice] = 0;
                        indice++;
                    }
                /* Cas général*/
                } else {
                    for (uint8_t i=0; i<nb_zeros; i++) {
                        res[mcu_courant][bloc_courant][indice] = 0;
                        indice++;
                    }
                    uint32_t ac = 0;
                    uint16_t nb_lus_ac = bitstream_read(stream, magnitude_ac, &ac, true);
                    if (nb_lus_ac != magnitude_ac){
                        fprintf(stderr, "Erreur de lecture ! ");
                    }
                    res[mcu_courant][bloc_courant][indice] = ((ac < (uint32_t) (1 << (magnitude_ac-1)))? (ac - ((1 << magnitude_ac)-1)) : ac);
                    indice++;
                }
            }
        }
    }
    jpeg_close(jdesc);
}

void extraction_couleur_echantillonage(const char *filename, int32_t ****res, uint32_t nb_mcu)
{
    // Permet d'extraire et décoder les coefficients AC/DC d'une image JPEG en couleurs échantillonnée.
    uint32_t predicateurs[] = {0, 0, 0};
    struct jpeg_desc *jdesc = jpeg_read(filename);
    struct bitstream *stream = jpeg_get_bitstream(jdesc);
    uint8_t huff_indice;
    uint16_t nb_composantes = 0;
    for (uint32_t mcu_courant = 0; mcu_courant < nb_mcu; mcu_courant++) {
        for(uint8_t index = 0; index < 3; index++) {
            uint8_t largeur_index = jpeg_get_frame_component_sampling_factor(jdesc, H, index);
            uint8_t hauteur_index = jpeg_get_frame_component_sampling_factor(jdesc, V, index);
            nb_composantes = largeur_index*hauteur_index;
            for (uint8_t bloc_courant = 0; bloc_courant < nb_composantes; bloc_courant++) {
                /* DC */
                huff_indice = !(index == 0); // 0 si c'est Y, 1 sinon
                struct huff_table *huffman_dc = jpeg_get_huffman_table(jdesc, DC, huff_indice);
                int8_t magnitude_dc = huffman_next_value(huffman_dc, stream);
                uint32_t dc = 0;
                uint16_t nb_lus_dc = bitstream_read(stream, magnitude_dc, &dc, true);
                if (nb_lus_dc != magnitude_dc) {
                    fprintf(stderr, "Erreur de lecture ! ");
                }
                dc = ((dc < (uint32_t) (1 << (magnitude_dc - 1))) ? (dc - ((1 << magnitude_dc) - 1)) : dc); // On décode DC
                res[mcu_courant][index][bloc_courant][0] = dc + predicateurs[index]; // On ajoute à la valeur courante de DC la valeur précédente
                predicateurs[index] = res[mcu_courant][index][bloc_courant][0];
                /* AC */
                uint8_t indice = 1;
                struct huff_table *huffman_ac = jpeg_get_huffman_table(jdesc, AC, huff_indice);
                while (indice < 64) {
                    int8_t rle_s = huffman_next_value(huffman_ac, stream);
                    uint8_t rle = (uint8_t) rle_s;
                    uint8_t nb_zeros = rle >> 4;
                    uint8_t magnitude_ac = rle & 0x0f;
                    /* Cas 0x00*/
                    if (nb_zeros == 0 && magnitude_ac == 0) {
                        while (indice < 64) {
                            res[mcu_courant][index][bloc_courant][indice] = 0;
                            indice++;
                        }
                    /* Cas 0xF0 */
                    } else if (nb_zeros == 0xf && magnitude_ac == 0) {
                        for (uint8_t i=0; i<16; i++) {
                            res[mcu_courant][index][bloc_courant][indice] = 0;
                            indice++;
                        }
                    /* Cas général*/
                    } else {
                        for (uint8_t i=0; i<nb_zeros; i++) {
                            res[mcu_courant][index][bloc_courant][indice] = 0;
                            indice++;
                        }
                        uint32_t ac = 0;
                        uint16_t nb_lus_ac = bitstream_read(stream, magnitude_ac, &ac, true);
                        if (nb_lus_ac != magnitude_ac){
                            fprintf(stderr, "Erreur de lecture ! ");
                        }
                        res[mcu_courant][index][bloc_courant][indice] = ((ac < (uint32_t) (1 << (magnitude_ac-1)))? (ac - ((1 << magnitude_ac)-1)) : ac);
                        indice++;
                    }

                }
            }

        }
    }
    jpeg_close(jdesc);
}
