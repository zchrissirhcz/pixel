#include "naive_relu.h"

void nc_relu_forward(NcBlob* bottom) {
    for (int i = 0; i < bottom->mass; i++) {
        bottom->data[i] = bottom->data[i] > 0 ? bottom->data[i] : 0;
    }
}
