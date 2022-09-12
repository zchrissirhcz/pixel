#include "lenet.h"

#include <opencv2/imgcodecs.hpp>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <assert.h>

#include "cnn.h"
#include "px_mnist.h"
#include "px_filesystem.h"

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

int main()
{
    test_mnist_train_test();

    return 0;
}
