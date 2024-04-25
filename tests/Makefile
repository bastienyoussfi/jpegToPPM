CC = gcc
LD = gcc

# -O0 désactive les optimisations à la compilation
# C'est utile pour débugger, par contre en "production"
# on active au moins les optimisations de niveau 2 (-O2).
CFLAGS = -Wall -Wextra -std=c99 -Iinclude -O0 -g
LDFLAGS = -lm

# Par défaut, on compile tous les fichiers source (.c) qui se trouvent dans le
# répertoire src/
SRC_FILES=$(wildcard src/*.c)
OLD_SRC_FILES=$(wildcard ../src/*.c)

# Par défaut, la compilation de src/toto.c génère le fichier objet obj/toto.o
OBJ_FILES=$(patsubst src/%.c,obj/%.o,$(SRC_FILES))
OLD_OBJ_FILES=$(patsubst ../src/%.c,../obj/%.o,$(SRC_FILES))

# Fichiers objet "prof". Lorsque vous aurez implémenté un de ces modules, il
# faudra le retirer de cette liste pour lier jpeg2ppm à votre implémentation du
# module en question. Le module htables_prof.o, qui contient la déclaration des tables
# de Huffman génériques sous forme de constantes, n'est pas à ré-implémenter.

OBJ_PROF_FILES = ../obj_prof/jpeg_reader_prof.o \
				 ../obj_prof/huffman_prof.o \
				 ../obj_prof/bitstream_prof.o

all : test_extraction_complete test_iDCT test_quantif_inverse test_zig_zag_inverse

test_extraction_complete: obj/test_extraction_complete.o ../obj/extraction_complete.o  ../obj/matrices.o $(OBJ_PROF_FILES)
	gcc -o test_extraction_complete obj/test_extraction_complete.o ../obj/extraction_complete.o ../obj/matrices.o $(OBJ_PROF_FILES)

obj/extraction_complete.o: src/test_extraction_complete.c
	gcc -o obj/test_extraction_complete.o -c src/test_extraction_complete.c $(CFLAGS)

test_iDCT: obj/test_iDCT.o ../obj/iDCT.o ../obj/matrices.o
	gcc -o test_iDCT obj/test_iDCT.o ../obj/iDCT.o ../obj/matrices.o $(LDFLAGS) $(CFLAGS)

obj/test_iDCT.o: src/test_iDCT.c
	gcc -o obj/test_iDCT.o -c src/test_iDCT.c $(LDFLAGS) $(CFLAGS)

test_quantif_inverse: obj/test_quantif_inverse.o ../obj/quantif_inverse.o ../obj/matrices.o
	gcc -o test_quantif_inverse obj/test_quantif_inverse.o ../obj/quantif_inverse.o ../obj/matrices.o $(LDFLAGS) $(CFLAGS)

obj/test_quantif_inverse.o: src/test_quantif_inverse.c
	gcc -o obj/test_quantif_inverse.o -c src/test_quantif_inverse.c $(CFLAGS)

test_zig_zag_inverse: obj/test_zig_zag_inverse.o ../obj/zig_zag_inverse.o ../obj/matrices.o
	gcc -o test_zig_zag_inverse obj/test_zig_zag_inverse.o ../obj/zig_zag_inverse.o ../obj/matrices.o $(LDFLAGS) $(CFLAGS)

obj/test_zig_zag_inverse.o: src/test_zig_zag_inverse.c
	gcc -o obj/test_zig_zag_inverse.o -c src/test_zig_zag_inverse.c $(CFLAGS)

obj/%.o: src/%.c
	$(CC) -c $(CFLAGS) $< -o $@

.PHONY: clean

clean:
		rm -rf $(OBJ_FILES)
