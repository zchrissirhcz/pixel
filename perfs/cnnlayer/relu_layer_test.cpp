#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common/pixel_benchmark.h"
#include "common/pixel_fast_rng.h"
#include "relu_layer.h"

static void relu_f32_forward_test();


void relu_f32_forward_test()
{
    uint32_t len = 100003 * 4;
    //uint32_t len = 4 * 4;
    //uint32_t loop_count = 10000;
    uint32_t loop_count = 1000;
    //uint32_t loop_count = 1;
    //uint32_t len = 5 * 4;
    float* src = (float*)malloc(len*sizeof(float));

    double t_start, t_cost;
    g_state.a = 7767517;
    t_start = pixel_get_current_time();
    for(uint32_t i=0; i<len; i++) {
        src[i] = pixel_fast_random_float(-12.3f, 12.3f);
    }
    t_cost = pixel_get_current_time() - t_start;
    printf("generate random numbers, time cost %.4lf ms\n", t_cost);

    float* dst_naive = (float*)malloc(len*sizeof(float));
    float* dst_asimd = (float*)malloc(len*sizeof(float));
    float* dst_asimd2 = (float*)malloc(len*sizeof(float));
    float* dst_asimd3 = (float*)malloc(len*sizeof(float));

    float* dst_asm = (float*)malloc(len*sizeof(float));
    float* dst_asm2 = (float*)malloc(len*sizeof(float));
    float* dst_asm3 = (float*)malloc(len*sizeof(float));

    // naive
    t_start = pixel_get_current_time();
    for (uint32_t i=0; i<loop_count; i++) {
        relu_f32_forward_naive(src, dst_naive, len);
    }
    t_cost = pixel_get_current_time() - t_start;
    printf("relu_f32_forward, naive, time cost %.4lf ms\n", t_cost);

    // asimd
    t_start = pixel_get_current_time();
    for (uint32_t i=0; i<loop_count; i++) {
        relu_f32_forward_asimd(src, dst_asimd, len);
    }
    t_cost = pixel_get_current_time() - t_start;
    printf("relu_f32_forward, asimd, time cost %.4lf ms\n", t_cost);

    // asimd2
    t_start = pixel_get_current_time();
    for (uint32_t i=0; i<loop_count; i++) {
        relu_f32_forward_asimd2(src, dst_asimd2, len);
    }
    t_cost = pixel_get_current_time() - t_start;
    printf("relu_f32_forward, asimd2, time cost %.4lf ms\n", t_cost);

    // asimd3
    t_start = pixel_get_current_time();
    for (uint32_t i=0; i<loop_count; i++) {
        relu_f32_forward_asimd3(src, dst_asimd3, len);
    }
    t_cost = pixel_get_current_time() - t_start;
    printf("relu_f32_forward, asimd3, time cost %.4lf ms\n", t_cost);


    // asm
    t_start = pixel_get_current_time();
    for (uint32_t i=0; i<loop_count; i++) {
        relu_f32_forward_asm(src, dst_asm, len);
    }
    t_cost = pixel_get_current_time() - t_start;
    printf("relu_f32_forward, asm, time cost %.4lf ms\n", t_cost);

    // asm2
    t_start = pixel_get_current_time();
    for (uint32_t i=0; i<loop_count; i++) {
        relu_f32_forward_asm2(src, dst_asm2, len);
    }
    t_cost = pixel_get_current_time() - t_start;
    printf("relu_f32_forward, asm2, time cost %.4lf ms\n", t_cost);

    // asm3
    t_start = pixel_get_current_time();
    for (uint32_t i=0; i<loop_count; i++) {
        relu_f32_forward_asm3(src, dst_asm3, len);
    }
    t_cost = pixel_get_current_time() - t_start;
    printf("relu_f32_forward, asm3, time cost %.4lf ms\n", t_cost);

    // validate
    int mis_asimd = 0;
    int mis_asimd2 = 0;
    int mis_asimd3 = 0;

    int mis_asm = 0;
    int mis_asm2 = 0;
    int mis_asm3 = 0;
    for (uint32_t i=0; i<len; i++) {
        if (dst_naive[i]!=dst_asimd[i]) {
            mis_asimd++;
        }
        if (dst_naive[i]!=dst_asimd2[i]) {
            mis_asimd2++;
        }
        if (dst_naive[i]!=dst_asimd3[i]) {
            mis_asimd3++;
        }
        if (dst_naive[i]!=dst_asm[i]) {
            mis_asm++;
        }
        if (dst_naive[i]!=dst_asm2[i]) {
            mis_asm2++;
        }
        if (dst_naive[i]!=dst_asm3[i]) {
            mis_asm3++;
        }
    }
    printf("mis_asimd=%d, mis_asimd2=%d, mis_asimd3=%d\n", mis_asimd, mis_asimd2, mis_asimd3);
    printf("mis_asm=%d, mis_asm2=%d, mis_asm3=%d\n", mis_asm, mis_asm2, mis_asm3);
    // for (uint32_t i=0; i<len; i++) {
    //     printf("src[%d]=%.2f\n", i, src[i]);
    // }


    free(dst_naive);
    free(dst_asimd);
    free(dst_asimd2);
    free(dst_asimd3);
    free(dst_asm);
    free(dst_asm2);
    free(dst_asm3);
}

int main() {

    relu_f32_forward_test();

    return 0;
}