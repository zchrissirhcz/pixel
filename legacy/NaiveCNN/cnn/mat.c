#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include "mat.h"
#include "px_arithm.h"
#include "px_assert.h"
#include "px_filesystem.h"
#include "px_image.h"
#include "blob.h"

void destroy_matrix(matrix_t* matrix)
{
    for (int i = 0; i < matrix->height; i++)
    {
        free(matrix->data[i]);
    }
    free(matrix->data);
    free(matrix);
}

// 每个点变换到矩阵中心点的对称点上
// A   B         D   C
//   O      =>     O
// C   D         B   A
matrix_t* get_rotate180_matrix(matrix_t* input)
{
    const int h = input->height;
    const int w = input->width;
    px_size_t size = px_create_size(h, w);
    matrix_t* output = create_matrix(size);
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

matrix_t* correlation_for_matrix(matrix_t* map, matrix_t* input, int type)
{
    // 这里的互相关是在后向传播时调用，类似于将Map反转180度再卷积
    // 为了方便计算，这里先将图像扩充一圈
    // 这里的卷积要分成两拨，偶数模板同奇数模板
    int i, j, c, r;
    int halfmapsizew;
    int halfmapsizeh;

    // 模板大小为偶数
    if (map->height % 2 == 0 && map->width % 2 == 0)
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
    px_size_t out_size = px_create_size(outSizeH, outSizeW);
    matrix_t* output = create_matrix(out_size);

    // 为了方便计算，将inputData扩大一圈
    px_pad_t pad = px_create_pad(map->height - 1, map->height - 1, map->width - 1, map->width - 1);
    matrix_t* expaned_input = matrix_copy_make_border(input, pad);

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

    destroy_matrix(expaned_input);

    px_size_t outSize = px_create_size(outSizeH, outSizeW);
    switch (type)
    {             // 根据不同的情况，返回不同的结果
    case NC_FULL: // 完全大小的情况
        return output;
    case NC_SAME:
    {
        px_pad_t cut_pad = px_create_pad(halfmapsizeh, halfmapsizeh, halfmapsizew, halfmapsizew);
        matrix_t* sameres = matrix_cut_make_border(output, cut_pad);
        destroy_matrix(output);
        return sameres;
    }
    case NC_VALID:
    {
        matrix_t* validres;
        if (map->height % 2 == 0 && map->width % 2 == 0)
        {
            px_pad_t cut_pad = px_create_pad(halfmapsizeh * 2 - 1, halfmapsizeh * 2 - 1, halfmapsizew * 2 - 1, halfmapsizew * 2 - 1);
            validres = matrix_cut_make_border(output, cut_pad);
        }
        else
        {
            px_pad_t cut_pad = px_create_pad(halfmapsizeh * 2, halfmapsizeh * 2, halfmapsizew * 2, halfmapsizew * 2);
            validres = matrix_cut_make_border(output, cut_pad);
        }
        destroy_matrix(output);
        return validres;
    }
    default:
        return output;
    }
}

// 卷积操作
matrix_t* conv_for_matrix(matrix_t* kernel, matrix_t* input, int type)
{
    // 卷积操作可以用旋转180度的特征模板相关来求
    matrix_t* flipped_kernel = get_rotate180_matrix(kernel); //旋转180度的特征模板
    matrix_t* res = correlation_for_matrix(flipped_kernel, input, type);
    destroy_matrix(flipped_kernel);
    return res;
}

// 这个是矩阵的上采样（等值内插），upc及upr是内插倍数
matrix_t* matrix_upsample(matrix_t* input, int width_multiplier, int height_multiplier)
{
    const int upr = height_multiplier;
    const int upc = width_multiplier;

    const int out_height = input->height * upr;
    const int out_width = input->width * upc;
    px_size_t out_size = px_create_size(out_height, out_width);
    matrix_t* output = create_matrix(out_size);

    for (int j = 0; j < output->height; j = j + upr)
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

matrix_t* matrix_copy_make_border(matrix_t* input, px_pad_t pad)
{
    // 向量边缘扩大
    const int out_height = input->height + pad.top + pad.bottom;
    const int out_width = input->width + pad.left + pad.right;
    px_size_t out_size = px_create_size(out_height, out_width);
    matrix_t* output = create_matrix(out_size);

    for (int j = 0; j < out_height; j++)
    {
        for (int i = 0; i < out_width; i++)
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
matrix_t* matrix_cut_make_border(matrix_t* input, px_pad_t pad)
{
    const int out_height = input->height - pad.top - pad.bottom;
    const int out_width = input->width - pad.left - pad.right;
    px_size_t out_size = px_create_size(out_height, out_width);
    matrix_t* output = create_matrix(out_size);

    for (int j = 0; j < input->height; j++)
    {
        for (int i = 0; i < input->width; i++)
        {
            if (j >= pad.top && i >= pad.left && j < (input->height - pad.top) && i < (input->width - pad.left))
            {
                output->data[j - pad.top][i - pad.left] = input->data[j][i]; // 复制原向量的数据
            }
        }
    }
    return output;
}

// 矩阵相加
void matrix_add(matrix_t* src1, matrix_t* src2, matrix_t* dst)
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
void matrix_multiply_lambda(matrix_t* res, matrix_t* mat, float lambda)
{
    for (int i = 0; i < mat->height; i++)
    {
        for (int j = 0; j < mat->width; j++)
        {
            res->data[i][j] = mat->data[i][j] * lambda;
        }
    }
}

// 矩阵各元素的和
float matrix_sum(matrix_t* mat)
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



matrix_t* create_matrix(px_size_t size)
{
    float** data = create_blob2d(size);
    matrix_t* matrix = (matrix_t*)malloc(sizeof(matrix_t));
    matrix->data = data;
    matrix->height = size.height;
    matrix->width = size.width;
    return matrix;
}

void save_matrix_to_file(matrix_t* matrix, FILE* fout)
{
    px_size_t size = px_create_size(matrix->height, matrix->width);
    save_blob2d_to_file(matrix->data, size, fout);
}

matrix_t* get_matrix_from_tensor(tensor_t* tensor, int i, int j)
{
    matrix_t* matrix = (matrix_t*)malloc(sizeof(matrix_t));
    matrix->height = tensor->height;
    matrix->width = tensor->width;
    matrix->data = tensor->data[i][j];
    return matrix;
}

matrix_t* get_matrix_from_cube(cube_t* cube, int i)
{
    matrix_t* matrix = (matrix_t*)malloc(sizeof(matrix_t));
    matrix->height = cube->height;
    matrix->width = cube->width;
    matrix->data = cube->data[i];
    return matrix;
}