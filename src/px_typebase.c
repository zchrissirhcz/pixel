#include "px_typebase.h"

px_size_t px_create_size(int height, int width)
{
    px_size_t sz;
    sz.height = height;
    sz.width = width;
    return sz;
}