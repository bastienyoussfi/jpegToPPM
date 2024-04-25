#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>


/* On définit la structure de bitstream */

struct bitstream {
    uint8_t *buf; //Tableau où on stocke tous les octets du fichier
    uint32_t buf_size; // Taille du fichier (en octets)
    uint32_t current_byte_pos; // Octet courant dans buf. Commence à 0 jusqu'à buf_size
    uint8_t current_bit_pos; // bit courant dans l'octet courant. Commence à 0, jusqu'à 7
};


/* On crée un bitstream positionné au début du fichier passé en argument */

struct bitstream *bitstream_create(const char *filename){

    // On ouvre le fichier
    FILE* fichier = NULL;
    fichier = fopen(filename, "r");
    if (fichier != NULL)
    {
        struct bitstream *stream = (struct bitstream *)malloc(sizeof(struct bitstream));
        assert(stream);
        fseek (fichier, 0, SEEK_END); //On se place à la fin du fichier
        uint32_t size = ftell (fichier); //Donne la place du curseur dans le fichier, donc la taille du fichier comme on s'est placés à la fin
        stream->buf_size = size;
        stream->buf = (uint8_t *) malloc(size);
        assert(stream->buf);
        fseek (fichier, 0, SEEK_SET); //On se replace au début du fichier
        fread(stream->buf, size, 1, fichier); //On remplit le fichier dans le tableau
        stream->current_byte_pos = 0;
        stream->current_bit_pos = 0;
        fclose(fichier); //On ferme le fichier
        return stream;
   }
   else
   {
       printf("Impossible d'ouvrir le fichier %s", filename);
       return NULL;
   }
}


void bitstream_close(struct bitstream *stream)
{
    free(stream->buf);
    free(stream);
}

bool bitstream_is_empty(struct bitstream *stream)
{
    //Bitstream devient vide si l'octet courant est la taille du tableau et le bit courant est 0
    return ((stream->current_byte_pos == (stream->buf_size)) && (stream->current_bit_pos == 0));
}



uint8_t bitstream_read(struct bitstream *stream, uint8_t nb_bits, uint32_t *dest, bool discard_byte_stuffing)
{
    *dest = 0;
    for (uint32_t i = 0; i < nb_bits; i++) {
        if (bitstream_is_empty(stream)) {
            fprintf(stderr, "Il n'y a plus de bits à lire !");
            return i; //On n'a pu lire que i bits
        }
        if (stream->current_byte_pos) {
            if (discard_byte_stuffing && !(stream->current_bit_pos) && (stream->buf[stream->current_byte_pos-1] == 0xff) && !(stream->buf[stream->current_byte_pos])) {
                stream->current_byte_pos ++; //L'octet est ignoré
            }
        }
        uint32_t bit = (stream->buf[stream->current_byte_pos] & (1 << (7 - stream->current_bit_pos))) >> (7-stream->current_bit_pos); //On fait un masque pour récupérer le bon bit, puis on décale à droite
        bit = bit * (1 << (nb_bits-i-1)); //On attribue le bon poids au bit
        (*dest) += bit;
        if (stream->current_bit_pos == 7) {
            stream->current_byte_pos += 1; //Si on arrive au bout de l'octet, il faut passer au suivant
        }
        stream->current_bit_pos = (stream->current_bit_pos + 1) % 8; //On passe au bit suivant
    }
    return nb_bits; //On a bien lu nb_bits bits
}
