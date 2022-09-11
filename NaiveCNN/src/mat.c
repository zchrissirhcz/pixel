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

matrix_t* create_matrix(int height, int width)
{
    matrix_t* matrix = (matrix_t*) malloc(sizeof(matrix_t));
    matrix->height = height;
    matrix->width = width;

    matrix->data = (float**) malloc(height*sizeof(float*));
    for (int i = 0; i < height; i++)
    {
        matrix->data[i] = (float*)malloc(width * sizeof(float));
    }
    return matrix;
}

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
matrix_t* get_rotate180_matrix(matrix_t* input, NcSize2D matSize)
{
    int outSizeW = matSize.width;
    int outSizeH = matSize.height;
    matrix_t* output = create_matrix(outSizeH, outSizeW);
    for (int i = 0; i < outSizeH; i++)
    {
        for (int j = 0; j < outSizeW; j++)
        {
            output->data[i][j] = input->data[outSizeH - i - 1][outSizeW - j - 1];
        }
    }

    return output;
}

// 关于卷积和相关操作的输出选项
// 这里共有三种选择：full、same、valid，分别表示
// full指完全，操作后结果的大小为inSize+(mapSize-1)
// same指同输入相同大小
// valid指完全操作后的大小，一般为inSize-(mapSize-1)大小，其不需要将输入添0扩大。

matrix_t* correlation(matrix_t* map, NcSize2D mapSize, matrix_t* input, NcSize2D inSize, int type)
{
    // 这里的互相关是在后向传播时调用，类似于将Map反转180度再卷积
    // 为了方便计算，这里先将图像扩充一圈
    // 这里的卷积要分成两拨，偶数模板同奇数模板
    int i, j, c, r;
    int halfmapsizew;
    int halfmapsizeh;

    // 模板大小为偶数
    if (mapSize.height % 2 == 0 && mapSize.width %2 == 0)
    {
        halfmapsizew = (mapSize.width) / 2; // 卷积模块的半瓣大小
        halfmapsizeh = (mapSize.height) / 2;
    }
    else
    {
        halfmapsizew = (mapSize.width - 1) / 2; // 卷积模块的半瓣大小
        halfmapsizeh = (mapSize.height - 1) / 2;
    }

    // 这里先默认进行full模式的操作，full模式的输出大小为inSize+(mapSize-1)
    int outSizeW = inSize.width + (mapSize.width - 1); // 这里的输出扩大一部分
    int outSizeH = inSize.height + (mapSize.height - 1);

    // 互相关的结果扩大了
    matrix_t* output = create_matrix(outSizeH, outSizeW);

    // 为了方便计算，将inputData扩大一圈
    matrix_t* expaned_input = mat_edge_expand(input, inSize, mapSize.width-1, mapSize.height-1);

    for (j = 0; j < outSizeH; j++)
    {
        for (i = 0; i < outSizeW; i++)
        {
            for (r = 0; r < mapSize.height; r++)
            {
                for (c = 0; c < mapSize.width; c++)
                {
                    output->data[j][i] = output->data[j][i] + map->data[r][c] * expaned_input->data[j + r][i + c];
                }
            }
        }
    }

    destroy_matrix(expaned_input);

    NcSize2D outSize = px_create_size(outSizeH, outSizeW);
    switch(type){ // 根据不同的情况，返回不同的结果
    case full: // 完全大小的情况
        return output;
    case same:{
        matrix_t* sameres = mat_edge_shrink(output, outSize, halfmapsizew, halfmapsizeh);
        destroy_matrix(output);
        return sameres;
    }
    case valid:{
        matrix_t* validres;
        if (mapSize.height % 2 == 0 && mapSize.width % 2 == 0)
        {
            validres = mat_edge_shrink(output, outSize, halfmapsizew * 2 - 1, halfmapsizeh * 2 - 1);
        }
        else
        {
            validres = mat_edge_shrink(output, outSize, halfmapsizew * 2, halfmapsizeh * 2);
        }
        destroy_matrix(output);
        return validres;
    }
    default:
        return output;
    }
}

// 卷积操作
matrix_t* conv(matrix_t* map, NcSize2D mapSize, matrix_t* input, NcSize2D inSize, int type)
{
    // 卷积操作可以用旋转180度的特征模板相关来求
    matrix_t* flipmap = get_rotate180_matrix(map,mapSize); //旋转180度的特征模板
    matrix_t* res = correlation(flipmap, mapSize, input, inSize, type);
    destroy_matrix(flipmap);
    return res;
}

