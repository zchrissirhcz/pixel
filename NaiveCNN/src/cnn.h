#ifndef RAW_CNN_H
#define RAW_CNN_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>
#include "mat.h"

#define AvePool 0
#define MaxPool 1
#define MinPool 2


// 卷积层
typedef struct ConvLayer{
	int in_width;   //输入图像的宽
	int in_height;  //输入图像的长
	int map_size;      //特征模板的大小，模板一般都是正方形

	int in_channels;   //输入图像的数目
	int out_channels;  //输出图像的数目

	// 关于特征模板的权重分布，这里是一个四维数组
	// 其大小为inChannels*outChannels*mapSize*mapSize大小
	// 这里用四维数组，主要是为了表现全连接的形式，实际上卷积层并没有用到全连接的形式
	// 这里的例子是DeapLearningToolboox里的CNN例子，其用到就是全连接
	float**** mapData;     //存放特征模块的数据
	float**** dmapData;    //存放特征模块的数据的局部梯度

	float* biasData;   //偏置，偏置的大小，为outChannels
	bool isFullConnect; //是否为全连接
	bool* connectModel; //连接模式（默认为全连接）

	// 下面三者的大小同输出的维度相同
	float*** v; // 进入激活函数的输入值
	float*** y; // 激活函数后神经元的输出

	// 输出像素的局部梯度
	float*** d; // 网络的局部梯度,δ值  
}ConvLayer;

// 池化层 pooling
typedef struct PoolingLayer{
	int in_width;   //输入图像的宽
	int in_height;  //输入图像的长
	int map_size;      //特征模板的大小

	int in_channels;   //输入图像的数目
	int out_channels;  //输出图像的数目

	int pool_type;     //Pooling的方法
	float* biasData;   //偏置

	float*** y; // 采样函数后神经元的输出,无激活函数
	float*** d; // 网络的局部梯度,δ值
}PoolingLayer;

// 输出层 全连接的神经网络
typedef struct InnerproductLayer{
	int inputNum;   //输入数据的数目
	int outputNum;  //输出数据的数目

	float** wData; // 权重数据，为一个inputNum*outputNum大小
	float* biasData;   //偏置，大小为outputNum大小

	// 下面三者的大小同输出的维度相同
	float* v; // 进入激活函数的输入值
	float* y; // 激活函数后神经元的输出
	float* d; // 网络的局部梯度,δ值

	bool isFullConnect; //是否为全连接
}InnerproductLayer;

typedef struct cnn_network{
	int layerNum;
	ConvLayer* C1;
	PoolingLayer* S2;
	ConvLayer* C3;
	PoolingLayer* S4;
	InnerproductLayer* O5;

	float* e; // 训练误差
	float* L; // 瞬时误差能量
}CNN;

typedef struct train_opts{
	int numepochs; // 训练的迭代次数
	float alpha; // 学习速率
}CNNOpts;


//---------------------------------------------------------------------
// function declarations
//---------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif

	// 保存cnn
	void save_cnn(CNN* cnn, const char* filename);

	// 导入cnn的数据
	void load_cnn(CNN* cnn, const char* filename);

	// 初始化卷积层
	ConvLayer* init_conv_layer(int in_width, int in_height, int map_size, int in_channels, int out_channels);
	
	// 初始化采样层
	PoolingLayer* init_pooling_layer(int in_width, int in_height, int map_size, int in_channels, int out_channels, int pool_type);
	
	// 初始化输出层
	InnerproductLayer* init_innerproduct_layer(int in_num, int out_num);

	// 激活函数 input是数据，inputNum说明数据数目，bas表明偏置
	float activation_sigma(float input, float bias); // sigma激活函数

	void cnn_forward(CNN* cnn, float** input_data); // 网络的前向传播
	
	void cnn_backward(CNN* cnn, float* output_data); // 网络的后向传播
	
	void cnn_applygrads(CNN* cnn, CNNOpts opts, float** input_data);
	
	void cnn_clear(CNN* cnn); // 将数据vyd清零

	/*
		Pooling Function
		input 输入数据
		inputNum 输入数据数目
		mapSize 求平均的模块区域
	*/
	void avg_pooling(float** out, NcSize2D out_size, float** in, NcSize2D in_size, int map_size); // 求平均值

	// 单层全连接神经网络的前向传播
	void nnff(float* output, float* input, float** wdata, float* bias, NcSize2D nn_size);

	// 保存CNN网络中的相关数据
	void save_cnndata(CNN* cnn, const char* filename, float** inputdata);

	// 返回向量最大数的序号
	int vecmax_index(float* vec, int veclength);

#ifdef __cplusplus
}
#endif

#endif
