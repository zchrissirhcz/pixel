#include "lenet.h"

#include <opencv2/imgcodecs.hpp>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <assert.h>

#include "px_mnist.h"
#include "px_filesystem.h"
#include "naive_cnn.h"

#include <opencv2/opencv.hpp>
#include "load_prefix.h"

matrix_t create_normalized_f32_matrix_from_u8_image(px_mnist_image_array_t* inputData, int n)
{
    const int height = inputData->images[n].height;
    const int width = inputData->images[n].width;
    matrix_t normalized_image = create_matrix(height, width);
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            normalized_image.data[i][j] = inputData->images->data[i * width + j] / 255.0f;
        }
    }
    return normalized_image;
}

void train_lenet_on_mnist(Lenet* net, px_mnist_image_array_t* inputData, px_mnist_label_array_t* outputData, LenetTrainOpts opts, int trainNum, FILE* fout)
{
    // 学习训练误差曲线
    net->L = (float*)malloc(trainNum * sizeof(float));
    for (int epoch_index = 0; epoch_index < opts.num_epochs; epoch_index++)
    {
        for (int n = 0; n < trainNum; n++)
        {
            matrix_t input = create_normalized_f32_matrix_from_u8_image(inputData, n);

            forward_lenet(net, &input);                             // 前向传播，这里主要计算各
            backward_lenet(net, outputData->one_hot_label[n].data); // 后向传播，这里主要计算各神经元的误差梯度

            //char* filedir="E:\\Code\\debug\\CNNData\\";
            //const char* filename=combine_strings(filedir,combine_strings(intTochar(n),".cnn"));
            //save_nndata(cnn,filename,inputData->ImgPtr[n].ImgData);
            apply_grads_on_lenet(net, opts, &input); // 更新权重

            clear_lenet(net);
            // 计算并保存误差能量
            float l = 0.0;
            int i;
            for (i = 0; i < net->O5->outputNum; i++)
            {
                l = l + net->err[i] * net->err[i];
            }
            if (n == 0)
            {
                net->L[n] = l / 2.0f;
            }
            else
            {
                net->L[n] = net->L[n - 1] * 0.99f + 0.01f * l / 2.0f;
            }
            fprintf(stdout, "-- [train] sample: %5d/%d, epoch: %d/%d, loss: %.4f\n",
                    n, trainNum,
                    epoch_index, opts.num_epochs,
                    net->L[n]);

            destroy_matrix_data(&input);
        }
    }
}

// do inference
float test_lenet_on_mnist(Lenet* net, px_mnist_image_array_t* inputData, px_mnist_label_array_t* outputData, int testNum)
{
    int n = 0;
    int incorrectnum = 0;
    for (n = 0; n < testNum; n++)
    {
        matrix_t cnn_input = create_normalized_f32_matrix_from_u8_image(inputData, n);
        forward_lenet(net, &cnn_input);
        if (argmax(net->O5->y, net->O5->outputNum) != argmax(outputData->one_hot_label[n].data, net->O5->outputNum))
        {
            incorrectnum++;
        }
        clear_lenet(net);
        destroy_matrix_data(&cnn_input);
    }
    return (float)incorrectnum / (float)testNum;
}

