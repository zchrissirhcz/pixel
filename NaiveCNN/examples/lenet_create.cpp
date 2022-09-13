#include "lenet.h"

static void create_lenet_C1_layer(Lenet* net)
{
    px_size_t in_size;
    in_size.width = net->input_width;
    in_size.height = net->input_height;
    px_size_t stride = px_create_size(1, 1);
    px_pad_t pad = px_create_pad(0, 0, 0, 0);
    net->C1 = init_conv_layer(in_size, 5, stride, pad, 1, 6);
}

static void create_lenet_S2_layer(Lenet* net)
{
    px_size_t in_size;
    in_size.width = net->C1->out_width;
    in_size.height = net->C1->out_height;
    px_size_t stride = px_create_size(2, 2);
    px_pad_t pad = px_create_pad(0, 0, 0, 0);
    net->S2 = init_pooling_layer(in_size, 2, stride, pad, 6, 6, NC_AvePool);
}

static void create_lenet_C3_layer(Lenet* net)
{
    px_size_t in_size;
    in_size.width = net->S2->out_width;
    in_size.height = net->S2->out_height;
    px_size_t stride = px_create_size(1, 1);
    px_pad_t pad = px_create_pad(0, 0, 0, 0);
    net->C3 = init_conv_layer(in_size, 5, stride, pad, 6, 12);
}

static void create_lenet_S4_layer(Lenet* net)
{
    px_size_t in_size;
    in_size.width = net->C3->out_width;
    in_size.height = net->C3->out_height;
    px_size_t stride = px_create_size(2, 2);
    px_pad_t pad = px_create_pad(0, 0, 0, 0);
    net->S4 = init_pooling_layer(in_size, 2, stride, pad, 12, 12, NC_AvePool);
}

static void create_lenet_O5_layer(Lenet* net)
{
    px_size_t in_size = px_create_size(net->S4->out_height, net->S4->out_width);
    int output_num = net->output_num;
    net->O5 = init_innerproduct_layer(in_size.width * in_size.height * 12, output_num);
}

Lenet* create_lenet(px_size_t input_size, int output_num)
{
    Lenet* net = (Lenet*)malloc(sizeof(Lenet));
    memset(net, 0, sizeof(Lenet));

    net->layerNum = 5;
    net->input_height = input_size.height;
    net->input_width = input_size.width;
    net->output_num = output_num;

    px_size_t inSize;
    int mapSize = 5;
    
    create_lenet_C1_layer(net);
    create_lenet_S2_layer(net);
    create_lenet_C3_layer(net);
    create_lenet_S4_layer(net);
    create_lenet_O5_layer(net);

    net->err = (float*)calloc(net->O5->output_num, sizeof(float));

    return net;
}
