#include "naive_cnn.h"
#include <stdlib.h>

#include "load_prefix.h"
#include "px_mnist.h"
#include "px_assert.h"

#include <string.h>
#include <vector>

#define NC_AvePool 0
#define NC_MaxPool 1
#define NC_MinPool 2

void nc_lenet5_train_setup(NcNet* net, px_size_t input_size, int output_size)
{
    //px_size_t input_size;
    //input_size.h = 28; input_size.w = 28;
    //int output_size = 10;
    //NcNet* net = (NcNet*)malloc(sizeof(NcNet));

    net->layers_num = 5;
    net->layers = (NcLayer**)malloc(sizeof(NcLayer*) * net->layers_num);
    for (int i = 0; i < net->layers_num; i++)
    {
        net->layers[i] = NULL;
    }

    net->blobs_num = 6;
    net->blobs = (NcBlob**)malloc(sizeof(NcBlob*) * net->blobs_num);
    for (int i = 0; i < net->blobs_num; i++)
    {
        net->blobs[i] = NULL;
    }

    char fout_pth[NC_MAX_PATH];
    sprintf(fout_pth, "%s/debug/xxx.txt", project_dir);
    FILE* fout = fopen(fout_pth, "w");
    if (net->blobs[1] == NULL)
    {
        fprintf(fout, "-- NULL\n");
    }
    else
    {
        puts("-- Not NULL\n");
    }
    fclose(fout);
    //------------------------
    // 指针应当赋予初始值。否则，debug模式下不会默认设定为NULL。
    //------------------------

    // allocate network's input blobs
    net->blobs[0] = nc_create_blob3d(28, 28, 1);

    px_size_t in_size;
    int map_size;
    int in_channels;
    int out_channels;
    int pool_type;
    std::vector<int> input_blobs_ids;
    std::vector<int> output_blobs_ids;
    NcBlob* output_blobs[NC_MAX_BLOB_PER_LAYER];
    for (int i = 0; i < NC_MAX_BLOB_PER_LAYER; i++)
    {
        output_blobs[i] = NULL;
    }

    NcLayer* layer = NULL;

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
    input_blobs_ids = {1};
    layer->input = nc_train_create_layer_input(input_blobs_ids.size(), input_blobs_ids.data(), net);

    // setup layer output
    // for each layer's output blobs, some may be allocated (in-place operation, like relu)
    // however, some are not allocated, such as convolution, pooling
    // it's each type of layer's reponsibility to calculate and allocate the un-allocated blob
    output_blobs_ids = {1};
    output_blobs[0] = nc_create_blob3d(C1->out_height, C1->out_width, C1->out_channels);
    layer->output = nc_train_create_layer_output(output_blobs_ids.size(), output_blobs_ids.data(), net, output_blobs);

    //----------------------------------------------------------------
    // S2 layer
    //----------------------------------------------------------------
    // create layer param
    in_size.height = C1->out_height;
    in_size.width = C1->out_width;
    map_size = 2;
    in_channels = C1->out_channels;
    out_channels = 6;
    pool_type = NC_AvePool;
    NcPoolingParam* S2 = nc_train_create_pooling_param(in_size.height, in_size.width, map_size, in_channels, out_channels, pool_type);

    // create layer
    layer = nc_create_layer();
    layer->type = NC_LAYER_POOLING;
    layer->param = S2;
    layer->forward = nc_train_forward_pooling;
    layer->backward = nc_train_backward_pooling;
    net->layers[1] = layer;

    // setup layer input
    input_blobs_ids = {1};
    layer->input = nc_train_create_layer_input(input_blobs_ids.size(), input_blobs_ids.data(), net);

    // setup layer output
    output_blobs_ids = {2};
    output_blobs[0] = nc_create_blob3d(S2->out_height, S2->out_width, S2->out_channels);
    layer->output = nc_train_create_layer_output(output_blobs_ids.size(), output_blobs_ids.data(), net, output_blobs);

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
    input_blobs_ids = {2};
    layer->input = nc_train_create_layer_input(input_blobs_ids.size(), input_blobs_ids.data(), net);

    // setup layer output
    output_blobs_ids = {3};
    output_blobs[0] = nc_create_blob3d(C3->out_height, C3->out_width, C3->out_channels);
    layer->output = nc_train_create_layer_output(output_blobs_ids.size(), output_blobs_ids.data(), net, output_blobs);

    //----------------------------------------------------------------
    // S4 layer
    //----------------------------------------------------------------
    // create layer param
    in_size.height = C3->out_height;
    in_size.width = C3->out_width;
    map_size = 2;
    in_channels = C3->out_channels;
    out_channels = 12;
    pool_type = NC_AvePool;
    NcPoolingParam* S4 = nc_train_create_pooling_param(in_size.height, in_size.width, map_size, in_channels, out_channels, pool_type);

    // create layer
    layer = nc_create_layer();
    layer->type = NC_LAYER_POOLING;
    layer->param = S4;
    layer->forward = nc_train_forward_pooling;
    layer->backward = nc_train_backward_pooling;
    net->layers[3] = layer;

    // setup layer input
    input_blobs_ids = {3};
    layer->input = nc_train_create_layer_input(input_blobs_ids.size(), input_blobs_ids.data(), net);

    // setup layer output
    output_blobs_ids = {4};
    output_blobs[0] = nc_create_blob3d(S4->out_height, S4->out_width, S4->out_channels);
    layer->output = nc_train_create_layer_output(output_blobs_ids.size(), output_blobs_ids.data(), net, output_blobs);

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
    input_blobs_ids = {4};
    layer->input = nc_train_create_layer_input(input_blobs_ids.size(), input_blobs_ids.data(), net);

    // setup layer output
    output_blobs_ids = {5};
    output_blobs[0] = nc_create_blob3d(O5->out_height, O5->out_width, O5->out_channels);
    layer->output = nc_train_create_layer_output(output_blobs_ids.size(), output_blobs_ids.data(), net, output_blobs);

    // --- Loss
    net->e = (float*)calloc(out_num, sizeof(float));

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
    for (int i = 0; i < net->layers_num; i++)
    {
        layer = net->layers[i];
        PX_LOGE("-- layer[%d]'s type: %s\n", i, layer_type_to_str(layer->type));
    }
}

