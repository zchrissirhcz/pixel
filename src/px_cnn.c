#include "px_cnn.h"
#include "px_assert.h"
#include <stdlib.h>
#include <string.h>

px_array_t* px_create_array(int len)
{
    px_array_t* array = (px_array_t*) malloc(sizeof(px_array_t));
    memset(array, 0, sizeof(px_array_t));
    array->data = (float*) malloc(sizeof(float) * len);
    array->len = len;

    return array;
}

void px_destroy_array(px_array_t* array)
{
    if (array)
    {
        if (array->data)
        {
            free(array->data);
            array->data = NULL;
        }
        free(array);
    }
}
