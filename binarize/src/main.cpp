#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <arm_neon.h>

#include "common/pixel_log.h"
#include "common/pixel_benchmark.h"
#include <opencv2/opencv.hpp>

void binarize_c_naive(size_t len, unsigned char* buf, unsigned char thresh) {
    for (size_t i=0; i<len; i++) {
        if (buf[i]>thresh) {
            buf[i]=255;
        }
    }
}

void binarize_c_fast(size_t len, unsigned char* buf, unsigned char thresh) {
    for (size_t i=0; i<len; i++) {
        *buf = *buf>thresh ? 255: *buf;
        buf++;
    }
}

void binarize_neon_intrinsics(size_t len, unsigned char* buf, unsigned char thresh) {
    uint8x16_t vthresh = vdupq_n_u8(thresh);
    uint8x16_t ff = vdupq_n_u8(255);
    for (int i=0; i+15<len; i+=16) {
        uint8x16_t value = vld1q_u8(buf);
        uint8x16_t mask = vcgtq_u8(value, vthresh);
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

int main() {
    cv::Mat image = cv::imread("mingren.bmp");
    cv::Mat gray;
    cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
    cv::Size size = gray.size();
    int h = size.height;
    int w = size.width;
    const size_t len = h * w;
    const size_t buf_size = len * sizeof(unsigned char);
    // unsigned char* data = (unsigned char*)malloc(buf_size);
    // for (size_t i=0; i<len; i++) {
    //     data[i] = get_random_uint8();
    // }
    unsigned char* data = gray.data;

    unsigned char* data_copy1 = (unsigned char*)malloc(buf_size);
    unsigned char* data_copy2 = (unsigned char*)malloc(buf_size);
    unsigned char* data_copy3 = (unsigned char*)malloc(buf_size);
    unsigned char* data_copy4 = (unsigned char*)malloc(buf_size);
    memcpy(data_copy1, data, buf_size);
    memcpy(data_copy2, data, buf_size);
    memcpy(data_copy3, data, buf_size);
    memcpy(data_copy4, data, buf_size);

    double t_start, t_c_naive, t_c_fast, t_neon_intrinsics, t_neon_intrinsics2;
    const int loop = 10;
    unsigned char thresh = 150;
    //==================================
    // c naive impl
    //==================================
    {
        t_start = pixel_get_current_time();
        for (int i=0; i<loop; i++) {
            binarize_c_naive(len, data_copy1, thresh);
        }
        t_c_naive = pixel_get_current_time() - t_start;
    }

    //==================================
    // c fast impl
    //==================================
    {
        t_start = pixel_get_current_time();
        for (int i=0; i<loop; i++) {
            binarize_c_fast(len, data_copy2, thresh);
        }
        t_c_fast = pixel_get_current_time() - t_start;
    }

    //==================================
    // arm neon intrinsics impl
    //==================================
    {
        t_start = pixel_get_current_time();
        for (int i=0; i<loop; i++) {
            binarize_neon_intrinsics(len, data_copy3, thresh);
        }
        t_neon_intrinsics = pixel_get_current_time() - t_start;
    }

    //=================================
    // validate correctness
    //=================================
    int fail_cnt = 0;
    for (size_t i=0; i<len; i++) {
        if (data_copy1[i]!=data_copy2[i]
                || data_copy1[i]!=data_copy3[i]
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


    cv::Mat result(size, CV_8UC1, data_copy2);
    cv::imwrite("mingren_gray_binarized.bmp", result);

    // ===========================
    // clean ups
    // ===========================
    //free(data);
    free(data_copy1);
    free(data_copy2);
    free(data_copy3);
    free(data_copy4);

    return 0;
}
