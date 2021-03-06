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

void matrix_transpose_u8_partition(unsigned char* src, uint32_t height, uint32_t width, unsigned char* dst, const uint32_t block_size)
{
    // 转置所有分块
    //const uint32_t block_size = 8;

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
void transpose_f32_4x4_asimd(float* data0, float* data1, float* data2, float* data3) {
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

void transpose_f32_8x8_asimd(float* data0, float* data1, float* data2, float* data3, float* data4, float* data5, float* data6, float* data7)
{
    // ----------------------------------------------
    // phase1
    float32x4_t q00 = vld1q_f32(data0);
    float32x4_t q10 = vld1q_f32(data1);
    float32x4x2_t r01_left = vtrnq_f32(q00, q10);

    float32x4_t q01 = vld1q_f32(data0+4);
    float32x4_t q11 = vld1q_f32(data1+4);
    float32x4x2_t r01_right = vtrnq_f32(q01, q11);

    float32x4_t q20 = vld1q_f32(data2);
    float32x4_t q30 = vld1q_f32(data3);
    float32x4x2_t r23_left = vtrnq_f32(q20, q30);

    float32x4_t q21 = vld1q_f32(data2+4);
    float32x4_t q31 = vld1q_f32(data3+4);
    float32x4x2_t r23_right = vtrnq_f32(q21, q31);

    float32x4_t q40 = vld1q_f32(data4);
    float32x4_t q50 = vld1q_f32(data5);
    float32x4x2_t r45_left = vtrnq_f32(q40, q50);

    float32x4_t q41 = vld1q_f32(data4+4);
    float32x4_t q51 = vld1q_f32(data5+4);
    float32x4x2_t r45_right = vtrnq_f32(q41, q51);

    float32x4_t q60 = vld1q_f32(data6);
    float32x4_t q70 = vld1q_f32(data7);
    float32x4x2_t r67_left = vtrnq_f32(q60, q70);

    float32x4_t q61 = vld1q_f32(data6+4);
    float32x4_t q71 = vld1q_f32(data7+4);
    float32x4x2_t r67_right = vtrnq_f32(q61, q71);

    q00 = vcombine_f32(vget_low_f32(r01_left.val[0]), vget_low_f32(r23_left.val[0])); //0, 10, 20, 30
    vst1q_f32(data0, q00);

    q01 = vcombine_f32(vget_low_f32(r45_left.val[0]), vget_low_f32(r67_left.val[0])); //40, 50, 60, 70
    vst1q_f32(data0+4, q01);

    q10 = vcombine_f32(vget_low_f32(r01_left.val[1]), vget_low_f32(r23_left.val[1])); //1, 11, 21, 31
    vst1q_f32(data1, q10);

    q11 = vcombine_f32(vget_low_f32(r45_left.val[1]), vget_low_f32(r67_left.val[1])); //41, 51, 61, 71
    vst1q_f32(data1+4, q11);

    q20 = vcombine_f32(vget_high_f32(r01_left.val[0]), vget_high_f32(r23_left.val[0])); //2, 12, 22, 32
    vst1q_f32(data2, q20);

    q21 = vcombine_f32(vget_high_f32(r45_left.val[0]), vget_high_f32(r67_left.val[0])); //42, 52, 62, 72
    vst1q_f32(data2+4, q21);

    q30 = vcombine_f32(vget_high_f32(r01_left.val[1]), vget_high_f32(r23_left.val[1])); //3, 13, 23, 33
    vst1q_f32(data3, q30);

    q31 = vcombine_f32(vget_high_f32(r45_left.val[1]), vget_high_f32(r67_left.val[1])); //43, 53, 63, 73
    vst1q_f32(data3+4, q31);

    q40 = vcombine_f32(vget_low_f32(r01_right.val[0]), vget_low_f32(r23_right.val[0])); //4, 14, 24, 34
    vst1q_f32(data4, q40);

    q41 = vcombine_f32(vget_low_f32(r45_right.val[0]), vget_low_f32(r67_right.val[0])); //44, 54, 64, 74
    vst1q_f32(data4+4, q41);

    q50 = vcombine_f32(vget_low_f32(r01_right.val[1]), vget_low_f32(r23_right.val[1])); //5, 15, 25, 35
    vst1q_f32(data5, q50);

    q51 = vcombine_f32(vget_low_f32(r45_right.val[1]), vget_low_f32(r67_right.val[1])); //45, 55, 65, 75
    vst1q_f32(data5+4, q51);

    q60 = vcombine_f32(vget_high_f32(r01_right.val[0]), vget_high_f32(r23_right.val[0])); //6, 16, 26, 36
    vst1q_f32(data6, q60);

    q61 = vcombine_f32(vget_high_f32(r45_right.val[0]), vget_high_f32(r67_right.val[0])); //46, 56, 66, 76
    vst1q_f32(data6+4, q61);

    q70 = vcombine_f32(vget_high_f32(r01_right.val[1]), vget_high_f32(r23_right.val[1])); //7, 17, 27, 37
    vst1q_f32(data7, q70);

    q71 = vcombine_f32(vget_high_f32(r45_right.val[1]), vget_high_f32(r67_right.val[1])); //47, 57, 67, 77
    vst1q_f32(data7+4, q71);


    // ----------------------------------------------
}



void transpose_u8_8x8_asimd(unsigned char* data0, unsigned char* data1, unsigned char* data2, unsigned char* data3,
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

void transpose_u8_16x16_asimd(unsigned char* data0, unsigned char* data1, unsigned char* data2, unsigned char* data3,
    unsigned char* data4, unsigned char* data5, unsigned char* data6, unsigned char* data7,
    unsigned char* data8, unsigned char* data9, unsigned char* data10, unsigned char* data11,
    unsigned char* data12, unsigned char* data13, unsigned char* data14, unsigned char* data15)
{
    uint8x16_t q0 = vld1q_u8(data0);
    uint8x16_t q1 = vld1q_u8(data1);
    uint8x16_t q2 = vld1q_u8(data2);
    uint8x16_t q3 = vld1q_u8(data3);
    uint8x16_t q4 = vld1q_u8(data4);
    uint8x16_t q5 = vld1q_u8(data5);
    uint8x16_t q6 = vld1q_u8(data6);
    uint8x16_t q7 = vld1q_u8(data7);
    uint8x16_t q8 = vld1q_u8(data8);
    uint8x16_t q9 = vld1q_u8(data9);
    uint8x16_t q10 = vld1q_u8(data10);
    uint8x16_t q11 = vld1q_u8(data11);
    uint8x16_t q12 = vld1q_u8(data12);
    uint8x16_t q13 = vld1q_u8(data13);
    uint8x16_t q14 = vld1q_u8(data14);
    uint8x16_t q15 = vld1q_u8(data15);
    // ----------------------------------------------
    // phase1
    uint8x16x2_t q01 = vtrnq_u8(q0, q1);
    uint8x16x2_t q23 = vtrnq_u8(q2, q3);
    uint8x16x2_t q45 = vtrnq_u8(q4, q5);
    uint8x16x2_t q67 = vtrnq_u8(q6, q7);
    uint8x16x2_t q89 = vtrnq_u8(q8, q9);
    uint8x16x2_t q1011 = vtrnq_u8(q10, q11);
    uint8x16x2_t q1213 = vtrnq_u8(q12, q13);
    uint8x16x2_t q1415 = vtrnq_u8(q14, q15);

    // phase2
    uint16x8_t v0 = vreinterpretq_u16_u8(q01.val[0]);
    uint16x8_t v1 = vreinterpretq_u16_u8(q01.val[1]);
    uint16x8_t v2 = vreinterpretq_u16_u8(q23.val[0]);
    uint16x8_t v3 = vreinterpretq_u16_u8(q23.val[1]);
    uint16x8_t v4 = vreinterpretq_u16_u8(q45.val[0]);
    uint16x8_t v5 = vreinterpretq_u16_u8(q45.val[1]);
    uint16x8_t v6 = vreinterpretq_u16_u8(q67.val[0]);
    uint16x8_t v7 = vreinterpretq_u16_u8(q67.val[1]);
    uint16x8_t v8 = vreinterpretq_u16_u8(q89.val[0]);
    uint16x8_t v9 = vreinterpretq_u16_u8(q89.val[1]);
    uint16x8_t v10 = vreinterpretq_u16_u8(q1011.val[0]);
    uint16x8_t v11 = vreinterpretq_u16_u8(q1011.val[1]);
    uint16x8_t v12 = vreinterpretq_u16_u8(q1213.val[0]);
    uint16x8_t v13 = vreinterpretq_u16_u8(q1213.val[1]);
    uint16x8_t v14 = vreinterpretq_u16_u8(q1415.val[0]);
    uint16x8_t v15 = vreinterpretq_u16_u8(q1415.val[1]);

    uint16x8x2_t v_tmp;
    v_tmp = vtrnq_u16(v0, v2); v0 = v_tmp.val[0]; v2 = v_tmp.val[1];
    v_tmp = vtrnq_u16(v1, v3); v1 = v_tmp.val[0]; v3 = v_tmp.val[1];
    v_tmp = vtrnq_u16(v4, v6); v4 = v_tmp.val[0]; v6 = v_tmp.val[1];
    v_tmp = vtrnq_u16(v5, v7); v5 = v_tmp.val[0]; v7 = v_tmp.val[1];
    v_tmp = vtrnq_u16(v8, v10); v8 = v_tmp.val[0]; v10 = v_tmp.val[1];
    v_tmp = vtrnq_u16(v9, v11); v9 = v_tmp.val[0]; v11 = v_tmp.val[1];
    v_tmp = vtrnq_u16(v12, v14); v12 = v_tmp.val[0]; v14 = v_tmp.val[1];
    v_tmp = vtrnq_u16(v13, v15); v13 = v_tmp.val[0]; v15 = v_tmp.val[1];

    // phase3
    uint32x4_t w0 = vreinterpretq_u32_u16(v0);
    uint32x4_t w1 = vreinterpretq_u32_u16(v1);
    uint32x4_t w2 = vreinterpretq_u32_u16(v2);
    uint32x4_t w3 = vreinterpretq_u32_u16(v3);
    uint32x4_t w4 = vreinterpretq_u32_u16(v4);
    uint32x4_t w5 = vreinterpretq_u32_u16(v5);
    uint32x4_t w6 = vreinterpretq_u32_u16(v6);
    uint32x4_t w7 = vreinterpretq_u32_u16(v7);
    uint32x4_t w8 = vreinterpretq_u32_u16(v8);
    uint32x4_t w9 = vreinterpretq_u32_u16(v9);
    uint32x4_t w10 = vreinterpretq_u32_u16(v10);
    uint32x4_t w11 = vreinterpretq_u32_u16(v11);
    uint32x4_t w12 = vreinterpretq_u32_u16(v12);
    uint32x4_t w13 = vreinterpretq_u32_u16(v13);
    uint32x4_t w14 = vreinterpretq_u32_u16(v14);
    uint32x4_t w15 = vreinterpretq_u32_u16(v15);

    uint32x4x2_t w_tmp;
    w_tmp = vtrnq_u32(w0, w4); w0=w_tmp.val[0]; w4=w_tmp.val[1];
    w_tmp = vtrnq_u32(w1, w5); w1=w_tmp.val[0]; w5=w_tmp.val[1];
    w_tmp = vtrnq_u32(w2, w6); w2=w_tmp.val[0]; w6=w_tmp.val[1];
    w_tmp = vtrnq_u32(w3, w7); w3=w_tmp.val[0]; w7=w_tmp.val[1];
    w_tmp = vtrnq_u32(w8, w12); w8=w_tmp.val[0]; w12=w_tmp.val[1];
    w_tmp = vtrnq_u32(w9, w13); w9=w_tmp.val[0]; w13=w_tmp.val[1];
    w_tmp = vtrnq_u32(w10, w14); w10=w_tmp.val[0]; w14=w_tmp.val[1];
    w_tmp = vtrnq_u32(w11, w15); w11=w_tmp.val[0]; w15=w_tmp.val[1];

    // phase 4
    uint64x2_t x_tmp;
    x_tmp = vcombine_u32(vget_low_u32(w0), vget_low_u32(w8)); q0 = vreinterpretq_u8_u64(x_tmp);
    x_tmp = vcombine_u32(vget_low_u32(w1), vget_low_u32(w9)); q1 = vreinterpretq_u8_u64(x_tmp);
    x_tmp = vcombine_u32(vget_low_u32(w2), vget_low_u32(w10)); q2 = vreinterpretq_u8_u64(x_tmp);
    x_tmp = vcombine_u32(vget_low_u32(w3), vget_low_u32(w11)); q3 = vreinterpretq_u8_u64(x_tmp);
    x_tmp = vcombine_u32(vget_low_u32(w4), vget_low_u32(w12)); q4 = vreinterpretq_u8_u64(x_tmp);
    x_tmp = vcombine_u32(vget_low_u32(w5), vget_low_u32(w13)); q5 = vreinterpretq_u8_u64(x_tmp);
    x_tmp = vcombine_u32(vget_low_u32(w6), vget_low_u32(w14)); q6 = vreinterpretq_u8_u64(x_tmp);
    x_tmp = vcombine_u32(vget_low_u32(w7), vget_low_u32(w15)); q7 = vreinterpretq_u8_u64(x_tmp);

    x_tmp = vcombine_u32(vget_high_u32(w0), vget_high_u32(w8)); q8 = vreinterpretq_u8_u64(x_tmp);
    x_tmp = vcombine_u32(vget_high_u32(w1), vget_high_u32(w9)); q9 = vreinterpretq_u8_u64(x_tmp);
    x_tmp = vcombine_u32(vget_high_u32(w2), vget_high_u32(w10)); q10 = vreinterpretq_u8_u64(x_tmp);
    x_tmp = vcombine_u32(vget_high_u32(w3), vget_high_u32(w11)); q11 = vreinterpretq_u8_u64(x_tmp);
    x_tmp = vcombine_u32(vget_high_u32(w4), vget_high_u32(w12)); q12 = vreinterpretq_u8_u64(x_tmp);
    x_tmp = vcombine_u32(vget_high_u32(w5), vget_high_u32(w13)); q13 = vreinterpretq_u8_u64(x_tmp);
    x_tmp = vcombine_u32(vget_high_u32(w6), vget_high_u32(w14)); q14 = vreinterpretq_u8_u64(x_tmp);
    x_tmp = vcombine_u32(vget_high_u32(w7), vget_high_u32(w15)); q15 = vreinterpretq_u8_u64(x_tmp);

    // ----------------------------------------------
    vst1q_u8(data0, q0);
    vst1q_u8(data1, q1);
    vst1q_u8(data2, q2);
    vst1q_u8(data3, q3);
    vst1q_u8(data4, q4);
    vst1q_u8(data5, q5);
    vst1q_u8(data6, q6);
    vst1q_u8(data7, q7);
    vst1q_u8(data8, q8);
    vst1q_u8(data9, q9);
    vst1q_u8(data10, q10);
    vst1q_u8(data11, q11);
    vst1q_u8(data12, q12);
    vst1q_u8(data13, q13);
    vst1q_u8(data14, q14);
    vst1q_u8(data15, q15);
}


#endif

// 基于 matrix_transpose_u8_partition8x8，把里面的分块内转置，改为neon intrinsic
void matrix_transpose_u8_partition_asimd(unsigned char* src, uint32_t height, uint32_t width, unsigned char* dst, const uint32_t block_size)
{
    // 转置所有分块
    //const uint32_t block_size = 8;

    uint32_t h_block = height - height % block_size;
    uint32_t w_block = width - width % block_size;
    uint32_t block_line_size = block_size * sizeof(unsigned char);
    uint32_t src_elem = width;
    uint32_t dst_elem = height;
    for (uint32_t i=0; i<h_block; i+=block_size) {
        for (uint32_t j=0; j<w_block; j+=block_size) {
            unsigned char* block_src = src + (i*width + j);
            unsigned char* block_dst = dst + (j*height + i);

            for (size_t k=0; k<block_size; k++) {
                memcpy(block_dst, block_src, block_line_size);
                block_src += src_elem;
                block_dst += dst_elem;
            }
        }
    }

    // 每个分块内部做转置
    for (uint32_t i=0; i<w_block; i+=block_size) {
        for (uint32_t j=0; j<h_block; j+=block_size) {
#if __ARM_NEON
            if (block_size==8) {
                unsigned char* block_ptr = dst + i*height + j;
                unsigned char* data0 = block_ptr;
                unsigned char* data1 = data0 + dst_elem;
                unsigned char* data2 = data1 + dst_elem;
                unsigned char* data3 = data2 + dst_elem;
                unsigned char* data4 = data3 + dst_elem;
                unsigned char* data5 = data4 + dst_elem;
                unsigned char* data6 = data5 + dst_elem;
                unsigned char* data7 = data6 + dst_elem;
                transpose_u8_8x8_asimd(data0, data1, data2, data3, data4, data5, data6, data7);
            }
            else if (block_size==16) {
                unsigned char* block_ptr = dst + i*height + j;
                unsigned char* data0 = block_ptr;
                unsigned char* data1 = data0 + dst_elem;
                unsigned char* data2 = data1 + dst_elem;
                unsigned char* data3 = data2 + dst_elem;
                unsigned char* data4 = data3 + dst_elem;
                unsigned char* data5 = data4 + dst_elem;
                unsigned char* data6 = data5 + dst_elem;
                unsigned char* data7 = data6 + dst_elem;
                unsigned char* data8 = data7 + dst_elem;
                unsigned char* data9 = data8 + dst_elem;
                unsigned char* data10 = data9 + dst_elem;
                unsigned char* data11 = data10 + dst_elem;
                unsigned char* data12 = data11 + dst_elem;
                unsigned char* data13 = data12 + dst_elem;
                unsigned char* data14 = data13 + dst_elem;
                unsigned char* data15 = data14 + dst_elem;
                transpose_u8_16x16_asimd(data0, data1, data2, data3, data4, data5, data6, data7, data8, data9, data10, data11, data12, data13, data14, data15);
            }
            else {
                for (uint32_t bi=0; bi<block_size; bi++) {
                    for (uint32_t bj=0; bj<bi; bj++) {
                        uint32_t src_idx = (i+bi)*height + (j+bj);
                        uint32_t dst_idx = (i+bj)*height + (j+bi);
                        unsigned char t = dst[dst_idx];
                        dst[dst_idx] = dst[src_idx];
                        dst[src_idx] = t;
                    }
                }
            }
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

//----------------------------------------------------------------------

void matrix_transpose_f32_naive(float* src, uint32_t height, uint32_t width, float* dst)
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

void matrix_transpose_f32_order_opt(float* src, uint32_t height, uint32_t width, float* dst)
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

void matrix_transpose_f32_partition(float* src, uint32_t height, uint32_t width, float* dst, const uint32_t block_size)
{
    // 转置所有分块
    //const uint32_t block_size = 8;

    uint32_t h_block = height - height % block_size;
    uint32_t w_block = width - width % block_size;
    uint32_t block_line_size = block_size * sizeof(float);
    uint32_t src_elem = width;
    uint32_t dst_elem = height;
    for (uint32_t i=0; i<h_block; i+=block_size) {
        for (uint32_t j=0; j<w_block; j+=block_size) {
            float* block_src = src + (i*width + j);
            float* block_dst = dst + (j*height + i);

            for (size_t k=0; k<block_size; k++) {
                memcpy(block_dst, block_src, block_line_size);
                block_src += src_elem;
                block_dst += dst_elem;
            }
        }
    }

    // 每个分块内部做转置
    for (uint32_t i=0; i<w_block; i+=block_size) {
        for (uint32_t j=0; j<h_block; j+=block_size) {
#if 0 // 用于调试，当block_size=4时，直接写出来每一个swap
            float* block_ptr0 = dst + (i*height + j);
            float* block_ptr1 = block_ptr0 + dst_elem;
            float* block_ptr2 = block_ptr1 + dst_elem;
            float* block_ptr3 = block_ptr2 + dst_elem;

            float t;

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
                    float t = dst[dst_idx];
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

// 基于 matrix_transpose_f32_partition8x8，把里面的分块内转置，改为neon intrinsic
void matrix_transpose_f32_partition_asimd(float* src, uint32_t height, uint32_t width, float* dst, const uint32_t block_size)
{
    // 转置所有分块
    //const uint32_t block_size = 8;

    uint32_t h_block = height - height % block_size;
    uint32_t w_block = width - width % block_size;
    uint32_t block_line_size = block_size * sizeof(float);
    uint32_t src_elem = width;
    uint32_t dst_elem = height;
    for (uint32_t i=0; i<h_block; i+=block_size) {
        for (uint32_t j=0; j<w_block; j+=block_size) {
            float* block_src = src + (i*width + j);
            float* block_dst = dst + (j*height + i);

            for (size_t k=0; k<block_size; k++) {
                memcpy(block_dst, block_src, block_line_size);
                block_src += src_elem;
                block_dst += dst_elem;
            }
        }
    }

    // 每个分块内部做转置
    for (uint32_t i=0; i<w_block; i+=block_size) {
        for (uint32_t j=0; j<h_block; j+=block_size) {
#if __ARM_NEON
            if (block_size==4) {
                float* block_ptr = dst + i*height + j;
                float* data0 = block_ptr;
                float* data1 = data0 + dst_elem;
                float* data2 = data1 + dst_elem;
                float* data3 = data2 + dst_elem;
                transpose_f32_4x4_asimd(data0, data1, data2, data3);
            }
            else if (block_size==8) {
                float* block_ptr = dst + i*height + j;
                float* data0 = block_ptr;
                float* data1 = data0 + dst_elem;
                float* data2 = data1 + dst_elem;
                float* data3 = data2 + dst_elem;
                float* data4 = data3 + dst_elem;
                float* data5 = data4 + dst_elem;
                float* data6 = data5 + dst_elem;
                float* data7 = data6 + dst_elem;
                transpose_f32_8x8_asimd(data0, data1, data2, data3, data4, data5, data6, data7);
            }
            else {
                for (uint32_t bi=0; bi<block_size; bi++) {
                    for (uint32_t bj=0; bj<bi; bj++) {
                        uint32_t src_idx = (i+bi)*height + (j+bj);
                        uint32_t dst_idx = (i+bj)*height + (j+bi);
                        float t = dst[dst_idx];
                        dst[dst_idx] = dst[src_idx];
                        dst[src_idx] = t;
                    }
                }
            }
#else
            for (uint32_t bi=0; bi<block_size; bi++) {
                for (uint32_t bj=0; bj<bi; bj++) {
                    uint32_t src_idx = (i+bi)*height + (j+bj);
                    uint32_t dst_idx = (i+bj)*height + (j+bi);
                    float t = dst[dst_idx];
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