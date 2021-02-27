#include "threshold.h"

void threshold_gray(unsigned char* input_gray, size_t height, size_t width, unsigned char* output_gray, unsigned char thresh)
{
    size_t total_len = height * width;
    for (size_t done=0; done<total_len; done++) {
        *output_gray = (input_gray[0]>thresh)?255:0;
        input_gray++;
        output_gray++;
    }
}