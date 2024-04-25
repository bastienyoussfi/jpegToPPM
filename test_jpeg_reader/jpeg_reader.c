#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "../include/bitstream.h"


enum color_component {
    Y,
    Cb,
    Cr,
    NB_COLOR_COMPONENTS
};

enum direction {
    H,
    V,
    NB_DIRECTIONS
};

enum sample_type {
    DC,
    AC,
    NB_SAMPLE_TYPES
};



////// Structure  //

struct jpeg_desc{
    //contient toutes les informations lues dans les entêtes des sections jpeg
    // est-ce que je dois malloc tout ça?
    // où dans jpeg_read?
    char *nom_fichier;
    uint16_t hauteur_image;
    uint16_t longueur_image;
    uint8_t nb_components;
    uint8_t nb_quantization_tables;
    uint8_t * quantization_table;
    uint8_t nb_huffman_tables_AC;
    uint8_t nb_huffman_tables_DC;
    struct huff_table *huffman_table_AC;
    struct huff_table *huffman_table_DC;
    uint8_t *frame_component_id;
    uint8_t **frame_component_sampling_factor; //H en prem V en deux.
    uint8_t *frame_component_quant_index;
    uint8_t *scan_component_id;
    uint8_t *scan_component_huffman_index_AC;
    uint8_t *scan_component_huffman_index_DC;
};



////// Fonctions sur jdesc //

struct jpeg_desc *jpeg_read(const char *filename)
{
    //ouvre le fichier JPEG, lit tous les entêtes de sections rencontrés et stocke les
    //informations requises dans le descripteur retourné. La lecture est stoppée au début des données
    //brutes codant l'image (après l'entête de la section SOS) ;
    // 0xffd8 = début image
    // 0xffd9 = fin image
    struct jpeg_desc *jpeg = (struct jpeg_desc *)malloc(sizeof(struct jpeg_desc));
    jpeg->nom_fichier = filename;
    uint32_t *bit_lu;
    struct bitstream *stream = bitstream_create(filename);
    bitstream_read(stream, 16, bit_lu, false);
    uint16_t SOI = 0xffd8; //debut image
    uint16_t SOF = 0xffd9; //fin image
    uint16_t APPO = 0xffe0;
    uint16_t COM = 0xfffe;
    uint16_t QDT = 0xffdb;
    uint16_t SOFO = 0xffc0;
    uint16_t DHT = 0xffc4;
    //à chaque fois après le marqueur on a la longueur de la section sur 2 octets puis les données.
    uint32_t *octets_courants;
    bitstream_read(stream, 8, bit_lu, false);
    uint16_t *longueur_section;

    if (*bit_lu != SOI){
        printf("Erreur lecture image");
    }
    while(*octets_courants != SOF){
        if (*octets_courants == APPO){
            bitstream_read(stream, 2, octets_courants, false);
        }
    }
    // APPO-
}

void jpeg_close(struct jpeg_desc *jpeg){
    //ferme un descripteur préalablement ouvert, en libérant toute la mémoire nécessaire.
    // ALED
}

struct bitstream *jpeg_get_bitstream(const struct jpeg_desc *jpeg){
    //retourne l'adresse d'une variable de type struct bitstream permettant
    //de lire les données brutes d'image encodées dans le flux
    //ALED
}

char *jpeg_get_filename(const struct jpeg_desc *jpeg){
    //retourne le nom de fichier de l'image ouverte
    return jpeg->nom_fichier;
}





////// Tables quantif //

uint8_t jpeg_get_nb_quantization_tables(const struct jpeg_desc *jpeg){
    //retourne le nombre de tables de quantification.
    return jpeg->nb_quantization_tables;
}

uint8_t *jpeg_get_quantization_table(const struct jpeg_desc *jpeg, uint8_t index){
    // retourne l'adresse mémoire de la i-ième table de quantification
    // un tableau de 64 octets non-signés.
    return jpeg->quantization_table[index];
}




////// Tables Huffman //

uint8_t jpeg_get_nb_huffman_tables(const struct jpeg_desc *jpeg, enum sample_type acdc){
    //retourne le nombre de tables de Huffman de type AC ou DC.
    if (acdc = AC){
        return jpeg->nb_huffman_tables_AC;
    }else if (acdc = DC){
        return jpeg->nb_huffman_tables_DC;
    }else{
        printf("erreur type de table de huffman");
    }
}

struct huff_table *jpeg_get_huffman_table(const struct jpeg_desc *jpeg, enum sample_type acdc, uint8_t index){
    //retourne un pointeur vers la i-ième table de Huffman de type DC ou AC
    //Le type de cette table est struct huff_table (fourni par huffman.h)
    if (acdc = AC){
        return jpeg->huffman_table_AC[index];
    }else if (acdc = DC){
        return jpeg->huffman_table_DC[index];
    }else{
        printf("erreur type de table de huffman");
    }
}




////// Frame Header //

uint16_t jpeg_get_image_size(struct jpeg_desc *jpeg, enum direction dir){
    //retourne la taille de l'image (nombre de pixels) dans la direction dir
    if (dir = H){
        return jpeg->longueur_image;
    }else if (dir = V){
        return jpeg->hauteur_image;
    }else{
        printf("erreur direction inconnue");
    }
}

uint8_t jpeg_get_nb_components(const struct jpeg_desc *jpeg){
    //retourne le nombre de composantes de couleur
    //(1 pour une image en niveau de gris, 3 pour une image en YCbCr, ...)
    return jpeg->nb_components;
}

uint8_t jpeg_get_frame_component_id(const struct jpeg_desc *jpeg,  uint8_t frame_comp_index){
    //retourne l'identifiant de la i\ieme composante définie dans le Frame Header.
    return jpeg->frame_component_id[frame_comp_index];

}

uint8_t jpeg_get_frame_component_sampling_factor(const struct jpeg_desc *jpeg, enum direction dir, uint8_t frame_comp_index){
    // retourne le facteur de sous-échantillonnage
    //dans la direction dir de la i-i ème composante de couleur
    if (dir = H){
        return jpeg->frame_component_sampling_factor[frame_comp_index][0];
    }
    else if (dir = V){
        return jpeg->frame_component_sampling_factor[frame_comp_index][1];
    }
}

uint8_t jpeg_get_frame_component_quant_index(const struct jpeg_desc *jpeg, uint8_t frame_comp_index){
    //retourne l'index de la table de quantification de la i-ième composante de couleur.
    return jpeg->frame_component_quant_index[frame_comp_index];
}



////// Scan Header //

uint8_t jpeg_get_scan_component_id(const struct jpeg_desc *jpeg, uint8_t scan_comp_index){
    //retourne l'identifiant de la i-ième composante lue dans le scan. Cet identifiant doit être un des
    //identifiants de composantes définis dans le Frame Header
    return jpeg->scan_component_id[scan_comp_index];
}

uint8_t jpeg_get_scan_component_huffman_index(const struct jpeg_desc *jpeg,enum sample_type acdc, uint8_t scan_comp_index){
    //retourne l'identifiant de la i-ième composante lue dans le scan. Cet identifiant doit être un des identifiants
    //de composantes définis dans le Frame Header
    if (acdc = AC){
        return jpeg->scan_component_huffman_index_DC[scan_comp_index];
    }else if (acdc = DC){
        return jpeg->scan_component_huffman_index_AC[scan_comp_index];
    }else{
        printf("erreur type de table de huffman");
    }
}
