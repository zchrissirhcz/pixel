#pragma once

#include "px_image.h"
#include "px_cnn.h"

typedef struct px_mnist_image_array_t
{
    int size;
    px_image_t* images;
    int n_cols;
    int n_rows;
} px_mnist_image_array_t;

typedef px_array_t px_mnist_label_t;

typedef struct px_mnist_label_array_t
{
    int size;
    int* label;
    px_mnist_label_t* one_hot_label;
} px_mnist_label_array_t;

typedef struct px_mnist_data_t
{
    px_mnist_image_array_t* train_images;
    px_mnist_label_array_t* train_labels;

    px_mnist_image_array_t* test_images;
    px_mnist_label_array_t* test_labels;
} px_mnist_data_t;

#ifdef __cplusplus
extern "C" {
#endif

px_mnist_label_array_t* px_read_mnist_label(const char* filename);
px_mnist_image_array_t* px_read_mnist_image(const char* filename);
void px_extract_mnist_image_and_save(const char* mnist_data_dir);
void px_destroy_mnist_image_array(px_mnist_image_array_t* image_array);
void px_destroy_mnist_label_array(px_mnist_label_array_t* label_array);
px_mnist_data_t* px_read_mnist_data(const char* mnist_data_dir);
void px_destroy_mnist_data(px_mnist_data_t* mnist_data);

#ifdef __cplusplus
}
#endif