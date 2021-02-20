#include "rgb2gray.h"
#include <stdint.h>

void rgb_to_gray_naive(unsigned char* rgb_buf, size_t height, size_t width, size_t rgb_linebytes, unsigned char* gray_buf, size_t gray_linebytes)
{
    for (size_t i=0; i<height; i++) {
        for (size_t j=0; j<width; j++) {
            size_t idx = i*rgb_linebytes + j*3;

            float r = rgb_buf[idx];
            float g = rgb_buf[idx+1];
            float b = rgb_buf[idx+2];
            float gray = (0.299*r + 0.587*g + 0.114*b);

            size_t dst_idx = i*gray_linebytes + j;
            gray_buf[dst_idx] = gray;
        }
    }
}


void rgb_to_gray_fast(unsigned char* rgb_buf, size_t height, size_t width, size_t rgb_linebytes, unsigned char* gray_buf, size_t gray_linebytes)
{
    for (size_t i=0; i<height; i++) {
        for (size_t j=0; j<width; j++) {
            size_t idx = i*rgb_linebytes + j*3;

#if 0 // 直观的定点化实现
            int r = rgb_buf[idx];
            int g = rgb_buf[idx+1];
            int b = rgb_buf[idx+2];
            int gray = (299*r + 587*g + 114*b) / 1000;
#elif 0 // 缩放倍数从1000改为1024
            int r = rgb_buf[idx];
            int g = rgb_buf[idx+1];
            int b = rgb_buf[idx+2];
            int gray = (306*r + 601*g + 117*b) / 1024;
#elif 0 // 移位操作替代除法，加速
            int r = rgb_buf[idx];
            int g = rgb_buf[idx+1];
            int b = rgb_buf[idx+2];
            int gray = (306*r + 601*g + 117*b) / 1024;
#elif 1 // 缩放256(2^8)而不是1024倍，移位会快吗？
            int r = rgb_buf[idx];
            int g = rgb_buf[idx+1];
            int b = rgb_buf[idx+2];
            int gray = (77*r + 151*g + 28*b) >> 8;
#endif
            size_t dst_idx = i*gray_linebytes + j;
            gray_buf[dst_idx] = gray;
        }
    }
}

void rgb_to_gray_fast2(unsigned char* rgb_buf, size_t height, size_t width, size_t rgb_linebytes, unsigned char* gray_buf, size_t gray_linebytes)
{
    const static int wr = (int)(0.299 * 256 + 0.5);
    const static int wg = (int)(0.587 * 256 + 0.5);
    const static int wb = (int)(0.114 * 256 + 0.5);
    size_t gray_gap = gray_linebytes - width;
    size_t rgb_gap = rgb_linebytes - width*3;
    for (size_t i=0; i<height; i++) {
        for (size_t j=0; j<width; j++) {
            *gray_buf = (wr*rgb_buf[0] + wg*rgb_buf[1] + wb*rgb_buf[2]) >> 8;
            rgb_buf += 3;
            gray_buf++;
        }
        gray_buf += gray_gap;
        rgb_buf += rgb_gap;
    }
}

void rgb_to_gray_asimd(unsigned char* rgb_buf, size_t height, size_t width, size_t rgb_linebytes, unsigned char* gray_buf, size_t gray_linebytes)
{
    size_t rgb_use_linebytes = width * 3;
    for (size_t i=0; i<height; i++) {
        size_t vec_size = 0;
    #ifdef PIXEL_SIMD_NEON
        //TODO
    #endif
        for (size_t j=vec_size; j<rgb_use_linebytes; j++) {
            //TODO
        }
    }
}

static inline uint8_t min3(uint8_t r, uint8_t g, uint8_t b) {
    uint8_t res = (r<g)?r:g;
    res = (res<b)?res:b;
    return res;
}

static inline uint8_t max3(uint8_t r, uint8_t g, uint8_t b) {
    uint8_t res = (r>g)?r:g;
    res = (res>b)?res:b;
    return res;
}

void f_rgb_to_gray(unsigned char* rgb_buf, size_t height, size_t width, size_t rgb_linebytes, unsigned char* gray_buf, size_t gray_linebytes, int mode)
{
    switch(mode) {
        case 0: // 均值灰度化
        {
            for (size_t i=0; i<height; i++) {
                for (size_t j=0; j<width; j++) {
                    size_t idx = i*rgb_linebytes + j*3;
                    int r = rgb_buf[idx];
                    int g = rgb_buf[idx+1];
                    int b = rgb_buf[idx+2];
                    int gray = (r + g + b) / 3;
                    size_t dst_idx = i*gray_linebytes + j;
                    gray_buf[dst_idx] = gray;
                }
            }
        }
        break;
        case 1: // 经典灰度化
        {
            for (size_t i=0; i<height; i++) {
                for (size_t j=0; j<width; j++) {
                    size_t idx = i*rgb_linebytes + j*3;
                    float r = rgb_buf[idx];
                    float g = rgb_buf[idx+1];
                    float b = rgb_buf[idx+2];
                    float gray = (0.299*r + 0.587*g + 0.114*b);
                    size_t dst_idx = i*gray_linebytes + j;
                    gray_buf[dst_idx] = gray;
                }
            }
        }
        break;
        case 2: // PhotoShop灰度化
        {
            for (size_t i=0; i<height; i++) {
                for (size_t j=0; j<width; j++) {
                    size_t idx = i*rgb_linebytes + j*3;
                    uint8_t r = rgb_buf[idx];
                    uint8_t g = rgb_buf[idx+1];
                    uint8_t b = rgb_buf[idx+2];
                    int min_val = min3(r, g, b);
                    int max_val = max3(r, g, b);
                    int gray = (min_val + max_val)/2;
                    size_t dst_idx = i*gray_linebytes + j;
                    gray_buf[dst_idx] = gray;
                }
            }
        }
        break;
    }
}
