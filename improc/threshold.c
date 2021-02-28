#include "threshold.h"
#include "simd/pixel_simd.h"

void threshold_gray_naive(unsigned char* input_gray, size_t height, size_t width, unsigned char* output_gray, unsigned char thresh, unsigned char minval, unsigned char maxval)
{
    size_t total_len = height * width;
    for (size_t done=0; done<total_len; done++) {
        *output_gray = (input_gray[0]>thresh) ? maxval : minval;
        input_gray++;
        output_gray++;
    }
}

void threshold_gray_asimd(unsigned char* input_gray, size_t height, size_t width, unsigned char* output_gray, unsigned char thresh, unsigned char minval, unsigned char maxval)
{
    size_t total_len = height * width;
    size_t done = 0;
#ifdef PIXEL_SIMD_NEON
    size_t step = 16;
    size_t vec_size = total_len - total_len % step;
    uint8x16_t v1;
    uint8x16_t vmask_gt;
    uint8x16_t vthresh = vdupq_n_u8(thresh);
    uint8x16_t vmaxval = vdupq_n_u8(maxval);
    uint8x16_t vminval = vdupq_n_u8(minval);
    for (size_t i=0; i<vec_size; i+=step) {
        v1 = vld1q_u8(input_gray);
        input_gray += step;
        vmask_gt = vcgtq_u8(v1, vthresh);
        v1 = vbslq_u8(vmask_gt, vmaxval, vminval);
        vst1q_u8(output_gray, v1);
        output_gray += step;
    }
    done = vec_size;
#endif
    for (; done<total_len; done++) {
        *output_gray = (input_gray[0]>thresh) ? maxval : minval;
        input_gray++;
        output_gray++;
    }
}

void threshold_gray_asm(unsigned char* input_gray, size_t height, size_t width, unsigned char* output_gray, unsigned char thresh, unsigned char minval, unsigned char maxval)
{
    size_t total_len = height * width;
    size_t done = 0;
#ifdef PIXEL_SIMD_NEON
    size_t step = 16;
    size_t vec_size = total_len - total_len % step;
    size_t neon_len = vec_size / step;
    #if __aarch64__
    __asm__ volatile(
        "dup v0.16b, %w6 \n"
        "dup v1.16b, %w7 \n"
        "dup v2.16b, %w8 \n"
        "0: \n"
        "ld1 {v3.16b}, [%0], #16 \n"
        "cmhi v4.16b, v3.16b, v0.16b \n"
        "bsl v4.16b, v1.16b, v2.16b \n"
        "st1 {v4.16b}, [%1], #16 \n"
        "subs %w2, %w2, #1 \n"
        "bne 0b \n"
        : "=r"(input_gray), //%0
        "=r"(output_gray), //%1
        "=r"(neon_len) //%2
        : "0"(input_gray),
        "1"(output_gray),
        "2"(neon_len),
        "r"(thresh), //%6
        "r"(maxval), //%7
        "r"(minval) //%8
        : "cc", "memory", "v0", "v1", "v2", "v3", "v4"
    );
    #else
    __asm__ volatile(
        "vdup.u8 q0, %6 \n" //vthresh
        "vdup.u8 q1, %7 \n" //vmaxval
        "vdup.u8 q2, %8 \n" //vminval
        "0: \n"
        "vld1.u8 {q3}, [%0]! \n"
        "vcgt.u8 q4, q3, q0 \n" // vmask_gt
        "vbsl.u8 q4, q1, q2 \n"
        "vst1.u8 {q4}, [%1]! \n"
        "subs %2, #1 \n"
        "bne 0b \n"
        : "=r"(input_gray), //%0
        "=r"(output_gray), //%1
        "=r"(neon_len) //%2
        : "0"(input_gray),
        "1"(output_gray),
        "2"(neon_len),
        "r"(thresh), //%6
        "r"(maxval), //%7
        "r"(minval) //%8
        : "cc", "memory", "q0", "q1", "q2", "q3", "q4"
    );
    #endif // __aarch64__
    done = vec_size;
#endif // PIXEL_SIMD_NEON

    for (; done<total_len; done++) {
        *output_gray = (input_gray[0]>thresh) ? maxval : minval;
        input_gray++;
        output_gray++;
    }
}

// --------------------------------

void threshold_rgb(unsigned char* rgb_buf, size_t height, size_t width, unsigned char* gray_buf, unsigned char thresh, unsigned char minval, unsigned char maxval)
{
    //TODO
}

void threshold_gray_inplace(unsigned char* gray_buf, size_t height, size_t width, unsigned char thresh, unsigned char minval, unsigned char maxval)
{
    //TODO
}
