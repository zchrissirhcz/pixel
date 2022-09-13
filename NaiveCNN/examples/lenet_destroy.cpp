#include "cnn.h"
#include "lenet.h"
#include "types.h"

static void destroy_lenet_C1_layer(Lenet* net)
{
    destroy_conv_layer(net->C1);
}

static void destroy_lenet_S2_layer(Lenet* net)
{
    destroy_pooling_layer(net->S2);
}

static void destroy_lenet_C3_layer(Lenet* net)
{
    destroy_conv_layer(net->C3);
}

static void destroy_lenet_S4_layer(Lenet* net)
{
    destroy_pooling_layer(net->S4);
}

static void destroy_lenet_O5_layer(Lenet* net)
{
    destroy_innerproduct_layer(net->O5);
}

void destroy_lenet(Lenet* net)
{
    destroy_lenet_C1_layer(net);
    destroy_lenet_S2_layer(net);
    destroy_lenet_C3_layer(net);
    destroy_lenet_S4_layer(net);
    destroy_lenet_O5_layer(net);

    free(net->L);
    free(net);
}
