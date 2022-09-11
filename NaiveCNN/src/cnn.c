#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>
#include "cnn.h"
#include "mat.h"
#include "naive_config.h"


ConvLayer* init_conv_layer(int in_width,int in_height,int map_size,int in_channels,int out_channels)
{
    ConvLayer* covL=(ConvLayer*)malloc(sizeof(ConvLayer));

    covL->in_height=in_height;
    covL->in_width=in_width;
    covL->map_size=map_size;

    covL->in_channels=in_channels;
    covL->out_channels=out_channels;

    covL->isFullConnect=true; // 默认为全连接

    // 权重空间的初始化，先行再列调用，[r][c]
    int i,j,c,r;
    srand((unsigned)time(NULL));
    covL->mapData=(float****)malloc(in_channels*sizeof(float***));
    for(i=0;i<in_channels;i++){
        covL->mapData[i]=(float***)malloc(out_channels*sizeof(float**));
        for(j=0;j<out_channels;j++){
            covL->mapData[i][j]=(float**)malloc(map_size*sizeof(float*));
            for(r=0;r<map_size;r++){
                covL->mapData[i][j][r]=(float*)malloc(map_size*sizeof(float));
                for(c=0;c<map_size;c++){
                    float randnum=(((float)rand()/(float)RAND_MAX)-0.5)*2;
                    covL->mapData[i][j][r][c]=randnum*sqrt((float)6.0/(float)(map_size*map_size*(in_channels+out_channels)));
                }
            }
        }
    }
    // 权重梯度变化
    covL->dmapData=(float****)malloc(in_channels*sizeof(float***));
    for(i=0;i< in_channels;i++){
        covL->dmapData[i]=(float***)malloc(out_channels*sizeof(float**));
        for(j=0;j< out_channels;j++){
            covL->dmapData[i][j]=(float**)malloc(map_size*sizeof(float*));
            for(r=0;r<map_size;r++){
                covL->dmapData[i][j][r]=(float*)calloc(map_size,sizeof(float));
            }
        }
    }

    covL->biasData=(float*)calloc(out_channels,sizeof(float));

    int outW=in_width-map_size+1;
    int outH=in_height-map_size+1;


    covL->d=(float***)malloc(out_channels *sizeof(float**));
    covL->v=(float***)malloc(out_channels *sizeof(float**));
    covL->y=(float***)malloc(out_channels *sizeof(float**));
    for(j=0;j< out_channels;j++){
        covL->d[j]=(float**)malloc(outH*sizeof(float*));
        covL->v[j]=(float**)malloc(outH*sizeof(float*));
        covL->y[j]=(float**)malloc(outH*sizeof(float*));
        for(r=0;r<outH;r++){
            covL->d[j][r]=(float*)calloc(outW,sizeof(float));
            covL->v[j][r]=(float*)calloc(outW,sizeof(float));
            covL->y[j][r]=(float*)calloc(outW,sizeof(float));
        }
    }

    return covL;
}

PoolingLayer* init_pooling_layer(int inputWidth,int inputHeight,int mapSize,int inChannels,int outChannels,int pool_type)
{
    PoolingLayer* poolL=(PoolingLayer*)malloc(sizeof(PoolingLayer));

    poolL->in_height=inputHeight;
    poolL->in_width=inputWidth;
    poolL->map_size=mapSize;
    poolL->in_channels=inChannels;
    poolL->out_channels=outChannels;
    poolL->pool_type=pool_type;

    poolL->biasData=(float*)calloc(outChannels,sizeof(float));

    int outW=inputWidth/mapSize;
    int outH=inputHeight/mapSize;

    int j,r;
    poolL->d=(float***)malloc(outChannels*sizeof(float**));
    poolL->y=(float***)malloc(outChannels*sizeof(float**));
    for(j=0;j<outChannels;j++){
        poolL->d[j]=(float**)malloc(outH*sizeof(float*));
        poolL->y[j]=(float**)malloc(outH*sizeof(float*));
        for(r=0;r<outH;r++){
            poolL->d[j][r]=(float*)calloc(outW,sizeof(float));
            poolL->y[j][r]=(float*)calloc(outW,sizeof(float));
        }
    }

    return poolL;
}