int test_mnist_train_test()
{
    char mnist_data_dir[256];
    sprintf(mnist_data_dir, "%s/mnist", project_dir);
    px_mnist_data_t* mnist_data = px_read_mnist_data(mnist_data_dir);

    NcSize2D inputSize = px_create_size(mnist_data->test_images->images[0].height, mnist_data->test_images->images[0].width);
    int outSize = mnist_data->test_labels->one_hot_label[0].len;

    // CNN structure init
    Lenet* net = (Lenet*)malloc(sizeof(Lenet));
    setup_lenet(net, inputSize, outSize);

    // CNN training
#if 0
    LenetTrainOpts opts;
    opts.num_epochs = 1;
    opts.alpha = 1.0;
    //int trainNum = 10;
    int trainNum = 1000;
    //int trainNum=55000;
    char train_log_pth[NC_MAX_PATH];
    sprintf(train_log_pth, "%s/debug/train-log.txt", project_dir);
    FILE* fout = fopen(train_log_pth, "w"); // log file
    CHECK_WRITE_FILE(fout, train_log_pth);

    train_lenet_on_mnist(cnn, mnist_data->train_images, mnist_data->train_labels, opts, trainNum, fout);

    fclose(fout);

    printf("train finished!!\n");
    char train_model_pth[NC_MAX_PATH];
    sprintf(train_model_pth, "%s/model/mnist-train.cnn", project_dir);
    save_cnn(cnn, train_model_pth);
    // save training error
    char train_err_pth[NC_MAX_PATH];
    sprintf(train_err_pth, "%s/debug/cnnL.ma", project_dir);
    FILE* fp = fopen(train_err_pth, "wb");
    CHECK_WRITE_FILE(fp, train_err_pth);
    fwrite(cnn->L, sizeof(float), trainNum, fp);
    fclose(fp);
#endif

    // CNN test
    printf("--- mnist test start\n");
    char test_model_pth[NC_MAX_PATH];
    sprintf(test_model_pth, "%s/model/mnist-train.cnn", project_dir);
    //sprintf(test_model_pth, "%s/model/mnist.cnn", project_dir);
    load_lenet(net, test_model_pth);
    //int testNum=10000; // too slow
    int testNum = 100;
    float incorrectRatio = 0.0;
    incorrectRatio = test_lenet_on_mnist(net, mnist_data->test_images, mnist_data->test_labels, testNum);
    float accuracy = 1 - incorrectRatio;
    printf("--- accuracy: %f\n", accuracy);
    printf("test finished!!\n");

    free(net);
    px_destroy_mnist_data(mnist_data);

    return 0;
}

void setup_lenet(Lenet* net, NcSize2D inputSize, int outputSize)
{
    net->layerNum = 5;

    NcSize2D inSize;
    int mapSize = 5;
    inSize.width = inputSize.width;
    inSize.height = inputSize.height;
    net->C1 = init_conv_layer(inSize.width, inSize.height, 5, 1, 6);

    inSize.width = inSize.width - mapSize + 1;
    inSize.height = inSize.height - mapSize + 1;
    net->S2 = init_pooling_layer(inSize.width, inSize.height, 2, 6, 6, NC_AvePool);

    inSize.width = inSize.width / 2;
    inSize.height = inSize.height / 2;
    net->C3 = init_conv_layer(inSize.width, inSize.height, 5, 6, 12);

    inSize.width = inSize.width - mapSize + 1;
    inSize.height = inSize.height - mapSize + 1;
    net->S4 = init_pooling_layer(inSize.width, inSize.height, 2, 12, 12, NC_AvePool);

    inSize.width = inSize.width / 2;
    inSize.height = inSize.height / 2;
    net->O5 = init_innerproduct_layer(inSize.width * inSize.height * 12, outputSize);

    net->err = (float*)calloc(net->O5->outputNum, sizeof(float));
}

//
//void mmm_conv(){
//    // 第一层的传播
//    int i,j,r,c;
//    // 第一层输出数据
//    NcSize2D mapSize={cnn->C1->map_size,cnn->C1->map_size};
//    NcSize2D inSize={cnn->C1->in_width,cnn->C1->in_height};
//    NcSize2D outSize={cnn->S2->in_width,cnn->S2->in_height };
//    for(i=0;i<(cnn->C1->out_channels);i++){
//        for(j=0;j<(cnn->C1->in_channels);j++){
//            float** mapout=conv(cnn->C1->mapData[j][i],mapSize,inputData,inSize,valid);
//            addmat(cnn->C1->v[i],cnn->C1->v[i],outSize,mapout,outSize);
//            for (r = 0; r < outSize.h; r++) {
//                free(mapout[r]);
//            }
//            free(mapout);
//        }
//        for (r = 0; r < outSize.h; r++) {
//            for (c = 0; c < outSize.w; c++) {
//                cnn->C1->y[i][r][c] = activation_sigma(cnn->C1->v[i][r][c], cnn->C1->biasData[i]);
//            }
//        }
//    }
//}

