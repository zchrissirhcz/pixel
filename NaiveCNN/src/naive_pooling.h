#ifndef NAIVE_POOLING_H
#define NAIVE_POOLING_H

#include "naive_cnn.h"

void nc_pooling_forward_nhwc(NcPoolingParam* param, NcBlob* bottom, NcBlob* top);

void nc_pooling_forward_nchw(NcPoolingParam* param, NcBlob* bottom, NcBlob* top);

void nc_pooling_test_nchw();

void nc_pooling_test_nhwc();

#endif