InnerproductLayer* init_innerproduct_layer(int inputNum,int outputNum)
{
    InnerproductLayer* outL=(InnerproductLayer*)malloc(sizeof(InnerproductLayer));

    outL->inputNum=inputNum;
    outL->outputNum=outputNum;


    outL->biasData=(float*)calloc(outputNum,sizeof(float));

    outL->d=(float*)calloc(outputNum,sizeof(float));
    outL->v=(float*)calloc(outputNum,sizeof(float));
    outL->y=(float*)calloc(outputNum,sizeof(float));

    // 权重的初始化
    outL->wData=(float**)malloc(outputNum*sizeof(float*)); // 输入行，输出列
    int i,j;
    srand((unsigned)time(NULL));
    for(i=0;i<outputNum;i++){
        outL->wData[i]=(float*)malloc(inputNum*sizeof(float));
        for(j=0;j<inputNum;j++){
            float randnum=(((float)rand()/(float)RAND_MAX)-0.5)*2; // 产生一个-1到1的随机数
            outL->wData[i][j]=randnum*sqrt((float)6.0/(float)(inputNum+outputNum));
        }
    }

    outL->isFullConnect=true;

    return outL;
}

int vecmax_index(float* vec, int veclength)// 返回向量最大数的序号
{
    int i;
    float maxnum=-1.0;
    int maxIndex=0;
    for(i=0;i<veclength;i++){
        if(maxnum<vec[i]){
            maxnum=vec[i];
            maxIndex=i;
        }
    }
    return maxIndex;
}

// 保存cnn
void save_cnn(CNN* cnn, const char* filename)
{
    FILE* fp=fopen(filename,"wb");
    CHECK_WRITE_FILE(fp, filename);

    int i,j,r;
    // C1的数据
    for (i = 0; i < cnn->C1->in_channels; i++) {
        for (j = 0; j < cnn->C1->out_channels; j++) {
            for (r = 0; r < cnn->C1->map_size; r++) {
                fwrite(cnn->C1->mapData[i][j][r], sizeof(float), cnn->C1->map_size, fp);
            }
        }
    }

    fwrite(cnn->C1->biasData,sizeof(float),cnn->C1->out_channels,fp);

    // C3网络
    for (i = 0; i < cnn->C3->in_channels; i++) {
        for (j = 0; j < cnn->C3->out_channels; j++) {
            for (r = 0; r < cnn->C3->map_size; r++) {
                fwrite(cnn->C3->mapData[i][j][r], sizeof(float), cnn->C3->map_size, fp);
            }
        }
    }

    fwrite(cnn->C3->biasData,sizeof(float),cnn->C3->out_channels,fp);

    // O5输出层
    for (i = 0; i < cnn->O5->outputNum; i++) {
        fwrite(cnn->O5->wData[i], sizeof(float), cnn->O5->inputNum, fp);
    }
    fwrite(cnn->O5->biasData,sizeof(float),cnn->O5->outputNum,fp);

    fclose(fp);
}
// 导入cnn的数据
void load_cnn(CNN* cnn, const char* filename)
{
    FILE* fp=fopen(filename,"rb");
    CHECK_READ_FILE(fp, filename);

    int i,j,c,r;
    // C1的数据
    for (i = 0; i < cnn->C1->in_channels; i++) {
        for (j = 0; j < cnn->C1->out_channels; j++) {
            for (r = 0; r < cnn->C1->map_size; r++) {
                for (c = 0; c < cnn->C1->map_size; c++) {
                    float* in = (float*)malloc(sizeof(float));
                    fread(in, sizeof(float), 1, fp);
                    cnn->C1->mapData[i][j][r][c] = *in;
                }
            }
        }
    }

    for (i = 0; i < cnn->C1->out_channels; i++) {
        fread(&cnn->C1->biasData[i], sizeof(float), 1, fp);
    }

    // C3网络
    for (i = 0; i < cnn->C3->in_channels; i++)
    {
        for (j = 0; j < cnn->C3->out_channels; j++) {
            for (r = 0; r < cnn->C3->map_size; r++) {
                for (c = 0; c < cnn->C3->map_size; c++) {
                    fread(&cnn->C3->mapData[i][j][r][c], sizeof(float), 1, fp);
                }
            }
        }
    }

    for (i = 0; i < cnn->C3->out_channels; i++) {
        fread(&cnn->C3->biasData[i], sizeof(float), 1, fp);
    }

    // O5输出层
    for (i = 0; i < cnn->O5->outputNum; i++) {
        for (j = 0; j < cnn->O5->inputNum; j++) {
            fread(&cnn->O5->wData[i][j], sizeof(float), 1, fp);
        }
    }

    for (i = 0; i < cnn->O5->outputNum; i++) {
        fread(&cnn->O5->biasData[i], sizeof(float), 1, fp);
    }

    fclose(fp);
}

