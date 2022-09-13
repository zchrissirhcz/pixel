
#include "lenet.h"

static void backward_lenet_O5_layer(Lenet* net)
{
    // 输出层O5
    for (int i = 0; i < net->O5->output_num; i++)
    {
        net->O5->d->data[i] = net->err[i] * sigma_derivation(net->O5->y->data[i]);
    }
}

static void backward_lenet_S4_layer(Lenet* net)
{
    // S4层，传递到S4层的误差
    // 这里没有激活函数
    const int out_height = net->S4->in_height / net->S4->map_size;
    const int out_width = net->S4->in_width / net->S4->map_size;
    px_size_t outSize = px_create_size(out_height, out_width);
    for (int i = 0; i < net->S4->out_channels; i++)
    {
        for (int r = 0; r < outSize.height; r++)
        {
            for (int c = 0; c < outSize.width; c++)
            {
                for (int j = 0; j < net->O5->output_num; j++)
                {
                    int wInt = i * outSize.width * outSize.height + r * outSize.width + c;
                    net->S4->d->data[i][r][c] = net->S4->d->data[i][r][c] + net->O5->d->data[j] * net->O5->wData->data[j][wInt];
                }
            }
        }
    }
}

static void backward_lenet_C3_layer(Lenet* net)
{
    // C3层
    // 由S4层传递的各反向误差,这里只是在S4的梯度上扩充一倍
    int mapdata = net->S4->map_size;
    px_size_t S4dSize = px_create_size(net->S4->in_height / net->S4->map_size, net->S4->in_width / net->S4->map_size);
    // 这里的Pooling是求平均，所以反向传递到下一神经元的误差梯度没有变化
    for (int i = 0; i < net->C3->out_channels; i++)
    {
        matrix_t* input = get_matrix_from_cube(net->S4->d, i);
        matrix_t* C3e = matrix_upsample(input, net->S4->map_size, net->S4->map_size);
        for (int r = 0; r < net->S4->in_height; r++)
        {
            for (int c = 0; c < net->S4->in_width; c++)
            {
                net->C3->d->data[i][r][c] = C3e->data[r][c] * sigma_derivation(net->C3->y->data[i][r][c]) / (float)(net->S4->map_size * net->S4->map_size);
            }
        }
        destroy_matrix(C3e);
        free(input);
    }
}

static void backward_lenet_S2_layer(Lenet* net)
{
    // S2层，S2层没有激活函数，这里只有卷积层有激活函数部分
    // 由卷积层传递给采样层的误差梯度，这里卷积层共有6*12个卷积模板
    px_size_t outSize;
    outSize.width = net->C3->in_width;
    outSize.height = net->C3->in_height;
    px_size_t inSize = px_create_size(net->S4->in_height, net->S4->in_width);
    px_size_t mapSize = px_create_size(net->C3->map_size, net->C3->map_size);
    for (int i = 0; i < net->S2->out_channels; i++)
    {
        for (int j = 0; j < net->C3->out_channels; j++)
        {
            matrix_t* map = get_matrix_from_tensor(net->C3->kernel, i, j);

            matrix_t* input = get_matrix_from_cube(net->C3->d, j);

            matrix_t* corr = correlation_for_matrix(map, input, NC_FULL);

            matrix_t* res = get_matrix_from_cube(net->S2->d, i);

            matrix_t* mat1 = get_matrix_from_cube(net->S2->d, i);

            matrix_add(mat1, corr, res);
            destroy_matrix(corr);
            free(map);
            free(input);
            free(res);
            free(mat1);
        }
        /*
        for(r=0;r<cnn->C3->inputHeight;r++)
            for(c=0;c<cnn->C3->inputWidth;c++)
                // 这里本来用于采样的激活
        */
    }
}

static void backward_lenet_C1_layer(Lenet* net)
{
    // C1层，卷积层
    int mapdata = net->S2->map_size;
    px_size_t S2dSize = px_create_size(net->S2->in_height / net->S2->map_size, net->S2->in_width / net->S2->map_size);
    // 这里的Pooling是求平均，所以反向传递到下一神经元的误差梯度没有变化
    for (int i = 0; i < net->C1->out_channels; i++)
    {
        matrix_t* input = get_matrix_from_cube(net->S2->d, i);
        matrix_t* C1e = matrix_upsample(input, net->S2->map_size, net->S2->map_size);
        for (int r = 0; r < net->S2->in_height; r++)
        {
            for (int c = 0; c < net->S2->in_width; c++)
            {
                net->C1->d->data[i][r][c] = C1e->data[r][c] * sigma_derivation(net->C1->y->data[i][r][c]) / (float)(net->S2->map_size * net->S2->map_size);
            }
        }
        destroy_matrix(C1e);
    }
}

void backward_lenet(Lenet* net, float* outputData)
{
    int i, j, c, r; // 将误差保存到网络中
    for (i = 0; i < net->O5->output_num; i++)
    {
        net->err[i] = net->O5->y->data[i] - outputData[i];
    }

    /*从后向前反向计算*/
    backward_lenet_O5_layer(net);
    backward_lenet_S4_layer(net);
    backward_lenet_C3_layer(net);
    backward_lenet_S2_layer(net);
    backward_lenet_C1_layer(net);
}
