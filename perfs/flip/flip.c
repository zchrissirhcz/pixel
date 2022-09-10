#include "flip.h"
#include "px_assert.h"
#include "px_compare.h"

#ifdef __ARM_NEON
#include <arm_neon.h>
#endif // __ARM_NEON

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void flip_horiz_rgb_naive(px_image_t* src, px_image_t* dst)
{
    PX_ASSERT(src != NULL && dst != NULL);
    PX_ASSERT(px_image_equal_in_shape(src, dst, false));
    PX_ASSERT(src->channel == 3);

    const int cn = 3;
    const int width = src->width;
    const int height = src->height;

    for (int i = 0; i < height; i++)
    {
        unsigned char* src_line = src->data + i * src->stride;
        unsigned char* dst_line = dst->data + i * dst->stride;
        for (int j = 0; j < width; j++)
        {
            size_t src_idx = j * cn;
            size_t dst_idx = (width-1-j) * cn;
            dst_line[dst_idx + 0] = src_line[src_idx + 0];
            dst_line[dst_idx + 1] = src_line[src_idx + 1];
            dst_line[dst_idx + 2] = src_line[src_idx + 2];
        }
    }
}

void flip_horiz_rgb_idxopt(px_image_t* src_image, px_image_t* dst_image)
{
    unsigned char* src = src_image->data;

    const int cn = 3;
    const int width = src_image->width;
    const int height = src_image->height;

    size_t linebytes = width * cn;
    src = src - linebytes;
    for (int i = 0; i < height; i++)
    {
        // src line, end to begin
        unsigned char* sp = src_image->data + (i + 1) * src_image->stride;
        // dst line, begin to end
        unsigned char* dp = dst_image->data + i * dst_image->stride;
        for (int j = 0; j < width; j++)
        {
            *dp = sp[-3];
            dp++;

            *dp = sp[-2];
            dp++;

            *dp = sp[-1];
            dp++;

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


void flip_horiz_gray_naive(px_image_t* src, px_image_t* dst)
{
    PX_ASSERT(src != NULL && dst != NULL);
    PX_ASSERT(px_image_equal_in_shape(src, dst, false));
    PX_ASSERT(src->channel == 1);

    const int width = src->width;
    const int height = src->height;

    for (int i = 0; i < height; i++)
    {
        unsigned char* dst_line = dst->data + i * dst->stride;
        unsigned char* src_line = src->data + i * dst->stride;
        for (int j = 0; j < width; j++)
        {
            dst_line[j] = src_line[width-1-j];
        }
    }
}

void flip_horiz_gray_asimd(px_image_t* src, px_image_t* dst)
{
    PX_ASSERT(src != NULL && dst != NULL);
    PX_ASSERT(px_image_equal_in_shape(src, dst, false));
    PX_ASSERT(src->channel == 1);

    const int width = src->width;
    const int height = src->height;

    for (int i = 0; i < height; i++)
    {
        unsigned char* dp = dst->data + i * dst->stride;
        unsigned char* sp = src->data + (i + 1) * dst->stride;

#if __ARM_NEON
        int nn = width >> 3;
        int remain = width - (nn << 3);
#else
        int remain = width;
#endif // __ARM_NEON

#if __ARM_NEON
        uint8x8_t vsrc;
        uint8x8_t vdst;
        for (int j = 0; j < nn; j++)
        {
            sp -= 8;
            vsrc = vld1_u8(sp);
            vdst = vrev64_u8(vsrc);
            vst1_u8(dp, vdst);
            dp += 8;
        }
#endif // __ARM_NEON

        for (; remain > 0; remain--)
        {
            sp--;
            *dp++ = *sp;
        }
    }
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