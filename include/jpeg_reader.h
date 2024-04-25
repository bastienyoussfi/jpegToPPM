#ifndef __JPEG_DESC_H__
#define __JPEG_DESC_H__

#include <stdint.h>
#include <stdbool.h>

#include "bitstream.h"


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


struct jpeg_desc;


// general
extern struct jpeg_desc *jpeg_read(const char *filename);
extern void jpeg_close(struct jpeg_desc *jpeg);
extern char *jpeg_get_filename(const struct jpeg_desc *jpeg);

// access to stream, placed just at the beginning of the scan raw data
extern struct bitstream *jpeg_get_bitstream(const struct jpeg_desc *jpeg);

// from DQT
extern uint8_t jpeg_get_nb_quantization_tables(const struct jpeg_desc *jpeg);
extern uint8_t *jpeg_get_quantization_table(const struct jpeg_desc *jpeg,
                                            uint8_t index);

// from DHT
extern uint8_t jpeg_get_nb_huffman_tables(const struct jpeg_desc *jpeg,
                                          enum sample_type acdc);
extern struct huff_table *jpeg_get_huffman_table(const struct jpeg_desc *jpeg,
                                                 enum sample_type acdc, uint8_t index);

// from Frame Header SOF0
extern uint16_t jpeg_get_image_size(struct jpeg_desc *jpeg, enum direction dir);
extern uint8_t jpeg_get_nb_components(const struct jpeg_desc *jpeg);

extern uint8_t jpeg_get_frame_component_id(const struct jpeg_desc *jpeg,
                                           uint8_t frame_comp_index);
extern uint8_t jpeg_get_frame_component_sampling_factor(const struct jpeg_desc *jpeg,
                                                        enum direction dir,
                                                        uint8_t frame_comp_index);
extern uint8_t jpeg_get_frame_component_quant_index(const struct jpeg_desc *jpeg,
                                                    uint8_t frame_comp_index);

// from Scan Header SOS
extern uint8_t jpeg_get_scan_component_id(const struct jpeg_desc *jpeg,
                                          uint8_t scan_comp_index);
extern uint8_t jpeg_get_scan_component_huffman_index(const struct jpeg_desc *jpeg,
                                                     enum sample_type,
                                                     uint8_t scan_comp_index);

#endif
