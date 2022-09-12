#include "cnn.h"
#include "lenet.h"

static void forward_lenet_C1_layer(Lenet* net, matrix_t* input)
{
    // 第一层输出数据
    px_size_t inSize = px_create_size(net->C1->in_height, net->C1->in_width);
    px_size_t mapSize = px_create_size(net->C1->map_size, net->C1->map_size);
    px_size_t outSize = px_create_size(net->S2->in_height, net->S2->in_width);
    for (int i = 0; i < net->C1->out_channels; i++)
    {
        for (int j = 0; j < net->C1->in_channels; j++)
        {
            matrix_t map;
            map.height = mapSize.height;
            map.width = mapSize.width;
            map.data = net->C1->mapData->data[j][i];
            matrix_t* mapout = conv(&map, input, NC_VALID);

            matrix_t res;
            res.height = mapSize.height;
            res.width = mapSize.width;
            res.data = net->C1->v->data[i];

            matrix_t mat1;
            mat1.height = outSize.height;
            mat1.width = outSize.width;
            mat1.data = net->C1->v->data[i];

            addmat(&mat1, mapout, &res);
            destroy_matrix_ptr(mapout);
        }
        for (int r = 0; r < outSize.height; r++)
        {
            for (int c = 0; c < outSize.width; c++)
            {
                net->C1->y->data[i][r][c] = activation_sigma(net->C1->v->data[i][r][c], net->C1->biasData->data[i]);
            }
        }
    }
}

static void forward_lenet_S2_layer(Lenet* net)
{
    px_size_t inSize;
    px_size_t outSize;

    outSize.width = net->C3->in_width;
    outSize.height = net->C3->in_height;
    inSize.width = net->S2->in_width;
    inSize.height = net->S2->in_height;
    for (int i = 0; i < net->S2->out_channels; i++)
    {
        if (net->S2->pool_type == NC_AvePool)
        {
            matrix_t pool_input;
            pool_input.height = inSize.height;
            pool_input.width = inSize.width;
            pool_input.data = net->C1->y->data[i];

            matrix_t pool_output;
            pool_output.height = outSize.height;
            pool_output.width = outSize.width;
            pool_output.data = net->S2->y->data[i];

            px_size_t kernel_size = px_create_size(net->S2->map_size, net->S2->map_size);
            forward_avg_pooling_for_matrix(&pool_input, &pool_output, kernel_size);
        }
    }
}

static void forward_lenet_C3_layer(Lenet* net)
{
    px_size_t inSize = px_create_size(net->C3->in_height, net->C3->in_width);
    px_size_t mapSize = px_create_size(net->C3->map_size, net->C3->map_size);
    px_size_t outSize = px_create_size(net->S4->in_height, net->S4->in_width);

    for (int i = 0; i < net->C3->out_channels; i++)
    {
        for (int j = 0; j < net->C3->in_channels; j++)
        {
            matrix_t map;
            map.height = mapSize.height;
            map.width = mapSize.width;
            map.data = net->C3->mapData->data[j][i];

            matrix_t tmp_input;
            tmp_input.height = inSize.height;
            tmp_input.width = inSize.height;
            tmp_input.data = net->S2->y->data[j];
            matrix_t* mapout = conv(&map, &tmp_input, NC_VALID);

            matrix_t res;
            res.height = mapSize.height;
            res.width = mapSize.width;
            res.data = net->C3->v->data[i];

            matrix_t mat1;
            mat1.height = outSize.height;
            mat1.width = outSize.width;
            mat1.data = net->C3->v->data[i];

            addmat(&mat1, mapout, &res);
            destroy_matrix_ptr(mapout);
        }
        for (int r = 0; r < outSize.height; r++)
        {
            for (int c = 0; c < outSize.width; c++)
            {
                net->C3->y->data[i][r][c] = activation_sigma(net->C3->v->data[i][r][c], net->C3->biasData->data[i]);
            }
        }
    }
}

static void forward_lenet_S4_layer(Lenet* net)
{
    px_size_t inSize;
    px_size_t outSize;
    
    inSize.width = net->S4->in_width;
    inSize.height = net->S4->in_height;
    outSize.width = inSize.width / net->S4->map_size;
    outSize.height = inSize.height / net->S4->map_size;
    for (int i = 0; i < (net->S4->out_channels); i++)
    {
        if (net->S4->pool_type == NC_AvePool)
        {
            matrix_t pool_input;
            pool_input.height = inSize.height;
            pool_input.width = inSize.width;
            pool_input.data = net->C3->y->data[i];

            matrix_t pool_output;
            pool_output.height = outSize.height;
            pool_output.width = outSize.width;
            pool_output.data = net->S4->y->data[i];

            px_size_t kernel_size = px_create_size(net->S4->map_size, net->S4->map_size);
            forward_avg_pooling_for_matrix(&pool_input, &pool_output, kernel_size);
        }
    }
}

static void forward_lenet_O5_layer(Lenet* net)
{
    // 首先需要将前面的多维输出展开成一维向量
    px_size_t inSize;
    px_size_t outSize;
    inSize.width = net->S4->in_width;
    inSize.height = net->S4->in_height;
    outSize.width = inSize.width / net->S4->map_size;
    outSize.height = inSize.height / net->S4->map_size;

    float* O5inData = (float*)malloc((net->O5->inputNum) * sizeof(float));
    for (int i = 0; i < (net->S4->out_channels); i++)
    {
        for (int r = 0; r < outSize.height; r++)
        {
            for (int c = 0; c < outSize.width; c++)
            {
                O5inData[i * outSize.height * outSize.width + r * outSize.width + c] = net->S4->y->data[i][r][c];
            }
        }
    }

    px_size_t nnSize = px_create_size(net->O5->outputNum, net->O5->inputNum);
    nnff(net->O5->v->data, O5inData, net->O5->wData->data, net->O5->biasData->data, nnSize);
    for (int i = 0; i < net->O5->outputNum; i++)
    {
        net->O5->y->data[i] = activation_sigma(net->O5->v->data[i], net->O5->biasData->data[i]);
    }
    free(O5inData);
}

// 这里 input 是图像数据， input[r][c],r行c列，这里根各权重模板是一致的
void forward_lenet(Lenet* net, matrix_t* input)
{
    forward_lenet_C1_layer(net, input);
    forward_lenet_S2_layer(net);
    forward_lenet_C3_layer(net);
    forward_lenet_S4_layer(net);
    forward_lenet_O5_layer(net);
}
