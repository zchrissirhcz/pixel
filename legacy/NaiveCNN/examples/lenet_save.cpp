#include "cnn.h"
#include "lenet.h"
#include "mat.h"
#include "px_cnn.h"
#include "px_typebase.h"
#include "px_filesystem.h"


static void save_lenet_input_data(Lenet* net, FILE* fout, matrix_t* inputdata)
{
    save_matrix_to_file(inputdata, fout);
}

static void save_conv_weight_data(ConvLayer* conv_layer, FILE* fout)
{
    // px_tensor_dim_t tensor_dim = px_create_tensor_dim(conv_layer->in_channels, conv_layer->out_channels, conv_layer->map_size, conv_layer->map_size);
    save_tensor_to_file(conv_layer->kernel, fout);
}

static void save_bias_data(array_t* bias, FILE* fout)
{
    save_array_to_file(bias, fout);
}

static void save_conv_layer_inference_data(ConvLayer* conv_layer, FILE* fout)
{
    save_conv_weight_data(conv_layer, fout);
    // conv_layer->out_channels
    save_bias_data(conv_layer->bias, fout);
}

static void save_innerproduct_weight_data(InnerproductLayer* innerproduct_layer, FILE* fout)
{
    const int height = innerproduct_layer->output_num;
    const int width = innerproduct_layer->input_num;
    px_size_t size = px_create_size(height, width);
    save_matrix_to_file(innerproduct_layer->weight, fout);
}

static void save_innerproduct_layer_inference_data(InnerproductLayer* ip_layer, FILE* fout)
{
    save_innerproduct_weight_data(ip_layer, fout);
    // ip_layer->outputNum
    save_bias_data(ip_layer->bias, fout);
}

static void save_conv_layer_train_data(ConvLayer* conv_layer, px_size_t output_size, FILE* fout)
{
    save_conv_layer_inference_data(conv_layer, fout);

    const int channel = conv_layer->out_channels;
    const int height = output_size.height;
    const int width = output_size.width;
    px_cube_dim_t cube_dim = px_create_cube_dim(channel, height, width);
    save_cube_to_file(conv_layer->v, fout);
    save_cube_to_file(conv_layer->d, fout);
    save_cube_to_file(conv_layer->y, fout);
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
    save_cube_to_file(pool_layer->d, fout);
    save_cube_to_file(pool_layer->y, fout);
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

    const int height = net->O5->output_num;
    const int width = net->O5->input_num;
    px_size_t size = px_create_size(height, width);
    save_array_to_file(net->O5->v, fout);
    save_array_to_file(net->O5->d, fout);
    save_array_to_file(net->O5->y, fout);
}

// 这是用于测试的函数, 保存CNN网络中的相关数据
void save_lenet_train_data(Lenet* net, const char* filename, matrix_t* inputdata)
{
    FILE* fout = fopen(filename, "wb");
    PX_CHECK_WRITE_FILE(fout, filename);

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
    PX_CHECK_WRITE_FILE(fout, filename);

    save_conv_layer_inference_data(net->C1, fout);
    save_conv_layer_inference_data(net->C3, fout);
    save_innerproduct_layer_inference_data(net->O5, fout);

    fclose(fout);
}