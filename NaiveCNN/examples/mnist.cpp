#include "mnist.h"
#include "naive_cnn.h"
#include "px_cnn.h"

#include <opencv2/opencv.hpp>

// 英特尔处理器和其他低端机用户必须翻转头字节。
static
int mnist_reverse_int(int i)
{
    unsigned char ch1, ch2, ch3, ch4;
    ch1 = i & 255;
    ch2 = (i >> 8) & 255;
    ch3 = (i >> 16) & 255;
    ch4 = (i >> 24) & 255;
    return((int) ch1 << 24) + ((int)ch2 << 16) + ((int)ch3 << 8) + ch4;
}

static
mnist_image_array_t* create_mnist_image_array(int number_of_images)
{
    mnist_image_array_t* imgarr = (mnist_image_array_t*)malloc(sizeof(mnist_image_array_t));
    imgarr->size = number_of_images;
    imgarr->images_f32 = (mnist_image_t*)malloc(number_of_images * sizeof(mnist_image_t));
    imgarr->images_u8 = (NcImage*)malloc(number_of_images * sizeof(NcImage));
    return imgarr;
}

static
int read_number_from_mnist_file(FILE* fin)
{
    int number = 0;
    fread((char*)&number, sizeof(number), 1, fin);
    number = mnist_reverse_int(number);
    return number;
}

// 从文件中读取sizeof(magic_number) 个字符到 &magic_number
static
int get_mnist_magic_number(FILE* fin)
{
    return read_number_from_mnist_file(fin);
}

// 获取训练或测试image的个数number_of_images
static
int get_mnist_number_of_images(FILE* fin)
{
    return read_number_from_mnist_file(fin);
}

//获取训练或测试image的个数number_of_images
static
int get_mnist_number_of_labels(FILE* fin)
{
    return read_number_from_mnist_file(fin);
}

// 获取训练或测试图像的高度Height
static
int get_mnist_image_height(FILE* fin)
{
    int n_rows = 0;
    fread((char*)&n_rows,sizeof(n_rows), 1, fin);
    n_rows = mnist_reverse_int(n_rows);
    return n_rows;
}

// 获取训练或测试图像的宽度Width
static
int get_mnist_image_width(FILE* fin)
{
    int n_cols = 0;
    fread((char*)&n_cols,sizeof(n_cols), 1, fin);
    n_cols = mnist_reverse_int(n_cols);
    return n_cols;
}

// 获取第i幅图像，保存到vec中
static
void read_mnist_single_image(const int n_rows, const int n_cols, mnist_image_array_t* imgarr, FILE* fin, int index)
{
    uint8_t* u8_data = imgarr->images_u8[index].data;
    matrix_t f32_data = imgarr->images_f32[index];
    fread(u8_data, n_rows * n_cols, 1, fin);
    for(int i = 0; i < n_rows; i++)
    {
        for(int j = 0; j < n_cols; j++)
        {
            f32_data.data[i][j] = u8_data[i * n_cols + j] / 255.0f;
        }
    }
}

mnist_image_array_t* read_mnist_image(const char* filename)
{
    FILE* fin = fopen(filename, "rb");
    CHECK_READ_FILE(fin, filename);

    int magic_number = get_mnist_magic_number(fin);
    int number_of_images = get_mnist_number_of_images(fin);
    int n_rows = get_mnist_image_height(fin);
    int n_cols = get_mnist_image_width(fin);
    
    mnist_image_array_t* imgarr = create_mnist_image_array(number_of_images);
    imgarr->n_cols = n_cols;
    imgarr->n_rows = n_rows;

    for(int i = 0; i < number_of_images; i++)
    {
        imgarr->images_f32[i] = create_matrix(n_rows, n_cols);
        imgarr->images_u8[i].height = n_rows;
        imgarr->images_u8[i].width = n_cols;
        imgarr->images_u8[i].channel = 1;
        imgarr->images_u8[i].data = (uint8_t*)malloc(n_rows * n_cols * sizeof(uint8_t));
        read_mnist_single_image(n_rows, n_cols, imgarr, fin, i);
    }

    fclose(fin);
    return imgarr;
}

static
mnist_label_array_t* create_mnist_label_array_t(int number_of_labels)
{
    mnist_label_array_t* label_array = (mnist_label_array_t*)malloc(sizeof(mnist_label_array_t));
    memset(label_array, 0, sizeof(mnist_label_array_t));
    label_array->size = number_of_labels;
    label_array->label = (int*)malloc(number_of_labels * sizeof(int));
    label_array->one_hot_label = (mnist_label_t*)malloc(number_of_labels*sizeof(mnist_label_t));
    return label_array;
}

static
uint8_t read_mnist_single_label(FILE* fin)
{
    unsigned char label = 0;
    fread(&label, sizeof(label), 1, fin);
    return label;
}

mnist_label_array_t* read_mnist_label(const char* filename)
{
    FILE* fin = fopen(filename, "rb");
    CHECK_READ_FILE(fin, filename);

    int label_long = 10;

    int magic_number = get_mnist_magic_number(fin);
    int number_of_labels = get_mnist_number_of_labels(fin);

    mnist_label_array_t* label_array = create_mnist_label_array_t(number_of_labels);

    for(int i = 0; i < number_of_labels; ++i)
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
void extract_mnist_image_and_save(const char* mnist_data_dir)
{
    char test_image_pth[NC_MAX_PATH];
    sprintf(test_image_pth, "%s/t10k-images.idx3-ubyte", mnist_data_dir);

    mnist_image_array_t* image_array = read_mnist_image(test_image_pth);
    PX_LOGE("=== got %d test images\n", image_array->size);

    for(int i = 0; i < image_array->size; i++)
    {
        char save_pth[NC_MAX_PATH];
        sprintf(save_pth, "%s/testImgs/%d.bmp", mnist_data_dir, i);
        NcImage image = image_array->images_u8[i];
        cv::Size size;
        size.height = image.height;
        size.width = image.width;

        cv::Mat mat(size, CV_8UC1, image.data);
        cv::imwrite(save_pth, mat);
        //printf("save_pth is %s\n", save_pth);
    }

    destroy_mnist_image_array(image_array);
}

void destroy_mnist_image_array(mnist_image_array_t* image_array)
{
    for (int i = 0; i < image_array->size; i++)
    {
        destroy_matrix_data(&image_array->images_f32[i]);
        free(image_array->images_u8[i].data);
    }
    free(image_array->images_f32);
    free(image_array->images_u8);
    free(image_array);
}

void destroy_mnist_label_array(mnist_label_array_t* label_array)
{
    for (int i = 0; i < label_array->size; i++)
    {
        free(label_array->one_hot_label[i].data);
    }
    free(label_array->one_hot_label);
    free(label_array->label);
    free(label_array);
}