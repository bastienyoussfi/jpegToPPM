#ifndef __HUFFMAN_H__
#define __HUFFMAN_H__

#include <stdint.h>
#include <stdbool.h>
#include "bitstream.h"


struct huff_table;

extern struct huff_table *huffman_load_table(struct bitstream *stream,
                                             uint16_t *nb_byte_read);

extern int8_t huffman_next_value(struct huff_table *table,
                                 struct bitstream *stream);

extern void huffman_free_table(struct huff_table *table);


#ifdef BLABLA
extern int8_t huffman_next_value_count(struct huff_table *table,
                                 struct bitstream *stream,
                                 uint8_t *nb_bits_read);
#endif
#endif
