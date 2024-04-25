#ifndef __QUANTIF_INVERSE_H__
#define __QUANTIF_INVERSE_H__

#include <stdint.h>
#include <stdbool.h>

extern void quantif_inv(uint8_t table_quantif[64], int32_t bloc_frequentiel[64], int32_t bloc_apres_quantif[64]);

#endif
