#pragma once

#include "naive_cnn.h"

void nc_conv2d(const NcBlob2D* input, const NcBlob2D* kernel, NcBlob2D* output, NcStride* stride);

void nc_convolution_forward_nhwc(NcConvolutionParam* param, NcBlob* bottom, NcBlob* top);
void nc_convolution_forward_nchw(NcConvolutionParam* param, NcBlob* bottom, NcBlob* top);

// unit test functions
void nc_convolution_test_nchw();
void nc_convolution_test_nhwc();