void load_lenet(Lenet* net, const char* filename)
{
    FILE* fp = fopen(filename, "rb");
    CHECK_READ_FILE(fp, filename);

    int i, j, c, r;
    // C1的数据
    for (i = 0; i < net->C1->in_channels; i++)
    {
        for (j = 0; j < net->C1->out_channels; j++)
        {
            for (r = 0; r < net->C1->map_size; r++)
            {
                for (c = 0; c < net->C1->map_size; c++)
                {
                    float* in = (float*)malloc(sizeof(float));
                    fread(in, sizeof(float), 1, fp);
                    net->C1->mapData[i][j][r][c] = *in;
                }
            }
        }
    }

    for (i = 0; i < net->C1->out_channels; i++)
    {
        fread(&net->C1->biasData[i], sizeof(float), 1, fp);
    }

    // C3网络
    for (i = 0; i < net->C3->in_channels; i++)
    {
        for (j = 0; j < net->C3->out_channels; j++)
        {
            for (r = 0; r < net->C3->map_size; r++)
            {
                for (c = 0; c < net->C3->map_size; c++)
                {
                    fread(&net->C3->mapData[i][j][r][c], sizeof(float), 1, fp);
                }
            }
        }
    }

    for (i = 0; i < net->C3->out_channels; i++)
    {
        fread(&net->C3->biasData[i], sizeof(float), 1, fp);
    }

    // O5输出层
    for (i = 0; i < net->O5->outputNum; i++)
    {
        for (j = 0; j < net->O5->inputNum; j++)
        {
            fread(&net->O5->wData[i][j], sizeof(float), 1, fp);
        }
    }

    for (i = 0; i < net->O5->outputNum; i++)
    {
        fread(&net->O5->biasData[i], sizeof(float), 1, fp);
    }

    fclose(fp);
}

void save_lenet(Lenet* cnn, const char* filename)
{
    FILE* fp = fopen(filename, "wb");
    CHECK_WRITE_FILE(fp, filename);

    int i, j, r;
    // C1的数据
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

    fwrite(cnn->C1->biasData, sizeof(float), cnn->C1->out_channels, fp);

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

    fwrite(cnn->C3->biasData, sizeof(float), cnn->C3->out_channels, fp);

    // O5输出层
    for (i = 0; i < cnn->O5->outputNum; i++)
    {
        fwrite(cnn->O5->wData[i], sizeof(float), cnn->O5->inputNum, fp);
    }
    fwrite(cnn->O5->biasData, sizeof(float), cnn->O5->outputNum, fp);

    fclose(fp);
}

