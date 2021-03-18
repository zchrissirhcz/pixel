#ifndef PIXEL_MATCALC_MATRIX_COLUMN_MAX_H
#define PIXEL_MATCALC_MATRIX_COLUMN_MAX_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

// get max values in each column in a matrix
void matrix_column_max_u8_naive(unsigned char* src, size_t height, size_t width, unsigned char* max_vals);
void matrix_column_max_u8_cacheline(unsigned char* src, size_t height, size_t width, unsigned char* max_vals);
void matrix_column_max_u8_cacheline_asimd(unsigned char* src, size_t height, size_t width, unsigned char* max_vals);

//----------------------------------------------------------------------

// get max values in each column in a matrix, together with indices
void matrix_column_max_and_idx_u8_naive(unsigned char* src, size_t height, size_t width, unsigned char* max_vals, size_t* max_indices);
void matrix_column_max_and_idx_u8_cacheline(unsigned char* src, size_t height, size_t width, unsigned char* max_vals, size_t* max_indices);
void matrix_column_max_and_idx_u8_cacheline_asimd(unsigned char* src, size_t height, size_t width, unsigned char* max_vals, size_t* max_indices);

#ifdef __cplusplus
}
#endif

#endif // PIXEL_MATCALC_MATRIX_COLUMN_MAX_H