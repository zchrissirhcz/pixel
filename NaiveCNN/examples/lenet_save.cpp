#include "cnn.h"
#include "lenet.h"
#include "px_cnn.h"
#include "px_typebase.h"

static void save_blob1d_to_file(float* data, int len, FILE* fout)
{
    fwrite(data, sizeof(float), len, fout);
}

static void save_blob2d_to_file(float** data, px_size_t size, FILE* fout)
{
    for (int i = 0; i < size.height; i++)
    {
        save_blob1d_to_file(data[i], size.width, fout);
    }
}

static void save_blob3d_to_file(float*** data, px_cube_dim_t cube_dim, FILE* fout)
{
    px_size_t size = px_create_size(cube_dim.height, cube_dim.width);
    for (int i = 0; i < cube_dim.channel; i++)
    {
        save_blob2d_to_file(data[i], size, fout);
    }
}

static void save_blob4d_to_file(float**** data, px_tensor_dim_t tensor_dim, FILE* fout)
{
    px_cube_dim_t cube_dim = px_create_cube_dim(tensor_dim.channel, tensor_dim.height, tensor_dim.width);
    for (int i = 0; i < tensor_dim.batch; i++)
    {
        save_blob3d_to_file(data[i], cube_dim, fout);
    }
}

static void save_lenet_input_data(Lenet* net, FILE* fout, float** inputdata)
{
    px_size_t size = px_create_size(net->C1->in_height, net->C1->in_width);
    save_blob2d_to_file(inputdata, size, fout);
}

static void save_conv_weight_data(ConvLayer* conv_layer, FILE* fout)
{
    px_tensor_dim_t tensor_dim = px_create_tensor_dim(conv_layer->in_channels, conv_layer->out_channels, conv_layer->map_size, conv_layer->map_size);
    save_blob4d_to_file(conv_layer->mapData, tensor_dim, fout);
}

static void save_bias_data(float* bias, int len, FILE* fout)
{
    save_blob1d_to_file(bias, len, fout);
}

static void save_conv_layer_inference_data(ConvLayer* conv_layer, FILE* fout)
{
    save_conv_weight_data(conv_layer, fout);
    save_bias_data(conv_layer->biasData, conv_layer->out_channels, fout);
}

static void save_innerproduct_weight_data(InnerproductLayer* innerproduct_layer, FILE* fout)
{
    const int height = innerproduct_layer->outputNum;
    const int width = innerproduct_layer->inputNum;
    px_size_t size = px_create_size(height, width);
    save_blob2d_to_file(innerproduct_layer->wData, size, fout);
}

static void save_innerproduct_layer_inference_data(InnerproductLayer* ip_layer, FILE* fout)
{
    save_innerproduct_weight_data(ip_layer, fout);
    save_bias_data(ip_layer->biasData, ip_layer->outputNum, fout);
}

static void save_conv_layer_train_data(ConvLayer* conv_layer, px_size_t output_size, FILE* fout)
{
    save_conv_layer_inference_data(conv_layer, fout);

    const int channel = conv_layer->out_channels;
    const int height = output_size.height;
    const int width = output_size.width;
    px_cube_dim_t cube_dim = px_create_cube_dim(channel, height, width);
    save_blob3d_to_file(conv_layer->v, cube_dim, fout);
    save_blob3d_to_file(conv_layer->d, cube_dim, fout);
    save_blob3d_to_file(conv_layer->y, cube_dim, fout);
}

static void save_lenet_C1_layer_train_data(Lenet* net, FILE* fout)
{
    const int out_height = net->S2->in_height;
    const int out_width = net->S2->in_width;
    px_size_t output_size = px_create_size(out_height, out_width);
    save_conv_layer_train_data(net->C1, output_size, fout);
}

static void save_lenet_C3_layer_train_data(Lenet* net, FILE* fout)
{
    const int out_height = net->S4->in_height;
    const int out_width = net->S4->in_width;
    px_size_t output_size = px_create_size(out_height, out_width);
    save_conv_layer_train_data(net->C3, output_size, fout);
}

static void save_pooling_layer_data(PoolingLayer* pool_layer, px_size_t output_size, FILE* fout)
{
    const int channel = pool_layer->out_channels;
    const int height = output_size.height;
    const int width = output_size.width;
    px_cube_dim_t cube_dim = px_create_cube_dim(channel, height, width);
    save_blob3d_to_file(pool_layer->d, cube_dim, fout);
    save_blob3d_to_file(pool_layer->y, cube_dim, fout);
}

static void save_lenet_S2_layer_train_data(Lenet* net, FILE* fout)
{
    const int out_height = net->C3->in_height;
    const int out_width = net->C3->in_width;
    px_size_t output_size = px_create_size(out_height, out_width);
    save_pooling_layer_data(net->S2, output_size, fout);
}

static void save_lenet_S4_layer_train_data(Lenet* net, FILE* fout)
{
    const int out_height = net->S4->in_height / net->S4->map_size;
    const int out_width = net->S4->in_width / net->S4->map_size;
    px_size_t output_size = px_create_size(out_height, out_width);
    save_pooling_layer_data(net->S4, output_size, fout);
}

static void save_lenet_O5_layer_train_data(Lenet* net, FILE* fout)
{
    save_innerproduct_layer_inference_data(net->O5, fout);

    const int height = net->O5->outputNum;
    const int width = net->O5->inputNum;
    px_size_t size = px_create_size(height, width);
    save_blob1d_to_file(net->O5->v, net->O5->outputNum, fout);
    save_blob1d_to_file(net->O5->d, net->O5->outputNum, fout);
    save_blob1d_to_file(net->O5->y, net->O5->outputNum, fout);
}

// 这是用于测试的函数, 保存CNN网络中的相关数据
void save_lenet_train_data(Lenet* net, const char* filename, float** inputdata)
{
    FILE* fout = fopen(filename, "wb");
    CHECK_WRITE_FILE(fout, filename);

    save_lenet_input_data(net, fout, inputdata);
    save_lenet_C1_layer_train_data(net, fout);
    save_lenet_S2_layer_train_data(net, fout);
    save_lenet_C3_layer_train_data(net, fout);
    save_lenet_S4_layer_train_data(net, fout);
    save_lenet_O5_layer_train_data(net, fout);

    fclose(fout);
}

void save_lenet_inference_data(Lenet* net, const char* filename)
{
    FILE* fout = fopen(filename, "wb");
    CHECK_WRITE_FILE(fout, filename);

    save_conv_layer_inference_data(net->C1, fout);
    save_conv_layer_inference_data(net->C3, fout);
    save_innerproduct_layer_inference_data(net->O5, fout);

    fclose(fout);
}