// 这里 input 是图像数据， input[r][c],r行c列，这里根各权重模板是一致的
void forward_lenet(Lenet* net, matrix_t* input)
{
    // 第一层的传播
    int i, j, r, c;
    // 第一层输出数据
    NcSize2D mapSize = px_create_size(net->C1->map_size, net->C1->map_size);
    NcSize2D inSize = px_create_size(net->C1->in_height, net->C1->in_width);
    NcSize2D outSize = px_create_size(net->S2->in_height, net->S2->in_width);
    for (i = 0; i < net->C1->out_channels; i++)
    {
        for (j = 0; j < net->C1->in_channels; j++)
        {
            matrix_t map;
            map.height = mapSize.height;
            map.width = mapSize.width;
            map.data = net->C1->mapData[j][i];
            matrix_t* mapout = conv(&map, input, NC_VALID);

            matrix_t res;
            res.height = mapSize.height;
            res.width = mapSize.width;
            res.data = net->C1->v[i];

            matrix_t mat1;
            mat1.height = outSize.height;
            mat1.width = outSize.width;
            mat1.data = net->C1->v[i];

            addmat(&mat1, mapout, &res);
            destroy_matrix_ptr(mapout);
        }
        for (r = 0; r < outSize.height; r++)
        {
            for (c = 0; c < outSize.width; c++)
            {
                net->C1->y[i][r][c] = activation_sigma(net->C1->v[i][r][c], net->C1->biasData[i]);
            }
        }
    }

    // 第二层的输出传播S2，采样层
    outSize.width = net->C3->in_width;
    outSize.height = net->C3->in_height;
    inSize.width = net->S2->in_width;
    inSize.height = net->S2->in_height;
    for (i = 0; i < net->S2->out_channels; i++)
    {
        if (net->S2->pool_type == NC_AvePool)
        {
            matrix_t pool_input;
            pool_input.height = inSize.height;
            pool_input.width = inSize.width;
            pool_input.data = net->C1->y[i];

            matrix_t pool_output;
            pool_output.height = outSize.height;
            pool_output.width = outSize.width;
            pool_output.data = net->S2->y[i];

            px_size_t kernel_size = px_create_size(net->S2->map_size, net->S2->map_size);
            forward_avg_pooling(&pool_input, &pool_output, kernel_size);
        }
    }

    // 第三层输出传播,这里是全连接
    outSize.width = net->S4->in_width;
    outSize.height = net->S4->in_height;
    inSize.width = net->C3->in_width;
    inSize.height = net->C3->in_height;
    mapSize.width = net->C3->map_size;
    mapSize.height = net->C3->map_size;
    for (i = 0; i < (net->C3->out_channels); i++)
    {
        for (j = 0; j < net->C3->in_channels; j++)
        {
            matrix_t map;
            map.height = mapSize.height;
            map.width = mapSize.width;
            map.data = net->C3->mapData[j][i];

            matrix_t tmp_input;
            tmp_input.height = inSize.height;
            tmp_input.width = inSize.height;
            tmp_input.data = net->S2->y[j];
            matrix_t* mapout = conv(&map, &tmp_input, NC_VALID);

            matrix_t res;
            res.height = mapSize.height;
            res.width = mapSize.width;
            res.data = net->C3->v[i];

            matrix_t mat1;
            mat1.height = outSize.height;
            mat1.width = outSize.width;
            mat1.data = net->C3->v[i];

            addmat(&mat1, mapout, &res);
            destroy_matrix_ptr(mapout);
        }
        for (r = 0; r < outSize.height; r++)
        {
            for (c = 0; c < outSize.width; c++)
            {
                net->C3->y[i][r][c] = activation_sigma(net->C3->v[i][r][c], net->C3->biasData[i]);
            }
        }
    }

    // 第四层的输出传播
    inSize.width = net->S4->in_width;
    inSize.height = net->S4->in_height;
    outSize.width = inSize.width / net->S4->map_size;
    outSize.height = inSize.height / net->S4->map_size;
    for (i = 0; i < (net->S4->out_channels); i++)
    {
        if (net->S4->pool_type == NC_AvePool)
        {
            matrix_t pool_input;
            pool_input.height = inSize.height;
            pool_input.width = inSize.width;
            pool_input.data = net->C3->y[i];

            matrix_t pool_output;
            pool_output.height = outSize.height;
            pool_output.width = outSize.width;
            pool_output.data = net->S4->y[i];

            px_size_t kernel_size = px_create_size(net->S4->map_size, net->S4->map_size);
            forward_avg_pooling(&pool_input, &pool_output, kernel_size);
        }
    }

    // 输出层O5的处理
    // 首先需要将前面的多维输出展开成一维向量
    float* O5inData = (float*)malloc((net->O5->inputNum) * sizeof(float));
    for (i = 0; i < (net->S4->out_channels); i++)
    {
        for (r = 0; r < outSize.height; r++)
        {
            for (c = 0; c < outSize.width; c++)
            {
                O5inData[i * outSize.height * outSize.width + r * outSize.width + c] = net->S4->y[i][r][c];
            }
        }
    }

    NcSize2D nnSize = px_create_size(net->O5->outputNum, net->O5->inputNum);
    nnff(net->O5->v, O5inData, net->O5->wData, net->O5->biasData, nnSize);
    for (i = 0; i < net->O5->outputNum; i++)
    {
        net->O5->y[i] = activation_sigma(net->O5->v[i], net->O5->biasData[i]);
    }
    free(O5inData);
}

