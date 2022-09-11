#include "mnist.h"

// 英特尔处理器和其他低端机用户必须翻转头字节。
static
int reverse_int(int i)
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
    imgarr->images = (mnist_image_t*)malloc(number_of_images * sizeof(mnist_image_t));
    return imgarr;
}

static
int read_number_from_mnist_file(FILE* fin)
{
    int number = 0;
    fread((char*)&number, sizeof(number), 1, fin);
    number = reverse_int(number);
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
    n_rows = reverse_int(n_rows);
    return n_rows;
}

// 获取训练或测试图像的宽度Width
static
int get_mnist_image_width(FILE* fin)
{
    int n_cols = 0;
    fread((char*)&n_cols,sizeof(n_cols), 1, fin);
    n_cols = reverse_int(n_cols);
    return n_cols;
}

// 获取第i幅图像，保存到vec中
static
void read_mnist_single_image(const int n_rows, const int n_cols, mnist_image_array_t* imgarr, FILE* fin, int i)
{
    for(int r = 0; r < n_rows; r++)
    {
        for(int c = 0; c < n_cols; c++)
        {
            unsigned char temp = 0;
            fread((char*) &temp, sizeof(temp), 1, fin);
            imgarr->images[i].data[r][c] = (float)temp / 255.0f;
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

    for(int i = 0; i < number_of_images; i++)
    {
        imgarr->images[i] = create_matrix(n_rows, n_cols);
        read_mnist_single_image(n_rows, n_cols, imgarr, fin, i);
    }

    fclose(fin);
    return imgarr;
}

static
mnist_label_array_t* create_mnist_label_array_t(int number_of_labels)
{
    mnist_label_array_t* labarr = (mnist_label_array_t*)malloc(sizeof(mnist_label_array_t));
    labarr->size = number_of_labels;
    labarr->labels = (mnist_label_t*)malloc(number_of_labels*sizeof(mnist_label_t));
    return labarr;
}

static
void read_mnist_single_label(FILE* fin, mnist_label_array_t* labarr, int i)
{
    unsigned char temp = 0;
    fread((char*) &temp, sizeof(temp), 1, fin);
    labarr->labels[i].data[(int)temp] = 1.0f;
}

mnist_label_array_t* read_mnist_label(const char* filename)
{
    FILE* fin = fopen(filename, "rb");
    CHECK_READ_FILE(fin, filename);

    int label_long = 10;

    int magic_number = get_mnist_magic_number(fin);
    int number_of_labels = get_mnist_number_of_labels(fin);

    mnist_label_array_t* labarr = create_mnist_label_array_t(number_of_labels);

    for(int i = 0; i < number_of_labels; ++i)
    {
        labarr->labels[i].len = 10;
        labarr->labels[i].data = (float*)calloc(label_long, sizeof(float));
        read_mnist_single_label(fin, labarr, i);
    }

    fclose(fin);
    return labarr;
}
