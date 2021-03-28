#include "matrix_transpose.h"
#include <stdlib.h>
#include <string.h>

#if __ARM_NEON
#include <arm_neon.h>
#endif // __ARM_NEON

void matrix_transpose_u8_naive(unsigned char* src, uint32_t height, uint32_t width, unsigned char* dst)
{
    // src: height x width
    // dst: width x height
    for (uint32_t i=0; i<height; i++) {
        uint32_t src_idx_base = i*width;
        for (uint32_t j=0; j<width; j++) {
            uint32_t dst_idx = j*height + i;
            uint32_t src_idx = src_idx_base + j;
            dst[dst_idx] = src[src_idx];
        }
    }
}

// 基于matrix_transpose_u8_naive()，调整了两个for的顺序
// 缓存一致性算法在读写内存的实现细节不同，写内存实现更加复杂。
// 在实现类似矩阵转置功能，即内存读写方面存在映射关系且无法保证内存访问都是连续操作情况下，可以尝试使用优先保证写内存连续，读内存跳跃的策略来保证性能。
// https://blog.csdn.net/yan31415/article/details/107611634
// 实测，无论是x64 linux还是armv8，这个结论都是错误的
void matrix_transpose_u8_order_opt(unsigned char* src, uint32_t height, uint32_t width, unsigned char* dst)
{
    for (uint32_t j=0; j<width; j++) {
        uint32_t dst_idx_base = j*height;
        for (uint32_t i=0; i<height; i++) {
            uint32_t dst_idx = dst_idx_base + i;
            uint32_t src_idx = i*width + j;
            dst[dst_idx] = src[src_idx];
        }
    }
}

void matrix_transpose_u8_partition(unsigned char* src, uint32_t height, uint32_t width, unsigned char* dst)
{
    // 转置所有分块
    const uint32_t block_size = 8;

    uint32_t h_block = height - height % block_size;
    uint32_t w_block = width - width % block_size;
    uint32_t block_line_size = block_size * sizeof(unsigned char);
    uint32_t src_linebytes = width * sizeof(unsigned char);
    uint32_t dst_linebytes = height * sizeof(unsigned char);
    for (uint32_t i=0; i<h_block; i+=block_size) {
        for (uint32_t j=0; j<w_block; j+=block_size) {
            unsigned char* block_src = src + (i*width + j);
            unsigned char* block_dst = dst + (j*height + i);
            
            for (size_t k=0; k<block_size; k++) {
                memcpy(block_dst, block_src, block_line_size);
                block_src += src_linebytes;
                block_dst += dst_linebytes;
            }
        }
    }

    // 每个分块内部做转置
    for (uint32_t i=0; i<w_block; i+=block_size) {
        for (uint32_t j=0; j<h_block; j+=block_size) {
#if 0 // 用于调试，当block_size=4时，直接写出来每一个swap
            unsigned char* block_ptr0 = dst + (i*height + j);
            unsigned char* block_ptr1 = block_ptr0 + dst_linebytes;
            unsigned char* block_ptr2 = block_ptr1 + dst_linebytes;
            unsigned char* block_ptr3 = block_ptr2 + dst_linebytes;

            unsigned char t;

            t = block_ptr1[0]; block_ptr1[0] = block_ptr0[1]; block_ptr0[1] = t; // 0,1 <=> 1,0
            t = block_ptr2[0]; block_ptr2[0] = block_ptr0[2]; block_ptr0[2] = t; // 0,2 <=> 2,0
            t = block_ptr3[0]; block_ptr3[0] = block_ptr0[3]; block_ptr0[3] = t; // 0,3 <=> 3,0
            
            t = block_ptr1[2]; block_ptr1[2] = block_ptr2[1]; block_ptr2[1] = t; // 1,2 <=> 2,1
            t = block_ptr1[3]; block_ptr1[3] = block_ptr3[1]; block_ptr3[1] = t; // 1,3 <=> 3,1

            t = block_ptr2[3]; block_ptr2[3] = block_ptr3[2]; block_ptr3[2] = t; // 2,3 <=> 3,2
#else
            for (uint32_t bi=0; bi<block_size; bi++) {
                for (uint32_t bj=0; bj<bi; bj++) {
                    uint32_t src_idx = (i+bi)*height + (j+bj);
                    uint32_t dst_idx = (i+bj)*height + (j+bi);
                    unsigned char t = dst[dst_idx];
                    dst[dst_idx] = dst[src_idx];
                    dst[src_idx] = t;
                }
            }
#endif
        }
    }

    // 处理 leftover 元素，包括行尾和列尾
    // 行尾
    for (uint32_t i=0; i<h_block; i++) {
        for (uint32_t j=w_block; j<width; j++) {
            uint32_t src_idx = i*width + j;
            uint32_t dst_idx = j*height + i;
            dst[dst_idx] = src[src_idx];
        }
    }
    // 列尾
    for (uint32_t i=h_block; i<height; i++) {
        for (uint32_t j=0; j<width; j++) {
            uint32_t src_idx = i*width + j;
            uint32_t dst_idx = j*height + i;
            dst[dst_idx] = src[src_idx];
        }
    }
}

