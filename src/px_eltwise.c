#include "px_cnn.h"
#include "px_assert.h"

void px_forward_eltwise_layer_for_array(const px_array_t* input, px_array_t* output, PxEltwiseFunction eltwise_func)
{
    PX_ASSERT(input != NULL && output != NULL);
    PX_ASSERT(input->data != NULL && output->data != NULL);
    PX_ASSERT(input->len <= output->len);

    for (int i = 0; i < input->len; i++)
    {
        output->data[i] = eltwise_func(input->data[i]);
    }
}

void px_forward_eltwise_layer_for_matrix(const px_matrix_t* input, px_matrix_t* output, PxEltwiseFunction eltwise_func)
{
    PX_ASSERT(input != NULL && output != NULL);
    PX_ASSERT(input->data != NULL && output->data != NULL);
    PX_ASSERT(input->height <= output->height);
    PX_ASSERT(input->width <= output->width);

    px_array_t input_array;
    input_array.data = input->data;
    input_array.len = px_get_matrix_area(input);

    px_array_t output_array;
    output_array.data = output->data;
    output_array.len = px_get_matrix_area(output);

    px_forward_eltwise_layer_for_array(&input_array, &output_array, eltwise_func);
}

void px_forward_eltwise_layer_for_cube(const px_cube_t* input, px_cube_t* output, PxEltwiseFunction eltwise_func)
{
    PX_ASSERT(input != NULL && output != NULL);
    PX_ASSERT(input->data != NULL && output->data != NULL);
    PX_ASSERT(input->height <= output->height);
    PX_ASSERT(input->width <= output->width);

    px_array_t input_array;
    input_array.data = input->data;
    input_array.len = px_get_cube_volume(input);

    px_array_t output_array;
    output_array.data = output->data;
    output_array.len = px_get_cube_volume(output);

    px_forward_eltwise_layer_for_array(&input_array, &output_array, eltwise_func);
}
