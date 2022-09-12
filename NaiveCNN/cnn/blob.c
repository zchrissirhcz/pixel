#include "cnn.h"

float* create_blob1d(int len)
{
    return (float*)malloc(sizeof(float) * len);
}

float** create_blob2d(px_size_t size)
{
    float** data = (float**)malloc(size.height * sizeof(float*));
    for (int i = 0; i < size.height; i++)
    {
        data[i] = (float*)malloc(size.width * sizeof(float));
    }
    return data;
}

float*** create_blob3d(px_cube_dim_t cube_dim)
{
    float*** data = (float***)malloc(cube_dim.channel * sizeof(float**));
    px_size_t size = px_create_size(cube_dim.height, cube_dim.width);
    for (int i = 0; i < cube_dim.channel; i++)
    {
        data[i] = create_blob2d(size);
    }
    return data;
}

float**** create_blob4d(px_tensor_dim_t tensor_dim)
{
    float**** data = (float****)malloc(tensor_dim.batch * sizeof(float***));
    px_cube_dim_t cube_dim = px_create_cube_dim(tensor_dim.channel, tensor_dim.height, tensor_dim.width);
    for (int i = 0; i < tensor_dim.batch; i++)
    {
        data[i] = create_blob3d(cube_dim);
    }
    return data;
}

void destroy_blob1d(float* data)
{
    free(data);
}

void destroy_blob2d(float** data, px_size_t size)
{
    for (int i = 0; i < size.height; i++)
    {
        destroy_blob1d(data[i]);
    }
    free(data);
}

void destroy_blob3d(float*** data, px_cube_dim_t cube_dim)
{
    px_size_t size = px_create_size(cube_dim.height, cube_dim.width);
    for (int i = 0; i < cube_dim.channel; i++)
    {
        destroy_blob2d(data[i], size);
    }
    free(data);
}

void destroy_blob4d(float**** data, px_tensor_dim_t tensor_dim)
{
    px_cube_dim_t cube_dim = px_create_cube_dim(tensor_dim.height, tensor_dim.height, tensor_dim.width);
    for (int i = 0; i < tensor_dim.batch; i++)
    {
        destroy_blob3d(data[i], cube_dim);
    }
    free(data);
}



void clear_blob1d(float* data, int len)
{
    for (int i = 0; i < len; i++)
    {
        data[i] = 0.0f;
    }
}

void clear_blob2d(float** data, px_size_t size)
{
    for (int i = 0; i < size.height; i++)
    {
        clear_blob1d(data[i], size.width);
    }
}

void clear_blob3d(float*** data, px_cube_dim_t cube_dim)
{
    px_size_t size = px_create_size(cube_dim.height, cube_dim.width);
    for (int i = 0; i < cube_dim.channel; i++)
    {
        clear_blob2d(data[i], size);
    }
}

void clear_blob4d(float**** data, px_tensor_dim_t tensor_dim)
{
    px_cube_dim_t cube_dim = px_create_cube_dim(tensor_dim.channel, tensor_dim.height, tensor_dim.width);
    for (int i = 0; i < tensor_dim.batch; i++)
    {
        clear_blob3d(data[i], cube_dim);
    }
}