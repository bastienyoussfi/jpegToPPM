#ifndef __IDTC__
#define __IDTC_H__

#include <stdint.h>
#include <stdbool.h>
#include "../include/matrices.h"

extern void iDCT_un_bloc(int32_t bloc_frequentiel[8][8], uint8_t bloc_spatial[8][8]);

extern float coefficient_C(float x);

#endif
