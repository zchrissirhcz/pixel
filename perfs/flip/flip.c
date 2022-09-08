#include "flip.h"

#ifdef __ARM_NEON
#include <arm_neon.h>
#endif // __ARM_NEON

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void flip_horiz_rgb_naive(unsigned char* src, size_t height, size_t width, unsigned char* dst)
{
    unsigned char* src_line = src;
    unsigned char* dst_line = dst;
    size_t linebytes = width * 3;
    for (size_t i=0; i<height; i++) {
        for (size_t j=0; j<width; j++) {
            size_t src_idx = j*3;
            size_t dst_idx = (width-1-j)*3;
            dst_line[dst_idx  ] = src_line[src_idx];
            dst_line[dst_idx+1] = src_line[src_idx+1];
            dst_line[dst_idx+2] = src_line[src_idx+2];
        }
        src_line += linebytes;
        dst_line += linebytes;
    }
}

void flip_horiz_rgb_idxopt(unsigned char* src, size_t height, size_t width, unsigned char* dst)
{
    size_t linebytes = width * 3;
    size_t double_linebytes = linebytes * 2;
    src = src - linebytes;
    for (size_t i=0; i<height; i++) {
        src += double_linebytes;
        for (size_t j=0; j<width; j++) {
            *dst = src[-3];
            dst++;

            *dst = src[-2];
            dst++;

            *dst = src[-1];
            dst++;

            src -= 3;
        }
    }
}

void flip_horiz_rgb_asimd(unsigned char* src, size_t height, size_t width, unsigned char* dst)
{
    size_t done = 0;
    size_t linebytes = width * 3;
#ifdef __ARM_NEON
    size_t step = 8 * 3; // 64bits * 3
    size_t vec_size = linebytes - linebytes % step;
    src = src - step;
    uint8x8x3_t vsrc;
    uint8x8x3_t vdst;
    for (size_t i=0; i<height; i++) {
        src = src + linebytes;
        for (size_t j=0; j<vec_size; j+=step) {
            vsrc = vld3_u8(src);
            src -= step;
            vdst.val[0] = vrev64_u8(vsrc.val[0]);
            vdst.val[1] = vrev64_u8(vsrc.val[1]);
            vdst.val[2] = vrev64_u8(vsrc.val[2]);
            vst3_u8(dst, vdst);
            dst += step;
        }
        done = vec_size;
        for (; done<linebytes; done+=3) {
            dst[0] = src[0];
            dst[1] = src[1];
            dst[2] = src[2];
            dst += 3;
            src -= 3;
        }
        src += linebytes;
    }
#else
    printf("TODO: not implemented yet!\n");
#endif // __ARM_NEON
}


void flip_horiz_gray_naive(unsigned char* src, size_t height, size_t width, unsigned char* dst)
{
    unsigned char* dst_line = dst;
    unsigned char* src_line = src;
    for (size_t i=0; i<height; i++) {
        for (size_t j=0; j<width; j++) {
            dst_line[j] = src_line[width-1-j];
        }
        dst_line += width;
        src_line += width;
    }
}

void flip_horiz_gray_asimd(unsigned char* src, size_t height, size_t width, unsigned char* dst)
{
#ifdef __ARM_NEON
    size_t done = 0;
    size_t step = 8;
    size_t vec_size = width - width % step;
    src = src - step;
    uint8x8_t vsrc;
    uint8x8_t vdst;
    for (size_t i=0; i<height; i++) {
        src = src + width;
        for (size_t j=0; j<vec_size; j+=step) {
            vsrc = vld1_u8(src);
            src -= step;
            vdst = vrev64_u8(vsrc);
            vst1_u8(dst, vdst);
            dst += step;
        }
        done = vec_size;
        for (; done<width; done++) {
            *dst = *src;
            dst++;
            src--;
        }
        src += width;
    }
#else
    printf("TODO: not implemented yet!\n");
#endif // __ARM_NEON
}



//----------------------------------------------------------------------

void flip_vert_rgb_naive(unsigned char* src, size_t height, size_t width, unsigned char* dst)
{
    size_t linebytes = width * 3;
    for (size_t i=0; i<height; i++) {
        for (size_t j=0; j<width; j++) {
            size_t src_idx = i*linebytes + j*3;
            size_t dst_idx = (height-1-i)*linebytes + j*3;
            dst[dst_idx  ] = src[src_idx];
            dst[dst_idx+1] = src[src_idx+1];
            dst[dst_idx+2] = src[src_idx+2];
        }
    }
}

void flip_vert_rgb_bylines(unsigned char* src, size_t height, size_t width, unsigned char* dst)
{
    size_t linebytes = width * 3;
    unsigned char* src_line = src;
    unsigned char* dst_line = dst + (height-1)*linebytes;
    for (size_t i=0; i<height; i++) {
        memcpy(dst_line, src_line, linebytes);
        dst_line -= linebytes;
        src_line += linebytes;
    }
}


void flip_vert_gray_naive(unsigned char* src, size_t height, size_t width, unsigned char* dst)
{
    size_t linebytes = width;
    for (size_t i=0; i<height; i++) {
        for (size_t j=0; j<width; j++) {
            size_t src_idx = i*linebytes + j;
            size_t dst_idx = (height-1-i)*linebytes + j;
            dst[dst_idx] = src[src_idx];
        }
    }
}

void flip_vert_gray_bylines(unsigned char* src, size_t height, size_t width, unsigned char* dst)
{
    size_t linebytes = width;
    unsigned char* src_line = src;
    unsigned char* dst_line = dst + (height-1)*linebytes;
    for (size_t i=0; i<height; i++) {
        memcpy(dst_line, src_line, linebytes);
        dst_line -= linebytes;
        src_line += linebytes;
    }
}