void backward_lenet(Lenet* net, float* outputData)
{
    int i, j, c, r; // 将误差保存到网络中
    for (i = 0; i < net->O5->outputNum; i++)
    {
        net->err[i] = net->O5->y[i] - outputData[i];
    }

    /*从后向前反向计算*/
    // 输出层O5
    for (i = 0; i < net->O5->outputNum; i++)
    {
        net->O5->d[i] = net->err[i] * sigma_derivation(net->O5->y[i]);
    }

    // S4层，传递到S4层的误差
    // 这里没有激活函数
    NcSize2D outSize = px_create_size(net->S4->in_height / net->S4->map_size, net->S4->in_width / net->S4->map_size);
    for (i = 0; i < net->S4->out_channels; i++)
    {
        for (r = 0; r < outSize.height; r++)
        {
            for (c = 0; c < outSize.width; c++)
            {
                for (j = 0; j < net->O5->outputNum; j++)
                {
                    int wInt = i * outSize.width * outSize.height + r * outSize.width + c;
                    net->S4->d[i][r][c] = net->S4->d[i][r][c] + net->O5->d[j] * net->O5->wData[j][wInt];
                }
            }
        }
    }

    // C3层
    // 由S4层传递的各反向误差,这里只是在S4的梯度上扩充一倍
    int mapdata = net->S4->map_size;
    NcSize2D S4dSize = px_create_size(net->S4->in_height / net->S4->map_size, net->S4->in_width / net->S4->map_size);
    // 这里的Pooling是求平均，所以反向传递到下一神经元的误差梯度没有变化
    for (i = 0; i < net->C3->out_channels; i++)
    {
        matrix_t input;
        input.height = S4dSize.height;
        input.width = S4dSize.width;
        input.data = net->S4->d[i];
        matrix_t* C3e = matrix_upsample(&input, net->S4->map_size, net->S4->map_size);
        for (r = 0; r < net->S4->in_height; r++)
        {
            for (c = 0; c < net->S4->in_width; c++)
            {
                net->C3->d[i][r][c] = C3e->data[r][c] * sigma_derivation(net->C3->y[i][r][c]) / (float)(net->S4->map_size * net->S4->map_size);
            }
        }
        destroy_matrix_ptr(C3e);
    }

    // S2层，S2层没有激活函数，这里只有卷积层有激活函数部分
    // 由卷积层传递给采样层的误差梯度，这里卷积层共有6*12个卷积模板
    outSize.width = net->C3->in_width;
    outSize.height = net->C3->in_height;
    NcSize2D inSize = px_create_size(net->S4->in_height, net->S4->in_width);
    NcSize2D mapSize = px_create_size(net->C3->map_size, net->C3->map_size);
    for (i = 0; i < net->S2->out_channels; i++)
    {
        for (j = 0; j < net->C3->out_channels; j++)
        {
            matrix_t map;
            map.height = mapSize.height;
            map.width = mapSize.width;
            map.data = net->C3->mapData[i][j];

            matrix_t input;
            input.height = inSize.height;
            input.width = inSize.width;
            input.data = net->C3->d[j];

            matrix_t* corr = correlation(&map, &input, NC_FULL);

            matrix_t res;
            res.height = outSize.height;
            res.width = outSize.width;
            res.data = net->S2->d[i];

            matrix_t mat1;
            mat1.height = outSize.height;
            mat1.width = outSize.width;
            mat1.data = net->S2->d[i];

            addmat(&mat1, corr, &res);
            destroy_matrix_ptr(corr);
        }
        /*
        for(r=0;r<cnn->C3->inputHeight;r++)
            for(c=0;c<cnn->C3->inputWidth;c++)
                // 这里本来用于采样的激活
        */
    }

    // C1层，卷积层
    mapdata = net->S2->map_size;
    NcSize2D S2dSize = px_create_size(net->S2->in_height / net->S2->map_size, net->S2->in_width / net->S2->map_size);
    // 这里的Pooling是求平均，所以反向传递到下一神经元的误差梯度没有变化
    for (i = 0; i < net->C1->out_channels; i++)
    {
        matrix_t input;
        input.height = S2dSize.height;
        input.width = S2dSize.width;
        input.data = net->S2->d[i];
        matrix_t* C1e = matrix_upsample(&input, net->S2->map_size, net->S2->map_size);
        for (r = 0; r < net->S2->in_height; r++)
        {
            for (c = 0; c < net->S2->in_width; c++)
            {
                net->C1->d[i][r][c] = C1e->data[r][c] * sigma_derivation(net->C1->y[i][r][c]) / (float)(net->S2->map_size * net->S2->map_size);
            }
        }
        destroy_matrix_ptr(C1e);
    }
}

