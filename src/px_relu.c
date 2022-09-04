#include "px_cnn.h"
#include "px_assert.h"

void px_relu_forward(px_array_t* input, px_array_t* output)
{
    PX_ASSERT(output != NULL);
    PX_ASSERT(input != NULL);
    PX_ASSERT(input->len == output->len);
    const int len = input->len;
    for (int i = 0; i < len; i++)
    {
        output->data[i] = (input->data[i] > 0) ? input->data[i] : 0;
    }
}