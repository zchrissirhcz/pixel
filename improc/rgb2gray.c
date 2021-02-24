#include "rgb2gray.h"
#include <stdint.h>
#include "dotproduct/pixel_simd.h"

static void pixel_rgb2gray_fast2(unsigned char* rgb_buf, size_t height, size_t width, size_t rgb_linebytes, unsigned char* gray_buf, size_t gray_linebytes);
static void HYW_rgb2gray(unsigned char* rgb_buf, size_t height, size_t width, size_t rgb_linebytes, unsigned char* gray_buf, size_t gray_linebytes, int mode);


void pixel_rgb2gray_naive(unsigned char* rgb_buf, size_t height, size_t width, size_t rgb_linebytes, unsigned char* gray_buf, size_t gray_linebytes)
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


void pixel_rgb2gray_fixed(unsigned char* rgb_buf, size_t height, size_t width, size_t rgb_linebytes, unsigned char* gray_buf, size_t gray_linebytes)
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

void pixel_rgb2gray_fixed2(unsigned char* rgb_buf, size_t height, size_t width, size_t rgb_linebytes, unsigned char* gray_buf, size_t gray_linebytes)
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

void pixel_rgb2gray_fixed_asimd(unsigned char* rgb_buf, size_t height, size_t width, size_t rgb_linebytes, unsigned char* gray_buf, size_t gray_linebytes)
{
    size_t rgb_used_linebytes = width * 3;

    const unsigned char weight_r = 77;
    const unsigned char weight_g = 151;
    const unsigned char weight_b = 28;
    const int shift_right = 8;

    size_t rgb_line_gap = rgb_linebytes - rgb_used_linebytes;
    size_t gray_line_gap = gray_linebytes - width;
#ifdef PIXEL_SIMD_NEON
    uint8x8_t v_weight_r = vdup_n_u8(weight_r);
    uint8x8_t v_weight_g = vdup_n_u8(weight_g);
    uint8x8_t v_weight_b = vdup_n_u8(weight_b);
    uint8x8x3_t v_rgb;
    uint16x8_t v_tmp;
    uint8x8_t v_gray;
    const size_t rgb_step = 24;
    const size_t gray_step = 8;
    size_t neon_gray_remain = width % gray_step;
    size_t neon_gray_vecsize = width - neon_gray_remain;
#endif

    unsigned char* rgb_linebuf = rgb_buf;
    unsigned char* gray_linebuf = gray_buf;
    for (size_t i=0; i<height; i++) {
        size_t gray_remain = width;
    #ifdef PIXEL_SIMD_NEON
        gray_remain = neon_gray_remain;
        for (size_t j=0; j<neon_gray_vecsize; j+=gray_step) {
            v_rgb = vld3_u8(rgb_linebuf);
            rgb_linebuf += rgb_step;
            v_tmp = vmull_u8(v_rgb.val[0], v_weight_r);
            v_tmp = vmlal_u8(v_tmp, v_rgb.val[1], v_weight_g);
            v_tmp = vmlal_u8(v_tmp, v_rgb.val[2], v_weight_b);

            //uint8x8_t vshrn_n_u16 (uint16x8_t __a, const int __b);
            //uint8x8_t vqshrn_n_u16 (uint16x8_t __a, const int __b);
            //v_gray = vshrq_n_u16(v_gray, shift_right);
            v_gray = vshrn_n_u16(v_tmp, 8);
            vst1_u8(gray_linebuf, v_gray);
            gray_linebuf += gray_step;
        }
    #endif
        for (size_t j=0; j<gray_remain; j++) {
            *gray_linebuf = (weight_r*rgb_linebuf[0] + weight_g*rgb_linebuf[1] + weight_b*rgb_linebuf[2]) >> 8;
            gray_linebuf ++;
            rgb_linebuf += 3;
        }
        rgb_linebuf += rgb_line_gap;
        gray_linebuf += gray_line_gap;
    }
}

