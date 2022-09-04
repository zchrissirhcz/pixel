#pragma once

#include "px_typebase.h"

typedef struct px_array_t
{
    float* data;
    int len;
} px_array_t;

typedef struct px_matrix_dim_t
{
    int h, w;
} px_matrix_dim_t;

typedef struct px_matrix_t
{
    float* data;
    int h, w; // px_matrix_dim_t
} px_matrix_t;

#ifdef __cplusplus
extern "C" {
#endif

void px_relu_forward(px_array_t* input, px_array_t* output);

px_array_t* px_create_array(int len);
void px_destroy_array(px_array_t* array);

px_matrix_t* px_make_matrix(px_matrix_dim_t dim);
void px_release_matrix(px_matrix_t* matrix);

int px_set_matrix_value(px_matrix_t* matrix, int i, int j, float value);
float px_get_matrix_value(const px_matrix_t* matrix, int i, int j);

int px_matrix_add_matrix(px_matrix_t* input, const px_matrix_t* plus);
int px_matrix_add_scalar(px_matrix_t* input, const float bias);

px_matrix_dim_t px_get_matrix_dim(const px_matrix_t* matrix);
int px_get_matrix_area(const px_matrix_t* matrix);

typedef float (*PxEltwiseFunction)(const float);

px_matrix_t* px_forward_sigmoid_layer_for_matrix(const px_matrix_t* input);
px_matrix_t* px_forward_eltwise_layer_for_matrix(const px_matrix_t* input, PxEltwiseFunction eltwise_func);

#ifdef __cplusplus
}
#endif