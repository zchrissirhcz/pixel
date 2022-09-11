#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include "mat.h"
#include "px_arithm.h"
#include "naive_cnn.h"

static
NcImage* nc_create_image_header(int height, int width, int channel)
{
    NcImage* im = (NcImage*)malloc(sizeof(NcImage));
    im->height = height;
    im->width = width;
    im->channel = channel;
    im->cstep = px_align_up(height * width, NC_IMAGE_ALIGN);
    im->elem_num = im->cstep * channel;
    im->data = NULL;
    return im;
}

NcImage* nc_create_image(int height, int width, int channel, unsigned char* data)
{
    NcImage* im = nc_create_image_header(height, width, channel);
    im->data = data;
    return im;
}

NcImage* nc_create_empty_image(int height, int width, int channel)
{
    NcImage* im = nc_create_image_header(height, width, channel);
    im->data = (unsigned char*)malloc(sizeof(unsigned char)*im->elem_num);
    return im;
}

static
void init_matrix(matrix_t* matrix, int height, int width)
{
    matrix->height = height;
    matrix->width = width;

    matrix->data = (float**) malloc(height*sizeof(float*));
    for (int i = 0; i < height; i++)
    {
        matrix->data[i] = (float*)malloc(width * sizeof(float));
    }
}

matrix_t* create_matrix_ptr(int height, int width)
{
    matrix_t* matrix = (matrix_t*) malloc(sizeof(matrix_t));
    init_matrix(matrix, height, width);
    return matrix;
}

void destroy_matrix_ptr(matrix_t* matrix)
{
    for (int i = 0; i < matrix->height; i++)
    {
        free(matrix->data[i]);
    }
    free(matrix->data);
    free(matrix);
}

matrix_t create_matrix(int height, int width)
{
    matrix_t matrix;
    init_matrix(&matrix, height, width);
    return matrix;
}

// 每个点变换到矩阵中心点的对称点上
// A   B         D   C
//   O      =>     O
// C   D         B   A
matrix_t* get_rotate180_matrix(matrix_t* input)
{
    const int h = input->height;
    const int w = input->width;
    matrix_t* output = create_matrix_ptr(h, w);
    for (int i = 0; i < h; i++)
    {
        for (int j = 0; j < w; j++)
        {
            output->data[i][j] = input->data[h - i - 1][w - j - 1];
        }
    }

    return output;
}

// 关于卷积和相关操作的输出选项
// 这里共有三种选择：full、same、valid，分别表示
// full指完全，操作后结果的大小为inSize+(mapSize-1)
// same指同输入相同大小
// valid指完全操作后的大小，一般为inSize-(mapSize-1)大小，其不需要将输入添0扩大。

matrix_t* correlation(matrix_t* map, matrix_t* input, int type)
{
    // 这里的互相关是在后向传播时调用，类似于将Map反转180度再卷积
    // 为了方便计算，这里先将图像扩充一圈
    // 这里的卷积要分成两拨，偶数模板同奇数模板
    int i, j, c, r;
    int halfmapsizew;
    int halfmapsizeh;

    // 模板大小为偶数
    if (map->height % 2 == 0 && map->width %2 == 0)
    {
        halfmapsizew = (map->width) / 2; // 卷积模块的半瓣大小
        halfmapsizeh = (map->height) / 2;
    }
    else
    {
        halfmapsizew = (map->width - 1) / 2; // 卷积模块的半瓣大小
        halfmapsizeh = (map->height - 1) / 2;
    }

    // 这里先默认进行full模式的操作，full模式的输出大小为inSize+(mapSize-1)
    int outSizeW = input->width + (map->width - 1); // 这里的输出扩大一部分
    int outSizeH = input->height + (map->height - 1);

    // 互相关的结果扩大了
    matrix_t* output = create_matrix_ptr(outSizeH, outSizeW);

    // 为了方便计算，将inputData扩大一圈
    px_pad_t pad = px_create_pad(map->height-1, map->height-1, map->width-1, map->width-1);
    matrix_t* expaned_input = mat_edge_expand(input, pad);

    for (j = 0; j < outSizeH; j++)
    {
        for (i = 0; i < outSizeW; i++)
        {
            for (r = 0; r < map->height; r++)
            {
                for (c = 0; c < map->width; c++)
                {
                    output->data[j][i] = output->data[j][i] + map->data[r][c] * expaned_input->data[j + r][i + c];
                }
            }
        }
    }

    destroy_matrix_ptr(expaned_input);

    NcSize2D outSize = px_create_size(outSizeH, outSizeW);
    switch(type){ // 根据不同的情况，返回不同的结果
    case full: // 完全大小的情况
        return output;
    case same:{
        matrix_t* sameres = mat_edge_shrink(output, halfmapsizew, halfmapsizeh);
        destroy_matrix_ptr(output);
        return sameres;
    }
    case valid:{
        matrix_t* validres;
        if (map->height % 2 == 0 && map->width % 2 == 0)
        {
            validres = mat_edge_shrink(output, halfmapsizew * 2 - 1, halfmapsizeh * 2 - 1);
        }
        else
        {
            validres = mat_edge_shrink(output, halfmapsizew * 2, halfmapsizeh * 2);
        }
        destroy_matrix_ptr(output);
        return validres;
    }
    default:
        return output;
    }
}

