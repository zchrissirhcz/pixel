#include "px_cnn.h"
#include <math.h>

void px_forward_tanh_layer_for_matrix(const px_matrix_t* input, px_matrix_t* output)
{
    return px_forward_eltwise_layer_for_matrix(input, output, tanhf);
}

px_cube_t* px_forward_tanh_layer_for_cube(const px_cube_t* input)
{
    return px_forward_eltwise_layer_for_cube(input, tanhf);
}