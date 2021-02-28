#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <opencv2/opencv.hpp>
#include "threshold.h"

#include "common/pixel_benchmark.h"

#include <arm_neon.h>

int main() {

    cv::Mat image = cv::imread("sky.png");
    cv::Mat gray;
    cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);

    cv::Size size = image.size();
    size_t height = (size_t)size.height;
    size_t width = (size_t)size.width;

    unsigned char* input_gray = gray.data;
    unsigned char* output_gray = NULL;

    cv::Mat result_naive(size, CV_8UC1);
    cv::Mat result_opencv(size, CV_8UC1);
    cv::Mat result_asimd(size, CV_8UC1);
    cv::Mat result_asm(size, CV_8UC1);

    double t_start, t_cost;
    unsigned char thresh = 60;

    output_gray = result_naive.data;
    t_start = pixel_get_current_time();
    threshold_gray_naive(gray.data, height, width, output_gray, thresh, 0, 255);
    t_cost = pixel_get_current_time() - t_start;
    printf("threshold, naive, time cost is %.4lf ms\n", t_cost);

    t_start = pixel_get_current_time();
    cv::threshold(gray, result_opencv, thresh, 255, 0);
    t_cost = pixel_get_current_time() - t_start;
    printf("threshold, opencv, time cost is %.4lf ms\n", t_cost);

    output_gray = result_asimd.data;
    t_start = pixel_get_current_time();
    threshold_gray_asimd(gray.data, height, width, output_gray, thresh, 0, 255);
    t_cost = pixel_get_current_time() - t_start;
    printf("threshold, asimd, time cost is %.4lf ms\n", t_cost);

    output_gray = result_asm.data;
    t_start = pixel_get_current_time();
    threshold_gray_asm(gray.data, height, width, output_gray, thresh, 0, 255);
    t_cost = pixel_get_current_time() - t_start;
    printf("threshold, asm, time cost is %.4lf ms\n", t_cost);

    cv::imwrite("sky_thresh60_naive.png", result_naive);
    cv::imwrite("sky_thresh60_opencv.png", result_opencv);
    cv::imwrite("sky_thresh60_asimd.png", result_asimd);
    cv::imwrite("sky_thresh60_asm.png", result_asm);

    return 0;
}

static void test() {
    //------------------------------
    // prepare input
    //------------------------------
    const size_t MAXLEN = 16;
    unsigned char* input_gray = (unsigned char*)malloc(MAXLEN);
    for (size_t i=0; i<MAXLEN; i++) {
        input_gray[i] = i;
    }
    unsigned char thresh = 8;
    unsigned char maxval = 66;
    unsigned char minval = 22;

    //------------------------------
    // naive implementation
    //------------------------------
    unsigned char* expected_output_gray = (unsigned char*)malloc(MAXLEN);
    for (size_t i=0; i<MAXLEN; i++) {
        if (input_gray[i]>thresh) {
            expected_output_gray[i] = maxval;
        } else {
            expected_output_gray[i] = minval;
        }
    }

    //------------------------------
    // arm v7 asm implementation
    //------------------------------
    unsigned char* asm_output_gray = (unsigned char*)malloc(MAXLEN);
    memset(asm_output_gray, 0, MAXLEN);

#if 0
    uint8x16_t vthresh = vdupq_n_u8(thresh);
    uint8x16_t vmaxval = vdupq_n_u8(maxval);
    uint8x16_t vminval = vdupq_n_u8(minval);

    uint8x16_t v1 = vld1q_u8(input_gray);
    uint8x16_t vmask_gt = vcgtq_u8(v1, vthresh);

    unsigned char* mask = (unsigned char*)malloc(MAXLEN);
    memset(mask, 0, MAXLEN);
    vst1q_u8(mask, vmask_gt);
    printf("mask:\n");
    for (size_t i=0; i<MAXLEN; i++) {
        printf("%d ", (int)mask[i]);
    }
    printf("\n");

    uint8x16_t v2 = vbslq_u8(vmask_gt, vmaxval, vminval);
    vst1q_u8(asm_output_gray, v2);
#else
    __asm__ volatile(
        "vdup.u8 q0, %4 \n" //vthresh
        "vdup.u8 q1, %5 \n" //vmaxval
        "vdup.u8 q2, %6 \n" //vminval
        "vld1.u8 {q3}, [%0] \n"
        "vcgt.u8 q4, q3, q0 \n" // vmask_gt
        "vbsl.u8 q4, q1, q2 \n"
        "vst1.u8 {q4}, [%1] \n"
        : "=r"(input_gray), //%0
        "=r"(asm_output_gray) //%1
        : "0"(input_gray),
        "1"(asm_output_gray),
        "r"(thresh), //%4
        "r"(maxval), //%5
        "r"(minval) //%6
        : "cc", "memory", "q0", "q1", "q2", "q3", "q4", "q5"
    );
#endif

    printf("input:\n");
    for (size_t i=0; i<MAXLEN; i++) {
        printf("%3d ", (int)input_gray[i]);
    }
    printf("\n");

    printf("extected output:\n");
    for (size_t i=0; i<MAXLEN; i++) {
        printf("%3d ", (int)expected_output_gray[i]);
    }
    printf("\n");

    printf("armv7 asm output:\n");
    for (size_t i=0; i<MAXLEN; i++) {
        printf("%3d ", (int)asm_output_gray[i]);
    }
    printf("\n");
}

int main2() {

    test();

    return 0;
}