void apply_grads_on_lenet(Lenet* lenet, LenetTrainOpts opts, matrix_t* input) // 更新权重
{
    // 这里存在权重的主要是卷积层和输出层
    // 更新这两个地方的权重就可以了
    int i, j, r, c;

    // C1层的权重更新
    NcSize2D dSize = px_create_size(lenet->S2->in_width, lenet->S2->in_height);
    NcSize2D ySize = px_create_size(lenet->C1->in_width, lenet->C1->in_height);
    NcSize2D mapSize = px_create_size(lenet->C1->map_size, lenet->C1->map_size);

    for (i = 0; i < lenet->C1->out_channels; i++)
    {
        for (j = 0; j < lenet->C1->in_channels; j++)
        {
            matrix_t* flipinput = get_rotate180_matrix(input);
            matrix_t map;
            map.height = dSize.height;
            map.width = dSize.width;
            map.data = lenet->C1->d[i];
            matrix_t* C1dk = conv(&map, flipinput, NC_VALID);
            multifactor(C1dk, C1dk, -1 * opts.lr);

            matrix_t res;
            res.height = mapSize.height;
            res.width = mapSize.width;
            res.data = lenet->C1->mapData[j][i];

            matrix_t mat1;
            mat1.height = mapSize.height;
            mat1.width = mapSize.width;
            mat1.data = lenet->C1->mapData[j][i];

            addmat(&mat1, C1dk, &res);
            destroy_matrix_ptr(C1dk);
            destroy_matrix_ptr(flipinput);
        }
        matrix_t mat2;
        mat2.height = dSize.height;
        mat2.width = dSize.width;
        mat2.data = lenet->C1->d[i];
        lenet->C1->biasData[i] = lenet->C1->biasData[i] - opts.lr * summat(&mat2);
    }

    // C3层的权重更新
    dSize.width = lenet->S4->in_width;
    dSize.height = lenet->S4->in_height;
    ySize.width = lenet->C3->in_width;
    ySize.height = lenet->C3->in_height;
    mapSize.width = lenet->C3->map_size;
    mapSize.height = lenet->C3->map_size;
    for (i = 0; i < lenet->C3->out_channels; i++)
    {
        for (j = 0; j < lenet->C3->in_channels; j++)
        {
            matrix_t tmp_input;
            tmp_input.height = ySize.height;
            tmp_input.width = ySize.width;
            tmp_input.data = lenet->S2->y[j];
            matrix_t* flipinput = get_rotate180_matrix(&tmp_input);

            matrix_t map;
            map.height = dSize.height;
            map.width = dSize.width;
            map.data = lenet->C3->d[i];
            matrix_t* C3dk = conv(&map, flipinput, NC_VALID);
            multifactor(C3dk, C3dk, -1.0 * opts.lr);

            matrix_t res;
            res.height = mapSize.height;
            res.width = mapSize.width;
            res.data = lenet->C3->mapData[j][i];

            matrix_t mat1;
            mat1.height = mapSize.height;
            mat1.width = mapSize.width;
            mat1.data = lenet->C3->mapData[j][i];

            addmat(&mat1, C3dk, &res);
            destroy_matrix_ptr(C3dk);
            destroy_matrix_ptr(flipinput);
        }
        matrix_t mat3;
        mat3.height = dSize.height;
        mat3.width = dSize.width;
        mat3.data = lenet->C3->d[i];
        lenet->C3->biasData[i] = lenet->C3->biasData[i] - opts.lr * summat(&mat3);
    }

    // 输出层
    // 首先需要将前面的多维输出展开成一维向量
    float* O5inData = (float*)malloc((lenet->O5->inputNum) * sizeof(float));
    NcSize2D outSize = px_create_size(lenet->S4->in_height / lenet->S4->map_size, lenet->S4->in_width / lenet->S4->map_size);
    for (i = 0; i < (lenet->S4->out_channels); i++)
    {
        for (r = 0; r < outSize.height; r++)
        {
            for (c = 0; c < outSize.width; c++)
            {
                O5inData[i * outSize.height * outSize.width + r * outSize.width + c] = lenet->S4->y[i][r][c];
            }
        }
    }

    for (j = 0; j < lenet->O5->outputNum; j++)
    {
        for (i = 0; i < lenet->O5->inputNum; i++)
        {
            lenet->O5->wData[j][i] = lenet->O5->wData[j][i] - opts.lr * lenet->O5->d[j] * O5inData[i];
        }
        lenet->O5->biasData[j] = lenet->O5->biasData[j] - opts.lr * lenet->O5->d[j];
    }
    free(O5inData);
}

