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

matrix_t* create_normalized_f32_matrix_from_u8_image(px_mnist_image_array_t* inputData, int n)
{
    const int height = inputData->images[n].height;
    const int width = inputData->images[n].width;
    px_size_t size = px_create_size(height, width);
    matrix_t* normalized_image = create_matrix(size);
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            normalized_image->data[i][j] = inputData->images->data[i * width + j] / 255.0f;
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
            matrix_t* input = create_normalized_f32_matrix_from_u8_image(inputData, n);

            forward_lenet(net, input);                             // 前向传播，这里主要计算各
            backward_lenet(net, outputData->one_hot_label[n].data); // 后向传播，这里主要计算各神经元的误差梯度

            //char* filedir="E:\\Code\\debug\\CNNData\\";
            //const char* filename=combine_strings(filedir,combine_strings(intTochar(n),".cnn"));
            //save_nndata(cnn,filename,inputData->ImgPtr[n].ImgData);
            apply_grads_on_lenet(net, opts, input); // 更新权重

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

            if (n % 100 == 0)
            {
                fprintf(stdout, "-- [train] sample: %5d/%d, epoch: %d/%d, loss: %.4f\n",
                    n, trainNum,
                    epoch_index, opts.num_epochs,
                    net->L[n]);
            }

            destroy_matrix(input);
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
        matrix_t* cnn_input = create_normalized_f32_matrix_from_u8_image(inputData, n);
        forward_lenet(net, cnn_input);
        array_t label_as_array;
        label_as_array.data = outputData->one_hot_label[n].data;
        label_as_array.len = net->O5->outputNum;
        if (argmax(net->O5->y) != argmax(&label_as_array))
        {
            incorrectnum++;
        }
        clear_lenet(net);
        destroy_matrix(cnn_input);
    }
    return (float)incorrectnum / (float)testNum;
}

int test_mnist_train_test()
{
    char mnist_data_dir[256];
    sprintf(mnist_data_dir, "%s/mnist", project_dir);
    px_mnist_data_t* mnist_data = px_read_mnist_data(mnist_data_dir);

    px_size_t inputSize = px_create_size(mnist_data->test_images->images[0].height, mnist_data->test_images->images[0].width);
    int outSize = mnist_data->test_labels->one_hot_label[0].len;

    // CNN structure init
    Lenet* net = (Lenet*)malloc(sizeof(Lenet));
    setup_lenet(net, inputSize, outSize);

    // CNN training
#if 1
    LenetTrainOpts opts;
    opts.num_epochs = 1;
    opts.lr = 0.1;
    //int trainNum = 10;
    int trainNum = 10000;
    //int trainNum = 55000;
    char train_log_pth[PX_MAX_PATH];
    sprintf(train_log_pth, "%s/debug/train-log.txt", project_dir);
    FILE* fout = fopen(train_log_pth, "w"); // log file
    PX_CHECK_WRITE_FILE(fout, train_log_pth);

    train_lenet_on_mnist(net, mnist_data->train_images, mnist_data->train_labels, opts, trainNum, fout);

    fclose(fout);

    printf("train finished!!\n");
    char train_model_pth[PX_MAX_PATH];
    sprintf(train_model_pth, "%s/model/mnist-train.cnn", project_dir);
    save_lenet_inference_data(net, train_model_pth);
    // save training error
    char train_err_pth[PX_MAX_PATH];
    sprintf(train_err_pth, "%s/debug/cnnL.ma", project_dir);
    FILE* fp = fopen(train_err_pth, "wb");
    PX_CHECK_WRITE_FILE(fp, train_err_pth);
    fwrite(net->L, sizeof(float), trainNum, fp);
    fclose(fp);
#endif

    printf("--- mnist test start\n");
    char test_model_pth[PX_MAX_PATH];
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

void setup_lenet(Lenet* net, px_size_t inputSize, int outputSize)
{
    net->layerNum = 5;

    px_size_t inSize;
    int mapSize = 5;
    inSize.width = inputSize.width;
    inSize.height = inputSize.height;
    net->C1 = init_conv_layer(inSize, 5, 1, 6);

    inSize.width = inSize.width - mapSize + 1;
    inSize.height = inSize.height - mapSize + 1;
    net->S2 = init_pooling_layer(inSize, 2, 6, 6, NC_AvePool);

    inSize.width = inSize.width / 2;
    inSize.height = inSize.height / 2;
    net->C3 = init_conv_layer(inSize, 5, 6, 12);

    inSize.width = inSize.width - mapSize + 1;
    inSize.height = inSize.height - mapSize + 1;
    net->S4 = init_pooling_layer(inSize, 2, 12, 12, NC_AvePool);

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
//    px_size_t mapSize={cnn->C1->map_size,cnn->C1->map_size};
//    px_size_t inSize={cnn->C1->in_width,cnn->C1->in_height};
//    px_size_t outSize={cnn->S2->in_width,cnn->S2->in_height };
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

int main()
{
    test_mnist_train_test();

    return 0;
}
