#include "px_mnist.h"
#include "px_cnn.h"
#include "px_endian.h"
#include "px_log.h"
#include "px_bmp.h"
#include "px_filesystem.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define CHECK_WRITE_FILE(fp, filename) \
    if (fp == NULL)                    \
        PX_LOGE("Failed to open file %s for write in %s:%d\n", filename, __FILE__, __LINE__);

#define CHECK_READ_FILE(fp, filename) \
    if (fp == NULL)                   \
        PX_LOGE("Failed to open file %s for read in %s:%d\n", filename, __FILE__, __LINE__);

static px_mnist_image_array_t* create_mnist_image_array(int number_of_images)
{
    px_mnist_image_array_t* image_array = (px_mnist_image_array_t*)malloc(sizeof(px_mnist_image_array_t));
    image_array->size = number_of_images;
    image_array->images = (px_image_t*)malloc(number_of_images * sizeof(px_image_t));
    return image_array;
}

static int read_integer_from_mnist_file(FILE* fin)
{
    int number = 0;
    fread((char*)&number, sizeof(number), 1, fin);
    if (px_is_little_endian())
    {
        number = px_reverse_int(number);
    }
    return number;
}

static int get_mnist_magic_number(FILE* fin)
{
    return read_integer_from_mnist_file(fin);
}

static int get_mnist_number_of_images(FILE* fin)
{
    return read_integer_from_mnist_file(fin);
}

static int get_mnist_number_of_labels(FILE* fin)
{
    return read_integer_from_mnist_file(fin);
}

static int get_mnist_image_height(FILE* fin)
{
    return read_integer_from_mnist_file(fin);
}

static int get_mnist_image_width(FILE* fin)
{
    return read_integer_from_mnist_file(fin);
}

// 获取第i幅图像，保存到vec中
static void read_mnist_single_image(const int n_rows, const int n_cols, px_mnist_image_array_t* image_array, FILE* fin, int index)
{
    uint8_t* u8_data = image_array->images[index].data;
    fread(u8_data, n_rows * n_cols, 1, fin);
}

px_mnist_image_array_t* px_read_mnist_image(const char* filename)
{
    FILE* fin = fopen(filename, "rb");
    CHECK_READ_FILE(fin, filename);

    int magic_number = get_mnist_magic_number(fin);
    int number_of_images = get_mnist_number_of_images(fin);
    int n_rows = get_mnist_image_height(fin);
    int n_cols = get_mnist_image_width(fin);

    px_mnist_image_array_t* image_array = create_mnist_image_array(number_of_images);
    image_array->n_cols = n_cols;
    image_array->n_rows = n_rows;

    for (int i = 0; i < number_of_images; i++)
    {
        image_array->images[i].height = n_rows;
        image_array->images[i].width = n_cols;
        image_array->images[i].channel = 1;
        image_array->images[i].data = (uint8_t*)malloc(n_rows * n_cols * sizeof(uint8_t));
        read_mnist_single_image(n_rows, n_cols, image_array, fin, i);
    }

    fclose(fin);
    return image_array;
}

static px_mnist_label_array_t* create_px_mnist_label_array_t(int number_of_labels)
{
    px_mnist_label_array_t* label_array = (px_mnist_label_array_t*)malloc(sizeof(px_mnist_label_array_t));
    memset(label_array, 0, sizeof(px_mnist_label_array_t));
    label_array->size = number_of_labels;
    label_array->label = (int*)malloc(number_of_labels * sizeof(int));
    label_array->one_hot_label = (px_mnist_label_t*)malloc(number_of_labels * sizeof(px_mnist_label_t));
    return label_array;
}

static uint8_t read_mnist_single_label(FILE* fin)
{
    unsigned char label = 0;
    fread(&label, sizeof(label), 1, fin);
    return label;
}

