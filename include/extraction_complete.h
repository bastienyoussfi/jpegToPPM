#ifndef __EXTRACTION_COMPLETE__
#define __EXTRACTION_COMPLETE__

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "../include/huffman.h"
#include "../include/bitstream.h"
#include "../include/jpeg_reader.h"
#include "../include/matrices.h"

extern void extraction_couleur(const char *filename, int32_t ***res, uint32_t nb_mcu);

extern void extraction(const char *filename, int32_t **res, uint32_t nb_blocs);

extern void extraction_couleur_echantillonage(const char *filename, int32_t ****res, uint32_t nb_mcu);


#endif