// 这里InputData是图像数据，inputData[r][c],r行c列，这里根各权重模板是一致的
void cnn_forward(CNN* cnn, float** inputData)
{
    // 第一层的传播
    int i,j,r,c;
    // 第一层输出数据
    NcSize2D mapSize = px_create_size(cnn->C1->map_size,cnn->C1->map_size);
    NcSize2D inSize = px_create_size(cnn->C1->in_height, cnn->C1->in_width);
    NcSize2D outSize = px_create_size(cnn->S2->in_height, cnn->S2->in_width);
    for(i = 0; i < cnn->C1->out_channels; i++)
    {
        for(j = 0; j < cnn->C1->in_channels; j++)
        {
            float** mapout = conv(cnn->C1->mapData[j][i],mapSize,inputData,inSize,valid);
            addmat(cnn->C1->v[i],cnn->C1->v[i],outSize,mapout,outSize);
            for (r = 0; r < outSize.height; r++)
            {
                free(mapout[r]);
            }
            free(mapout);
        }
        for (r = 0; r < outSize.height; r++)
        {
            for (c = 0; c < outSize.width; c++)
            {
                cnn->C1->y[i][r][c] = activation_sigma(cnn->C1->v[i][r][c], cnn->C1->biasData[i]);
            }
        }
    }

    // 第二层的输出传播S2，采样层
    outSize.width = cnn->C3->in_width;
    outSize.height = cnn->C3->in_height;
    inSize.width = cnn->S2->in_width;
    inSize.height = cnn->S2->in_height;
    for(i = 0; i < cnn->S2->out_channels; i++)
    {
        if (cnn->S2->pool_type == AvePool)
        {
            avg_pooling(cnn->S2->y[i], outSize, cnn->C1->y[i], inSize, cnn->S2->map_size);
        }
    }

    // 第三层输出传播,这里是全连接
    outSize.width = cnn->S4->in_width;
    outSize.height = cnn->S4->in_height;
    inSize.width = cnn->C3->in_width;
    inSize.height = cnn->C3->in_height;
    mapSize.width = cnn->C3->map_size;
    mapSize.height = cnn->C3->map_size;
    for(i = 0; i < (cnn->C3->out_channels); i++)
    {
        for(j = 0; j < cnn->C3->in_channels; j++)
        {
            float** mapout = conv(cnn->C3->mapData[j][i],mapSize,cnn->S2->y[j],inSize,valid);
            addmat(cnn->C3->v[i],cnn->C3->v[i],outSize,mapout,outSize);
            for (r = 0; r < outSize.height; r++)
            {
                free(mapout[r]);
            }
            free(mapout);
        }
        for (r = 0; r < outSize.height; r++)
        {
            for (c = 0; c < outSize.width; c++)
            {
                cnn->C3->y[i][r][c] = activation_sigma(cnn->C3->v[i][r][c], cnn->C3->biasData[i]);
            }
        }
    }

    // 第四层的输出传播
    inSize.width = cnn->S4->in_width;
    inSize.height = cnn->S4->in_height;
    outSize.width = inSize.width / cnn->S4->map_size;
    outSize.height = inSize.height / cnn->S4->map_size;
    for(i = 0; i < (cnn->S4->out_channels); i++)
    {
        if (cnn->S4->pool_type == AvePool)
        {
            avg_pooling(cnn->S4->y[i], outSize, cnn->C3->y[i], inSize, cnn->S4->map_size);
        }
    }

    // 输出层O5的处理
    // 首先需要将前面的多维输出展开成一维向量
    float* O5inData=(float*)malloc((cnn->O5->inputNum)*sizeof(float));
    for (i = 0; i < (cnn->S4->out_channels); i++) {
        for (r = 0; r < outSize.height; r++) {
            for (c = 0; c < outSize.width; c++) {
                O5inData[i*outSize.height*outSize.width + r * outSize.width + c] = cnn->S4->y[i][r][c];
            }
        }
    }

    NcSize2D nnSize = px_create_size(cnn->O5->outputNum, cnn->O5->inputNum);
    nnff(cnn->O5->v,O5inData,cnn->O5->wData,cnn->O5->biasData,nnSize);
    for (i = 0; i < cnn->O5->outputNum; i++)
    {
        cnn->O5->y[i] = activation_sigma(cnn->O5->v[i], cnn->O5->biasData[i]);
    }
    free(O5inData);
}

