#include "px_cnn.h"
#include <math.h>

static float sigmoid(const float x)
{
    return 1 / (1 + exp(-x));
}

void px_forward_sigmoid_layer_for_matrix(const px_matrix_t* input, px_matrix_t* output)
{
    return px_forward_eltwise_layer_for_matrix(input, output, sigmoid);
}

void px_forward_sigmoid_layer_for_cube(const px_cube_t* input, px_cube_t* output)
{
    return px_forward_eltwise_layer_for_cube(input, output, sigmoid);
}