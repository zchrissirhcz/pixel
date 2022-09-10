#include "naive_cnn.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include <assert.h>

#include "naive_convolution.h"

float nc_get_random_float(float s, float t){
    float v = (float)(rand()) / RAND_MAX;
    v = v * (t - s) + s;
    return v;
}

NcBlob* nc_blob_make_empty(int n, int h, int w, int c) {
    NcBlob* blob = (NcBlob*)malloc(sizeof(NcBlob));
    blob->n = n;
    blob->h = h;
    blob->w = w;
    blob->c = c;
    blob->nstep = h * w * c; //TODO: do align here
    blob->mass = blob->nstep * n;
    blob->order = NHWC; //TODO:
    blob->data = NULL;
    blob->rely_cnt_total = 0;
    blob->rely_cnt = 0;
    return blob;
}

NcBlob* nc_blob_make(int n, int h, int w, int c) {
    NcBlob* blob = nc_blob_make_empty(n, h, w, c);
    blob->data = (float*)calloc(blob->mass, sizeof(float));
    return blob;
}

NcBlob* nc_blob_make3d(int h, int w, int c) {
    return nc_blob_make(1, h, w, c);
}

NcBlob* nc_blob_make2d(int h, int w) {
    return nc_blob_make(1, h, w, 1);
}

NcBlob* nc_blob_make_random(int n, int h, int w, int c, float s, float t) {
    NcBlob* blob = nc_blob_make(n, h, w, c);

    srand((unsigned)time(NULL));
    for (int i = 0; i < blob->mass; i++) {
        blob->data[i] = nc_get_random_float(s, t);
    }
    return blob;
}

NcBlob* nc_blob_make_same(int n, int h, int w, int c, float v) {
    NcBlob* blob = nc_blob_make(n, h, w, c);

    srand((unsigned)time(NULL));
    for (int i = 0; i < blob->mass; i++) {
        blob->data[i] = v;
    }
    return blob;
}

NcBlob* nc_blob_make2d_empty(int h, int w) {
    NcBlob* blob = nc_blob_make_empty(1, h, w, 1);
    return blob;
}

// change a blob's geometry properties:
// free a blob's already allocated data buffer,
// and re-assigne a different dimenssions, then re-allocated data buffer
// the blob's logical properties such as rely_cnt_total
void nc_blob_data_realloc(NcBlob* blob, int n, int h, int w, int c){
    if (blob->data) {
        free(blob->data);
        blob->data = NULL;
    }
    blob->n = n;
    blob->h = h;
    blob->w = w;
    blob->c = c;
    blob->nstep = h * w * c; //TODO: consider align here
    blob->mass = blob->nstep*n;
    blob->data = (float*)calloc(blob->mass, sizeof(float));
}

void nc_blob_data_realloc3d(NcBlob* blob, int h, int w, int c) {
    nc_blob_data_realloc(blob, 1, h, w, c);
}

NcStride* nc_stride_make(int h, int w) {
    NcStride* stride = (NcStride*)malloc(sizeof(NcStride));
    stride->h = h;
    stride->w = w;
    return stride;
}

void nc_free_layer(NcLayer* layer) {
    NcLayerType layer_type = layer->type;
    switch (layer_type) {
        case NC_LAYER_CONVOLUTION: {
            //printf("-- free convolution layer\n");
            NcConvolutionParam* param = (NcConvolutionParam*)layer->param;
            free(param->bias);
            //puts("\t free bias ok");

            free(param->weight);
            //puts("\t free weight ok");

            if(param->d)free(param->d);
            //puts("\t free d ok");

            if(param->d_weight)free(param->d_weight);
            //puts("\t free d_weight ok");

            if(param->v)free(param->v);
            //puts("\t free v ok");

            if(param->y)free(param->y);
            //puts("\t free y ok");

            free(param);
            //puts("\t free param ok");
        }break;
        case NC_LAYER_POOLING: {
            //printf("-- free poolingg layer\n");
            NcPoolingParam* param = (NcPoolingParam*)layer->param;

            if(param->d)free(param->d);
            //puts("\t free d ok");

            if(param->y)free(param->y);
            //puts("\t free y ok");

            free(param);
            //puts("\t free param ok");
        }break;
        case NC_LAYER_INNERPRODUCT: {
            //printf("-- free innerproduct layer\n");
            NcInnerproductParam* param = (NcInnerproductParam*)layer->param;
            //puts("\t-- try to free bias");
            free(param->bias);
            //puts("\t free bias ok");

            free(param->weight);
            //puts("\t free weight ok");

            if(param->d)free(param->d);
            //puts("\t free d ok");

            if(param->v)free(param->v);
            //puts("\t free v ok");

            if(param->y)free(param->y);
            //puts("\t free y ok");

            free(param);
            //puts("\t free param ok");
        }break;
        default: {
            fprintf(stderr, "Error! Not supported layer param code %d\n", layer_type);
        }
    }
}

void nc_net_forward(NcNet* net, NcImage* image, int label) {
    NcLayer* layer = NULL;
    for (int i = 0; i < net->layers_num; i++) {
        layer = net->layers[i];
        printf("\t-> layer: %d/%d ", i, net->layers_num);
        layer->forward(layer->param, layer->input, layer->output);
    }
}

void nc_net_backward(NcNet* net) {
    NcLayer* layer = NULL;
    for (int i = 0; i < net->layers_num; i++) {
        layer = net->layers[i];
        printf("\t<= layer: %d/%d ", i, net->layers_num);
        layer->backward(layer->param, layer->input, layer->output);
    }
}

