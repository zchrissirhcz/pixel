#include "cnn.h"
#include "blob.h"

array_t* create_array(int len)
{
    float* data = create_blob1d(len);
    array_t* array = (array_t*)malloc(sizeof(array_t));
    array->len = len;
    array->data = data;
    return array;
}

void clear_array(array_t* array)
{
    clear_blob1d(array->data, array->len);
}

void save_array_to_file(array_t* array, FILE* fout)
{
    save_blob1d_to_file(array->data, array->len, fout);
}