#if __ARM_NEON
void transpose_f32_4x4(float* data0, float* data1, float* data2, float* data3) {
    float32x4_t q0 = vld1q_f32(data0);
    float32x4_t q1 = vld1q_f32(data1);
    float32x4_t q2 = vld1q_f32(data2);
    float32x4_t q3 = vld1q_f32(data3);

    // ----------------------------------------------
    float32x4x2_t q01 = vtrnq_f32(q0, q1);
    float32x4x2_t q23 = vtrnq_f32(q2, q3);
 
    float32x4_t qq0 = q01.val[0];
    float32x2_t d00 = vget_low_f32(qq0);
    float32x2_t d01 = vget_high_f32(qq0);
 
    float32x4_t qq1 = q01.val[1];
    float32x2_t d10 = vget_low_f32(qq1);
    float32x2_t d11 = vget_high_f32(qq1);
 
    float32x4_t qq2 = q23.val[0];
    float32x2_t d20 = vget_low_f32(qq2);
    float32x2_t d21 = vget_high_f32(qq2);
 
    float32x4_t qq3 = q23.val[1];
    float32x2_t d30 = vget_low_f32(qq3);
    float32x2_t d31 = vget_high_f32(qq3);
 
    q0 = vcombine_f32(d00, d20);
    q1 = vcombine_f32(d10, d30);
    q2 = vcombine_f32(d01, d21);
    q3 = vcombine_f32(d11, d31);
    // ----------------------------------------------

    vst1q_f32(data0, q0);
    vst1q_f32(data1, q1);
    vst1q_f32(data2, q2);
    vst1q_f32(data3, q3);
}



void transpose_u8_8x8(unsigned char* data0, unsigned char* data1, unsigned char* data2, unsigned char* data3, 
    unsigned char* data4, unsigned char* data5, unsigned char* data6, unsigned char* data7)
{
    uint8x8_t d0 = vld1_u8(data0);
    uint8x8_t d1 = vld1_u8(data1);
    uint8x8_t d2 = vld1_u8(data2);
    uint8x8_t d3 = vld1_u8(data3);
    uint8x8_t d4 = vld1_u8(data4);
    uint8x8_t d5 = vld1_u8(data5);
    uint8x8_t d6 = vld1_u8(data6);
    uint8x8_t d7 = vld1_u8(data7);
    // ----------------------------------------------
    // phase1
    uint8x8x2_t d01 = vtrn_u8(d0, d1);
    uint8x8x2_t d23 = vtrn_u8(d2, d3);
    uint8x8x2_t d45 = vtrn_u8(d4, d5);
    uint8x8x2_t d67 = vtrn_u8(d6, d7);

    // phase2
    uint16x4_t v0 = vreinterpret_u16_u8(d01.val[0]);
    uint16x4_t v1 = vreinterpret_u16_u8(d01.val[1]);
    uint16x4_t v2 = vreinterpret_u16_u8(d23.val[0]);
    uint16x4_t v3 = vreinterpret_u16_u8(d23.val[1]);
    uint16x4_t v4 = vreinterpret_u16_u8(d45.val[0]);
    uint16x4_t v5 = vreinterpret_u16_u8(d45.val[1]);
    uint16x4_t v6 = vreinterpret_u16_u8(d67.val[0]);
    uint16x4_t v7 = vreinterpret_u16_u8(d67.val[1]);

    uint16x4x2_t v_tmp;
    v_tmp = vtrn_u16(v0, v2); v0 = v_tmp.val[0]; v2 = v_tmp.val[1];
    v_tmp = vtrn_u16(v1, v3); v1 = v_tmp.val[0]; v3 = v_tmp.val[1];
    v_tmp = vtrn_u16(v4, v6); v4 = v_tmp.val[0]; v6 = v_tmp.val[1];
    v_tmp = vtrn_u16(v5, v7); v5 = v_tmp.val[0]; v7 = v_tmp.val[1];

    // phase3
    uint32x2_t w0 = vreinterpret_u32_u16(v0);
    uint32x2_t w1 = vreinterpret_u32_u16(v1);
    uint32x2_t w2 = vreinterpret_u32_u16(v2);
    uint32x2_t w3 = vreinterpret_u32_u16(v3);
    uint32x2_t w4 = vreinterpret_u32_u16(v4);
    uint32x2_t w5 = vreinterpret_u32_u16(v5);
    uint32x2_t w6 = vreinterpret_u32_u16(v6);
    uint32x2_t w7 = vreinterpret_u32_u16(v7);

    uint32x2x2_t w_tmp;
    w_tmp = vtrn_u32(w0, w4); w0=w_tmp.val[0]; w4=w_tmp.val[1];
    w_tmp = vtrn_u32(w1, w5); w1=w_tmp.val[0]; w5=w_tmp.val[1];
    w_tmp = vtrn_u32(w2, w6); w2=w_tmp.val[0]; w6=w_tmp.val[1];
    w_tmp = vtrn_u32(w3, w7); w3=w_tmp.val[0]; w7=w_tmp.val[1];

    d0 = vreinterpret_u8_u32(w0);
    d1 = vreinterpret_u8_u32(w1);
    d2 = vreinterpret_u8_u32(w2);
    d3 = vreinterpret_u8_u32(w3);
    d4 = vreinterpret_u8_u32(w4);
    d5 = vreinterpret_u8_u32(w5);
    d6 = vreinterpret_u8_u32(w6);
    d7 = vreinterpret_u8_u32(w7);

    // ----------------------------------------------
    vst1_u8(data0, d0);
    vst1_u8(data1, d1);
    vst1_u8(data2, d2);
    vst1_u8(data3, d3);
    vst1_u8(data4, d4);
    vst1_u8(data5, d5);
    vst1_u8(data6, d6);
    vst1_u8(data7, d7);
}


