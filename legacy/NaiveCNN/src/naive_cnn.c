#include "naive_cnn.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include <assert.h>
#include "px_filesystem.h"
#include "px_assert.h"

float nc_get_random_float(float min, float max)
{
    float v = (float)(rand()) / (double)RAND_MAX;
    v = v * (max - min) + min;
    return v;
}

NcBlob* nc_create_empty_blob(int batch, int height, int width, int channel)
{
    NcBlob* blob = (NcBlob*)malloc(sizeof(NcBlob));
    blob->batch = batch;
    blob->height = height;
    blob->width = width;
    blob->channel = channel;
    blob->nstep = height * width * channel; //TODO: do align here
    blob->mass = blob->nstep * batch;
    blob->order = NHWC; //TODO:
    blob->data = NULL;
    blob->rely_cnt_total = 0;
    blob->rely_cnt = 0;
    return blob;
}

NcBlob* nc_create_blob(int n, int h, int w, int c)
{
    NcBlob* blob = nc_create_empty_blob(n, h, w, c);
    blob->data = (float*)calloc(blob->mass, sizeof(float));
    return blob;
}

NcBlob* nc_create_blob3d(int h, int w, int c)
{
    return nc_create_blob(1, h, w, c);
}

NcBlob* nc_create_blob2d(int h, int w)
{
    return nc_create_blob(1, h, w, 1);
}

NcBlob* nc_create_blob_random(int batch, int height, int width, int channel, float s, float t)
{
    NcBlob* blob = nc_create_blob(batch, height, width, channel);

    srand((unsigned)time(NULL));
    for (int i = 0; i < blob->mass; i++)
    {
        blob->data[i] = nc_get_random_float(s, t);
    }
    return blob;
}

NcBlob* nc_create_blob_same(int batch, int height, int width, int channel, float value)
{
    NcBlob* blob = nc_create_blob(batch, height, width, channel);

    srand((unsigned)time(NULL));
    for (int i = 0; i < blob->mass; i++)
    {
        blob->data[i] = value;
    }
    return blob;
}

NcBlob* nc_create_blob2d_empty(int h, int w)
{
    NcBlob* blob = nc_create_empty_blob(1, h, w, 1);
    return blob;
}

// change a blob's geometry properties:
// free a blob's already allocated data buffer,
// and re-assigne a different dimenssions, then re-allocated data buffer
// the blob's logical properties such as rely_cnt_total
void nc_blob_data_realloc(NcBlob* blob, int batch, int height, int width, int channel)
{
    if (blob->data)
    {
        free(blob->data);
        blob->data = NULL;
    }
    blob->batch = batch;
    blob->height = height;
    blob->width = width;
    blob->channel = channel;
    blob->nstep = height * width * channel; //TODO: consider align here
    blob->mass = blob->nstep * batch;
    blob->data = (float*)calloc(blob->mass, sizeof(float));
}

void nc_blob_data_realloc3d(NcBlob* blob, int h, int w, int c)
{
    nc_blob_data_realloc(blob, 1, h, w, c);
}

void nc_destroy_layer(NcLayer* layer)
{
    NcLayerType layer_type = layer->type;
    switch (layer_type)
    {
    case NC_LAYER_CONVOLUTION:
    {
        //PX_LOGE("-- free convolution layer\n");
        NcConvolutionParam* param = (NcConvolutionParam*)layer->param;
        free(param->bias);
        //puts("\t free bias ok");

        free(param->weight);
        //puts("\t free weight ok");

        if (param->d) free(param->d);
        //puts("\t free d ok");

        if (param->d_weight) free(param->d_weight);
        //puts("\t free d_weight ok");

        if (param->v) free(param->v);
        //puts("\t free v ok");

        if (param->y) free(param->y);
        //puts("\t free y ok");

        free(param);
        //puts("\t free param ok");
    }
    break;
    case NC_LAYER_POOLING:
    {
        //PX_LOGE("-- free poolingg layer\n");
        NcPoolingParam* param = (NcPoolingParam*)layer->param;

        if (param->d) free(param->d);
        //puts("\t free d ok");

        if (param->y) free(param->y);
        //puts("\t free y ok");

        free(param);
        //puts("\t free param ok");
    }
    break;
    case NC_LAYER_INNERPRODUCT:
    {
        //PX_LOGE("-- free innerproduct layer\n");
        NcInnerproductParam* param = (NcInnerproductParam*)layer->param;
        //puts("\t-- try to free bias");
        free(param->bias);
        //puts("\t free bias ok");

        free(param->weight);
        //puts("\t free weight ok");

        if (param->d) free(param->d);
        //puts("\t free d ok");

        if (param->v) free(param->v);
        //puts("\t free v ok");

        if (param->y) free(param->y);
        //puts("\t free y ok");

        free(param);
        //puts("\t free param ok");
    }
    break;
    default:
    {
        fprintf(stderr, "Error! Not supported layer param code %d\n", layer_type);
    }
    }
}

void nc_net_forward(NcNet* net, px_image_t* image, int label)
{
    NcLayer* layer = NULL;
    for (int i = 0; i < net->layers_num; i++)
    {
        layer = net->layers[i];
        PX_LOGE("\t-> layer: %d/%d ", i, net->layers_num);
        layer->forward(layer->param, layer->input, layer->output);
    }
}

void nc_net_backward(NcNet* net)
{
    NcLayer* layer = NULL;
    for (int i = 0; i < net->layers_num; i++)
    {
        layer = net->layers[i];
        PX_LOGE("\t<= layer: %d/%d ", i, net->layers_num);
        layer->backward(layer->param, layer->input, layer->output);
    }
}

// train a classification network
void nc_train_cls_net(NcNet* net, NcTrainConfig* train_cfg, LenetDataConfig* data_cfg)
{
    // setup log
    FILE* fout = fopen(train_cfg->log_pth, "w");
    PX_CHECK_WRITE_FILE(fout, train_cfg->log_pth);

    px_image_t* image = NULL;
    int label = 0;
    for (int i = 0; i < train_cfg->num_epoch; i++)
    {
        for (int j = 0; j < train_cfg->train_num; j++)
        {
            image = data_cfg->images[i];
            label = data_cfg->labels[i];
            PX_LOGE("Iteration %d/%d\n", j, train_cfg->train_num);
            nc_net_forward(net, image, label);
            //nc_net_backward(net);
        }
    }

    fclose(fout);
}

NcLayer* nc_create_layer()
{
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

void nc_blob_nchw_to_nhwc()
{
    //TODO
}

void nc_blob_nhwc_to_nchw()
{
    //TODO
}

const char* layer_type_to_str(NcLayerType type)
{
    switch (type)
    {
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

void nc_swap_rgb_and_bgr_inplace(px_image_t* im)
{
    PX_ASSERT(im->channel == 3);

    const int cn = im->channel;
    for (int i = 0; i < im->height; i++)
    {
        uint8_t* ptr = im->data + i * im->width * im->channel;
        for (int j = 0; j < im->width; j++)
        {
            uint8_t t = ptr[0];
            ptr[0] = ptr[2];
            ptr[2] = t;
            ptr += cn;
        }
    }
}