void nc_train_trial()
{
    px_size_t input_size;
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
    LenetDataConfig data_cfg;
    sprintf(data_cfg.dataset, "%s", "MNIST");
    sprintf(data_cfg.splitset, "%s", "train");
    nc_cls_data_loader(&data_cfg);

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
    //    train_lenet_on_mnist(cnn,trainImg,trainLabel,opts,trainNum, fout);
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
    for (int i = 0; i < net->layers_num; i++)
    {
        layer = net->layers[i];
        nc_destroy_layer(layer);
    }
    printf("=== free layers OK\n");
    free(net->layers);
    free(net->e);
    free(net);
}

void nc_lenet5_infer_setup(NcNet* net)
{
    net->layers_num = 5;
    net->layers = (NcLayer**)malloc(sizeof(NcLayer*) * net->layers_num);
    for (int i = 0; i < net->layers_num; i++)
    {
        net->layers[i] = nc_create_layer();
    }

    net->blobs_num = 6;
    net->blobs = (NcBlob**)malloc(sizeof(NcBlob*) * net->blobs_num);
    for (int i = 0; i < net->blobs_num; i++)
    {
        net->blobs[i] = nc_create_empty_blob(0, 0, 0, 0);
    }

    int map_size;
    int in_channels;
    int out_channels;
    NcPoolingType pool_type;
    int input_blobs_num;
    int output_blobs_num;

    std::vector<int> input_blob_ids;
    std::vector<int> output_blob_ids;

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
    input_blob_ids = {1};
    input_blob_ids[0] = 0;
    layer->input = nc_infer_create_layer_input(input_blob_ids.size(), input_blob_ids.data(), net);

    // setup layer output
    output_blob_ids = {1};
    layer->output = nc_infer_create_layer_output(output_blob_ids.size(), output_blob_ids.data(), net);

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
    input_blob_ids = {1};
    layer->input = nc_infer_create_layer_input(input_blob_ids.size(), input_blob_ids.data(), net);

    // setup layer output
    output_blob_ids = {1};
    layer->output = nc_infer_create_layer_output(output_blob_ids.size(), output_blob_ids.data(), net);

    // create layer param
    map_size = 2;
    in_channels = C1->out_channels;
    out_channels = C1->out_channels;
    pool_type = NC_POOLING_AVERAGE;
    NcPaddingType pad_type = NC_PADDING_CONV_CAFFE; // TODO: fix this
    NcPoolingParam* S2 = nc_infer_create_pooling_param(map_size, in_channels, pool_type, pad_type);
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
    input_blob_ids = {2};
    layer->input = nc_infer_create_layer_input(input_blob_ids.size(), input_blob_ids.data(), net);

    // setup layer output
    output_blob_ids = {3};
    layer->output = nc_infer_create_layer_output(output_blob_ids.size(), output_blob_ids.data(), net);

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
    input_blob_ids = {3};
    layer->input = nc_infer_create_layer_input(input_blob_ids.size(), input_blob_ids.data(), net);

    // setup layer output
    output_blob_ids = {4};
    layer->output = nc_infer_create_layer_output(output_blob_ids.size(), output_blob_ids.data(), net);

    // create layer param
    map_size = 2;
    in_channels = C3->out_channels;
    out_channels = 12;
    pool_type = NC_POOLING_AVERAGE;
    NcPoolingParam* S4 = nc_infer_create_pooling_param(map_size, in_channels, pool_type, pad_type);
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
    input_blob_ids = {4};
    layer->input = nc_infer_create_layer_input(input_blob_ids.size(), input_blob_ids.data(), net);

    // setup layer output
    output_blob_ids = {5};
    layer->output = nc_infer_create_layer_output(output_blob_ids.size(), output_blob_ids.data(), net);

    // setup layer param
    int in_num = layer->input->blobs[0]->nstep;
    int out_num = 10; // num classes
    NcInnerproductParam* O5 = nc_infer_create_innerproduct_param(in_num, out_num);
    layer->param = O5;

    // --- Loss
    net->e = NULL;

    // --- print each layer's type
    for (int i = 0; i < net->layers_num; i++)
    {
        layer = net->layers[i];
        PX_LOGE("-- layer[%d]'s type: %s\n", i, layer_type_to_str(layer->type));
    }
}

