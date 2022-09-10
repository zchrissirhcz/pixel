#ifndef NAIVE_RELU_H
#define NAIVE_RELU_H

#include "naive_cnn.h"

void nc_relu_forward(NcBlob* bottom);

void nc_relu6_forward(NcBlob* bottom);

#endif