#include "histogram.h"
#include "common/pixel_log.h"

void histogram_gray_naive(unsigned char* gray_buf, size_t height, size_t width, unsigned int* hist)
{
    size_t total_len = height * width;
    for (size_t i=0; i<total_len; i++) {
        hist[*gray_buf]++;
        gray_buf++;
    }
}

void histogram_gray_asimd(unsigned char* gray_buf, size_t height, size_t width, unsigned int* hist)
{
    //TODO: use this https://stackoverflow.com/a/38511540

    // size_t total_len = height * width;
    // size_t step = 8;
    // size_t vec_size = total_len - total_len % step;
    // for (size_t i=0; i<vec_size; i+=step) {
    //     hist[gray_buf[0]]++;
    //     hist[gray_buf[1]]++;
    //     hist[gray_buf[2]]++;
    //     hist[gray_buf[3]]++;
    //     hist[gray_buf[4]]++;
    //     hist[gray_buf[5]]++;
    //     hist[gray_buf[6]]++;
    //     hist[gray_buf[7]]++;

    //     gray_buf+=8;
    // }

    // for (size_t i=vec_size; i<total_len; i++) {
    //     hist[*gray_buf]++;
    //     gray_buf++;
    // }
}

void histogram_rgb_naive(unsigned char* rgb, size_t height, size_t width, unsigned int* hist, int mode)
{
    size_t total_len = height * width;
    if (mode<0 || mode>3) {
        PIXEL_LOGE("invalid channel_idx\n");;
        return ;
    }

    unsigned char* src = NULL;
    int gray;
    switch(mode) {
        case 0: { // gray histogram
            for (size_t i=0; i<total_len; i++) {
                gray = (rgb[0] + rgb[1] + rgb[2])/3;
                hist[gray]++;
                rgb += 3;
            }
        }
        break;

        case 1: { // R histogram
            src = rgb;
            for (size_t i=0; i<total_len; i++) {
                hist[*src]++;
                src += 3;
            }
        }
        break;

        case 2: { // G histogram
            src = rgb + 1;
            for (size_t i=0; i<total_len; i++) {
                hist[*src]++;
                src += 3;
            }
        }
        break;

        case 3: { // B histogram
            src = rgb + 2;
            for (size_t i=0; i<total_len; i++) {
                hist[*src]++;
                src += 3;
            }
        }
        break;
    }
}
