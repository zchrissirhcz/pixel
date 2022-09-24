#include "px_image.h"

void px_resize(px_image_t* src, px_image_t* dst, px_size_t dsize, PX_INTERP_MODE mode)
{
    if (mode == PX_INTERP_NEAREST)
    {
        px_resize_nearest(src, dst, dsize);
    }
    else if (mode == PX_INTERP_LINEAR)
    {
        px_resize_linear(src, dst, dsize);
    }
    else if (mode == PX_INTERP_CUBIC)
    {
        //px_resize_cubic(src, dst, dsize);
    }
}