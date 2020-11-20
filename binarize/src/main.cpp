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

void binarize_c_naive(unsigned char* buf, int h, int w) {
    int len = h*w*3;
    for (int i=0; i<len; i++) {
        if (buf[i]>0) {
            buf[i]=255;
        }
    }
}

void binarize_c_fast(unsigned char* buf, int h, int w) {
    int len = h*w*3;
    for (int i=0; i<len; i++) {
        *buf = *buf>0 ? 255: *buf;
        buf++;
    }
}

void binarize_neon_intrinsics(unsigned char* buf, int h, int w) {
    int len = h*w*3;

    uint8x16_t zero = vdupq_n_u8(0);
    uint8x16_t ff = vdupq_n_u8(255);
    for (int i=0; i+15<len; i+=16) {
        uint8x16_t value = vld1q_u8(buf);
        uint8x16_t mask = vcgtq_u8(value, zero);
        value = vbslq_u8(mask, ff, value);
        vst1q_u8(buf, value);
        buf += 16;
    }

    int remain = len % 16;
    for (int i=0; i<remain; i++) {
        if (*buf>0) {
            *buf = 255;
        }
    }
}

void binarize_neon_intrinsics2(unsigned char* buf, int h, int w) {
    int len = h*w*3;

    uint8x16_t zero = vdupq_n_u8(0);
    uint8x16_t ff = vdupq_n_u8(255);
    for (int i=0; i+31<len; i+=32) {
        unsigned char* buf_0 = buf;
        unsigned char* buf_1 = buf + 16;

        uint8x16_t value_0 = vld1q_u8(buf_0);
        uint8x16_t mask_0 = vcgtq_u8(value_0, zero);
        value_0 = vbslq_u8(mask_0, ff, value_0);
        vst1q_u8(buf_0, value_0);

        uint8x16_t value_1 = vld1q_u8(buf_1);
        uint8x16_t mask_1 = vcgtq_u8(value_1, zero);
        value_1 = vbslq_u8(mask_1, ff, value_1);
        vst1q_u8(buf_1, value_1);

        buf += 32;
    }

    int remain = len % 32;
    for (int i=0; i<remain; i++) {
        if (*buf>0) {
            *buf = 255;
        }
    }
}

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

int main() {

    int h = 640;
    int w = 480;
    const int len = h*w*3;
    unsigned char* data = (unsigned char*)malloc(len);
    for (int i=0; i<len; i++) {
        data[i] = get_random_uint8();
    }

    unsigned char* data_copy1 = (unsigned char*)malloc(len);
    unsigned char* data_copy2 = (unsigned char*)malloc(len);
    unsigned char* data_copy3 = (unsigned char*)malloc(len);
    unsigned char* data_copy4 = (unsigned char*)malloc(len);
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
            binarize_c_naive(data_copy1, h, w);
        }
        t_c_naive = now_ms() - t_start;
    }

    //==================================
    // c fast impl
    //==================================
    {
        t_start = now_ms();
        for (int i=0; i<loop; i++) {
            binarize_c_fast(data_copy2, h, w);
        }
        t_c_fast = now_ms() - t_start;
    }


    //==================================
    // arm neon intrinsics impl
    //==================================
    {
        t_start = now_ms();
        for (int i=0; i<loop; i++) {
            binarize_neon_intrinsics(data_copy3, h, w);
        }
        t_neon_intrinsics = now_ms() - t_start;
    }

    //==================================
    // arm neon intrinsics impl
    //==================================
    {
        t_start = now_ms();
        for (int i=0; i<loop; i++) {
            binarize_neon_intrinsics2(data_copy4, h, w);
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

    return 0;
}
