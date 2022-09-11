#pragma once

#include "cnn.h"

typedef matrix_t mnist_image_t;

typedef struct mnist_image_array_t
{
    int size;
    mnist_image_t* images;
} mnist_image_array_t;

typedef px_array_t mnist_label_t;

typedef struct mnist_label_array_t
{
    int size;
    mnist_label_t* labels;
} mnist_label_array_t;

#ifdef __cplusplus
extern "C" {
#endif

mnist_label_array_t* read_mnist_label(const char* filename);
mnist_image_array_t* read_mnist_image(const char* filename);
void extract_mnist_image_and_save();
void nc_read_mnist_image(const char* filename, NcImage*** _images, int* _image_num);

#ifdef __cplusplus
}
#endif