// 激活函数 input是数据，inputNum说明数据数目，bias表明偏置
float activation_sigma(float input,float bias) // sigma激活函数
{
    float temp=input+bias;
    return (float)1.0/((float)(1.0+exp(-temp)));
}

void avg_pooling(float** output, NcSize2D outputSize,float** input, NcSize2D inputSize,int mapSize)
{
    int outputW = inputSize.width / mapSize;
    int outputH = inputSize.height / mapSize;
    if (outputSize.width != outputW || outputSize.height != outputH)
    {
        PX_LOGE("ERROR: output size is wrong!!");
    }

    int i,j,m,n;
    for (i = 0; i < outputH; i++)
    {
        for (j = 0; j < outputW; j++)
        {
            float sum = 0.0;
            for (m = i * mapSize; m < i * mapSize + mapSize; m++)
            {
                for (n = j * mapSize; n < j * mapSize + mapSize; n++)
                {
                    sum = sum + input[m][n];
                }
            }
            output[i][j] = sum / (float)(mapSize*mapSize);
        }
    }
}

// dot production for two vector
float dot_product(float* vec1,float* vec2,int vecL)
{
    float m=0;
    for (int i = 0; i < vecL; i++) {
        m += vec1[i] * vec2[i];
    }
    return m;
}

void nnff(float* output,float* input,float** wdata,float* bas, NcSize2D nnSize)
{
    const int w = nnSize.width;
    const int h = nnSize.height;

    int i;
    for (i = 0; i < h; i++)
    {
        output[i] = dot_product(input, wdata[i], w) + bas[i];
    }
}

// Logic激活函数的自变量微分
float sigma_derivation(float y)
{
    return y * (1-y); // 这里y是指经过激活函数的输出值，而不是自变量
}

