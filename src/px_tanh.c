#include "px_cnn.h"
#include <math.h>

px_matrix_t* px_forward_tanh_layer_for_matrix(const px_matrix_t* input)
{
    return px_forward_eltwise_layer_for_matrix(input, tanhf);
}

px_cube_t* px_forward_tanh_layer_for_cube(const px_cube_t* input)
{
    return px_forward_eltwise_layer_for_cube(input, tanhf);
}