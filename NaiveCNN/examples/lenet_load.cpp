#include "lenet.h"
#include "px_cnn.h"
#include "px_filesystem.h"

static void load_blob1d(float* data, int len, FILE* fin)
{
    fread(data, sizeof(float) * len, 1, fin);
}

static void load_blob2d(float** data, px_size_t size, FILE* fin)
{
    for (int i = 0; i < size.height; i++)
    {
        load_blob1d(data[i], size.width, fin);
    }
}

static void load_blob3d(float*** data, px_cube_dim_t cube_dim, FILE* fin)
{
    px_size_t size = px_create_size(cube_dim.height, cube_dim.width);
    for (int i = 0; i < cube_dim.channel; i++)
    {
        load_blob2d(data[i], size, fin);
    }
}


static void load_bias(float* data, int len, FILE* fin)
{
    load_blob1d(data, len, fin);
}

static void load_weight4d(float**** weight, px_tensor_dim_t tensor_dim, FILE* fin)
{
    px_cube_dim_t cube_dim = px_create_cube_dim(tensor_dim.channel, tensor_dim.height, tensor_dim.width);
    for (int i = 0; i < tensor_dim.batch; i++)
    {
        load_blob3d(weight[i], cube_dim, fin);
    }
}

static void load_weight2d(float** weight, px_size_t size, FILE* fin)
{
    load_blob2d(weight, size, fin);
}


static void load_lenet_C1_data(Lenet* net, FILE* fin)
{
    const int batch = net->C1->in_channels;
    const int channel = net->C1->out_channels;
    const int height = net->C1->map_size;
    const int width = net->C1->map_size;
    px_tensor_dim_t tensor_dim = px_create_tensor_dim(batch, channel, height, width);
    load_weight4d(net->C1->mapData->data, tensor_dim, fin);
    load_bias(net->C1->biasData->data, net->C1->out_channels, fin);
}

static void load_lenet_C3_data(Lenet* net, FILE* fin)
{
    const int batch = net->C3->in_channels;
    const int channel = net->C3->out_channels;
    const int height = net->C3->map_size;
    const int width = net->C3->map_size;
    px_tensor_dim_t tensor_dim = px_create_tensor_dim(batch, channel, height, width);
    load_weight4d(net->C3->mapData->data, tensor_dim, fin);

    load_bias(net->C3->biasData->data, net->C3->out_channels, fin);
}

static void load_lenet_O5_data(Lenet* net, FILE* fin)
{
    const int height = net->O5->outputNum;
    const int width = net->O5->inputNum;
    px_size_t size = px_create_size(height, width);
    load_weight2d(net->O5->wData, size, fin);

    load_bias(net->O5->biasData->data, net->O5->outputNum, fin);
}

void load_lenet(Lenet* net, const char* filename)
{
    FILE* fin = fopen(filename, "rb");
    PX_CHECK_READ_FILE(fin, filename);

    load_lenet_C1_data(net, fin);
    load_lenet_C3_data(net, fin);
    load_lenet_O5_data(net, fin);

    fclose(fin);
}