void nc_infer(NcNet* net, px_image_t* image)
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

// load data here
void nc_cls_data_loader(LenetDataConfig* cfg)
{
    if (strcmp(cfg->dataset, "MNIST") == 0)
    {
        if (strcmp(cfg->splitset, "train") == 0)
        {
            char train_image_pth[NC_MAX_PATH];
            sprintf(train_image_pth, "%s/mnist/train-images.idx3-ubyte", project_dir);

            px_mnist_image_array_t* image_array = px_read_mnist_image(train_image_pth);
            cfg->images = &image_array->images;

            int* labels;
            int label_num;
            char train_label_pth[NC_MAX_PATH];
            sprintf(train_label_pth, "%s/mnist/train-labels.idx1-ubyte", project_dir);
            px_mnist_label_array_t* label_array = px_read_mnist_label(train_label_pth);

            PX_ASSERT(label_array->size == image_array->size);

            cfg->labels = label_array->label;
            cfg->num = image_array->size;

            // dump images and labels
            // the result is correct
#ifdef DUMP_MNIST_TRAIN
            char save_pth[NC_MAX_PATH];
            for (int i = 0; i < image_num; i++)
            {
                memset(save_pth, 0, NC_MAX_PATH);
                sprintf(save_pth, "%s/debug/mnist_train/%d_%d.bmp", project_dir, i, labels[i]);

                FILE* fp = fopen(save_pth, "wb");
                unsigned int err = loadbmp_encode_file(save_pth, images[i]->data, images[i]->w, images[i]->h, 1);

                if (err)
                {
                    PX_LOGE("LoadBMP Load Error: %u\n", err);
                }
                fclose(fp);
            }
#endif
        }
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
    LenetDataConfig data_cfg;
    sprintf(data_cfg.dataset, "%s", "MNIST");
    sprintf(data_cfg.splitset, "%s", "train");
    nc_cls_data_loader(&data_cfg);

    // ---! infer
    //for (int i = 0; i < data_cfg.num; i++) {
    for (int i = 0; i < 1; i++)
    {
        px_image_t* image = data_cfg.images[i];
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

int main()
{
    nc_train_trial();
    //nc_infer_trial();

    //nc_convolution_test_nchw();
    //nc_convolution_test_nhwc();

    //nc_pooling_test_nchw();
    //nc_pooling_test_nhwc();

    return 0;
}