px_mnist_label_array_t* px_read_mnist_label(const char* filename)
{
    FILE* fin = fopen(filename, "rb");
    CHECK_READ_FILE(fin, filename);

    int label_long = 10;

    int magic_number = get_mnist_magic_number(fin);
    int number_of_labels = get_mnist_number_of_labels(fin);

    px_mnist_label_array_t* label_array = create_px_mnist_label_array_t(number_of_labels);

    for (int i = 0; i < number_of_labels; ++i)
    {
        label_array->one_hot_label[i].len = 10;
        label_array->one_hot_label[i].data = (float*)calloc(label_long, sizeof(float));

        uint8_t label = read_mnist_single_label(fin);
        label_array->one_hot_label[i].data[label] = 1.0f;
    }

    fclose(fin);
    return label_array;
}

// extract images from original mnist data
// save each image to file
// for simplicity, only do it on test images
void px_extract_mnist_image_and_save(const char* mnist_data_dir, const char* mnist_filename, const char* save_dir)
{
    char test_image_pth[PX_MAX_PATH];
    sprintf(test_image_pth, "%s/%s", mnist_data_dir, mnist_filename);

    px_mkdir(save_dir);

    px_mnist_image_array_t* image_array = px_read_mnist_image(test_image_pth);
    PX_LOGE("=== got %d test images\n", image_array->size);

    for (int i = 0; i < image_array->size; i++)
    {
        char save_path[PX_MAX_PATH];
        sprintf(save_path, "%s/%d.bmp", save_dir, i);
        px_image_t image = image_array->images[i];

        px_write_bmp(save_path, image.height, image.width, image.channel, image.data);
    }

    px_destroy_mnist_image_array(image_array);
}

void px_destroy_mnist_image_array(px_mnist_image_array_t* image_array)
{
    for (int i = 0; i < image_array->size; i++)
    {
        free(image_array->images[i].data);
    }
    free(image_array->images);
    free(image_array);
}

void px_destroy_mnist_label_array(px_mnist_label_array_t* label_array)
{
    for (int i = 0; i < label_array->size; i++)
    {
        free(label_array->one_hot_label[i].data);
    }
    free(label_array->one_hot_label);
    free(label_array->label);
    free(label_array);
}

px_mnist_data_t* px_read_mnist_data(const char* mnist_data_dir)
{
    char train_label_pth[PX_MAX_PATH];
    sprintf(train_label_pth, "%s/train-labels.idx1-ubyte", mnist_data_dir);
    px_mnist_label_array_t* train_labels = px_read_mnist_label(train_label_pth);

    char train_image_pth[PX_MAX_PATH];
    sprintf(train_image_pth, "%s/train-images.idx3-ubyte", mnist_data_dir);
    px_mnist_image_array_t* train_images = px_read_mnist_image(train_image_pth);

    char test_label_pth[PX_MAX_PATH];
    sprintf(test_label_pth, "%s/t10k-labels.idx1-ubyte", mnist_data_dir);
    px_mnist_label_array_t* test_labels = px_read_mnist_label(test_label_pth);

    char test_image_pth[PX_MAX_PATH];
    sprintf(test_image_pth, "%s/t10k-images.idx3-ubyte", mnist_data_dir);
    px_mnist_image_array_t* test_images = px_read_mnist_image(test_image_pth);

    px_mnist_data_t* mnist_data = (px_mnist_data_t*)malloc(sizeof(px_mnist_data_t));
    mnist_data->train_images = train_images;
    mnist_data->train_labels = train_labels;
    mnist_data->test_images = test_images;
    mnist_data->test_labels = test_labels;

    return mnist_data;
}

void px_destroy_mnist_data(px_mnist_data_t* mnist_data)
{
    px_destroy_mnist_image_array(mnist_data->train_images);
    px_destroy_mnist_image_array(mnist_data->test_images);
    px_destroy_mnist_label_array(mnist_data->train_labels);
    px_destroy_mnist_label_array(mnist_data->test_labels);
    free(mnist_data);
}