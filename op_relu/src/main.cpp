#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <iostream>
#include <chrono>
#include <random>
#include <arm_neon.h>

using namespace std;

// return current time in milliseconds
static double now_ms(void)
{
    struct timespec res;
    clock_gettime(CLOCK_REALTIME, &res);
    return 1000.0*res.tv_sec + (double)res.tv_nsec/1e6;
}

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


namespace float32 {
    void op_relu_c_naive(float* buf, int len) {
        for (int i=0; i<len; i++) {
            if (buf[i]<0) {
                buf[i]=0;
            }
        }
    }

    void op_relu_c_fast(float* buf, int len) {
        for (int i=0; i<len; i++) {
            *buf = *buf<0 ? 0: *buf;
            buf++;
        }
    }

    void op_relu_neon_intrinsics(float* buf, int len) {
        float32x4_t zero = vdupq_n_f32(0);
        for (int i=0; i+3<len; i+=4) {
            float32x4_t value = vld1q_f32(buf);
            float32x4_t mask = vcltq_f32(value, zero);
            value = vbslq_f32(mask, zero, value);
            vst1q_f32(buf, value);
            buf += 4;
        }

        int remain = len % 4;
        for (int i=0; i<remain; i++) {
            if (*buf<0) {
                *buf = 0;
            }
        }
    }

    void op_relu_neon_intrinsics2(float* buf, int len) {
        float32x4_t zero = vdupq_n_f32(0);
        for (int i=0; i+7<len; i+=8) {
            float* buf_0 = buf;
            float* buf_1 = buf + 4;

            float32x4_t value_0 = vld1q_f32(buf_0);
            float32x4_t mask_0 = vcltq_f32(value_0, zero);
            value_0 = vbslq_f32(mask_0, zero, value_0);
            vst1q_f32(buf_0, value_0);

            float32x4_t value_1 = vld1q_f32(buf_1);
            float32x4_t mask_1 = vcltq_f32(value_1, zero);
            value_1 = vbslq_f32(mask_1, zero, value_1);
            vst1q_f32(buf_1, value_1);

            buf += 8;
        }

        int remain = len % 8;
        for (int i=0; i<remain; i++) {
            if (*buf<0) {
                *buf = 0;
            }
        }
    }

} // namespace float


int get_random_int(int a, int b) {
    if (a>b) {
        swap(a, b);
    }
    static random_device rd;
    static default_random_engine engine(rd());
    uniform_int_distribution<int> dist(a, b);
    return dist(engine);
}

uint8_t get_random_uint8() {
    return static_cast<uint8_t>(get_random_int(0, 255));
}

int8_t get_random_int8() {
    return static_cast<int8_t>(get_random_int(-128, 127));
}

float get_random_float(float a, float b) {
    if (a>b) {
        swap(a, b);
    }
    static random_device rd;
    static default_random_engine engine(rd());
    uniform_real_distribution<float> dist(a, b);
    return dist(engine);
}

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
        t_start = now_ms();
        for (int i=0; i<loop; i++) {
            int8::op_relu_c_naive(data_copy1, len);
        }
        t_c_naive = now_ms() - t_start;
    }

    //==================================
    // c fast impl
    //==================================
    {
        t_start = now_ms();
        for (int i=0; i<loop; i++) {
            int8::op_relu_c_fast(data_copy2, len);
        }
        t_c_fast = now_ms() - t_start;
    }


    //==================================
    // arm neon intrinsics impl
    //==================================
    {
        t_start = now_ms();
        for (int i=0; i<loop; i++) {
            int8::op_relu_neon_intrinsics(data_copy3, len);
        }
        t_neon_intrinsics = now_ms() - t_start;
    }

    //==================================
    // arm neon intrinsics impl
    //==================================
    {
        t_start = now_ms();
        for (int i=0; i<loop; i++) {
            int8::op_relu_neon_intrinsics2(data_copy4, len);
        }
        t_neon_intrinsics2 = now_ms() - t_start;
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
        fprintf(stderr, "[bad] fail count is %d\n", fail_cnt);
    } else {
        fprintf(stdout, "[good] result match\n");
    }
    printf("[c naive] cost %g ms\n", t_c_naive);
    printf("[c fast] cost %g ms\n", t_c_fast);
    printf("[neon intrinsics] cost %g ms\n", t_neon_intrinsics);
    printf("[neon intrinsics2] cost %g ms\n", t_neon_intrinsics2);


    // ===========================
    // clean ups
    // ===========================
    free(data);
    free(data_copy1);
    free(data_copy2);
    free(data_copy3);
    free(data_copy4);
}

void test_op_relu_float() {
    fprintf(stdout, "--- %s ---\n", __FUNCTION__);

    int h = 224;
    int w = 224;
    int c = 32;
    const int len = h*w*c;
    int buf_size = len * sizeof(float);
    float* data = (float*)malloc(buf_size);
    for (int i=0; i<len; i++) {
        data[i] = get_random_float(-233.f, 233.f);
    }

    float* data_copy1 = (float*)malloc(buf_size);
    float* data_copy2 = (float*)malloc(buf_size);
    float* data_copy3 = (float*)malloc(buf_size);
    float* data_copy4 = (float*)malloc(buf_size);
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
        t_start = now_ms();
        for (int i=0; i<loop; i++) {
            float32::op_relu_c_naive(data_copy1, len);
        }
        t_c_naive = now_ms() - t_start;
    }

    //==================================
    // c fast impl
    //==================================
    {
        t_start = now_ms();
        for (int i=0; i<loop; i++) {
            float32::op_relu_c_fast(data_copy2, len);
        }
        t_c_fast = now_ms() - t_start;
    }


    //==================================
    // arm neon intrinsics impl
    //==================================
    {
        t_start = now_ms();
        for (int i=0; i<loop; i++) {
            float32::op_relu_neon_intrinsics(data_copy3, len);
        }
        t_neon_intrinsics = now_ms() - t_start;
    }

    //==================================
    // arm neon intrinsics impl
    //==================================
    {
        t_start = now_ms();
        for (int i=0; i<loop; i++) {
            float32::op_relu_neon_intrinsics2(data_copy4, len);
        }
        t_neon_intrinsics2 = now_ms() - t_start;
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
        fprintf(stderr, "[bad] fail count is %d\n", fail_cnt);
    } else {
        fprintf(stdout, "[good] result match\n");
    }
    printf("[c naive] cost %g ms\n", t_c_naive);
    printf("[c fast] cost %g ms\n", t_c_fast);
    printf("[neon intrinsics] cost %g ms\n", t_neon_intrinsics);
    printf("[neon intrinsics2] cost %g ms\n", t_neon_intrinsics2);


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
    test_op_relu_int8();
    test_op_relu_float();

    return 0;
}
