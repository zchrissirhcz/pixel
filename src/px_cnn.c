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
    const int len = px_get_matrix_area(input);

    for (int i = 0; i < len; i++)
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

px_cube_t* px_forward_eltwise_layer_for_cube(const px_cube_t* input, PxEltwiseFunction eltwise_func)
{
    px_cube_dim_t input_dim = px_get_cube_dim(input);
    px_cube_t* output = px_make_cube(input_dim);
    const int len = px_get_cube_volume(input);

    for (int i = 0; i < len; i++)
    {
        output->data[i] = eltwise_func(input->data[i]);
    }
    return output;
}

px_cube_dim_t px_get_cube_dim(const px_cube_t* cube)
{
    px_cube_dim_t cube_dim = {0};
    cube_dim.h = cube->h;
    cube_dim.w = cube->w;
    cube_dim.c = cube->c;
    return cube_dim;
}

px_cube_t* px_make_cube(const px_cube_dim_t dim)
{
    const int c = dim.c;
    const int h = dim.h;
    const int w = dim.w;
    px_cube_t* cube = (px_cube_t*)malloc(sizeof(px_cube_t));
    const size_t buf_size = c * h * w * sizeof(float);
    cube->data = (float*) malloc(buf_size);
    cube->c = c;
    cube->h = h;
    cube->w = w;
    return cube;
}

int px_get_cube_volume(const px_cube_t* cube)
{
    return cube->h * cube->w * cube->c;
}

px_matrix_t* px_copy_make_border_for_matrix(const px_matrix_t* matrix, const px_pad_t pad)
{
    if (pad.top<0 || pad.bottom<0 || pad.left<0 || pad.right<0) {
        PX_LOGE("invalid padding\n");
        return NULL;
    }
    if (matrix==NULL || matrix->data==NULL || matrix->h<0 || matrix->w<0) {
        PX_LOGE("invalid matrix\n");
        return NULL;
    }

    const int padded_h = matrix->h + pad.top + pad.bottom;
    const int padded_w = matrix->w + pad.left + pad.right;
    px_matrix_dim_t padded_dim = {0};
    padded_dim.h = padded_h;
    padded_dim.w = padded_w;
    px_matrix_t* padded_matrix = px_make_matrix(padded_dim);

    const int pad_value = 0;
    for (int i=0; i<padded_h; i++) {
        for (int j=0; j<padded_w; j++) {
            if (i<pad.top || i>=matrix->h+pad.top
             || j<pad.left || j>=matrix->w+pad.left)
            {
                px_set_matrix_value(padded_matrix, i, j, pad_value);
            }
            else
            {
                const int si = i - pad.top;
                const int sj = j - pad.left;
                const float value = px_get_matrix_value(matrix, si, sj);
                px_set_matrix_value(padded_matrix, i, j, value);
            }
        }
    }

    return padded_matrix;
}

void px_release_cube(px_cube_t* cube)
{
    if (cube != NULL)
    {
        if (cube->data)
        {
            free(cube->data);
            cube->data = NULL;
        }
        free(cube);
    }
}

px_pad_t px_make_pad(const int top, const int bottom, const int left, const int right)
{
    px_pad_t pad;
    pad.top = top;
    pad.bottom = bottom;
    pad.left = left;
    pad.right = right;

    return pad;
}

px_matrix_dim_t px_get_channel_wise_matrix_dim(const px_cube_t* cube)
{
    px_matrix_dim_t matrix_dim = {0};
    matrix_dim.h = cube->h;
    matrix_dim.w = cube->w;
    return matrix_dim;
}

float* px_get_matrix_data_from_cube(const px_cube_t* cube, const int channel_idx)
{
    const int cstep = cube->h * cube->w;
    return cube->data + channel_idx * cstep;
}

px_matrix_t px_get_matrix_from_cube(const px_cube_t* cube, const int channel_idx)
{
    px_matrix_t matrix = {0};
    matrix.h = cube->h;
    matrix.w = cube->w;
    const int cstep = cube->h * cube->w;
    matrix.data = cube->data + channel_idx * cstep;
    return matrix;
}
