#ifndef PIXEL_MATCALC_MATRIX_COLUMN_MAX_H
#define PIXEL_MATCALC_MATRIX_COLUMN_MAX_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

void matrix_column_max_u8_naive(unsigned char* src, size_t height, size_t width, unsigned char* result);

#ifdef __cplusplus
}
#endif

#endif // PIXEL_MATCALC_MATRIX_COLUMN_MAX_H