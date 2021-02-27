#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "common/pixel_log.h"
#include "common/pixel_benchmark.h"
#include "dotproduct/pixel_simd.h"
#include "common/pixel_check.h"

#include <opencv2/opencv.hpp>

void binarize_naive(size_t len, unsigned char* buf, unsigned char thresh) {
    for (size_t i=0; i<len; i++) {
        if (buf[i]>thresh) {
            buf[i]=255;
        }
    }
}

void binarize_fast(size_t len, unsigned char* buf, unsigned char thresh) {
    for (size_t i=0; i<len; i++) {
        *buf = *buf>thresh ? 255: *buf;
        buf++;
    }
}

void binarize_asimd(size_t len, unsigned char* buf, unsigned char thresh) {
    size_t vec_size = 0;
#ifdef PIXEL_SIMD_NEON
    const size_t step = 16;
    vec_size = len - len % step;
    uint8x16_t vthresh = vdupq_n_u8(thresh);
    uint8x16_t ff = vdupq_n_u8(255);
    for (size_t i=0; i<len; i+=step) {
        uint8x16_t value = vld1q_u8(buf);
        uint8x16_t mask = vcgtq_u8(value, vthresh);
        value = vbslq_u8(mask, ff, value);
        vst1q_u8(buf, value);
        buf += step;
    }
#endif
    for (size_t i=vec_size; i<len; i++) {
        if (*buf>thresh) {
            *buf = 255;
        }
        buf++;
    }
}

void binarize_psimd(size_t len, unsigned char* buf, unsigned char thresh)
{
    const size_t step = PIXEL_VQ_BYTES / sizeof(uint8_t);
    size_t vec_size = len - len % step;
    v_uint8x16 value;
    v_uint8x16 mask;
    v_uint8x16 vthresh = vq_setvalue_u8(thresh);
    uint8x16_t ff = vdupq_n_u8(255);
    for (size_t i=0; i<vec_size; i+=step) {
        value = vq_load_u8(buf);
        mask = vq_cmpgt_u8(value, vthresh);
        value = vq_bitselect_u8(mask, ff, value);
        vq_store_u8(buf, value);
        buf += step;
    }

    for (size_t i=vec_size; i<len; i++) {
        if ((*buf)>thresh) {
            *buf = 255;
        }
        buf++;
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
    unsigned char* data = gray.data;

    unsigned char* data_copy1 = (unsigned char*)malloc(buf_size);
    unsigned char* data_copy2 = (unsigned char*)malloc(buf_size);
    unsigned char* data_copy3 = (unsigned char*)malloc(buf_size);
    unsigned char* data_copy4 = (unsigned char*)malloc(buf_size);
    memcpy(data_copy1, data, buf_size);
    memcpy(data_copy2, data, buf_size);
    memcpy(data_copy3, data, buf_size);
    memcpy(data_copy4, data, buf_size);

    double t_start;
    const int loop = 10;
    unsigned char thresh = 150;
    // plain C imple
    t_start = pixel_get_current_time();
    for (int i=0; i<loop; i++) {
        binarize_naive(len, data_copy1, thresh);
    }
    double t_naive = pixel_get_current_time() - t_start;
    
    // fast C impl
    t_start = pixel_get_current_time();
    for (int i=0; i<loop; i++) {
        binarize_fast(len, data_copy2, thresh);
    }
    double t_fast = pixel_get_current_time() - t_start;
    int not_equal_count_fast = check_array_equal_u8(len, data_copy2, data_copy1);

    // asimd impl
    t_start = pixel_get_current_time();
    for (int i=0; i<loop; i++) {
        binarize_asimd(len, data_copy3, thresh);
    }
    double t_asimd = pixel_get_current_time() - t_start;
    int not_equal_count_asimd = check_array_equal_u8(len, data_copy3, data_copy1);

    // psimd impl
    t_start = pixel_get_current_time();
    for (int i=0; i<loop; i++) {
        binarize_psimd(len, data_copy4, thresh);
    }
    double t_psimd = pixel_get_current_time() - t_start;
    int not_equal_count_psimd = check_array_equal_u8(len, data_copy4, data_copy1);

    PIXEL_LOGD("naive C impl cost %.6lf ms\n", t_naive);
    PIXEL_LOGD("fast C impl cost %.6lf ms, mismatch count=%d\n", t_fast, not_equal_count_fast);
    PIXEL_LOGD("asimd impl cost %.6lf ms, mismatch count=%d\n", t_asimd, not_equal_count_asimd);
    PIXEL_LOGD("psimd impl cost %.6lf ms, mismatch count=%d\n", t_psimd, not_equal_count_psimd);


    cv::Mat result(size, CV_8UC1, data_copy2);
    cv::imwrite("mingren_gray_binarized.bmp", result);

    // ===========================
    // clean ups
    // ===========================
    free(data_copy1);
    free(data_copy2);
    free(data_copy3);
    free(data_copy4);

    return 0;
}