// 卷积操作
matrix_t* conv(matrix_t* map, matrix_t* input, int type)
{
    // 卷积操作可以用旋转180度的特征模板相关来求
    matrix_t* flipmap = get_rotate180_matrix(map); //旋转180度的特征模板
    matrix_t* res = correlation(flipmap, input, type);
    destroy_matrix_ptr(flipmap);
    return res;
}

// 这个是矩阵的上采样（等值内插），upc及upr是内插倍数
matrix_t* up_sample(matrix_t* input, int upc, int upr)
{
    const int out_height = input->height * upr;
    const int out_width = input->width * upc;
    matrix_t* output = create_matrix_ptr(out_height, out_width);

    for(int j = 0; j < output->height; j = j + upr)
    {
        // 宽的扩充
        for (int i = 0; i < output->width; i = i + upc)
        {
            for (int m = 0; m < upc; m++)
            {
                output->data[j][i + m] = input->data[j / upr][i / upc];
            }
        }

        // 高的扩充
        for (int n = 1; n < upr; n++)
        {
            for (int i = 0; i < output->width; i++)
            {
                output->data[j + n][i] = output->data[j][i];
            }
        }
    }
    return output;
}

// 给二维矩阵边缘扩大，增加addw大小的0值边
matrix_t* mat_edge_expand(matrix_t* input, px_pad_t pad)
{   
    // 向量边缘扩大
    const int out_height = input->height + pad.top + pad.bottom;
    const int out_width = input->width + pad.left + pad.right;
    matrix_t* output = create_matrix_ptr(out_height, out_width);

    for(int j = 0; j < out_height; j++)
    {
        for(int i = 0; i < out_width; i++)
        {
            if (j < pad.top || i < pad.left || j >= (input->height + pad.top) || i >= (input->width + pad.left))
            {
                output->data[j][i] = 0.0f;
            }
            else
            {
                output->data[j][i] = input->data[j - pad.top][i - pad.left]; // 复制原向量的数据
            }
        }
    }
    return output;
}

// 给二维矩阵边缘缩小，擦除shrinkc大小的边
matrix_t* mat_edge_shrink(matrix_t* input, int shrinkc, int shrinkr)
{
    const int out_height = input->height - 2 * shrinkr;
    const int out_width = input->width - 2 * shrinkc;
    matrix_t* output = create_matrix_ptr(out_height, out_width);

    for(int j = 0; j < input->height; j++)
    {
        for(int i = 0; i < input->width; i++)
        {
            if (j >= shrinkr && i >= shrinkc && j < (input->height - shrinkr) && i < (input->width - shrinkc))
            {
                output->data[j - shrinkr][i - shrinkc] = input->data[j][i]; // 复制原向量的数据
            }
        }
    }
    return output;
}

void save_mat_to_file(matrix_t* mat, const char* filename)
{
    FILE* fp = fopen(filename, "wb");
    CHECK_WRITE_FILE(fp, filename);

    for (int i = 0; i < mat->height; i++)
    {
        fwrite(mat->data[i], sizeof(float), mat->width, fp);
    }
    fclose(fp);
}

// 矩阵相加
void addmat(matrix_t* src1, matrix_t* src2, matrix_t* dst)
{
    if (src1->width != src2->width || src1->height != src2->height)
    {
        PX_LOGE("ERROR: Size is not same!");
        return;
    }

    for (int i = 0; i < src1->height; i++)
    {
        for (int j = 0; j < src1->width; j++)
        {
            dst->data[i][j] = src1->data[i][j] + src2->data[i][j];
        }
    }
}

// 矩阵乘以系数
void multifactor(matrix_t* res, matrix_t* mat, float factor)
{
    for (int i = 0; i < mat->height; i++)
    {
        for (int j = 0; j < mat->width; j++)
        {
            res->data[i][j] = mat->data[i][j] * factor;
        }
    }
}

// 矩阵各元素的和
float summat(matrix_t* mat)
{
    float sum = 0.0f;
    for (int i = 0; i < mat->height; i++)
    {
        for (int j = 0; j < mat->width; j++)
        {
            sum = sum + mat->data[i][j];
        }
    }
    return sum;
}
