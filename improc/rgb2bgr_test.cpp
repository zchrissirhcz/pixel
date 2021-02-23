#include <stdio.h>
#include "common/pixel_cpu_affinity.h"
#include "common/pixel_benchmark.h"
#include "common/pixel_log.h"
#include "common/pixel_cpu_affinity.h"

#include <opencv2/opencv.hpp>

#include "rgb2bgr.h"

int main() {
    size_t mask = 0;
    for (int i = 0; i < 8; ++i) {
      if (i >= 5) {
        mask |= (1 << i);
      }
    }
    int ret = set_sched_affinity(mask);

    cv::Mat image = cv::imread("sky.jpg");
    cv::Size size = image.size();
    size_t height = (int)size.height;
    size_t width = (int)size.width;
    size_t src_linebytes = width * 3;

    cv::Mat mat_naive = cv::Mat(size, CV_8UC3);
    cv::Mat mat_idxopt = cv::Mat(size, CV_8UC3);
    cv::Mat mat_asimd = cv::Mat(size, CV_8UC3);
    cv::Mat mat_asm = cv::Mat(size, CV_8UC3);
    cv::Mat mat_opencv = cv::Mat(size, CV_8UC3);

    size_t dst_linebytes = width * 3;
    unsigned char* src_buf = image.data;
    unsigned char* dst_buf;

    double t_start, t_cost1, t_cost2, t_cost3, t_cost4, t_cost5;

    dst_buf = mat_naive.data;
    t_start = pixel_get_current_time();
    rgb2bgr_naive(src_buf, height, width, src_linebytes, dst_buf, dst_linebytes);
    t_cost1 = pixel_get_current_time() - t_start;
    PIXEL_LOGD("rgb2bgr, naive impl cost %.4lf ms", t_cost1);

    dst_buf = mat_idxopt.data;
    t_start = pixel_get_current_time();
    rgb2bgr_idxopt(src_buf, height, width, src_linebytes, dst_buf, dst_linebytes);
    t_cost2 = pixel_get_current_time() - t_start;
    PIXEL_LOGD("rgb2bgr, idxopt cost %.4lf ms", t_cost2);

    dst_buf = mat_asimd.data;
    t_start = pixel_get_current_time();
    rgb2bgr_asimd(src_buf, height, width, src_linebytes, dst_buf, dst_linebytes);
    t_cost3 = pixel_get_current_time() - t_start;
    PIXEL_LOGD("rgb2bgr, asimd cost %.4lf ms", t_cost3);

    dst_buf = mat_asm.data;
    t_start = pixel_get_current_time();
    rgb2bgr_asm(src_buf, height, width, src_linebytes, dst_buf, dst_linebytes);
    t_cost4 = pixel_get_current_time() - t_start;
    PIXEL_LOGD("rgb2bgr, asm cost %.4lf ms", t_cost4);

    t_start = pixel_get_current_time();
    cv::cvtColor(image, mat_opencv, cv::COLOR_BGR2RGB);
    t_cost5 = pixel_get_current_time() - t_start;
    PIXEL_LOGD("rgb2bgr, opencv cost %.4lf ms", t_cost5);

    cv::imwrite("sky_rgb_naive.bmp", mat_naive);
    cv::imwrite("sky_rgb_idxopt.bmp", mat_idxopt);
    cv::imwrite("sky_rgb_asimd.bmp", mat_asimd);
    cv::imwrite("sky_rgb_asm.bmp", mat_asm);

    // ---------
    double t_cost6, t_cost7, t_cost8, t_cost9;
    cv::Mat image_shadow6 = image.clone();
    cv::Mat image_shadow7 = image.clone();
    cv::Mat image_shadow8 = image.clone();
    cv::Mat image_shadow9 = image.clone();

    t_start = pixel_get_current_time();
    src_buf = image_shadow6.data;
    rgb2bgr_inplace_naive(src_buf, height, width, src_linebytes);
    t_cost6 = pixel_get_current_time() - t_start;
    PIXEL_LOGD("rgb2bgr_inplace, naive impl cost %.4lf ms", t_cost6);

    t_start = pixel_get_current_time();
    src_buf = image_shadow7.data;
    rgb2bgr_inplace_naive2(src_buf, height, width, src_linebytes);
    t_cost7 = pixel_get_current_time() - t_start;
    PIXEL_LOGD("rgb2bgr_inplace, naive2 impl cost %.4lf ms", t_cost7);

    t_start = pixel_get_current_time();
    src_buf = image_shadow8.data;
    rgb2bgr_inplace_asm(src_buf, height, width, src_linebytes);
    t_cost8 = pixel_get_current_time() - t_start;
    PIXEL_LOGD("rgb2bgr_inplace, asm impl cost %.4lf ms", t_cost8);

    t_start = pixel_get_current_time();
    cv::cvtColor(image_shadow9, image_shadow9, cv::COLOR_BGR2RGB);
    t_cost9 = pixel_get_current_time() - t_start;
    PIXEL_LOGD("rgb2bgr_inplace, opencv cost %.4lf ms", t_cost9);

    cv::imwrite("sky_rgb6.bmp", image_shadow6);
    cv::imwrite("sky_rgb7.bmp", image_shadow7);
    cv::imwrite("sky_rgb5.bmp", image_shadow8);
    cv::imwrite("sky_rgb5.bmp", image_shadow9);

    return 0;
}



#include <arm_neon.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void swap_u8();

int main1() {
    swap_u8();

    return 0;
}

void swap_u8() {
#ifdef __aarch64__
    #define NUM 48
    uint8_t* src = (uint8_t*)malloc(sizeof(uint8_t)*NUM);
    for (size_t i=0; i<NUM; i++) {
        src[i] = i;
    }
    uint8_t* dst = (uint8_t*)malloc(sizeof(uint8_t)*NUM);

    // 这里的汇编来自 https://developer.arm.com/architectures/instruction-sets/simd-isas/neon/neon-programmers-guide-for-armv8-a/coding-for-neon/single-page
    // 注意：armv8和armv7的汇编写法不一样！
    // 经过测试，armv8里的大小写并不敏感，比如LD3换成ld3，可以替换使用
    // 另外 https://aijishu.com/a/1060000000116427 这篇也参考到了
    __asm__ volatile(
        "ld3    { v0.16b, v1.16b, v2.16b }, [%1]\n"
        "mov    v3.16b, v0.16b       \n"
        "mov    v0.16b, v2.16b       \n"
        "mov    v2.16b, v3.16b        \n"
        "st3    { v0.16b, v1.16b, v2.16b }, [%0]"
        : "=r"(dst), //%0
        "=r"(src)    //%1
        : "0"(dst),
        "1"(src)
        : "cc", "memory", "v0", "v1", "v2"
    );
#else
    #define NUM 24
    uint8_t* src = (uint8_t*)malloc(sizeof(uint8_t)*NUM);
    for (size_t i=0; i<NUM; i++) {
        src[i] = i;
    }
    uint8_t* dst = (uint8_t*)malloc(sizeof(uint8_t)*NUM);

    __asm__ volatile(
        "vld3.u8    {d0-d2}, [%1]       \n"
        "vswp.u8       d0, d2               \n"
        "vst3.u8    {d0-d2}, [%0]       \n"
        : "=r"(dst), //%0
        "=r"(src)    //%1
        : "0"(dst),
        "1"(src)
        : "cc", "memory", "q0", "q1", "q2"
    );
#endif
    for (size_t i=0; i<NUM; i++) {
        printf("dst[%zu]=%d\n", i, (int)dst[i]);
    }
    free(src);
    free(dst);
}
