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

px_matrix_t* px_create_matrix(px_matrix_dim_t dim)
{
    const int height = dim.height;
    const int width = dim.width;
    px_matrix_t* matrix = (px_matrix_t*)malloc(sizeof(px_matrix_t));
    const size_t buf_size = height * width * sizeof(float);
    matrix->data = (float*)malloc(buf_size);
    matrix->height = height;
    matrix->width = width;

    // https://stackoverflow.com/questions/370195/when-and-why-will-a-compiler-initialise-memory-to-0xcd-0xdd-etc-on-malloc-fre
    memset(matrix->data, 0xcd, buf_size);
    // -431602080.000000

    return matrix;
}

void px_destroy_matrix(px_matrix_t* matrix)
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
    PX_ASSERT(i >= 0 && j >= 0 && i < matrix->height && j < matrix->width);

    int idx = i * matrix->width + j;
    matrix->data[idx] = value;
    return 0;
}

float px_get_matrix_value(const px_matrix_t* matrix, int i, int j)
{
    PX_ASSERT(matrix != NULL && matrix->data != NULL);
    PX_ASSERT(i >= 0 && j >= 0 && i < matrix->height && j < matrix->width);
    
    int idx = i * matrix->width + j;
    return matrix->data[idx];
}

int px_matrix_add_matrix(px_matrix_t* input, const px_matrix_t* plus)
{
    PX_ASSERT(input != NULL && plus != NULL);
    PX_ASSERT(input->height == plus->height && input->width == plus->width);
    PX_ASSERT(input->data != NULL && plus->data != NULL);
    
    for (int i = 0; i < input->height; i++)
    {
        for (int j = 0; j < input->width; j++)
        {
            int idx = i * input->width + j;
            input->data[idx] += plus->data[idx];
        }
    }
    return 0;
}

int px_matrix_add_scalar(px_matrix_t* input, const float bias)
{
    PX_ASSERT(input != NULL && input->data != NULL);
    PX_ASSERT(input->height > 0 && input->width > 0);
    
    const int size = input->height * input->width;
    for (int i = 0; i < size; i++)
    {
        input->data[i] += bias;
    }
    return 0;
}

px_matrix_dim_t px_get_matrix_dim(const px_matrix_t* matrix)
{
    px_matrix_dim_t matrix_dim = {0};
    matrix_dim.height = matrix->height;
    matrix_dim.width = matrix->width;
    return matrix_dim;
}

int px_get_matrix_area(const px_matrix_t* matrix)
{
    return matrix->height * matrix->width;
}

px_stride_t px_create_stride(const int height, const int width)
{
    return px_create_size(height, width);
}

px_cube_dim_t px_get_cube_dim(const px_cube_t* cube)
{
    px_cube_dim_t cube_dim = {0};
    cube_dim.height = cube->height;
    cube_dim.width = cube->width;
    cube_dim.channel = cube->channel;
    return cube_dim;
}

px_cube_t* px_create_cube(const px_cube_dim_t dim)
{
    const int c = dim.channel;
    const int h = dim.height;
    const int w = dim.width;
    px_cube_t* cube = (px_cube_t*)malloc(sizeof(px_cube_t));
    const size_t buf_size = c * h * w * sizeof(float);
    cube->data = (float*) malloc(buf_size);
    cube->channel = c;
    cube->height = h;
    cube->width = w;
    return cube;
}

int px_get_cube_volume(const px_cube_t* cube)
{
    return cube->height * cube->width * cube->channel;
}

px_matrix_t* px_copy_make_border_for_matrix(const px_matrix_t* matrix, const px_pad_t pad)
{
    if (pad.top < 0 || pad.bottom < 0 || pad.left < 0 || pad.right < 0)
    {
        PX_LOGE("invalid padding\n");
        return NULL;
    }
    if (matrix == NULL || matrix->data == NULL || matrix->height < 0 || matrix->width < 0)
    {
        PX_LOGE("invalid matrix\n");
        return NULL;
    }

    const int padded_h = matrix->height + pad.top + pad.bottom;
    const int padded_w = matrix->width + pad.left + pad.right;
    px_matrix_dim_t padded_dim = {0};
    padded_dim.height = padded_h;
    padded_dim.width = padded_w;
    px_matrix_t* padded_matrix = px_create_matrix(padded_dim);

    const int pad_value = 0;
    for (int i = 0; i < padded_h; i++)
    {
        for (int j = 0; j < padded_w; j++)
        {
            if (i < pad.top || i >= matrix->height + pad.top
             || j < pad.left || j >= matrix->width + pad.left)
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

void px_destroy_cube(px_cube_t* cube)
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

px_pad_t px_create_pad(const int top, const int bottom, const int left, const int right)
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
    matrix_dim.height = cube->height;
    matrix_dim.width = cube->width;
    return matrix_dim;
}

float* px_get_matrix_data_from_cube(const px_cube_t* cube, const int channel_idx)
{
    const int cstep = cube->height * cube->width;
    return cube->data + channel_idx * cstep;
}

px_matrix_t px_get_matrix_from_cube(const px_cube_t* cube, const int channel_idx)
{
    px_matrix_t matrix = {0};
    matrix.height = cube->height;
    matrix.width = cube->width;
    const int cstep = cube->height * cube->width;
    matrix.data = cube->data + channel_idx * cstep;
    return matrix;
}

px_kernel_size_t px_create_kernel_size(const int height, const int width)
{
    return px_create_size(height, width);
}

px_matrix_dim_t px_create_matrix_dim(const int height, const int width)
{
    return px_create_size(height, width);
}