// 这个是矩阵的上采样（等值内插），upc及upr是内插倍数
float** up_sample(float** mat, NcSize2D matSize, int upc,int upr)
{
    int i, j, m, n;
    int c = matSize.width;
    int r = matSize.height;
    float** res=(float**)malloc((r*upr)*sizeof(float*)); // 结果的初始化
    for (i = 0; i < (r*upr); i++)
    {
        res[i] = (float*)malloc((c*upc) * sizeof(float));
    }

    for(j = 0; j < r * upr; j = j+upr)
    {
        // 宽的扩充
        for (i = 0; i < c*upc; i = i + upc)
        {
            for (m = 0; m < upc; m++)
            {
                res[j][i + m] = mat[j / upr][i / upc];
            }
        }

        // 高的扩充
        for (n = 1; n < upr; n++)
        {
            for (i = 0; i < c*upc; i++)
            {
                res[j + n][i] = res[j][i];
            }
        }
    }
    return res;
}

// 给二维矩阵边缘扩大，增加addw大小的0值边
matrix_t* mat_edge_expand(matrix_t* mat, NcSize2D matSize, int addc,int addr)
{   
    // 向量边缘扩大
    int i, j;
    int c = matSize.width;
    int r = matSize.height;
    const int out_height = r + 2 * addr;
    const int out_width = c + 2 * addc;
    matrix_t* res = create_matrix(out_height, out_width);

    for(j = 0; j < r+2*addr; j++)
    {
        for(i = 0; i < c+2*addc; i++)
        {
            if (j < addr || i < addc || j >= (r + addr) || i >= (c + addc))
            {
                res->data[j][i] = (float)0.0;
            }
            else
            {
                res->data[j][i] = mat->data[j - addr][i - addc]; // 复制原向量的数据
            }
        }
    }
    return res;
}

// 给二维矩阵边缘缩小，擦除shrinkc大小的边
matrix_t* mat_edge_shrink(matrix_t* mat, NcSize2D matSize, int shrinkc, int shrinkr)
{
    // 向量的缩小，宽缩小addw，高缩小addh
    int i, j;
    int c = matSize.width;
    int r = matSize.height;
    const int out_height = r - 2 * shrinkr;
    const int out_width = c - 2 * shrinkc;
    matrix_t* res = create_matrix(out_height, out_width);

    for(j = 0; j < r; j++)
    {
        for(i = 0; i < c; i++)
        {
            if (j >= shrinkr && i >= shrinkc && j < (r - shrinkr) && i < (c - shrinkc))
            {
                res->data[j - shrinkr][i - shrinkc] = mat->data[j][i]; // 复制原向量的数据
            }
        }
    }
    return res;
}

void savemat(float** mat, NcSize2D matSize,const char* filename)
{
    FILE* fp=fopen(filename,"wb");
    CHECK_WRITE_FILE(fp, filename);

    for (int i = 0; i < matSize.height; i++)
    {
        fwrite(mat[i], sizeof(float), matSize.width, fp);
    }
    fclose(fp);
}

// 矩阵相加
void addmat(matrix_t* res, matrix_t* mat1, NcSize2D matSize1, matrix_t* mat2, NcSize2D matSize2)
{
    int i, j;
    if (matSize1.width != matSize2.width || matSize1.height != matSize2.height)
    {
        PX_LOGE("ERROR: Size is not same!");
    }

    for (i = 0; i < matSize1.height; i++)
    {
        for (j = 0; j < matSize1.width; j++)
        {
            res->data[i][j] = mat1->data[i][j] + mat2->data[i][j];
        }
    }
}

// 矩阵乘以系数
void multifactor(matrix_t* res, matrix_t* mat, NcSize2D matSize, float factor)
{
    int i, j;
    for (i = 0; i < matSize.height; i++)
    {
        for (j = 0; j < matSize.width; j++)
        {
            res->data[i][j] = mat->data[i][j] * factor;
        }
    }
}

// 矩阵各元素的和
float summat(matrix_t* mat, NcSize2D matSize)
{
    float sum=0.0;
    int i, j;
    for (i = 0; i < matSize.height; i++)
    {
        for (j = 0; j < matSize.width; j++)
        {
            sum = sum + mat->data[i][j];
        }
    }
    return sum;
}
