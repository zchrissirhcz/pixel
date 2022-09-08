#include "threshold.h"

#if __ARM_NEON
#include <arm_neon.h>
#endif // __ARM_NEON

void threshold_gray_naive(unsigned char* input_gray, size_t height, size_t width, unsigned char* output_gray, unsigned char thresh, unsigned char minval, unsigned char maxval)
{
    size_t total_len = height * width;
    for (size_t done=0; done<total_len; done++)
    {
        *output_gray = (input_gray[0]>thresh) ? maxval : minval;
        input_gray++;
        output_gray++;
    }
}

void threshold_gray_asimd(unsigned char* input_gray, size_t height, size_t width, unsigned char* output_gray, unsigned char thresh, unsigned char minval, unsigned char maxval)
{
    size_t total_len = height * width;
    size_t done = 0;
#if __ARM_NEON
    size_t step = 16;
    size_t vec_size = total_len - total_len % step;
    uint8x16_t v1;
    uint8x16_t vmask_gt;
    uint8x16_t vthresh = vdupq_n_u8(thresh);
    uint8x16_t vmaxval = vdupq_n_u8(maxval);
    uint8x16_t vminval = vdupq_n_u8(minval);
    for (size_t i=0; i<vec_size; i+=step)
    {
        v1 = vld1q_u8(input_gray);
        input_gray += step;
        vmask_gt = vcgtq_u8(v1, vthresh);
        v1 = vbslq_u8(vmask_gt, vmaxval, vminval);
        vst1q_u8(output_gray, v1);
        output_gray += step;
    }
    done = vec_size;
#endif // __ARM_NEON
    for (; done<total_len; done++)
    {
        *output_gray = (input_gray[0]>thresh) ? maxval : minval;
        input_gray++;
        output_gray++;
    }
}

void threshold_gray_asm(unsigned char* input_gray, size_t height, size_t width, unsigned char* output_gray, unsigned char thresh, unsigned char minval, unsigned char maxval)
{
    size_t total_len = height * width;
    size_t done = 0;
#if __ARM_NEON
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
#endif // __ARM_NEON

    for (; done<total_len; done++) {
        *output_gray = (input_gray[0]>thresh) ? maxval : minval;
        input_gray++;
        output_gray++;
    }
}

// --------------------------------

void threshold_rgb_naive(unsigned char* rgb_buf, size_t height, size_t width, unsigned char* gray_buf, unsigned char thresh, unsigned char minval, unsigned char maxval)
{
    size_t total_len = height * width;
    unsigned char gray;
    for (size_t i=0; i<total_len; i++)
    {
#if 1
        gray = (0.299*rgb_buf[0] + 0.587*rgb_buf[1] + 0.114*rgb_buf[2]);
#elif 0
        gray = (rgb_buf[0] + rgb_buf[1] + rgb_buf[2]) / 3;
#else
        gray = (77*rgb_buf[0] + 151*rgb_buf[1] + 28*rgb_buf[2]) >> 8;
#endif
        rgb_buf += 3;
        *gray_buf = (gray>thresh) ? maxval : minval;
        gray_buf ++;
    }
}