// train a classification network
void nc_train_cls_net(NcNet* net, NcTrainConfig* train_cfg, NcClsDataConfig* data_cfg) {
    // setup log
    FILE* fout = fopen(train_cfg->log_pth, "w");
    CHECK_WRITE_FILE(fout, train_cfg->log_pth);

    NcImage* image = NULL;
    int label = 0;
    for (int i = 0; i < train_cfg->num_epoch; i++) {
        for (int j = 0; j < train_cfg->train_num; j++) {
            image = data_cfg->images[i];
            label = data_cfg->labels[i];
            printf("Iteration %d/%d\n", j, train_cfg->train_num);
            nc_net_forward(net, image, label);
            //nc_net_backward(net);
        }
    }

    fclose(fout);
}

// load data here
void nc_cls_data_loader(NcClsDataConfig* cfg) {
    if (strcmp(cfg->dataset, "MNIST") == 0) {
        if (strcmp(cfg->splitset, "train") == 0) {
            char train_image_pth[NC_MAX_PATH];
            sprintf(train_image_pth, "%s/mnist/train-images.idx3-ubyte", project_dir);

            NcImage** images;
            int image_num;
            nc_read_mnist_image(train_image_pth, &images, &image_num);
            cfg->images = images;

            int* labels;
            int label_num;
            char train_label_pth[NC_MAX_PATH];
            sprintf(train_label_pth, "%s/mnist/train-labels.idx1-ubyte", project_dir);
            nc_read_mnist_label(train_label_pth, &labels, &label_num);

            assert(label_num == image_num);

            cfg->images = images;
            cfg->labels = labels;
            cfg->num = image_num;

            // dump images and labels
            // the result is correct
#ifdef DUMP_MNIST_TRAIN
            char save_pth[NC_MAX_PATH];
            for (int i = 0; i < image_num; i++) {
                memset(save_pth, 0, NC_MAX_PATH);
                sprintf(save_pth, "%s/debug/mnist_train/%d_%d.bmp", project_dir, i, labels[i]);

                FILE* fp = fopen(save_pth, "wb");
                unsigned int err = loadbmp_encode_file(save_pth, images[i]->data, images[i]->w, images[i]->h, 1);

                if (err) {
                    printf("LoadBMP Load Error: %u\n", err);
                }
                fclose(fp);
            }
#endif
        }
    }
}

static int reverse_int(int i)
{
    unsigned char ch1, ch2, ch3, ch4;
    ch1 = i & 255;
    ch2 = (i >> 8) & 255;
    ch3 = (i >> 16) & 255;
    ch4 = (i >> 24) & 255;
    return((int)ch1 << 24) + ((int)ch2 << 16) + ((int)ch3 << 8) + ch4;
}

// 读入图像
void nc_read_mnist_image(const char* filename, NcImage*** _images, int* _image_num)
{
    FILE* fp = fopen(filename, "rb");
    CHECK_READ_FILE(fp, filename);

    int magic_number = 0;
    int number_of_images = 0;
    int n_rows = 0;
    int n_cols = 0;
    //从文件中读取sizeof(magic_number) 个字符到 &magic_number
    fread(&magic_number, sizeof(magic_number), 1, fp);
    magic_number = reverse_int(magic_number);
    //获取训练或测试image的个数number_of_images
    fread(&number_of_images, sizeof(number_of_images), 1, fp);
    number_of_images = reverse_int(number_of_images);
    //获取训练或测试图像的高度Height
    fread((char*)&n_rows, sizeof(n_rows), 1, fp);
    n_rows = reverse_int(n_rows);
    //获取训练或测试图像的宽度Width
    fread((char*)&n_cols, sizeof(n_cols), 1, fp);
    n_cols = reverse_int(n_cols);

    //printf("--- n_rows=%d, n_cols=%d\n", n_rows, n_cols);
    //获取第i幅图像，保存到vec中
    int i;

    // 图像数组的初始化
    NcImage** images = (NcImage**)malloc(sizeof(NcImage*)*number_of_images);

    for (i = 0; i < number_of_images; ++i) {
        images[i] = nc_make_empty_image(n_rows, n_cols, 1);
        fread(images[i]->data, images[i]->elem_num, 1, fp);
    }

    fclose(fp);

    *_images = images;
    *_image_num = number_of_images;
}


// 读入标签
void nc_read_mnist_label(const char* filename, int** _labels, int* _label_num)
{
    FILE* fp = fopen(filename, "rb");
    CHECK_READ_FILE(fp, filename);

    int magic_number = 0;
    int number_of_labels = 0;
    //int label_long = 10;

    //从文件中读取sizeof(magic_number) 个字符到 &magic_number
    fread((char*)&magic_number, sizeof(magic_number), 1, fp);
    magic_number = reverse_int(magic_number);
    //获取训练或测试image的个数number_of_images
    fread((char*)&number_of_labels, sizeof(number_of_labels), 1, fp);
    number_of_labels = reverse_int(number_of_labels);


    // 图像标记数组的初始化
    int* labels = (int*)malloc(sizeof(int)*number_of_labels);

    for (int i = 0; i < number_of_labels; i++) {
        unsigned char temp = 0;
        fread(&temp, sizeof(temp), 1, fp);
        labels[i] = temp;
    }

    fclose(fp);

    *_labels = labels;
    *_label_num = number_of_labels;
}

NcLayer* nc_make_layer() {
    NcLayer* layer = (NcLayer*)malloc(sizeof(NcLayer));
    layer->backward = NULL;
    layer->forward = NULL;
    layer->infer = NULL;
    layer->input = NULL;
    layer->output = NULL;
    layer->param = NULL;
    layer->type = NC_LAYER_UNKNOWN;
    return layer;
}


void nc_blob_nchw_to_nhwc() {
    //TODO
}

void nc_blob_nhwc_to_nchw() {
    //TODO
}