void cnn_backward(CNN* cnn,float* outputData)
{
    int i,j,c,r; // 将误差保存到网络中
    for (i = 0; i < cnn->O5->outputNum; i++)
    {
        cnn->e[i] = cnn->O5->y[i] - outputData[i];
    }

    /*从后向前反向计算*/
    // 输出层O5
    for (i = 0; i < cnn->O5->outputNum; i++)
    {
        cnn->O5->d[i] = cnn->e[i] * sigma_derivation(cnn->O5->y[i]);
    }

    // S4层，传递到S4层的误差
    // 这里没有激活函数
    NcSize2D outSize = px_create_size(cnn->S4->in_height/cnn->S4->map_size, cnn->S4->in_width/cnn->S4->map_size);
    for (i = 0; i < cnn->S4->out_channels; i++)
    {
        for (r = 0; r < outSize.height; r++)
        {
            for (c = 0; c < outSize.width; c++)
            {
                for (j = 0; j < cnn->O5->outputNum; j++)
                {
                    int wInt = i * outSize.width*outSize.height + r * outSize.width + c;
                    cnn->S4->d[i][r][c] = cnn->S4->d[i][r][c] + cnn->O5->d[j] * cnn->O5->wData[j][wInt];
                }
            }
        }
    }

    // C3层
    // 由S4层传递的各反向误差,这里只是在S4的梯度上扩充一倍
    int mapdata = cnn->S4->map_size;
    NcSize2D S4dSize = px_create_size(cnn->S4->in_height /cnn->S4->map_size, cnn->S4->in_width/cnn->S4->map_size);
    // 这里的Pooling是求平均，所以反向传递到下一神经元的误差梯度没有变化
    for(i = 0; i < cnn->C3->out_channels; i++)
    {
        float** C3e = up_sample(cnn->S4->d[i],S4dSize,cnn->S4->map_size,cnn->S4->map_size);
        for (r = 0; r < cnn->S4->in_height; r++)
        {
            for (c = 0; c < cnn->S4->in_width; c++)
            {
                cnn->C3->d[i][r][c] = C3e[r][c] * sigma_derivation(cnn->C3->y[i][r][c]) / (float)(cnn->S4->map_size*cnn->S4->map_size);
            }
        }
        for (r = 0; r < cnn->S4->in_height; r++)
        {
            free(C3e[r]);
        }
        free(C3e);
    }

    // S2层，S2层没有激活函数，这里只有卷积层有激活函数部分
    // 由卷积层传递给采样层的误差梯度，这里卷积层共有6*12个卷积模板
    outSize.width = cnn->C3->in_width;
    outSize.height = cnn->C3->in_height;
    NcSize2D inSize = px_create_size(cnn->S4->in_height, cnn->S4->in_width);
    NcSize2D mapSize = px_create_size(cnn->C3->map_size, cnn->C3->map_size);
    for(i = 0; i < cnn->S2->out_channels; i++)
    {
        for(j = 0; j < cnn->C3->out_channels; j++)
        {
            float** corr = correlation(cnn->C3->mapData[i][j],mapSize,cnn->C3->d[j],inSize,full);
            addmat(cnn->S2->d[i],cnn->S2->d[i],outSize,corr,outSize);
            for (r = 0; r < outSize.height; r++)
            {
                free(corr[r]);
            }
            free(corr);
        }
        /*
        for(r=0;r<cnn->C3->inputHeight;r++)
            for(c=0;c<cnn->C3->inputWidth;c++)
                // 这里本来用于采样的激活
        */
    }

    // C1层，卷积层
    mapdata = cnn->S2->map_size;
    NcSize2D S2dSize = px_create_size(cnn->S2->in_height/cnn->S2->map_size, cnn->S2->in_width/cnn->S2->map_size);
    // 这里的Pooling是求平均，所以反向传递到下一神经元的误差梯度没有变化
    for(i = 0; i < cnn->C1->out_channels; i++)
    {
        float** C1e = up_sample(cnn->S2->d[i],S2dSize,cnn->S2->map_size,cnn->S2->map_size);
        for (r = 0; r < cnn->S2->in_height; r++)
        {
            for (c = 0; c < cnn->S2->in_width; c++)
            {
                cnn->C1->d[i][r][c] = C1e[r][c] * sigma_derivation(cnn->C1->y[i][r][c]) / (float)(cnn->S2->map_size*cnn->S2->map_size);
            }
        }
        for (r = 0; r < cnn->S2->in_height; r++)
        {
            free(C1e[r]);
        }
        free(C1e);
    }
}

