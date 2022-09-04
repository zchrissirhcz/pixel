#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <random>
#include <arm_neon.h>

#include "common/pixel_log.h"
#include "common/pixel_benchmark.h"

namespace int8 {
    void op_relu_c_naive(int8_t* buf, int len) {
        for (int i=0; i<len; i++) {
            if (buf[i]<0) {
                buf[i]=0;
            }
        }
    }

    void op_relu_c_fast(int8_t* buf, int len) {
        for (int i=0; i<len; i++) {
            *buf = *buf<0 ? 0: *buf;
            buf++;
        }
    }

    void op_relu_neon_intrinsics(int8_t* buf, int len) {
        int8x16_t zero = vdupq_n_s8(0);
        for (int i=0; i+15<len; i+=16) {
            int8x16_t value = vld1q_s8(buf);
            int8x16_t mask = vcltq_s8(value, zero);
            value = vbslq_s8(mask, zero, value);
            vst1q_s8(buf, value);
            buf += 16;
        }

        int remain = len % 16;
        for (int i=0; i<remain; i++) {
            if (*buf<0) {
                *buf = 0;
            }
        }
    }

    void op_relu_neon_intrinsics2(int8_t* buf, int len) {
        int8x16_t zero = vdupq_n_s8(0);
        for (int i=0; i+31<len; i+=32) {
            int8_t* buf_0 = buf;
            int8_t* buf_1 = buf + 16;

            int8x16_t value_0 = vld1q_s8(buf_0);
            int8x16_t mask_0 = vcltq_s8(value_0, zero);
            value_0 = vbslq_s8(mask_0, zero, value_0);
            vst1q_s8(buf_0, value_0);

            int8x16_t value_1 = vld1q_s8(buf_1);
            int8x16_t mask_1 = vcltq_s8(value_1, zero);
            value_1 = vbslq_s8(mask_1, zero, value_1);
            vst1q_s8(buf_1, value_1);

            buf += 32;
        }

        int remain = len % 32;
        for (int i=0; i<remain; i++) {
            if (*buf<0) {
                *buf = 0;
            }
        }
    }

} // namespace int8

void test_op_relu_int8() {
    fprintf(stdout, "--- %s ---\n", __FUNCTION__);

    int h = 224;
    int w = 224;
    int c = 32;
    const int len = h*w*c;
    int8_t* data = (int8_t*)malloc(len);
    for (int i=0; i<len; i++) {
        data[i] = get_random_int8();
    }

    int8_t* data_copy1 = (int8_t*)malloc(len);
    int8_t* data_copy2 = (int8_t*)malloc(len);
    int8_t* data_copy3 = (int8_t*)malloc(len);
    int8_t* data_copy4 = (int8_t*)malloc(len);
    memcpy(data_copy1, data, len);
    memcpy(data_copy2, data, len);
    memcpy(data_copy3, data, len);
    memcpy(data_copy4, data, len);

    const int loop = 100;

    double t_start, t_c_naive, t_c_fast, t_neon_intrinsics, t_neon_intrinsics2;
    //==================================
    // c naive impl
    //==================================
    {
        t_start = pixel_get_current_time();
        for (int i=0; i<loop; i++) {
            int8::op_relu_c_naive(data_copy1, len);
        }
        t_c_naive = pixel_get_current_time() - t_start;
    }

    //==================================
    // c fast impl
    //==================================
    {
        t_start = pixel_get_current_time();
        for (int i=0; i<loop; i++) {
            int8::op_relu_c_fast(data_copy2, len);
        }
        t_c_fast = pixel_get_current_time() - t_start;
    }


    //==================================
    // arm neon intrinsics impl
    //==================================
    {
        t_start = pixel_get_current_time();
        for (int i=0; i<loop; i++) {
            int8::op_relu_neon_intrinsics(data_copy3, len);
        }
        t_neon_intrinsics = pixel_get_current_time() - t_start;
    }

    //==================================
    // arm neon intrinsics impl
    //==================================
    {
        t_start = pixel_get_current_time();
        for (int i=0; i<loop; i++) {
            int8::op_relu_neon_intrinsics2(data_copy4, len);
        }
        t_neon_intrinsics2 = pixel_get_current_time() - t_start;
    }

    //=================================
    // validate correctness
    //=================================
    int fail_cnt = 0;
    for (int i=0; i<len; i++) {
        if (data_copy1[i]!=data_copy2[i]
                || data_copy1[i]!=data_copy3[i]
                || data_copy1[i]!=data_copy4[i]
           ) {
            fail_cnt++;
        }
    }
    if (fail_cnt>0) {
        PIXEL_LOGE("[bad] fail count is %d", fail_cnt);
    } else {
        PIXEL_LOGD("[good] result match");
    }
    PIXEL_LOGD("[c naive] cost %g ms", t_c_naive);
    PIXEL_LOGD("[c fast] cost %g ms", t_c_fast);
    PIXEL_LOGD("[neon intrinsics] cost %g ms", t_neon_intrinsics);
    PIXEL_LOGD("[neon intrinsics2] cost %g ms", t_neon_intrinsics2);


    // ===========================
    // clean ups
    // ===========================
    free(data);
    free(data_copy1);
    free(data_copy2);
    free(data_copy3);
    free(data_copy4);
}


int main() {
    //test_op_relu_int8();
    test_relu();
    //test_relu_inplace();

    return 0;
}
