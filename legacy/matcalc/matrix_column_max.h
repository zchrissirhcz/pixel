#pragma once

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// get max values in each column in a matrix
void matrix_column_max_u8_naive(unsigned char* src, size_t height, size_t width, unsigned char* max_vals);
void matrix_column_max_u8_cacheline(unsigned char* src, size_t height, size_t width, unsigned char* max_vals);
void matrix_column_max_u8_cacheline_asimd(unsigned char* src, size_t height, size_t width, unsigned char* max_vals);

//----------------------------------------------------------------------

// get max values in each column in a matrix, together with indices
void matrix_column_max_and_idx_u8_naive(unsigned char* src, size_t height, size_t width, unsigned char* max_vals, uint32_t* max_indicies);
void matrix_column_max_and_idx_u8_cacheline(unsigned char* src, size_t height, size_t width, unsigned char* max_vals, uint32_t* max_indicies);
void matrix_column_max_and_idx_u8_cacheline_asimd(unsigned char* src, size_t height, size_t width, unsigned char* max_vals, uint32_t* max_indicies);

#ifdef __cplusplus
}
#endif
