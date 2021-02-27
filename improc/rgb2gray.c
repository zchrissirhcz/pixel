#include "rgb2gray.h"
#include <stdint.h>
#include "dotproduct/pixel_simd.h"

static void pixel_rgb2gray_fast2(unsigned char* rgb_buf, size_t height, size_t width, size_t rgb_linebytes, unsigned char* gray_buf, size_t gray_linebytes);
static void HYW_rgb2gray(unsigned char* rgb_buf, size_t height, size_t width, size_t rgb_linebytes, unsigned char* gray_buf, size_t gray_linebytes, int mode);


void pixel_rgb2gray_naive(unsigned char* rgb_buf, size_t height, size_t width, unsigned char* gray_buf)
{
    size_t gray_len = height * width;
    size_t rgb_len = gray_len * 3;

    size_t i=0; // rgb_idx
    size_t j=0; // gray_idx
    for( ; j<gray_len; j++, i+=3) {
        float r = rgb_buf[i];
        float g = rgb_buf[i+1];
        float b = rgb_buf[i+2];
        float gray = (0.299*r + 0.587*g + 0.114*b);
        gray_buf[j] = gray;
    }
}


void pixel_rgb2gray_fixed(unsigned char* rgb_buf, size_t height, size_t width, unsigned char* gray_buf)
{
    size_t gray_len = height * width;
    size_t rgb_len = gray_len * 3;
    for (size_t gray_idx=0, rgb_idx=0; gray_idx<gray_len; gray_idx++, rgb_idx+=3) {
#if 0 // 直观的定点化实现
        int r = rgb_buf[rgb_idx];
        int g = rgb_buf[rgb_idx+1];
        int b = rgb_buf[rgb_idx+2];
        int gray = (299*r + 587*g + 114*b) / 1000;
#elif 0 // 缩放倍数从1000改为1024
        int r = rgb_buf[rgb_idx];
        int g = rgb_buf[rgb_idx+1];
        int b = rgb_buf[rgb_idx+2];
        int gray = (306*r + 601*g + 117*b) / 1024;
#elif 0 // 移位操作替代除法，加速
        int r = rgb_buf[rgb_idx];
        int g = rgb_buf[rgb_idx+1];
        int b = rgb_buf[rgb_idx+2];
        int gray = (306*r + 601*g + 117*b) / 1024;
#elif 1 // 缩放256(2^8)而不是1024倍，是为了控制在short范围内, 对应到后续neon vmull/vmlal 长指令
        int r = rgb_buf[rgb_idx];
        int g = rgb_buf[rgb_idx+1];
        int b = rgb_buf[rgb_idx+2];
        int gray = (77*r + 151*g + 28*b) >> 8;
#endif
        gray_buf[gray_idx] = gray;
    }
}

void pixel_rgb2gray_fixed2(unsigned char* rgb_buf, size_t height, size_t width, unsigned char* gray_buf)
{
    const static int wr = (int)(0.299 * 256 + 0.5);
    const static int wg = (int)(0.587 * 256 + 0.5);
    const static int wb = (int)(0.114 * 256 + 0.5);
    size_t len = height * width;
    for (size_t i=0; i<len; i++) {
        *gray_buf = (wr*rgb_buf[0] + wg*rgb_buf[1] + wb*rgb_buf[2]) >> 8;
        rgb_buf += 3;
        gray_buf++;
    }
}

void pixel_rgb2gray_fixed_asimd(unsigned char* rgb_buf, size_t height, size_t width, unsigned char* gray_buf)
{
    size_t done = 0;
    size_t num_pixels = width * height;

    const unsigned char weight_r = 77;
    const unsigned char weight_g = 151;
    const unsigned char weight_b = 28;
    const int shift_right = 8;

#ifdef PIXEL_SIMD_NEON
    size_t gray_step = 8;
    size_t rgb_step = gray_step * 3;
    size_t gray_vec_size = num_pixels - num_pixels % gray_step;

    uint8x8x3_t v_rgb;
    uint16x8_t v_tmp;
    uint8x8_t v_gray;
    uint8x8_t v_weight_r = vdup_n_u8(weight_r);
    uint8x8_t v_weight_g = vdup_n_u8(weight_g);
    uint8x8_t v_weight_b = vdup_n_u8(weight_b);

    for (size_t i=0; i<gray_vec_size; i+=gray_step) {
        v_rgb = vld3_u8(rgb_buf);
        rgb_buf += rgb_step;
        v_tmp = vmull_u8(v_rgb.val[0], v_weight_r);
        v_tmp = vmlal_u8(v_tmp, v_rgb.val[1], v_weight_g);
        v_tmp = vmlal_u8(v_tmp, v_rgb.val[2], v_weight_b);

        //uint8x8_t vshrn_n_u16 (uint16x8_t __a, const int __b);
        //uint8x8_t vqshrn_n_u16 (uint16x8_t __a, const int __b);
        //v_gray = vshrq_n_u16(v_gray, shift_right);
        v_gray = vshrn_n_u16(v_tmp, 8);
        vst1_u8(gray_buf, v_gray);
        gray_buf += gray_step;
    }
    done = gray_vec_size;
#endif

    for (; done<num_pixels; done++) {
        *gray_buf = (weight_r*rgb_buf[0] + weight_g*rgb_buf[1] + weight_b*rgb_buf[2]) >> 8;
        rgb_buf += 3;
        gray_buf++;
    }
}

//https://computer-vision-talks.com/2011-02-08-a-very-fast-bgra-to-grayscale-conversion-on-iphone/
void pixel_rgb2gray_fixed_asm(unsigned char* rgb_buf, size_t height, size_t width, unsigned char* gray_buf)
{
    size_t done = 0;
    size_t num_pixels = height * width;
#ifdef PIXEL_SIMD_NEON
    #ifdef __aarch64__
    asm volatile(
        "lsr %2, %2, #3 \n"
        "# build three constant weights: \n"
        "mov w4, #77 \n"
        "mov w5, #151 \n"
        "mov w6, #28 \n"
        "dup v4.8b, w4 \n"
        "dup v5.8b, w5 \n"
        "dup v6.8b, w6 \n"
        "0: \n"
        "# load 8 pixels: \n"
        "ld3 {v0.8b, v1.8b, v2.8b}, [%1], #24 \n"
        "# do the weight average: \n"
        "umull v7.8h, v0.8b, v4.8b \n"
        "umlal v7.8h, v1.8b, v5.8b \n"
        "umlal v7.8h, v2.8b, v6.8b \n"
        "shrn v7.8b, v7.8h, #8 \n"
        "st1 {v7.8b}, [%0], #8 \n"
        "subs %w2, %w2, #1 \n"
        "bne 0b \n"
        : "=r"(gray_buf), //%0
        "=r"(rgb_buf), //%1
        "=r"(num_pixels) //%2
        : "0"(gray_buf), 
        "1"(rgb_buf),
        "2"(num_pixels)
        : "cc", "memory", "w4", "w5", "w6", "v0", "v1", "v2", "v4", "v5", "v6", "v7"
    );
    #else
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
    const unsigned char weight_r = 77;
    const unsigned char weight_g = 151;
    const unsigned char weight_b = 28;
    for (; done<num_pixels; done++) {
        *gray_buf = (weight_r*rgb_buf[0] + weight_g*rgb_buf[1] + weight_b*rgb_buf[2]) >> 8;
        rgb_buf += 3;
        gray_buf++;
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