void threshold_rgb_asimd(unsigned char* rgb_buf, size_t height, size_t width, unsigned char* gray_buf, unsigned char thresh, unsigned char minval, unsigned char maxval)
{
    size_t total_len = height * width;
    size_t done = 0;
    const unsigned char weight_r_u8 = 77;
    const unsigned char weight_g_u8 = 151;
    const unsigned char weight_b_u8 = 28;
    const int weight_r = weight_r_u8;
    const int weight_g = weight_g_u8;
    const int weight_b = weight_b_u8;
#ifdef PIXEL_SIMD_NEON
    size_t step = 8;
    size_t rgb_step = step * 3;
    size_t vec_size = total_len - total_len % step;
    uint8x8x3_t v1;
    uint8x8_t vmask_gt;
    uint8x8_t vthresh = vdup_n_u8(thresh);
    uint8x8_t vmaxval = vdup_n_u8(maxval);
    uint8x8_t vminval = vdup_n_u8(minval);
    uint8x8_t v_weight_r = vdup_n_u8(weight_r_u8);
    uint8x8_t v_weight_g = vdup_n_u8(weight_g_u8);
    uint8x8_t v_weight_b = vdup_n_u8(weight_b_u8);
    uint16x8_t v_tmp;
    uint8x8_t v_gray;
    for (size_t i=0; i<vec_size; i+=step) {
        v1 = vld3_u8(rgb_buf);
        rgb_buf += rgb_step;
        
        v_tmp = vmull_u8(v1.val[0], v_weight_r);
        v_tmp = vmlal_u8(v_tmp, v1.val[1], v_weight_g);
        v_tmp = vmlal_u8(v_tmp, v1.val[2], v_weight_b);
        v_gray = vshrn_n_u16(v_tmp, 8);

        vmask_gt = vcgt_u8(v_gray, vthresh);
        v_gray = vbsl_u8(vmask_gt, vmaxval, vminval);
        vst1_u8(gray_buf, v_gray);
        gray_buf += step;
    }
    done = vec_size;
#endif // PIXEL_SIMD_NEON

    unsigned char gray;
    for (; done<total_len; done++) {
        gray = (weight_r*rgb_buf[0] + weight_g*rgb_buf[1] + weight_b*rgb_buf[2]) >> 8;
        rgb_buf += 3;
        *gray_buf = (gray>thresh) ? maxval : minval;
        gray_buf++;
    }
}

void threshold_rgb_asm(unsigned char* rgb_buf, size_t height, size_t width, unsigned char* gray_buf, unsigned char thresh, unsigned char minval, unsigned char maxval){
    size_t total_len = height * width;
    size_t done = 0;
    const unsigned char weight_r_u8 = 77;
    const unsigned char weight_g_u8 = 151;
    const unsigned char weight_b_u8 = 28;
    const int weight_r = weight_r_u8;
    const int weight_g = weight_g_u8;
    const int weight_b = weight_b_u8;
#ifdef PIXEL_SIMD_NEON
    size_t step = 8;
    size_t vec_size = total_len - total_len % step;
    size_t neon_len = vec_size / step;
    #ifdef __aarch64__

    #else
    __asm__ volatile(
        "vdup.u8 d0, %6 \n" //vthresh
        "vdup.u8 d1, %7 \n" //vmaxval
        "vdup.u8 d2, %8 \n" //vminval
        "mov r3, #77 \n" //weight_r
        "mov r4, #151 \n" //weight_g
        "mov r5, #28 \n" //weight_b
        "vdup.u8 d3, r3 \n"
        "vdup.u8 d4, r4 \n"
        "vdup.u8 d5, r5 \n"
        "0: \n"
        "vld3.u8 {d6-d8}, [%0]! \n"
        "# do the weight average: \n"
        "vmull.u8 q6, d6, d3 \n"
        "vmlal.u8 q6, d7, d4 \n"
        "vmlal.u8 q6, d8, d5 \n"
        "# shift and store: \n"
        "vshrn.u16 d9, q6, #8 \n" //gray value
        "vcgt.u8 d10, d9, d0 \n"
        "vbsl.u8 d10, d1, d2 \n"
        "vst1.u8 {d10}, [%1]! \n"
        "subs %2, #1 \n"
        "bne  0b \n"
        : "=r"(rgb_buf), //%0
        "=r"(gray_buf), //%1
        "=r"(neon_len) //%2
        : "0"(rgb_buf),
        "1"(gray_buf),
        "2"(neon_len),
        "r"(thresh), //%6
        "r"(maxval), //%7
        "r"(minval) //%8
        : "cc", "memory", "r3", "r4", "r5", "q0", "q1", "q2", "q3", "q4", "q5", "q6"
    );
    #endif
    done = vec_size;
#endif // PIXEL_SIMD_NEON
    unsigned char gray;
    for (; done<total_len; done++) {
        gray = (weight_r*rgb_buf[0] + weight_g*rgb_buf[1] + weight_b*rgb_buf[2]) >> 8;
        rgb_buf += 3;
        *gray_buf = (gray>thresh) ? maxval : minval;
        gray_buf++;
    }
}

void threshold_gray_inplace(unsigned char* gray_buf, size_t height, size_t width, unsigned char thresh, unsigned char minval, unsigned char maxval)
{
    //TODO
}
