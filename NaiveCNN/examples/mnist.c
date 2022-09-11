#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <assert.h>

#include "mnist.h"
#include "cnn.h"

#define LOADBMP_IMPLEMENTATION
#include "loadbmp.h"

#include "naive_cnn.h"

#if __ANDROID__
    const char* project_dir = "/data/local/tmp";
#elif __linux__
    static const char* project_dir = "/home/zz/work/NaiveCNN/";
#elif _MSC_VER
    static const char* project_dir = "F:/zhangzhuo/dev/NaiveCNN/";
#elif __APPLE__ && !(__ARM_NEON)
    static const char* project_dir = "/Users/chris/work/gitee/NaiveCNN/";
#elif __APPLE__ && __ARM_NEON
    static const char* project_dir = "/Users/zz/work/pixel/NaiveCNN/";
#else
#pragma error
#endif


//英特尔处理器和其他低端机用户必须翻转头字节。
int reverse_int(int i)
{
    unsigned char ch1, ch2, ch3, ch4;
    ch1 = i & 255;
    ch2 = (i >> 8) & 255;
    ch3 = (i >> 16) & 255;
    ch4 = (i >> 24) & 255;
    return((int) ch1 << 24) + ((int)ch2 << 16) + ((int)ch3 << 8) + ch4;
}

MnistImgArr* read_mnist_image(const char* filename) // 读入图像
{
    FILE* fp = fopen(filename, "rb");
    CHECK_READ_FILE(fp, filename);

    int magic_number = 0;
    int number_of_images = 0;
    int n_rows = 0;
    int n_cols = 0;
    //从文件中读取sizeof(magic_number) 个字符到 &magic_number
    fread((char*)&magic_number, sizeof(magic_number), 1, fp);
    magic_number = reverse_int(magic_number);
    //获取训练或测试image的个数number_of_images
    fread((char*)&number_of_images,sizeof(number_of_images),1,fp);
    number_of_images = reverse_int(number_of_images);
    //获取训练或测试图像的高度Height
    fread((char*)&n_rows,sizeof(n_rows),1,fp);
    n_rows = reverse_int(n_rows);
    //获取训练或测试图像的宽度Width
    fread((char*)&n_cols,sizeof(n_cols),1,fp);
    n_cols = reverse_int(n_cols);
    //获取第i幅图像，保存到vec中
    int i,r,c;

    // 图像数组的初始化
    MnistImgArr* imgarr = (MnistImgArr*)malloc(sizeof(MnistImgArr));
    imgarr->ImgNum = number_of_images;
    imgarr->ImgPtr = (mnist_img_t*)malloc(number_of_images*sizeof(mnist_img_t));

    for(i = 0; i < number_of_images; i++)
    {
        imgarr->ImgPtr[i].height = n_rows;
        imgarr->ImgPtr[i].width = n_cols;
        imgarr->ImgPtr[i].data = (float**)malloc(n_rows*sizeof(float*));
        for(r = 0; r < n_rows; r++)
        {
            imgarr->ImgPtr[i].data[r] = (float*)malloc(n_cols*sizeof(float));
            for(c = 0; c < n_cols; c++)
            {
                unsigned char temp = 0;
                fread((char*) &temp, sizeof(temp),1,fp);
                imgarr->ImgPtr[i].data[r][c] = (float)temp/255.0f;
            }
        }
    }

    fclose(fp);
    return imgarr;
}

// 读入标签
MnistLabelArr* read_mnist_label(const char* filename)
{
    FILE* fp=fopen(filename,"rb");
    CHECK_READ_FILE(fp, filename);

    int magic_number = 0;
    int number_of_labels = 0;
    int label_long = 10;

    //从文件中读取sizeof(magic_number) 个字符到 &magic_number
    fread((char*)&magic_number,sizeof(magic_number),1,fp);
    magic_number = reverse_int(magic_number);
    //获取训练或测试image的个数number_of_images
    fread((char*)&number_of_labels,sizeof(number_of_labels),1,fp);
    number_of_labels = reverse_int(number_of_labels);

    int i;

    // 图像标记数组的初始化
    MnistLabelArr* labarr=(MnistLabelArr*)malloc(sizeof(MnistLabelArr));
    labarr->LabelNum=number_of_labels;
    labarr->LabelPtr=(MnistLabel*)malloc(number_of_labels*sizeof(MnistLabel));

    for(i = 0; i < number_of_labels; ++i)
    {
        labarr->LabelPtr[i].l=10;
        labarr->LabelPtr[i].LabelData=(float*)calloc(label_long,sizeof(float));
        unsigned char temp = 0;
        fread((char*) &temp, sizeof(temp),1,fp);
        labarr->LabelPtr[i].LabelData[(int)temp]=1.0;
    }

    fclose(fp);
    return labarr;
}

