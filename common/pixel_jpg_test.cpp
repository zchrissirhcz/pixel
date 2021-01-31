#include "pixel_jpg.h"
#include "pixel_log.h"
#include "pixel_benchmark.h"
#include <opencv2/opencv.hpp>

//jpg encode/decode tutorial
//https://zhuanlan.zhihu.com/p/27296876


//http://www.robertstocker.co.uk/jpeg/jpeg_new_7.htm
/*
YCbCr (256 levels) can be computed directly from 8-bit RGB as follows:

Y = 0.299 R + 0.587 G + 0.114 B
Cb = - 0.1687 R - 0.3313 G + 0.5 B + 128
Cr = 0.5 R - 0.4187 G - 0.0813 B + 128
*/
static void pxl_rgb_to_ycbcr(uint8_t r, uint8_t g, uint8_t b, uint8_t* _y, uint8_t* _cb, uint8_t* _cr)
{
    *_y = (uint8_t)(0.299 * r + 0.587*g + 0.114*b);
    *_cb = (uint8_t)(-0.1687*r - 0.3313*g + 0.5*b + 128);
    *_cr = (uint8_t)(0.5*r - 0.4187*g - 0.0813*b + 128);
}

/*
RGB can be computed directly from YCbCr (256 levels) as follows:

R = Y + 1.402 (Cr-128)
G = Y - 0.34414 (Cb-128) - 0.71414 (Cr-128)
B = Y + 1.772 (Cb-128)
*/
static void pxl_ycbcr_to_rgb(uint8_t y, uint8_t cb, uint8_t cr, uint8_t* _r, uint8_t* _g, uint8_t* _b)
{
    *_r = (uint8_t)(y + 1.402 * (cr-128));
    *_g = (uint8_t)(y - 0.34414 * (cb - 128) - 0.71414 * (cr - 128));
    *_b = (uint8_t)(y + 1.772 * (cb - 128));
}

// faster than OpenCV, while result not exactly the same as opencv
static void pxl_downsample2x_c1(uint8_t* src, uint8_t* dst, int src_h, int src_w, int src_linebytes)
{
    int dst_h = src_h / 2;
    int dst_w = src_w / 2;
    for (int i = 0; i < dst_h; i++) {
        for (int j = 0; j < dst_w; j++) {
            int dst_idx = i * dst_w + j;
            int src_idx1 = 2 * i * src_linebytes + 2 * j;
            int src_idx2 = src_idx1 + 1;
            int src_idx3 = src_idx1 + src_linebytes;
            int src_idx4 = src_idx1 + src_linebytes + 1;
            int val = ((int)(src[src_idx1]) + (int)(src[src_idx2]) + (int)(src[src_idx3]) + (int)(src[src_idx4])) / 4;
            if (val > 255) {
                val = 255;
            }
            dst[dst_idx] = (uint8_t)val;
        }
    }
}


// DCT:离散余弦变换
// https://blog.csdn.net/tengfei0973/article/details/103186028
void pxl_dct(int* signal, int signal_len, float* result)
{
    int* x = signal;
    int N = signal_len;
    float* X = result;

    float alpha[2] = {
        sqrtf(1.0f / N), //coeff when k=0
        sqrtf(2.0f / N)  //coeff when k!=0
    };
    const float PI = acos(-1);

    float A;
    for (int k = 0; k < N; k++) {
        if (k == 0){
            A = alpha[0];
        }
        else {
            A = alpha[1];
        }
        float sum = 0;
        for (int n = 0; n < N; n++) {
            sum += x[n] * cos(PI * (2 * n + 1) * k / 2 * N);
        }
        X[k] = A * sum;
    }
}


//#define SHOW_RESULT


int main() {
    PIXEL_LOGD("hello, jpg!");

    // cv::Mat src = cv::imread("duck.jpg");
    //cv::Mat src = cv::imread("river_bank.jpg");
    cv::Mat src = cv::imread("paris.jpg");

    cv::Size size = src.size();
    int h = size.height;
    int w = size.width;
    unsigned char* data = src.data;
    unsigned char r, g, b;
    int linebytes = src.step[0];
    
    cv::Mat y_mat(size, CV_8UC1);
    unsigned char* y_data = y_mat.data;

    cv::Mat cb_mat(size, CV_8UC1);
    unsigned char* cb_data = cb_mat.data;

    cv::Mat cr_mat(size, CV_8UC1);
    unsigned char* cr_data = cr_mat.data;
    
    unsigned char y, cb, cr;

    double t_start = pixel_get_current_time();
    for (int i=0; i<h; i++) {
        for (int j=0; j<w; j++) {
            int idx = i*linebytes + j*3;
            b = data[idx];
            g = data[idx+1];
            r = data[idx+2];
            pxl_rgb_to_ycbcr(r, g, b, &y, &cb, &cr);

            int ycbcr_idx = i*w + j;
            y_data[ycbcr_idx] = y;
            cb_data[ycbcr_idx] = cb;
            cr_data[ycbcr_idx] = cr; 
        }
    }
    
    cv::Size half_size;
    half_size.height = size.height / 2;
    half_size.width = size.width / 2;

    cv::Mat half_cb_mat;

    double t_ocv_resize_start = pixel_get_current_time();
    cv::resize(cb_mat, half_cb_mat, half_size);
    double t_ocv_resize_cost = pixel_get_current_time() - t_ocv_resize_start;

    cv::Mat half_cr_mat;
    cv::resize(cr_mat, half_cr_mat, half_size);
    
    
    cv::Mat downsampled_cb(half_size, CV_8UC1);
    double t_pxl_downsample_start = pixel_get_current_time();
    pxl_downsample2x_c1(cb_mat.data, downsampled_cb.data, cb_mat.rows, cb_mat.cols, cb_mat.step[0]);
    double t_pxl_downsample_cost = pixel_get_current_time() - t_pxl_downsample_start;

    double t_cost = pixel_get_current_time() - t_start;

    printf("total time %lf ms, opencv resize cb %lf ms, naive downsample2x cost %lf ms\n", t_cost, t_ocv_resize_cost, t_pxl_downsample_cost);

    cv::imwrite("paris_opencv_resized_cb.bmp", half_cb_mat);
    cv::imwrite("paris_pixel_resized_cb.bmp", downsampled_cb);

#ifdef SHOW_RESULT
    cv::namedWindow("image");
    cv::moveWindow("image", 100, 100);
    cv::imshow("image", src);

    cv::namedWindow("y");
    cv::moveWindow("y", 100, 100);
    cv::imshow("y", y_mat);

    cv::namedWindow("cb");
    cv::moveWindow("cb", 100, 100);
    cv::imshow("cb", half_cb_mat);

    cv::namedWindow("cr");
    cv::moveWindow("cr", 100, 100);
    cv::imshow("cr", half_cr_mat);

    cv::namedWindow("downsampled_cb");
    cv::moveWindow("downsampled_cb", 100, 100);
    cv::imshow("downsampled_cb", downsampled_cb);

    cv::waitKey(0);
#endif

    return 0;
}