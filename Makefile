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

# Par défaut, la compilation de src/toto.c génère le fichier objet obj/toto.o
OBJ_FILES=$(patsubst src/%.c,obj/%.o,$(SRC_FILES))

# Fichiers objet "prof". Lorsque vous aurez implémenté un de ces modules, il
# faudra le retirer de cette liste pour lier jpeg2ppm à votre implémentation du
# module en question. Le module htables_prof.o, qui contient la déclaration des tables
# de Huffman génériques sous forme de constantes, n'est pas à ré-implémenter.

#gcc -o YCbCr_to_RBG obj/YCbCr_to_RGB.o obj/i

OBJ_PROF_FILES = obj_prof/jpeg_reader_prof.o \
				 obj_prof/huffman_prof.o


OBJ_RGB =		 obj/iDCT.o \

all: jpeg2ppm

jpeg2ppm: $(OBJ_FILES) $(OBJ_PROF_FILES)
	$(LD) $(OBJ_FILES) $(OBJ_PROF_FILES) $(LDFLAGS) -o $@

obj/iDCT.o: src/iDCT.c
	gcc -o obj/iDCT.o -c src/iDCT.c $(LDFLAGS)


obj/%.o: src/%.c
	$(CC) -c $(CFLAGS) $< -o $@

.PHONY: clean
	images

clean:
	rm -rf jpeg2ppm $(OBJ_FILES)

ppmclean:
	rm -rf images/*.ppm images/*.pgm
