#include "px_image_drawing.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <algorithm>

static void circle(unsigned char* src, int w, int h, int channels, int stride, int dx, int dy, int dr, unsigned char* dcolor)
{
    int up_limit = std::max(dy-dr, 0);
    int down_limit = std::min(h, dy+dr);
    //unsigned char ddcolor[channels*2*dr];
    unsigned char* ddcolor = (unsigned char*)malloc(channels * 2 * dr);
    if (ddcolor == NULL)
        return;
    auto size = sizeof(unsigned char);
    double ddr = dr*dr;
    int offset = channels*(int)size;
    for(int i=0; i<2*dr; i++)
    {
        memcpy(ddcolor+i*channels, dcolor, offset);
    }

    for(int y=up_limit; y<down_limit; y++)
    {
        int ddx = sqrt(ddr-(y-dy)*(y-dy));
        //int ddx = fast_sqrt(ddr-(y-dy)*(y-dy));
        int left_limit = std::max(dx-ddx, 0);
        int right_limit = std::min(w, dx+ddx);

        memcpy(src+y*stride+left_limit*channels, ddcolor, (right_limit-left_limit)*offset);
    }
    free(ddcolor);
}

void px_draw_circle(px_image_t* im, const px_point_t* center, int radius, px_color_t color)
{
    unsigned char dcolor[3] = {color.b, color.g, color.r};
    circle(im->data, im->width, im->height, im->channel, im->stride, center->x, center->y, radius, dcolor);
}

static void draw_solid_rect(px_image_t* im, int dx1, int dy1, int dx2, int dy2, px_color_t color)
{
    int linebytes = im->stride;
    int channel = im->channel;
    for (int h = dy1; h <= dy2; h++)
    {
        for (int w = dx1; w <= dx2; w++)
        {
            im->data[h*linebytes + w*channel] = color.b;
            im->data[h*linebytes + w*channel + 1] = color.g;
            im->data[h*linebytes + w*channel + 2] = color.r;
        }
    }
}

void px_draw_rectangle(px_image_t* im, px_rect_t r, px_color_t color, int thickness)
{
    //draw rect
    int dx1, dx2, dy1, dy2;
    int half_thick_small = thickness / 2;
    int half_thick_big = thickness - thickness / 2;

    // up horizontal line
    dy1 = r.y1 - half_thick_small;
    if (dy1 < 0)
    {
        dy1 = 0;
    }
    dy2 = r.y1 + half_thick_big;
    if (dy2 >= im->height - 1)
    {
        dy2 = im->height - 1;
    }

    dx1 = r.x1 - half_thick_small;
    if (dx1 < 0)
    {
        dx1 = 0;
    }
    dx2 = r.x2 + half_thick_big;
    if (dx2 >= im->width - 1)
    {
        dx2 = im->width - 1;
    }
    draw_solid_rect(im, dx1, dy1, dx2, dy2, color);

    // down horizontal line
    dy1 = r.y2 - half_thick_small;
    if (dy1 < 0)
    {
        dy1 = 0;
    }
    dy2 = r.y2 + half_thick_big;
    if (dy2 >= im->height - 1)
    {
        dy2 = im->height - 1;
    }

    dx1 = r.x1 - half_thick_small;
    if (dx1 < 0)
    {
        dx1 = 0;
    }
    dx2 = r.x2 + half_thick_big;
    if (dx2 >= im->width - 1)
    {
        dx2 = im->width - 1;
    }
    draw_solid_rect(im, dx1, dy1, dx2, dy2, color);

    // left vertical line
    dy1 = r.y1 - half_thick_small;
    if (dy1 < 0)
    {
        dy1 = 0;
    }
    dy2 = r.y2 + half_thick_big;
    if (dy2 >= im->height - 1)
    {
        dy2 = im->height - 1;
    }
    dx1 = r.x1 - half_thick_small;
    if (dx1 < 0)
    {
        dx1 = 0;
    }
    dx2 = r.x1 + half_thick_big;
    if (dx2 >= im->width - 1)
    {
        dx2 = im->width - 1;
    }
    draw_solid_rect(im, dx1, dy1, dx2, dy2, color);

    // right vertical line
    dy1 = r.y1 - half_thick_small;
    if (dy1 < 0)
    {
        dy1 = 0;
    }
    dy2 = r.y2 + half_thick_big;
    if (dy2 >= im->height - 1)
    {
        dy2 = im->height - 1;
    }

    dx1 = r.x2 - half_thick_small;
    if (dx1 < 0)
    {
        dx1 = 0;
    }
    dx2 = r.x2 + half_thick_big;
    if (dx2 >= im->width - 1)
    {
        dx2 = im->width - 1;
    }
    draw_solid_rect(im, dx1, dy1, dx2, dy2, color);
}