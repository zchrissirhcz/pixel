#pragma once

#include "px_typebase.h"

typedef struct px_array_t
{
    float* data;
    int len;
} px_array_t;

#ifdef __cplusplus
extern "C" {
#endif

void px_relu_forward(px_array_t* input, px_array_t* output);

px_array_t* px_create_array(int len);
void px_destroy_array(px_array_t* array);

#ifdef __cplusplus
}
#endif