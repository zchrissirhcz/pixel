#include "cnn.h"
#include "blob.h"

tensor_t* create_tensor(px_tensor_dim_t tensor_dim)
{
    float**** data = create_blob4d(tensor_dim);
    tensor_t* tensor = (tensor_t*)malloc(sizeof(tensor_t));
    tensor->data = data;
    tensor->batch = tensor_dim.batch;
    tensor->channel = tensor_dim.channel;
    tensor->height = tensor_dim.height;
    tensor->width = tensor_dim.width;
    return tensor;
}

void clear_tensor(tensor_t* tensor)
{
    px_tensor_dim_t tensor_dim = px_create_tensor_dim(tensor->batch, tensor->channel, tensor->height, tensor->width);
    clear_blob4d(tensor->data, tensor_dim);
}

void save_tensor_to_file(tensor_t* tensor, FILE* fout)
{
    px_tensor_dim_t tensor_dim = px_create_tensor_dim(tensor->batch, tensor->channel, tensor->height, tensor->width);
    save_blob4d_to_file(tensor->data, tensor_dim, fout);
}

void destroy_tensor(tensor_t* tensor)
{
    free(tensor->data);
    free(tensor);
}