void mnist_cnn_train(CNN* cnn, MnistImgArr* inputData, MnistLabelArr* outputData, CNNOpts opts, int trainNum, FILE* fout)
{
    // 学习训练误差曲线
    cnn->L = (float*)malloc(trainNum * sizeof(float));
    int e;
    for (e = 0; e < opts.numepochs; e++)
    {
        int n = 0;
        for (n = 0; n < trainNum; n++)
        {
            matrix_t input;
            input.height = inputData->ImgPtr[n].height;
            input.width = inputData->ImgPtr[n].width;
            input.data = inputData->ImgPtr[n].data;
            cnn_forward(cnn, &input);  // 前向传播，这里主要计算各
            cnn_backward(cnn, outputData->LabelPtr[n].LabelData); // 后向传播，这里主要计算各神经元的误差梯度


            //char* filedir="E:\\Code\\debug\\CNNData\\";
            //const char* filename=combine_strings(filedir,combine_strings(intTochar(n),".cnn"));
            //save_nndata(cnn,filename,inputData->ImgPtr[n].ImgData);
            cnn_applygrads(cnn, opts, &input); // 更新权重

            cnn_clear(cnn);
            // 计算并保存误差能量
            float l = 0.0;
            int i;
            for (i = 0; i < cnn->O5->outputNum; i++) {
                l = l + cnn->e[i] * cnn->e[i];
            }
            if (n == 0) {
                cnn->L[n] = l / 2.0f;
            }
            else {
                cnn->L[n] = cnn->L[n - 1] * 0.99f + 0.01f*l / 2.0f;
            }
            fprintf(fout, "-- [train] sample: %5d/%d, epoch: %d/%d, loss: %.4f\n",
                n, trainNum,
                e, opts.numepochs,
                cnn->L[n]
            );
        }
    }
}

// do inference
float mnist_cnn_test(CNN* cnn, MnistImgArr* inputData, MnistLabelArr* outputData, int testNum)
{
    int n = 0;
    int incorrectnum = 0;
    for (n = 0; n < testNum; n++)
    {
        matrix_t input;
        input.height = inputData->ImgPtr[n].height;
        input.width = inputData->ImgPtr[n].width;
        input.data = inputData->ImgPtr[n].data;
        cnn_forward(cnn, &input);
        if (argmax(cnn->O5->y, cnn->O5->outputNum) != argmax(outputData->LabelPtr[n].LabelData, cnn->O5->outputNum)) {
            incorrectnum++;
        }
        cnn_clear(cnn);
    }
    return (float)incorrectnum / (float)testNum;
}


// extract images from original mnist data
// save each image to file
// for simplicity, only do it on test images
void extract_mnist_image_and_save()
{
    char test_image_pth[NC_MAX_PATH];
    sprintf(test_image_pth, "%s/mnist/t10k-images.idx3-ubyte", project_dir);

    NcImage** images;
    int image_num;
    nc_read_mnist_image(test_image_pth, &images, &image_num);
    printf("=== got %d test images\n", image_num);

    for(int i=0; i < image_num; i++)
    {
        char save_pth[NC_MAX_PATH];
        sprintf(save_pth, "%s/mnist/testImgs/%d.bmp", project_dir, i);
        FILE* fp = fopen(save_pth, "wb");
        unsigned int err = loadbmp_encode_file(save_pth, images[i]->data, images[i]->width, images[i]->height, 1);

        if (err){
            printf("LoadBMP Load Error: %u\n", err);
        }
        fclose(fp);
    }

    for(int i = 0; i < image_num; i++)
    {
        free(images[i]->data);
        free(images[i]);
    }
    free(images);
}