void clear_lenet(Lenet* net)
{
    // 将神经元的部分数据清除
    int j, c, r;
    
    // C1 layer
    for (j = 0; j < net->C1->out_channels; j++)
    {
        for (r = 0; r < net->S2->in_height; r++)
        {
            for (c = 0; c < net->S2->in_width; c++)
            {
                net->C1->d[j][r][c] = (float)0.0;
                net->C1->v[j][r][c] = (float)0.0;
                net->C1->y[j][r][c] = (float)0.0;
            }
        }
    }
    
    // S2 layer
    for (j = 0; j < net->S2->out_channels; j++)
    {
        for (r = 0; r < net->C3->in_height; r++)
        {
            for (c = 0; c < net->C3->in_width; c++)
            {
                net->S2->d[j][r][c] = 0;
                net->S2->y[j][r][c] = 0;
            }
        }
    }
    
    // C3 layer
    for (j = 0; j < net->C3->out_channels; j++)
    {
        for (r = 0; r < net->S4->in_height; r++)
        {
            for (c = 0; c < net->S4->in_width; c++)
            {
                net->C3->d[j][r][c] = (float)0.0;
                net->C3->v[j][r][c] = (float)0.0;
                net->C3->y[j][r][c] = (float)0.0;
            }
        }
    }
    
    // S4 layer
    for (j = 0; j < net->S4->out_channels; j++)
    {
        for (r = 0; r < net->S4->in_height / net->S4->map_size; r++)
        {
            for (c = 0; c < net->S4->in_width / net->S4->map_size; c++)
            {
                net->S4->d[j][r][c] = (float)0.0;
                net->S4->y[j][r][c] = (float)0.0;
            }
        }
    }
    
    // O5 layer
    for (j = 0; j < net->O5->outputNum; j++)
    {
        net->O5->d[j] = (float)0.0;
        net->O5->v[j] = (float)0.0;
        net->O5->y[j] = (float)0.0;
    }
}

