#include "px_cnn.h"
#include "px_assert.h"
#include "px_log.h"
#include <stdlib.h>
#include <string.h>

px_array_t* px_create_array(int len)
{
    px_array_t* array = (px_array_t*) malloc(sizeof(px_array_t));
    memset(array, 0, sizeof(px_array_t));
    array->data = (float*) malloc(sizeof(float) * len);
    array->len = len;

    return array;
}

void px_destroy_array(px_array_t* array)
{
    if (array)
    {
        if (array->data)
        {
            free(array->data);
            array->data = NULL;
        }
        free(array);
    }
}

px_matrix_t* px_make_matrix(px_matrix_dim_t dim)
{
    const int h = dim.h;
    const int w = dim.w;
    px_matrix_t* matrix = (px_matrix_t*)malloc(sizeof(px_matrix_t));
    const size_t buf_size = h * w * sizeof(float);
    matrix->data = (float*)malloc(buf_size);
    matrix->h = h;
    matrix->w = w;

    // https://stackoverflow.com/questions/370195/when-and-why-will-a-compiler-initialise-memory-to-0xcd-0xdd-etc-on-malloc-fre
    memset(matrix->data, 0xcd, buf_size);
    // -431602080.000000

    return matrix;
}

void px_release_matrix(px_matrix_t* matrix)
{
    if (matrix != NULL)
    {
        if (matrix->data)
        {
            free(matrix->data);
            matrix->data = NULL;
        }
        free(matrix);
    }
}

int px_set_matrix_value(px_matrix_t* matrix, int i, int j, float value)
{
    PX_ASSERT(matrix != NULL && matrix->data != NULL);
    PX_ASSERT(i >= 0 && j >= 0 && i < matrix->h && j < matrix->w);

    int idx = i * matrix->w + j;
    matrix->data[idx] = value;
    return 0;
}

float px_get_matrix_value(const px_matrix_t* matrix, int i, int j)
{
    PX_ASSERT(matrix != NULL && matrix->data != NULL);
    PX_ASSERT(i >= 0 && j >= 0 && i < matrix->h && j < matrix->w);
    
    int idx = i * matrix->w + j;
    return matrix->data[idx];
}

int px_matrix_add_matrix(px_matrix_t* input, const px_matrix_t* plus)
{
    PX_ASSERT(input != NULL && plus != NULL);
    PX_ASSERT(input->h == plus->h && input->w == plus->w);
    PX_ASSERT(input->data != NULL && plus->data != NULL);
    
    for (int i = 0; i < input->h; i++)
    {
        for (int j = 0; j < input->w; j++)
        {
            int idx = i * input->w + j;
            input->data[idx] += plus->data[idx];
        }
    }
    return 0;
}

int px_matrix_add_scalar(px_matrix_t* input, const float bias)
{
    PX_ASSERT(input != NULL && input->data != NULL);
    PX_ASSERT(input->h > 0 && input->w > 0);
    
    const int size = input->h * input->w;
    for (int i = 0; i < size; i++)
    {
        input->data[i] += bias;
    }
    return 0;
}

px_matrix_dim_t px_get_matrix_dim(const px_matrix_t* matrix)
{
    px_matrix_dim_t matrix_dim = {0};
    matrix_dim.h = matrix->h;
    matrix_dim.w = matrix->w;
    return matrix_dim;
}

int px_get_matrix_area(const px_matrix_t* matrix)
{
    return matrix->h * matrix->w;
}

px_matrix_t* px_forward_eltwise_layer_for_matrix(const px_matrix_t* input, PxEltwiseFunction eltwise_func)
{
    px_matrix_dim_t input_dim = px_get_matrix_dim(input);
    px_matrix_t* output = px_make_matrix(input_dim);
    const int area = px_get_matrix_area(input);

    for (int i=0; i<area; i++)
    {
        output->data[i] = eltwise_func(input->data[i]);
    }
    return output;
}

px_stride_t px_make_stride(const int h, const int w)
{
    px_stride_t stride;
    stride.h = h;
    stride.w = w;

    return stride;
}
