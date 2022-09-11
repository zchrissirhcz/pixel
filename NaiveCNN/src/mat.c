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

// 矩阵翻转180度
float** rotate180(float** mat, NcSize2D matSize)
{
    int i, c, r;
    int outSizeW = matSize.width;
    int outSizeH = matSize.height;
    float** outputData=(float**)malloc(outSizeH*sizeof(float*));
    for (i = 0; i < outSizeH; i++)
    {
        outputData[i] = (float*)malloc(outSizeW * sizeof(float));
    }

    for (r = 0; r < outSizeH; r++)
    {
        for (c = 0; c < outSizeW; c++)
        {
            outputData[r][c] = mat[outSizeH - r - 1][outSizeW - c - 1];
        }
    }

    return outputData;
}

// 关于卷积和相关操作的输出选项
// 这里共有三种选择：full、same、valid，分别表示
// full指完全，操作后结果的大小为inSize+(mapSize-1)
// same指同输入相同大小
// valid指完全操作后的大小，一般为inSize-(mapSize-1)大小，其不需要将输入添0扩大。

float** correlation(float** map, NcSize2D mapSize,float** inputData, NcSize2D inSize,int type)// 互相关
{
    // 这里的互相关是在后向传播时调用，类似于将Map反转180度再卷积
    // 为了方便计算，这里先将图像扩充一圈
    // 这里的卷积要分成两拨，偶数模板同奇数模板
    int i, j, c, r;
    int halfmapsizew;
    int halfmapsizeh;

    // 模板大小为偶数
    if(mapSize.height % 2 == 0 && mapSize.width %2 == 0)
    {
        halfmapsizew = (mapSize.width) / 2; // 卷积模块的半瓣大小
        halfmapsizeh = (mapSize.height) / 2;
    }
    else{
        halfmapsizew = (mapSize.width - 1) / 2; // 卷积模块的半瓣大小
        halfmapsizeh = (mapSize.height - 1) / 2;
    }

    // 这里先默认进行full模式的操作，full模式的输出大小为inSize+(mapSize-1)
    int outSizeW = inSize.width + (mapSize.width - 1); // 这里的输出扩大一部分
    int outSizeH = inSize.height + (mapSize.height - 1);
    float** outputData = (float**)malloc(outSizeH*sizeof(float*)); // 互相关的结果扩大了
    for (i = 0; i < outSizeH; i++)
    {
        outputData[i] = (float*)calloc(outSizeW, sizeof(float));
    }

    // 为了方便计算，将inputData扩大一圈
    float** exInputData = mat_edge_expand(inputData, inSize, mapSize.width-1, mapSize.height-1);

    for (j = 0; j < outSizeH; j++)
    {
        for (i = 0; i < outSizeW; i++)
        {
            for (r = 0; r < mapSize.height; r++)
            {
                for (c = 0; c < mapSize.width; c++)
                {
                    outputData[j][i] = outputData[j][i] + map[r][c] * exInputData[j + r][i + c];
                }
            }
        }
    }

    for (i = 0; i < inSize.height + 2 * (mapSize.height - 1); i++)
    {
        free(exInputData[i]);
    }
    free(exInputData);

    NcSize2D outSize = px_create_size(outSizeH, outSizeW);
    switch(type){ // 根据不同的情况，返回不同的结果
    case full: // 完全大小的情况
        return outputData;
    case same:{
        float** sameres=mat_edge_shrink(outputData,outSize,halfmapsizew,halfmapsizeh);
        for (i = 0; i < outSize.height; i++)
        {
            free(outputData[i]);
        }
        free(outputData);
        return sameres;
    }
    case valid:{
        float** validres;
        if (mapSize.height % 2 == 0 && mapSize.width % 2 == 0)
        {
            validres = mat_edge_shrink(outputData, outSize, halfmapsizew * 2 - 1, halfmapsizeh * 2 - 1);
        }
        else
        {
            validres = mat_edge_shrink(outputData, outSize, halfmapsizew * 2, halfmapsizeh * 2);
        }
        for (i = 0; i < outSize.height; i++)
        {
            free(outputData[i]);
        }
        free(outputData);
        return validres;
    }
    default:
        return outputData;
    }
}

float** conv(float** map, NcSize2D mapSize,float** inputData, NcSize2D inSize,int type) // 卷积操作
{
    // 卷积操作可以用旋转180度的特征模板相关来求
    float** flipmap=rotate180(map,mapSize); //旋转180度的特征模板
    float** res=correlation(flipmap,mapSize,inputData,inSize,type);
    int i;
    for (i = 0; i < mapSize.height; i++)
    {
        free(flipmap[i]);
    }
    free(flipmap);
    return res;
}

// 这个是矩阵的上采样（等值内插），upc及upr是内插倍数
float** up_sample(float** mat, NcSize2D matSize,int upc,int upr)
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
float** mat_edge_expand(float** mat, NcSize2D matSize,int addc,int addr)
{ // 向量边缘扩大
    int i, j;
    int c = matSize.width;
    int r = matSize.height;
    float** res = (float**)malloc((r+2*addr)*sizeof(float*)); // 结果的初始化
    for (i = 0; i < (r + 2 * addr); i++)
    {
        res[i] = (float*)malloc((c + 2 * addc) * sizeof(float));
    }

    for(j = 0; j < r+2*addr; j++)
    {
        for(i = 0; i < c+2*addc; i++)
        {
            if (j < addr || i < addc || j >= (r + addr) || i >= (c + addc))
            {
                res[j][i] = (float)0.0;
            }
            else
            {
                res[j][i] = mat[j - addr][i - addc]; // 复制原向量的数据
            }
        }
    }
    return res;
}

// 给二维矩阵边缘缩小，擦除shrinkc大小的边
float** mat_edge_shrink(float** mat, NcSize2D matSize,int shrinkc,int shrinkr)
{ // 向量的缩小，宽缩小addw，高缩小addh
    int i, j;
    int c = matSize.width;
    int r = matSize.height;
    float** res=(float**)malloc((r-2*shrinkr)*sizeof(float*)); // 结果矩阵的初始化
    for (i = 0; i < (r - 2 * shrinkr); i++)
    {
        res[i] = (float*)malloc((c - 2 * shrinkc) * sizeof(float));
    }

    for(j = 0; j < r; j++)
    {
        for(i = 0; i < c; i++)
        {
            if (j >= shrinkr && i >= shrinkc && j < (r - shrinkr) && i < (c - shrinkc))
            {
                res[j - shrinkr][i - shrinkc] = mat[j][i]; // 复制原向量的数据
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

void addmat(float** res, float** mat1, NcSize2D matSize1, float** mat2, NcSize2D matSize2)// 矩阵相加
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
            res[i][j] = mat1[i][j] + mat2[i][j];
        }
    }
}

void multifactor(float** res, float** mat, NcSize2D matSize, float factor)// 矩阵乘以系数
{
    int i, j;
    for (i = 0; i < matSize.height; i++)
    {
        for (j = 0; j < matSize.width; j++)
        {
            res[i][j] = mat[i][j] * factor;
        }
    }
}

float summat(float** mat, NcSize2D matSize) // 矩阵各元素的和
{
    float sum=0.0;
    int i, j;
    for (i = 0; i < matSize.height; i++)
    {
        for (j = 0; j < matSize.width; j++)
        {
            sum = sum + mat[i][j];
        }
    }
    return sum;
}
