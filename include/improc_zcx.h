#pragma once

#include "px_typebase.h"
#include "px_image.h"

#include <string>

void cv_image_padding(unsigned char* image, int height, int width, int* pad, unsigned char* out, std::string method = "reflect");
void copy_cut_border(px_image_t* src, px_image_t* dst, px_pad_t pad);

void cv_gaussian_filter(unsigned char* image, unsigned char* out_image, int height, int width, int kernel_size);

int transform_bilinear2d(unsigned char* output_data, int output_height, int output_width, int channels, \
    unsigned char* input_data, int input_orig_height, int input_orig_width, px_pointf_t* quad, bool align_corners = false);

int back_to_image_bilinear2d(unsigned char* output_data, int output_height, int output_width, int channels, \
    unsigned char* input_data, int input_orig_height, int input_orig_width, px_pointf_t* quad, bool align_corners = false);


int upsample_bilinear2d(float *output_data, const float *input_data, int input_height, int input_width,
    int output_height, int output_width, int channels, bool align_corners);

// nearest interpolate function
int upsample_nearest2d(unsigned char *output_data, const unsigned char *input_data, int input_height, int input_width,
    int output_height, int output_width, int channels);
