#include "matrix_column_max.h"
#include <stdlib.h>
#include <string.h>

#ifdef __ARM_NEON
#include <arm_neon.h>
#endif

void matrix_column_max_u8_naive(unsigned char* src, size_t height, size_t width, unsigned char* max_vals)
{
    for (size_t i = 0; i < width; i++) {
        for (size_t j = 0; j < height; j++) {
            size_t idx = j * width + i;
            if (src[idx] > max_vals[i]) {
                max_vals[i] = src[idx];
            }
        }
    }
}

void matrix_column_max_u8_cacheline(unsigned char* src, size_t height, size_t width, unsigned char* max_vals)
{
    memcpy(max_vals, src, width*sizeof(unsigned char));
    unsigned char* src_line = src + width;
    for (size_t i = 1; i < height; i++) {
        for (size_t j = 0; j < width; j++) {
            if (src_line[j] > max_vals[j]) {
                max_vals[j] = src_line[j];
            }
        }
        src_line += width;
    }
}

void matrix_column_max_u8_cacheline_asimd(unsigned char* src, size_t height, size_t width, unsigned char* max_vals)
{
    memcpy(max_vals, src, width*sizeof(unsigned char));
    unsigned char* src_line = src + width;

#if __ARM_NEON
    size_t step = 16;
    size_t vec_size = width - width % step;
    uint16x8_t vprev_indices;
    uint16x8_t vcur_indices;
#endif // __ARM_NEON

    for (size_t i=1; i<height; i++) {
        size_t done = 0;
#if __ARM_NEON
        vprev_indices = vdupq_n_u16(i-1);
        vcur_indices = vdupq_n_u16(i);
        for (size_t j=0; j<vec_size; j+=step) {
            uint8x16_t vsrc = vld1q_u8(src_line + j);
            uint8x16_t vmax_vals = vld1q_u8(max_vals + j);
            uint8x16_t vmask_gt = vcgtq_u8(vsrc, vmax_vals);
            vmax_vals = vbslq_u8(vmask_gt, vsrc, vmax_vals);
            vst1q_u8(max_vals + j, vmax_vals);
        }
        done = vec_size;
#endif // __ARM_NEON
        for (size_t j=done; j<width; j++) {
            if (src_line[j] > max_vals[j]) {
                max_vals[j] = src_line[j];
            }
        }
        src_line += width;
    }
}

//----------------------------------------------------------------------

void matrix_column_max_and_idx_u8_naive(unsigned char* src, size_t height, size_t width, unsigned char* max_vals, size_t* max_indices)
{
    for (size_t i = 0; i < width; i++) {
        for (size_t j = 0; j < height; j++) {
            size_t idx = j * width + i;
            if (src[idx] > max_vals[i]) {
                max_vals[i] = src[idx];
                max_indices[i] = j;
            }
        }
    }
}

void matrix_column_max_and_idx_u8_cacheline(unsigned char* src, size_t height, size_t width, unsigned char* max_vals, size_t* max_indices)
{
    memcpy(max_vals, src, width*sizeof(unsigned char));
    memset(max_indices, 0, width * sizeof(size_t));
    unsigned char* src_line = src + width;
    for (size_t i = 1; i < height; i++) {
        for (size_t j = 0; j < width; j++) {
            if (src_line[j] > max_vals[j]) {
                max_vals[j] = src_line[j];
                max_indices[j] = i;
            }
        }
        src_line += width;
    }
}

void matrix_column_max_and_idx_u8_cacheline_asimd(unsigned char* src, size_t height, size_t width, unsigned char* max_vals, size_t* max_indices)
{
    memcpy(max_vals, src, width*sizeof(unsigned char));
    unsigned char* src_line = src;

#if __ARM_NEON
    size_t step = 16;
    size_t vec_size = width - width % step;
    uint16x8_t vprev_indices;
    uint16x8_t vcur_indices;
#endif // __ARM_NEON

    for (size_t i=1; i<height; i++) {
        size_t done = 0;
#if __ARM_NEON
        vprev_indices = vdupq_n_u16(i-1);
        vcur_indices = vdupq_n_u16(i);
        for (size_t j=0; j<vec_size; j+=step) {
            uint8x16_t vsrc = vld1q_u8(src_line + j);
            uint8x16_t vmax_vals = vld1q_u8(max_vals + j);
            uint8x16_t vmask_gt = vcgtq_u8(vsrc, vmax_vals);
            vmax_vals = vbslq_u8(vmask_gt, vsrc, vmax_vals);
            vst1q_u8(max_vals + j, vmax_vals);

            // vmax_indices_high = vbslq_u8(vget_high_u8(vmask_gt), vprev_indices, vcur_indices);
            // vmax_indices_low = vbslq_u8(vget_low_u8(vmask_gt), vprev_indices, vcur_indices);
            // vst1q_u16(max_indices, );
        }
        done = vec_size;
#endif // __ARM_NEON
        for (size_t j=done; j<width; j++) {
            if (src_line[j] > max_vals[j]) {
                max_vals[j] = src_line[j];
            }
            max_indices[i] = j;
        }
        src_line += width;
    }
}