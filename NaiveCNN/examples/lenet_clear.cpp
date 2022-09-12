
#include "lenet.h"
#include "px_cnn.h"

static void clear_conv_layer(ConvLayer* conv_layer, px_size_t output_size)
{
    const int channel = conv_layer->out_channels;
    const int height = output_size.height;
    const int width = output_size.width;
    px_cube_dim_t cube_dim = px_create_cube_dim(channel, height, width);
    
    clear_blob3d(conv_layer->d, cube_dim);
    clear_blob3d(conv_layer->v, cube_dim);
    clear_blob3d(conv_layer->y, cube_dim);
}

static void clear_lenet_C1_layer(Lenet* net)
{
    const int out_height = net->S2->in_height;
    const int out_width = net->S2->in_width;
    px_size_t output_size = px_create_size(out_height, out_width);
    clear_conv_layer(net->C1, output_size);
}

static void clear_pooling_layer(PoolingLayer* pool_layer, px_size_t output_size)
{
    const int channel = pool_layer->out_channels;
    const int height = output_size.height;
    const int width = output_size.width;
    px_cube_dim_t cube_dim = px_create_cube_dim(channel, height, width);

    clear_blob3d(pool_layer->d, cube_dim);
    clear_blob3d(pool_layer->y, cube_dim);
}

static void clear_lenet_S2_layer(Lenet* net)
{
    const int out_height = net->C3->in_height;
    const int out_width = net->C3->in_width;
    px_size_t output_size = px_create_size(out_height, out_width);
    clear_pooling_layer(net->S2, output_size);
}

static void clear_lenet_C3_layer(Lenet* net)
{
    const int out_height = net->S4->in_height;
    const int out_width = net->S4->in_width;
    px_size_t output_size = px_create_size(out_height, out_width);
    clear_conv_layer(net->C3, output_size);
}

static void clear_lenet_S4_layer(Lenet* net)
{
    const int out_height = net->S4->in_height / net->S4->map_size;
    const int out_width = net->S4->in_width / net->S4->map_size;
    px_size_t output_size = px_create_size(out_height, out_width);
    clear_pooling_layer(net->S4, output_size);
}

static void clear_lenet_O5_layer(Lenet* net)
{
    const int len = net->O5->outputNum;
    clear_blob1d(net->O5->d, len);
    clear_blob1d(net->O5->v, len);
    clear_blob1d(net->O5->y, len);
}

// 将神经元的部分数据清除
void clear_lenet(Lenet* net)
{
    clear_lenet_C1_layer(net);
    clear_lenet_S2_layer(net);
    clear_lenet_C3_layer(net);
    clear_lenet_S4_layer(net);
    clear_lenet_O5_layer(net);
}
