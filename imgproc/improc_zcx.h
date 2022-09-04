#pragma once

#include <vector>
#include <iostream>
#include <cmath>
#include <iomanip>
#include <string>

typedef struct __tag_float_point__
{
    float x;
    float y;
} FPOINT_;


void cv_image_padding(unsigned char* image, int height, int width, int* pad, unsigned char* out, std::string method = "reflect");
void cv_image_remove_padding(unsigned char* pad_image, int pad_height, int pad_width, int* pad, unsigned char* out);

void cv_gaussian_filter(unsigned char* image, unsigned char* out_image, int height, int width, int kernel_size);

int transform_bilinear2d(unsigned char* output_data, int output_height, int output_width, int channels, \
    unsigned char* input_data, int input_orig_height, int input_orig_width, FPOINT_* quad, bool align_corners = false);

int back_to_image_bilinear2d(unsigned char* output_data, int output_height, int output_width, int channels, \
    unsigned char* input_data, int input_orig_height, int input_orig_width, FPOINT_* quad, bool align_corners = false);


int upsample_bilinear2d(float *output_data, const float *input_data, int input_height, int input_width,
    int output_height, int output_width, int channels, bool align_corners);

// nearest interpolate function
int upsample_nearest2d(unsigned char *output_data, const unsigned char *input_data, int input_height, int input_width,
    int output_height, int output_width, int channels);
