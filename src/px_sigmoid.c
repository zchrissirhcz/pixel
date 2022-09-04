#include "px_cnn.h"
#include <math.h>


static float sigmoid(const float x)
{
    return 1 / (1 + exp(-x));
}

px_matrix_t* px_forward_sigmoid_layer_for_matrix(const px_matrix_t* input)
{
    return px_forward_eltwise_layer_for_matrix(input, sigmoid);
}