// 这是用于测试的函数, 保存CNN网络中的相关数据
void save_lenet_data(Lenet* net, const char* filename, float** inputdata)
{
    FILE* fp = fopen(filename, "wb");
    CHECK_WRITE_FILE(fp, filename);

    int i, j, r;
    
    // C1 layer
    for (i = 0; i < net->C1->in_height; i++)
    {
        fwrite(inputdata[i], sizeof(float), net->C1->in_width, fp);
    }
    for (i = 0; i < net->C1->in_channels; i++)
    {
        for (j = 0; j < net->C1->out_channels; j++)
        {
            for (r = 0; r < net->C1->map_size; r++)
            {
                fwrite(net->C1->mapData[i][j][r], sizeof(float), net->C1->map_size, fp);
            }
        }
    }

    fwrite(net->C1->biasData, sizeof(float), net->C1->out_channels, fp);

    for (j = 0; j < net->C1->out_channels; j++)
    {
        for (r = 0; r < net->S2->in_height; r++)
        {
            fwrite(net->C1->v[j][r], sizeof(float), net->S2->in_width, fp);
        }
        for (r = 0; r < net->S2->in_height; r++)
        {
            fwrite(net->C1->d[j][r], sizeof(float), net->S2->in_width, fp);
        }
        for (r = 0; r < net->S2->in_height; r++)
        {
            fwrite(net->C1->y[j][r], sizeof(float), net->S2->in_width, fp);
        }
    }

    // S2 layer
    for (j = 0; j < net->S2->out_channels; j++)
    {
        for (r = 0; r < net->C3->in_height; r++)
        {
            fwrite(net->S2->d[j][r], sizeof(float), net->C3->in_width, fp);
        }
        for (r = 0; r < net->C3->in_height; r++)
        {
            fwrite(net->S2->y[j][r], sizeof(float), net->C3->in_width, fp);
        }
    }

    // C3 layer
    for (i = 0; i < net->C3->in_channels; i++)
    {
        for (j = 0; j < net->C3->out_channels; j++)
        {
            for (r = 0; r < net->C3->map_size; r++)
            {
                fwrite(net->C3->mapData[i][j][r], sizeof(float), net->C3->map_size, fp);
            }
        }
    }

    fwrite(net->C3->biasData, sizeof(float), net->C3->out_channels, fp);

    for (j = 0; j < net->C3->out_channels; j++)
    {
        for (r = 0; r < net->S4->in_height; r++)
        {
            fwrite(net->C3->v[j][r], sizeof(float), net->S4->in_width, fp);
        }
        for (r = 0; r < net->S4->in_height; r++)
        {
            fwrite(net->C3->d[j][r], sizeof(float), net->S4->in_width, fp);
        }
        for (r = 0; r < net->S4->in_height; r++)
        {
            fwrite(net->C3->y[j][r], sizeof(float), net->S4->in_width, fp);
        }
    }

    // S4 layer
    for (j = 0; j < net->S4->out_channels; j++)
    {
        for (r = 0; r < net->S4->in_height / net->S4->map_size; r++)
        {
            fwrite(net->S4->d[j][r], sizeof(float), net->S4->in_width / net->S4->map_size, fp);
        }
        for (r = 0; r < net->S4->in_height / net->S4->map_size; r++)
        {
            fwrite(net->S4->y[j][r], sizeof(float), net->S4->in_width / net->S4->map_size, fp);
        }
    }

    // O5 layer
    for (i = 0; i < net->O5->outputNum; i++)
    {
        fwrite(net->O5->wData[i], sizeof(float), net->O5->inputNum, fp);
    }
    fwrite(net->O5->biasData, sizeof(float), net->O5->outputNum, fp);
    fwrite(net->O5->v, sizeof(float), net->O5->outputNum, fp);
    fwrite(net->O5->d, sizeof(float), net->O5->outputNum, fp);
    fwrite(net->O5->y, sizeof(float), net->O5->outputNum, fp);

    fclose(fp);
}

int main()
{
    test_mnist_train_test();

    return 0;
}