//https://computer-vision-talks.com/2011-02-08-a-very-fast-bgra-to-grayscale-conversion-on-iphone/
void pixel_rgb2gray_fixed_asm(unsigned char* rgb_buf, size_t height, size_t width, size_t rgb_linebytes, unsigned char* gray_buf, size_t gray_linebytes)
{
    const unsigned char weight_r = 77;
    const unsigned char weight_g = 151;
    const unsigned char weight_b = 28;
#ifdef __ARM_NEON
    #ifdef __aarch64__
    #else
    const size_t step = 24;
    size_t rgb_used_linebytes = width * 3;
    size_t remain = rgb_used_linebytes % step;
    size_t vec_size = rgb_used_linebytes - remain;
    size_t pre_neon_len = vec_size / step;
    size_t rgb_line_gap = rgb_linebytes - rgb_used_linebytes;
    size_t gray_line_gap = gray_linebytes - width;
    unsigned char* rgb_linebuf = rgb_buf;
    unsigned char* gray_linebuf = gray_buf;

    size_t gray_step = step / 3; // 8
    size_t gray_remain = width % gray_step;
    for (size_t i=0; i<height; i++) {
        size_t neon_len = pre_neon_len;
        __asm__ volatile(
            "vdup.u8    d16, %6 \n"
            "vdup.u8    d17, %7  \n"
            "vdup.u8    d18, %8 \n"
            "0: \n"
            "vld3.u8    {d0-d2}, [%1]! \n"
            "vmull.u8   q2, d0, d16 \n"
            "vmlal.u8   q2, d1, d17 \n"
            "vmlal.u8   q2, d2, d18 \n"
            "vshrn.u16  d3, q2, #8 \n"
            "vst1.u8    {d3}, [%0]! \n"
            "subs       %2, #1 \n"
            "bne        0b \n"
            : "=r"(gray_linebuf), //%0
            "=r"(rgb_linebuf), //%1
            "=r"(neon_len) //%2
            : "0"(gray_linebuf),
            "1"(rgb_linebuf),
            "2"(neon_len),
            "r"(weight_r), //%6 
            "r"(weight_g), //%7
            "r"(weight_b) //%8
            : "cc", "memory", "q0", "q1", "q2", "q8", "q9"
        );
        for (size_t j=0; j<gray_remain; j++) {
            *gray_linebuf = (weight_r*rgb_linebuf[0] + weight_g*rgb_linebuf[1] + weight_b*rgb_linebuf[2]) >> 8;
            gray_linebuf++;
            rgb_linebuf+=3;
        }
        rgb_linebuf += rgb_line_gap;
        gray_linebuf += gray_line_gap;
    }
    #endif
#endif
}


//https://computer-vision-talks.com/2011-02-08-a-very-fast-bgra-to-grayscale-conversion-on-iphone/
void pixel_rgb2gray_fixed_asm0(unsigned char* rgb_buf, size_t height, size_t width, size_t rgb_linebytes, unsigned char* gray_buf, size_t gray_linebytes)
{
#ifdef __ARM_NEON
    #ifdef __aarch64__
    __asm__ volatile(
        "lsr          %2, %2, #3      \n"
        "# build the three constants: \n"
        "mov         r4, #77          \n" // Blue channel multiplier
        "mov         r5, #151         \n" // Green channel multiplier
        "mov         r6, #28          \n" // Red channel multiplier
        "vdup.u8      d4, r4           \n"
        "vdup.u8      d5, r5           \n"
        "vdup.u8      d6, r6           \n"
        "0:                       \n"
        "# load 8 pixels:             \n"
        "vld3.u8      {d0-d2}, [%1]!   \n"
        "# do the weight average:     \n"
        "vmull.u8    q7, d0, d4       \n"
        "vmlal.u8    q7, d1, d5       \n"
        "vmlal.u8    q7, d2, d6       \n"
        "# shift and store:           \n"
        "vshrn.u16   d7, q7, #8       \n" // Divide q3 by 256 and store in the d7
        "vst1.u8      {d7}, [%0]!      \n"
        "subs        %2, #1       \n" // Decrement iteration count
        "bne         0b            \n" // Repeat unil iteration count is not zero
        : "=r"(gray_buf), //%0
        "=r"(rgb_buf), //%1
        "=r"(num_pixels) //%2
        : "0"(gray_buf), 
        "1"(rgb_buf),
        "2"(num_pixels)
        : "cc", "memory", "r4", "r5", "r6", "q0", "q1", "q2", "q3", "q7"
    );
    #else
    size_t num_pixels = height * width;
    asm volatile(
        "lsr          %2, %2, #3      \n"
        "# build the three constants: \n"
        "mov         r4, #77          \n" // Blue channel multiplier
        "mov         r5, #151         \n" // Green channel multiplier
        "mov         r6, #28          \n" // Red channel multiplier
        "vdup.u8      d4, r4           \n"
        "vdup.u8      d5, r5           \n"
        "vdup.u8      d6, r6           \n"
        "0:                       \n"
        "# load 8 pixels:             \n"
        "vld3.u8      {d0-d2}, [%1]!   \n"
        "# do the weight average:     \n"
        "vmull.u8    q7, d0, d4       \n"
        "vmlal.u8    q7, d1, d5       \n"
        "vmlal.u8    q7, d2, d6       \n"
        "# shift and store:           \n"
        "vshrn.u16   d7, q7, #8       \n" // Divide q3 by 256 and store in the d7
        "vst1.u8      {d7}, [%0]!      \n"
        "subs        %2, #1       \n" // Decrement iteration count
        "bne         0b            \n" // Repeat unil iteration count is not zero
        : "=r"(gray_buf), //%0
        "=r"(rgb_buf), //%1
        "=r"(num_pixels) //%2
        : "0"(gray_buf), 
        "1"(rgb_buf),
        "2"(num_pixels)
        : "cc", "memory", "r4", "r5", "r6", "q0", "q1", "q2", "q3", "q7"
    );
    #endif
#endif
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

// 《图像视频滤镜与人像美颜美妆算法解析》 by Hu Yaowu et al.  `HYW` is short for its author.
void HYW_rgb2gray(unsigned char* rgb_buf, size_t height, size_t width, size_t rgb_linebytes, unsigned char* gray_buf, size_t gray_linebytes, int mode)
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