#endif

// 基于 matrix_transpose_u8_partition，把里面的分块内转置，改为neon intrinsic
void matrix_transpose_u8_asimd(unsigned char* src, uint32_t height, uint32_t width, unsigned char* dst)
{
    // 转置所有分块
    const uint32_t block_size = 8;

    uint32_t h_block = height - height % block_size;
    uint32_t w_block = width - width % block_size;
    uint32_t block_line_size = block_size * sizeof(unsigned char);
    uint32_t src_linebytes = width * sizeof(unsigned char);
    uint32_t dst_linebytes = height * sizeof(unsigned char);
    for (uint32_t i=0; i<h_block; i+=block_size) {
        for (uint32_t j=0; j<w_block; j+=block_size) {
            unsigned char* block_src = src + (i*width + j);
            unsigned char* block_dst = dst + (j*height + i);
            
            for (size_t k=0; k<block_size; k++) {
                memcpy(block_dst, block_src, block_line_size);
                block_src += src_linebytes;
                block_dst += dst_linebytes;
            }
        }
    }

    // 每个分块内部做转置
    for (uint32_t i=0; i<w_block; i+=block_size) {
        for (uint32_t j=0; j<h_block; j+=block_size) {
#if __ARM_NEON
            unsigned char* block_ptr = dst + i*height + j;
            unsigned char* data0 = block_ptr;
            unsigned char* data1 = data0 + dst_linebytes;
            unsigned char* data2 = data1 + dst_linebytes;
            unsigned char* data3 = data2 + dst_linebytes;
            unsigned char* data4 = data3 + dst_linebytes;
            unsigned char* data5 = data4 + dst_linebytes;
            unsigned char* data6 = data5 + dst_linebytes;
            unsigned char* data7 = data6 + dst_linebytes;
            transpose_u8_8x8(data0, data1, data2, data3, data4, data5, data6, data7);
#else
            for (uint32_t bi=0; bi<block_size; bi++) {
                for (uint32_t bj=0; bj<bi; bj++) {
                    uint32_t src_idx = (i+bi)*height + (j+bj);
                    uint32_t dst_idx = (i+bj)*height + (j+bi);
                    unsigned char t = dst[dst_idx];
                    dst[dst_idx] = dst[src_idx];
                    dst[src_idx] = t;
                }
            }
#endif // __ARM_NEON
        }
    }

    // 处理 leftover 元素，包括行尾和列尾
    // 行尾
    for (uint32_t i=0; i<h_block; i++) {
        for (uint32_t j=w_block; j<width; j++) {
            uint32_t src_idx = i*width + j;
            uint32_t dst_idx = j*height + i;
            dst[dst_idx] = src[src_idx];
        }
    }
    // 列尾
    for (uint32_t i=h_block; i<height; i++) {
        for (uint32_t j=0; j<width; j++) {
            uint32_t src_idx = i*width + j;
            uint32_t dst_idx = j*height + i;
            dst[dst_idx] = src[src_idx];
        }
    }
}