void cnn_applygrads(CNN* cnn,CNNOpts opts,float** inputData) // 更新权重
{
    // 这里存在权重的主要是卷积层和输出层
    // 更新这两个地方的权重就可以了
    int i,j,r,c;

    // C1层的权重更新
    NcSize2D dSize = px_create_size(cnn->S2->in_width, cnn->S2->in_height);
    NcSize2D ySize = px_create_size(cnn->C1->in_width, cnn->C1->in_height);
    NcSize2D mapSize = px_create_size(cnn->C1->map_size, cnn->C1->map_size);

    for(i = 0; i < cnn->C1->out_channels; i++)
    {
        for(j = 0; j < cnn->C1->in_channels; j++)
        {
            float** flipinputData = rotate180(inputData,ySize);
            float** C1dk = conv(cnn->C1->d[i],dSize,flipinputData,ySize,valid);
            multifactor(C1dk,C1dk,mapSize,-1*opts.alpha);
            addmat(cnn->C1->mapData[j][i],cnn->C1->mapData[j][i],mapSize,C1dk,mapSize);
            for (r = 0; r < (dSize.height - (ySize.height - 1)); r++)
            {
                free(C1dk[r]);
            }
            free(C1dk);
            for (r = 0; r < ySize.height; r++)
            {
                free(flipinputData[r]);
            }
            free(flipinputData);
        }
        cnn->C1->biasData[i] = cnn->C1->biasData[i]-opts.alpha*summat(cnn->C1->d[i],dSize);
    }

    // C3层的权重更新
    dSize.width = cnn->S4->in_width;
    dSize.height = cnn->S4->in_height;
    ySize.width = cnn->C3->in_width;
    ySize.height = cnn->C3->in_height;
    mapSize.width = cnn->C3->map_size;
    mapSize.height = cnn->C3->map_size;
    for(i = 0; i < cnn->C3->out_channels; i++)
    {
        for(j = 0; j<cnn->C3->in_channels; j++)
        {
            float** flipinputData=rotate180(cnn->S2->y[j],ySize);
            float** C3dk= conv(cnn->C3->d[i],dSize,flipinputData,ySize,valid);
            multifactor(C3dk,C3dk,mapSize,-1.0*opts.alpha);
            addmat(cnn->C3->mapData[j][i],cnn->C3->mapData[j][i],mapSize,C3dk,mapSize);
            for (r = 0; r < (dSize.height - (ySize.height - 1)); r++)
            {
                free(C3dk[r]);
            }
            free(C3dk);
            for (r = 0; r < ySize.height; r++)
            {
                free(flipinputData[r]);
            }
            free(flipinputData);
        }
        cnn->C3->biasData[i]=cnn->C3->biasData[i]-opts.alpha*summat(cnn->C3->d[i],dSize);
    }

    // 输出层
    // 首先需要将前面的多维输出展开成一维向量
    float* O5inData = (float*)malloc((cnn->O5->inputNum)*sizeof(float));
    NcSize2D outSize = px_create_size(cnn->S4->in_height/cnn->S4->map_size, cnn->S4->in_width/cnn->S4->map_size);
    for (i = 0; i < (cnn->S4->out_channels); i++)
    {
        for (r = 0; r < outSize.height; r++)
        {
            for (c = 0; c < outSize.width; c++)
            {
                O5inData[i*outSize.height*outSize.width + r * outSize.width + c] = cnn->S4->y[i][r][c];
            }
        }
    }

    for(j = 0; j < cnn->O5->outputNum; j++)
    {
        for (i = 0; i < cnn->O5->inputNum; i++)
        {
            cnn->O5->wData[j][i] = cnn->O5->wData[j][i] - opts.alpha*cnn->O5->d[j] * O5inData[i];
        }
        cnn->O5->biasData[j]=cnn->O5->biasData[j]-opts.alpha*cnn->O5->d[j];
    }
    free(O5inData);
}

void cnn_clear(CNN* cnn)
{
    // 将神经元的部分数据清除
    int j,c,r;
    // C1网络
    for(j = 0; j < cnn->C1->out_channels; j++)
    {
        for(r = 0; r < cnn->S2->in_height; r++)
        {
            for(c = 0; c < cnn->S2->in_width; c++)
            {
                cnn->C1->d[j][r][c] = (float)0.0;
                cnn->C1->v[j][r][c] = (float)0.0;
                cnn->C1->y[j][r][c] = (float)0.0;
            }
        }
    }
    // S2网络
    for(j = 0; j < cnn->S2->out_channels; j++)
    {
        for(r = 0; r < cnn->C3->in_height; r++)
        {
            for(c = 0; c < cnn->C3->in_width; c++)
            {
                cnn->S2->d[j][r][c]=0;
                cnn->S2->y[j][r][c] = 0;
            }
        }
    }
    // C3网络
    for(j = 0; j < cnn->C3->out_channels; j++)
    {
        for(r = 0; r < cnn->S4->in_height; r++)
        {
            for(c = 0; c < cnn->S4->in_width; c++)
            {
                cnn->C3->d[j][r][c] = (float)0.0;
                cnn->C3->v[j][r][c] = (float)0.0;
                cnn->C3->y[j][r][c] = (float)0.0;
            }
        }
    }
    // S4网络
    for(j = 0; j < cnn->S4->out_channels; j++)
    {
        for(r = 0; r < cnn->S4->in_height/cnn->S4->map_size; r++)
        {
            for(c = 0; c < cnn->S4->in_width/cnn->S4->map_size; c++)
            {
                cnn->S4->d[j][r][c] = (float)0.0;
                cnn->S4->y[j][r][c] = (float)0.0;
            }
        }
    }
    // O5输出
    for(j = 0; j < cnn->O5->outputNum; j++)
    {
        cnn->O5->d[j] = (float)0.0;
        cnn->O5->v[j] = (float)0.0;
        cnn->O5->y[j] = (float)0.0;
    }
}

