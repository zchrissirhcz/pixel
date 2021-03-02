#ifndef PIXEL_CLIP_H
#define PIXEL_CLIP_H

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

#endif // PIXEL_CLIP_H