int test_mnist_train_test()
{
    char train_label_pth[NC_MAX_PATH];
    sprintf(train_label_pth, "%s/mnist/train-labels.idx1-ubyte", project_dir);
    MnistLabelArr* trainLabel=read_mnist_label(train_label_pth);

    char train_image_pth[NC_MAX_PATH];
    sprintf(train_image_pth, "%s/mnist/train-images.idx3-ubyte", project_dir);
    MnistImgArr* trainImg=read_mnist_image(train_image_pth);

    char test_label_pth[NC_MAX_PATH];
    sprintf(test_label_pth, "%s/mnist/t10k-labels.idx1-ubyte", project_dir);
    MnistLabelArr* testLabel= read_mnist_label(test_label_pth);

    char test_image_pth[NC_MAX_PATH];
    sprintf(test_image_pth, "%s/mnist/t10k-images.idx3-ubyte", project_dir);
    MnistImgArr* testImg= read_mnist_image(test_image_pth);

    NcSize2D inputSize = px_create_size(testImg->ImgPtr[0].height, testImg->ImgPtr[0].width);
    int outSize=testLabel->LabelPtr[0].l;

    // CNN structure init
    CNN* cnn=(CNN*)malloc(sizeof(CNN));
    lenet5_setup(cnn,inputSize,outSize);

    // CNN training
#if 1
    CNNOpts opts;
    opts.numepochs=1;
    opts.alpha=1.0;
    int trainNum=10;
    //int trainNum=55000;
    char train_log_pth[NC_MAX_PATH];
    sprintf(train_log_pth, "%s/debug/train-log.txt", project_dir);
    FILE* fout = fopen(train_log_pth, "w"); // log file
    CHECK_WRITE_FILE(fout, train_log_pth);

    mnist_cnn_train(cnn,trainImg,trainLabel,opts,trainNum, fout);

    fclose(fout);

    printf("train finished!!\n");
    char train_model_pth[NC_MAX_PATH];
    sprintf(train_model_pth, "%s/model/mnist-train.cnn", project_dir);
    save_cnn(cnn, train_model_pth);
    // save training error
    char train_err_pth[NC_MAX_PATH];
    sprintf(train_err_pth, "%s/debug/cnnL.ma", project_dir);
    FILE* fp=fopen(train_err_pth,"wb");
    CHECK_WRITE_FILE(fp, train_err_pth);
    fwrite(cnn->L,sizeof(float),trainNum,fp);
    fclose(fp);
#endif


    // CNN test
    printf("--- mnist test start\n");
    char test_model_pth[NC_MAX_PATH];
    //sprintf(test_model_pth, "%s/model/mnist-train.cnn", project_dir);
    sprintf(test_model_pth, "%s/model/mnist.cnn", project_dir);
    load_cnn(cnn, test_model_pth);
    //int testNum=10000; // too slow
    int testNum = 10;
    float incorrectRatio=0.0;
    incorrectRatio=mnist_cnn_test(cnn,testImg,testLabel,testNum);
    float accuracy = 1 - incorrectRatio;
    printf("--- accuracy: %f\n", accuracy);
    printf("test finished!!\n");

    free(cnn);

    return 0;
}


char* layer_type_str(NcLayerType type) {
    switch(type) {
        case NC_LAYER_DATA:
            return "NC_DATA";
        case NC_LAYER_CONVOLUTION:
            return "NC_CONVOLUTION";
        case NC_LAYER_POOLING:
            return "NC_POOLING";
        case NC_LAYER_INNERPRODUCT:
            return "NC_INNERPRODUCT";
        case NC_LAYER_LOSS:
            return "NC_LOSS";
        default:
            return "UNKNOWN";
    }
}

