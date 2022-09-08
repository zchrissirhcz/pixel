#pragma once

static int pixel_clip(int val, int minval, int maxval);




int pixel_clip(int val, int minval, int maxval)
{
    if (val<minval) {
        return minval;
    }
    if (val>maxval) {
        return maxval;
    }
    return val;
}