// 这是用于测试的函数
void save_cnndata(CNN* cnn,const char* filename,float** inputdata) // 保存CNN网络中的相关数据
{
    FILE* fp=fopen(filename,"wb");
    CHECK_WRITE_FILE(fp, filename);

    // C1的数据
    int i,j,r;
    // C1网络
    for (i = 0; i < cnn->C1->in_height; i++)
    {
        fwrite(inputdata[i], sizeof(float), cnn->C1->in_width, fp);
    }
    for (i = 0; i < cnn->C1->in_channels; i++)
    {
        for (j = 0; j < cnn->C1->out_channels; j++)
        {
            for (r = 0; r < cnn->C1->map_size; r++)
            {
                fwrite(cnn->C1->mapData[i][j][r], sizeof(float), cnn->C1->map_size, fp);
            }
        }
    }

    fwrite(cnn->C1->biasData,sizeof(float),cnn->C1->out_channels,fp);

    for(j = 0; j < cnn->C1->out_channels; j++)
    {
        for(r = 0; r < cnn->S2->in_height; r++)
        {
            fwrite(cnn->C1->v[j][r],sizeof(float),cnn->S2->in_width,fp);
        }
        for(r = 0; r < cnn->S2->in_height; r++)
        {
            fwrite(cnn->C1->d[j][r],sizeof(float),cnn->S2->in_width,fp);
        }
        for(r = 0; r < cnn->S2->in_height; r++)
        {
            fwrite(cnn->C1->y[j][r],sizeof(float),cnn->S2->in_width,fp);
        }
    }

    // S2网络
    for(j = 0; j < cnn->S2->out_channels; j++)
    {
        for(r = 0; r < cnn->C3->in_height; r++)
        {
            fwrite(cnn->S2->d[j][r],sizeof(float),cnn->C3->in_width,fp);
        }
        for(r = 0; r < cnn->C3->in_height; r++)
        {
            fwrite(cnn->S2->y[j][r],sizeof(float),cnn->C3->in_width,fp);
        }
    }
    // C3网络
    for (i = 0; i < cnn->C3->in_channels; i++)
    {
        for (j = 0; j < cnn->C3->out_channels; j++)
        {
            for (r = 0; r < cnn->C3->map_size; r++)
            {
                fwrite(cnn->C3->mapData[i][j][r], sizeof(float), cnn->C3->map_size, fp);
            }
        }
    }

    fwrite(cnn->C3->biasData,sizeof(float),cnn->C3->out_channels,fp);

    for(j = 0; j < cnn->C3->out_channels; j++)
    {
        for(r = 0; r <cnn->S4->in_height; r++)
        {
            fwrite(cnn->C3->v[j][r],sizeof(float),cnn->S4->in_width,fp);
        }
        for(r = 0; r < cnn->S4->in_height; r++)
        {
            fwrite(cnn->C3->d[j][r],sizeof(float),cnn->S4->in_width,fp);
        }
        for(r = 0; r < cnn->S4->in_height; r++)
        {
            fwrite(cnn->C3->y[j][r],sizeof(float),cnn->S4->in_width,fp);
        }
    }

    // S4网络
    for(j = 0; j <cnn->S4->out_channels; j++)
    {
        for(r = 0; r < cnn->S4->in_height / cnn->S4->map_size; r++)
        {
            fwrite(cnn->S4->d[j][r],sizeof(float),cnn->S4->in_width /cnn->S4->map_size,fp);
        }
        for(r = 0; r < cnn->S4->in_height / cnn->S4->map_size; r++)
        {
            fwrite(cnn->S4->y[j][r],sizeof(float),cnn->S4->in_width /cnn->S4->map_size,fp);
        }
    }

    // O5输出层
    for (i = 0; i < cnn->O5->outputNum; i++)
    {
        fwrite(cnn->O5->wData[i], sizeof(float), cnn->O5->inputNum, fp);
    }
    fwrite(cnn->O5->biasData,sizeof(float),cnn->O5->outputNum,fp);
    fwrite(cnn->O5->v,sizeof(float),cnn->O5->outputNum,fp);
    fwrite(cnn->O5->d,sizeof(float),cnn->O5->outputNum,fp);
    fwrite(cnn->O5->y,sizeof(float),cnn->O5->outputNum,fp);

    fclose(fp);
}