// setup lenet5
void lenet5_setup(CNN* cnn, NcSize2D inputSize,int outputSize)
{
    cnn->layerNum = 5;

    NcSize2D inSize;
    int mapSize = 5;
    inSize.width = inputSize.width;
    inSize.height = inputSize.height;
    cnn->C1 = init_conv_layer(inSize.width, inSize.height, 5, 1, 6);

    inSize.width = inSize.width - mapSize + 1;
    inSize.height = inSize.height - mapSize + 1;
    cnn->S2 = init_pooling_layer(inSize.width, inSize.height, 2, 6, 6, AvePool);

    inSize.width = inSize.width / 2;
    inSize.height = inSize.height / 2;
    cnn->C3 = init_conv_layer(inSize.width, inSize.height, 5, 6, 12);

    inSize.width = inSize.width - mapSize + 1;
    inSize.height = inSize.height - mapSize + 1;
    cnn->S4 = init_pooling_layer(inSize.width, inSize.height, 2, 12, 12, AvePool);

    inSize.width = inSize.width / 2;
    inSize.height = inSize.height / 2;
    cnn->O5 = init_innerproduct_layer(inSize.width * inSize.height * 12, outputSize);

    cnn->e = (float*)calloc(cnn->O5->outputNum,sizeof(float));
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


void nc_lenet5_train_setup(NcNet* net, NcSize2D input_size, int output_size){
    //NcSize2D input_size;
    //input_size.h = 28; input_size.w = 28;
    //int output_size = 10;
    //NcNet* net = (NcNet*)malloc(sizeof(NcNet));

    net->layers_num = 5;
    net->layers = (NcLayer**)malloc(sizeof(NcLayer*)*net->layers_num);
    for (int i = 0; i < net->layers_num; i++) {
        net->layers[i] = NULL;
    }

    net->blobs_num = 6;
    net->blobs = (NcBlob**)malloc(sizeof(NcBlob*)*net->blobs_num);
    for (int i = 0; i < net->blobs_num; i++) {
        net->blobs[i] = NULL;
    }

    char fout_pth[NC_MAX_PATH];
    sprintf(fout_pth, "%s/debug/xxx.txt", project_dir);
    FILE* fout = fopen(fout_pth, "w");
    if (net->blobs[1] == NULL) {
        fprintf(fout, "-- NULL\n");
    }
    else {
        puts("-- Not NULL\n");
    }
    fclose(fout);
    //------------------------
    // 指针应当赋予初始值。否则，debug模式下不会默认设定为NULL。
    //------------------------

    // allocate network's input blobs
    net->blobs[0] = nc_create_blob3d(28, 28, 1);

    NcSize2D in_size;
    int map_size;
    int in_channels;
    int out_channels;
    int pool_type;
    int input_blobs_num;
    int output_blobs_num;
    int* input_blob_ids;
    int* output_blob_ids;
    NcBlob* output_blobs[NC_MAX_BLOB_PER_LAYER];
    for (int i = 0; i < NC_MAX_BLOB_PER_LAYER; i++) {
        output_blobs[i] = NULL;
    }

    NcLayer* layer=NULL;

    //----------------------------------------------------------------
    // C1 layer
    //----------------------------------------------------------------
    // create layer param
    map_size = 5;
    in_size.width = input_size.width;
    in_size.height = input_size.height;
    in_channels = 1;
    out_channels = 6;
    NcConvolutionParam* C1 = nc_train_create_convolution_param(in_size.height, in_size.width, map_size, in_channels, out_channels);

    // create layer
    layer = nc_create_layer();
    layer->type = NC_LAYER_CONVOLUTION;
    layer->param = C1;
    layer->forward = nc_train_forward_convolution;
    layer->backward = nc_train_forward_convolution;
    net->layers[0] = layer;

    // setup layer input
    // to handle different size of input,
    // we need to re-setup each layer's input & output every forward pass

    // we always assume that each layer's input blobs are already allocated
    // so, we don't need to re-calculate or re-assign each layer's input blobs' dimensions
    input_blobs_num = 1;
    input_blob_ids = (int[]) { 0 };
    layer->input = nc_train_create_layer_input(input_blobs_num, input_blob_ids, net);

    // setup layer output
    // for each layer's output blobs, some may be allocated (in-place operation, like relu)
    // however, some are not allocated, such as convolution, pooling
    // it's each type of layer's reponsibility to calculate and allocate the un-allocated blob
    output_blobs_num = 1;
    output_blob_ids = (int[]) { 1 };
    output_blobs[0] = nc_create_blob3d(C1->out_height, C1->out_width, C1->out_channels);
    layer->output = nc_train_create_layer_output(output_blobs_num, output_blob_ids, net, output_blobs);

    //----------------------------------------------------------------
    // S2 layer
    //----------------------------------------------------------------
    // create layer param
    in_size.height = C1->out_height;
    in_size.width = C1->out_width;
    map_size = 2;
    in_channels = C1->out_channels;
    out_channels = 6;
    pool_type = AvePool;
    NcPoolingParam* S2 = nc_train_create_pooling_param(in_size.height, in_size.width, map_size, in_channels, out_channels, pool_type);

    // create layer
    layer = nc_create_layer();
    layer->type = NC_LAYER_POOLING;
    layer->param = S2;
    layer->forward = nc_train_forward_pooling;
    layer->backward = nc_train_backward_pooling;
    net->layers[1] = layer;

    // setup layer input
    input_blobs_num = 1;
    input_blob_ids = (int[]) { 1 };
    layer->input = nc_train_create_layer_input(input_blobs_num, input_blob_ids, net);

    // setup layer output
    output_blobs_num = 1;
    output_blob_ids = (int[]) { 2 };
    output_blobs[0] = nc_create_blob3d(S2->out_height, S2->out_width, S2->out_channels);
    layer->output = nc_train_create_layer_output(output_blobs_num, output_blob_ids, net, output_blobs);

    //----------------------------------------------------------------
    // C3 layer
    //----------------------------------------------------------------
    // create layer param
    in_size.height = S2->out_height;
    in_size.width = S2->out_width;
    map_size = 5;
    in_channels = S2->out_channels;
    out_channels = 12;
    NcConvolutionParam* C3 = nc_train_create_convolution_param(in_size.height, in_size.width, map_size, in_channels, out_channels);

    // create layer
    layer = nc_create_layer();
    layer->type = NC_LAYER_CONVOLUTION;
    layer->param = C3;
    layer->forward = nc_train_forward_convolution;
    layer->backward = nc_train_backward_convolution;
    net->layers[2] = layer;

    // setup layer input
    input_blobs_num = 1;
    input_blob_ids = (int[]) { 2 };
    layer->input = nc_train_create_layer_input(input_blobs_num, input_blob_ids, net);

    // setup layer output
    output_blobs_num = 1;
    output_blob_ids = (int[]) { 3 };
    output_blobs[0] = nc_create_blob3d(C3->out_height, C3->out_width, C3->out_channels);
    layer->output = nc_train_create_layer_output(output_blobs_num, output_blob_ids, net, output_blobs);

    //----------------------------------------------------------------
    // S4 layer
    //----------------------------------------------------------------
    // create layer param
    in_size.height = C3->out_height;
    in_size.width = C3->out_width;
    map_size = 2;
    in_channels = C3->out_channels;
    out_channels = 12;
    pool_type = AvePool;
    NcPoolingParam* S4 = nc_train_create_pooling_param(in_size.height, in_size.width, map_size, in_channels, out_channels, pool_type);

    // create layer
    layer = nc_create_layer();
    layer->type = NC_LAYER_POOLING;
    layer->param = S4;
    layer->forward = nc_train_forward_pooling;
    layer->backward = nc_train_backward_pooling;
    net->layers[3] = layer;

    // setup layer input
    input_blobs_num = 1;
    input_blob_ids = (int[]) { 3 };
    layer->input = nc_train_create_layer_input(input_blobs_num, input_blob_ids, net);

    // setup layer output
    output_blobs_num = 1;
    output_blob_ids = (int[]) { 4 };
    output_blobs[0] = nc_create_blob3d(S4->out_height, S4->out_width, S4->out_channels);
    layer->output = nc_train_create_layer_output(output_blobs_num, output_blob_ids, net, output_blobs);

    //----------------------------------------------------------------
    // O5 layer
    //----------------------------------------------------------------
    // create layer param
    in_size.width = S4->out_width;
    in_size.height = S4->out_height;
    int in_num = in_size.height * in_size.width * S4->out_channels;
    int out_num = output_size;
    NcInnerproductParam* O5 = nc_train_create_innerproduct_param(in_num, out_num);

    // create layer
    layer = nc_create_layer();
    layer->type = NC_LAYER_INNERPRODUCT;
    layer->param = O5;
    layer->forward = nc_train_forward_innerproduct;
    layer->backward = nc_train_backward_innerproduct;
    net->layers[4] = layer;

    // setup layer input
    input_blobs_num = 1;
    input_blob_ids = (int[]) { 4 };
    layer->input = nc_train_create_layer_input(input_blobs_num, input_blob_ids, net);

    // setup layer output
    output_blobs_num = 1;
    output_blob_ids = (int[]) { 5 };
    output_blobs[0] = nc_create_blob3d(O5->out_height, O5->out_width, O5->out_channels);
    layer->output = nc_train_create_layer_output(output_blobs_num, output_blob_ids, net, output_blobs);

    // --- Loss
    net->e = (float*)calloc(out_num,sizeof(float));

    // pointing each layer's input&output blobs to network's blobs
    // this can be optimized, i.e. reuse memory during each inference
    //NcLayerInput* input = NULL;
    //NcLayerOutput* output = NULL;
    //for (int i = 0; i < net->layers_num; i++) {
    //	input = net->layers[i]->input;
    //	for (int j = 0; j < input->blob_num; j++) {
    //		input->blobs[j] = net->blobs[input->blob_ids[j]];
    //	}
    //	output = net->layers[i]->output;
    //	for (int j = 0; j < output->blob_num; j++) {
    //		output->blobs[j] = net->blobs[output->blob_ids[j]];
    //	}
    //}

    // --- print each layer's type
    for(int i=0; i<net->layers_num; i++) {
        layer = net->layers[i];
        printf("-- layer[%d]'s type: %s\n", i,
            layer_type_str(layer->type));
    }
}

void nc_train_trial()
{
    NcSize2D input_size;
    input_size.height = 28;
    input_size.width = 28;
    int output_size = 10;
    NcNet* net = (NcNet*)malloc(sizeof(NcNet));
    nc_lenet5_train_setup(net, input_size, output_size);

    //----- training

    // ---[train config]
    NcTrainConfig train_cfg;
    train_cfg.num_epoch = 1;
    train_cfg.alpha = 1.0f;
    //train_cfg.train_num = 55000;
    train_cfg.train_num = 1; // make it faster!
    sprintf(train_cfg.log_pth, "%s/debug/nc-train-log.txt", project_dir);

    // ---[data config]
    NcClsDataConfig data_cfg;
    sprintf(data_cfg.dataset, "%s", "MNIST");
    sprintf(data_cfg.splitset, "%s", "train");
    nc_cls_data_loader(&data_cfg, project_dir);

    // ---! train
    nc_train_cls_net(net, &train_cfg, &data_cfg);

//    CNNOpts opts;
//    opts.numepochs=1;
//    opts.alpha=1.0;
//    int trainNum=55000;
//    //int trainNum=100;
//    char train_log_pth[NC_MAX_PATH];
//    sprintf(train_log_pth, "%s/debug/train-log.txt", project_dir);
//    FILE* fout = fopen(train_log_pth, "w"); // log file
//    CHECK_WRITE_FILE(fout, train_log_pth);
//
//    mnist_cnn_train(cnn,trainImg,trainLabel,opts,trainNum, fout);
//
//    fclose(fout);
//
//    printf("train finished!!\n");
//    char train_model_pth[NC_MAX_PATH];
//    sprintf(train_model_pth, "%s/model/mnist-train.cnn", project_dir);
//    save_cnn(cnn, train_model_pth);
//    // save training error
//    char train_err_pth[NC_MAX_PATH];
//    sprintf(train_err_pth, "%s/debug/cnnL.ma", project_dir);
//    FILE* fp=fopen(train_err_pth,"wb");
//    CHECK_WRITE_FILE(fp, train_err_pth);
//    fwrite(cnn->L,sizeof(float),trainNum,fp);
//    fclose(fp);



    // free the network related memory
    printf("=== free layers begin\n");
    NcLayer* layer;
    for(int i=0; i<net->layers_num; i++) {
        layer = net->layers[i];
        nc_destroy_layer(layer);
    }
    printf("=== free layers OK\n");
    free(net->layers);
    free(net->e);
    free(net);
}

void nc_lenet5_infer_setup(NcNet* net) {
    net->layers_num = 5;
    net->layers = (NcLayer**)malloc(sizeof(NcLayer*)*net->layers_num);
    for (int i = 0; i < net->layers_num; i++) {
        net->layers[i] = nc_create_layer();
    }

    net->blobs_num = 6;
    net->blobs = (NcBlob**)malloc(sizeof(NcBlob*)*net->blobs_num);
    for (int i = 0; i < net->blobs_num; i++) {
        net->blobs[i] = nc_create_empty_blob(0, 0, 0, 0);
    }

    int map_size;
    int in_channels;
    int out_channels;
    int pool_type;
    int input_blobs_num;
    int output_blobs_num;
    int* input_blob_ids;
    int* output_blob_ids;

    NcLayer* layer = NULL;

    //----------------------------------------------------------------
    // C1 layer
    //----------------------------------------------------------------
    // point to net layer
    layer = net->layers[0];

    // setup layer type & infer function
    layer->type = NC_LAYER_CONVOLUTION;
    layer->infer = nc_infer_convolution;

    // setup layer input
    input_blobs_num = 1;
    input_blob_ids = (int[]) { 0 };
    layer->input = nc_infer_create_layer_input(input_blobs_num, input_blob_ids, net);

    // setup layer output
    output_blobs_num = 1;
    output_blob_ids = (int[]) { 1 };
    layer->output = nc_infer_create_layer_output(output_blobs_num, output_blob_ids, net);

    // create layer param
    map_size = 5;
    in_channels = 1;
    out_channels = 6;
    NcConvolutionParam* C1 = nc_infer_create_convolution_param(map_size, in_channels, out_channels, NC_PADDING_CONV_CAFFE);
    layer->param = C1;

    //----------------------------------------------------------------
    // S2 layer
    //----------------------------------------------------------------
    // point to net layer
    layer = net->layers[1];

    // setup layer type & infer function
    layer->type = NC_LAYER_POOLING;
    layer->infer = nc_infer_pooling;

    // setup layer input
    input_blobs_num = 1;
    input_blob_ids = (int[]) { 1 };
    layer->input = nc_infer_create_layer_input(input_blobs_num, input_blob_ids, net);

    // setup layer output
    output_blobs_num = 1;
    output_blob_ids = (int[]) { 2 };
    layer->output = nc_infer_create_layer_output(output_blobs_num, output_blob_ids, net);

    // create layer param
    map_size = 2;
    in_channels = C1->out_channels;
    out_channels = C1->out_channels;
    pool_type = AvePool;
    NcPoolingParam* S2 = nc_infer_create_pooling_param(map_size, in_channels, out_channels, pool_type);
    layer->param = S2;

    //----------------------------------------------------------------
    // C3 layer
    //----------------------------------------------------------------
    // point to net layer
    layer = net->layers[2];

    // setup layer type & infer function
    layer->type = NC_LAYER_CONVOLUTION;
    layer->infer = nc_infer_convolution;

    // setup layer input
    input_blobs_num = 1;
    input_blob_ids = (int[]) { 2 };
    layer->input = nc_infer_create_layer_input(input_blobs_num, input_blob_ids, net);

    // setup layer output
    output_blobs_num = 1;
    output_blob_ids = (int[]) { 3 };
    layer->output = nc_infer_create_layer_output(output_blobs_num, output_blob_ids, net);

    // create layer param
    map_size = 5;
    in_channels = S2->out_channels;
    out_channels = 12;
    NcConvolutionParam* C3 = nc_infer_create_convolution_param(map_size, in_channels, out_channels, NC_PADDING_CONV_CAFFE);
    layer->param = C3;

    //----------------------------------------------------------------
    // S4 layer
    //----------------------------------------------------------------
    // point to net layer
    layer = net->layers[3];

    // setup layer type & infer function
    layer->type = NC_LAYER_POOLING;
    layer->infer = nc_infer_pooling;

    // setup layer input
    input_blobs_num = 1;
    input_blob_ids = (int[]) { 3 };
    layer->input = nc_infer_create_layer_input(input_blobs_num, input_blob_ids, net);

    // setup layer output
    output_blobs_num = 1;
    output_blob_ids = (int[]) { 4 };
    layer->output = nc_infer_create_layer_output(output_blobs_num, output_blob_ids, net);

    // create layer param
    map_size = 2;
    in_channels = C3->out_channels;
    out_channels = 12;
    pool_type = AvePool;
    NcPoolingParam* S4 = nc_infer_create_pooling_param(map_size, in_channels, out_channels, pool_type);
    layer->param = S4;

    //----------------------------------------------------------------
    // O5 layer
    //----------------------------------------------------------------
    // point to net layer
    layer = net->layers[4];

    // setup layer type & infer function
    layer->type = NC_LAYER_INNERPRODUCT;
    layer->infer = nc_infer_innerproduct;

    // setup layer input
    input_blobs_num = 1;
    input_blob_ids = (int[]) { 4 };
    layer->input = nc_infer_create_layer_input(input_blobs_num, input_blob_ids, net);

    // setup layer output
    output_blobs_num = 1;
    output_blob_ids = (int[]) { 5 };
    layer->output = nc_infer_create_layer_output(output_blobs_num, output_blob_ids, net);

    // setup layer param
    int in_num = layer->input->blobs[0]->nstep;
    int out_num = 10; // num classes
    NcInnerproductParam* O5 = nc_infer_create_innerproduct_param(in_num, out_num);
    layer->param = O5;

    // --- Loss
    net->e = NULL;

    // --- print each layer's type
    for (int i = 0; i < net->layers_num; i++) {
        layer = net->layers[i];
        printf("-- layer[%d]'s type: %s\n", i,
            layer_type_str(layer->type));
    }
}

void nc_infer(NcNet* net, NcImage* image)
{
    // allocate network's input blobs
    //net->blobs[0] = nc_create_blob3d(image->h, image->w, image->c);
    nc_blob_data_realloc3d(net->blobs[0], image->height, image->width, image->channel);

    // assign each blobs' rely cnt
    for (int i = 0; i < net->blobs_num; i++)
    {
        net->blobs[i]->rely_cnt = net->blobs[i]->rely_cnt_total;
    }

    NcLayer* layer = NULL;
    for (int i = 0; i < net->layers_num; i++)
    {
        layer = net->layers[i];
        layer->infer(layer->param, layer->input, layer->output);
    }
}

// only do inference
// this will consider arbitrary input
// and consider memory saving
void nc_infer_trial()
{
    NcNet* net = (NcNet*)malloc(sizeof(NcNet));
    nc_lenet5_infer_setup(net);

    // ---[data config]
    NcClsDataConfig data_cfg;
    sprintf(data_cfg.dataset, "%s", "MNIST");
    sprintf(data_cfg.splitset, "%s", "train");
    nc_cls_data_loader(&data_cfg, project_dir);

    // ---! infer
    //for (int i = 0; i < data_cfg.num; i++) {
    for (int i = 0; i <1; i++) {
        NcImage* image = data_cfg.images[i];
        nc_infer(net, image);
    }

    //// free the network related memory
    //printf("=== free layers begin\n");
    //NcLayer* layer;
    //for (int i = 0; i < net->layers_num; i++) {
    //	layer = net->layers[i];
    //	printf("free %d-th layer\n", i);
    //	nc_destroy_layer(layer);
    //}
    //printf("=== free layers OK\n");
    //free(net->layers);
    //if(net->e)free(net->e);
    //if (net->L)free(net->L);
    //free(net);
}

int main() {
    //extract_mnist_image_and_save();
    //test_mnist_train_test();
    //nc_train_trial();
    nc_infer_trial();

    //nc_convolution_test_nchw();
    //nc_convolution_test_nhwc();

    //nc_pooling_test_nchw();
    //nc_pooling_test_nhwc();

    return 0;
}

