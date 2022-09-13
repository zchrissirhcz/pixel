#include "lenet.h"

static void apply_grads_on_lenet_C1_layer(Lenet* net, LenetTrainOpts opts, matrix_t* input)
{
    // C1层的权重更新
    px_size_t dSize = px_create_size(net->S2->in_width, net->S2->in_height);
    px_size_t ySize = px_create_size(net->C1->in_width, net->C1->in_height);
    px_size_t mapSize = px_create_size(net->C1->map_size, net->C1->map_size);

    for (int i = 0; i < net->C1->out_channels; i++)
    {
        for (int j = 0; j < net->C1->in_channels; j++)
        {
            matrix_t* flipinput = get_rotate180_matrix(input);
            matrix_t* map = get_matrix_from_cube(net->C1->d, i);
            matrix_t* C1dk = conv_for_matrix(map, flipinput, NC_VALID);
            matrix_multiply_lambda(C1dk, C1dk, -1 * opts.lr);

            matrix_t* res = get_matrix_from_tensor(net->C1->kernel, j, i);
            matrix_t* mat1 = get_matrix_from_tensor(net->C1->kernel, j, i);

            matrix_add(mat1, C1dk, res);
            destroy_matrix(C1dk);
            destroy_matrix(flipinput);
            free(res);
            free(mat1);
            free(map);
        }
        matrix_t mat2;
        mat2.height = dSize.height;
        mat2.width = dSize.width;
        mat2.data = net->C1->d->data[i];
        net->C1->bias->data[i] = net->C1->bias->data[i] - opts.lr * matrix_sum(&mat2);
    }
}

static void apply_grads_on_lenet_C3_layer(Lenet* net, LenetTrainOpts opts)
{
    px_size_t dSize;
    px_size_t ySize;
    px_size_t kernel_size;

    // C3层的权重更新
    dSize.width = net->S4->in_width;
    dSize.height = net->S4->in_height;
    ySize.width = net->C3->in_width;
    ySize.height = net->C3->in_height;
    kernel_size.width = net->C3->map_size;
    kernel_size.height = net->C3->map_size;
    for (int i = 0; i < net->C3->out_channels; i++)
    {
        for (int j = 0; j < net->C3->in_channels; j++)
        {
            matrix_t* input = get_matrix_from_cube(net->S2->y, j);
            matrix_t* flipinput = get_rotate180_matrix(input);

            matrix_t* map = get_matrix_from_cube(net->C3->d, i);
            matrix_t* C3dk = conv_for_matrix(map, flipinput, NC_VALID);
            matrix_multiply_lambda(C3dk, C3dk, -1.0 * opts.lr);

            matrix_t* res = get_matrix_from_tensor(net->C3->kernel, j, i);

            matrix_t* mat1 = get_matrix_from_tensor(net->C3->kernel, j, i);

            matrix_add(mat1, C3dk, res);
            destroy_matrix(C3dk);
            destroy_matrix(flipinput);
            free(res);
            free(mat1);
            free(input);
            free(map);
        }
        matrix_t mat3;
        mat3.height = dSize.height;
        mat3.width = dSize.width;
        mat3.data = net->C3->d->data[i];
        net->C3->bias->data[i] = net->C3->bias->data[i] - opts.lr * matrix_sum(&mat3);
    }
}

static void apply_grads_on_lenet_O5_layer(Lenet* net, LenetTrainOpts opts)
{
    // 输出层
    // 首先需要将前面的多维输出展开成一维向量
    float* O5inData = (float*)malloc((net->O5->input_num) * sizeof(float));
    px_size_t out_size = px_create_size(net->S4->in_height / net->S4->map_size, net->S4->in_width / net->S4->map_size);
    for (int i = 0; i < (net->S4->out_channels); i++)
    {
        for (int r = 0; r < out_size.height; r++)
        {
            for (int c = 0; c < out_size.width; c++)
            {
                O5inData[i * out_size.height * out_size.width + r * out_size.width + c] = net->S4->y->data[i][r][c];
            }
        }
    }

    for (int j = 0; j < net->O5->output_num; j++)
    {
        for (int i = 0; i < net->O5->input_num; i++)
        {
            net->O5->weight->data[j][i] = net->O5->weight->data[j][i] - opts.lr * net->O5->d->data[j] * O5inData[i];
        }
        net->O5->bias->data[j] = net->O5->bias->data[j] - opts.lr * net->O5->d->data[j];
    }
    free(O5inData);
}

void apply_grads_on_lenet(Lenet* net, LenetTrainOpts opts, matrix_t* input) // 更新权重
{
    // 这里存在权重的主要是卷积层和输出层
    // 更新这两个地方的权重就可以了
    apply_grads_on_lenet_C1_layer(net, opts, input);
    apply_grads_on_lenet_C3_layer(net, opts);
    apply_grads_on_lenet_O5_layer(net, opts);
}
