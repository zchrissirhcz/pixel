#include "pixel_jpg.h"
#include "pixel_log.h"
#include "pixel_benchmark.h"
#include <opencv2/opencv.hpp>


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


int main() {
    PIXEL_LOGD("hello, jpg!");

    // cv::Mat src = cv::imread("duck.jpg");
    cv::Mat src = cv::imread("girl.jpg");

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
    cv::resize(cb_mat, half_cb_mat, half_size);

    cv::Mat half_cr_mat;
    cv::resize(cr_mat, half_cr_mat, half_size);

    cv::imshow("image", src);
    cv::imshow("y", y_mat);
    cv::imshow("cb", half_cb_mat);
    cv::imshow("cr", half_cr_mat);
    cv::waitKey(0);

    return 0;
}