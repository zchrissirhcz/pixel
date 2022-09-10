#include "px_cnn.h"
#include "px_assert.h"

static float relu(float x)
{
    return (x > 0) ? x: 0;
}

void px_relu_forward(px_array_t* input, px_array_t* output)
{
    PX_ASSERT(output != NULL);
    PX_ASSERT(input != NULL);
    PX_ASSERT(input->len == output->len);
    const int len = input->len;
    for (int i = 0; i < len; i++)
    {
        output->data[i] = (input->data[i] > 0) ? input->data[i] : 0;
    }
}

void px_forward_relu_layer_for_matrix(const px_matrix_t* input, px_matrix_t* output)
{
    return px_forward_eltwise_layer_for_matrix(input, output, relu);
}

void px_forward_relu_layer_for_cube(const px_cube_t* input, px_cube_t* output)
{
    return px_forward_eltwise_layer_for_cube(input, output, relu);
}