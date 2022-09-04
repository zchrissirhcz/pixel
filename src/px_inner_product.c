#include "px_cnn.h"
#include "px_assert.h"

float px_inner_product(px_array_t* v1, px_array_t* v2)
{
    PX_ASSERT(v1 != NULL && v2 != NULL);
    PX_ASSERT(v1->data != NULL && v2->data != NULL);
    PX_ASSERT(v1->len == v2->len);

    float sum = 0;
    const int len = v1->len;
    for (int i = 0; i < len; i++)
    {
        sum += v1->data[i] * v2->data[i];
    }
    return sum;
}