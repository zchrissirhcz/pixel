#include <stdio.h>
#include <stdlib.h>
#include <string.h>

namespace fc {

static inline int align_up(int x, int n) {
    return ((x + n - 1) / n) * n;
}

unsigned char* imread_bmp(const char* filename, int* _width, int* _height, int* _channel);
void imwrite_bmp(const char* filename, int width, int height, int channel,
                 const unsigned char* data, const int src_align=1);
//TODO: imwrite_bmp support saving gray image (channel=1)

unsigned char* imread_bmp(const char* filename, int* _width, int* _height, int* _channel) {
    const int align = 4;

    FILE* fp = fopen(filename, "rb");

    if (!fp) {
        fprintf(stderr, "failed to open file %s\n", filename);
        return NULL;
    }

    // bmp header: 54 bytes
    //  file_header: 14 bytes
    //  info_header: 40 bytes
    unsigned char hd[54];  // 14+40
    memset(hd, 0, 54);

    if (0 == fread(hd, 54, 1, fp)) {
        fclose(fp);
        fprintf(stderr, "invalid BMP header\n");
        return NULL;
    }
    if (0 != strncmp((const char*)hd, "BM", 2)) {
        fclose(fp);
        fprintf(stderr, "invalid BMP signature\n");
        return NULL;
    }
    if (hd[28] != 24 && hd[28] != 32) {
        fclose(fp);
        fprintf(stderr, "invalid BMP bpp");
        return NULL;
    }

    int width = (unsigned int)hd[18] << 0 | (unsigned int)hd[19] << 8 | (unsigned int)hd[20] << 16 |
                (unsigned int)hd[21] << 24;
    int height = (unsigned int)hd[22] << 0 | (unsigned int)hd[23] << 8 |
                 (unsigned int)hd[24] << 16 | (unsigned int)hd[25] << 24;
    int channel = hd[28] / 8;

    unsigned char* data = NULL;
    if (width > 0 && height > 0) {
        // w*3: we only need to store BGR instead of BGRA image
        int line_bytes = align_up(width * channel, align);
        data = (unsigned char*)malloc(line_bytes * height);
        memset(data, 0, width * channel * height);
        if (!data) {
            fclose(fp);
            fprintf(stderr, "Out of memery when loading BMP\n");
            return NULL;
        }

        int pad = line_bytes - width * channel;  // for read
        unsigned char bmp_pad[3];
        for (int y = height - 1; y != -1; y--) {
            for (int x = 0; x < width; x++) {
                // get pixel with BGR order
                unsigned int i = (x + y * width) * channel;
                if (0 == fread(data + i, 3, 1, fp)) {
                    free(data);
                    fclose(fp);
                    fprintf(stderr, "invalid BMP file format\n");
                    return NULL;
                }

                // if we ignore alpha channel, we do this:
                // if (channel == 4)
                //    data[i + 3] = 255;
            }

            if (pad != fread(bmp_pad, 1, pad, fp)) {
                free(data);
                fclose(fp);
                fprintf(stderr, "invalid BMP file format\n");
                return NULL;
            }
        }
    }

    // write back
    *_width = width;
    *_height = height;
    *_channel = channel;

    fclose(fp);

    return data;
}

void imwrite_bmp(const char* filename, int width, int height, int channel,
                 const unsigned char* data, const int src_align) {
    const int dst_align = 4;

    FILE* fp = fopen(filename, "wb");
    if (!fp) {
        fprintf(stderr, "failed to write file %s\n", filename);
        return;
    }

    unsigned char hd[54] = {
        // bmp header
        'B', 'M', 0, 0, 0, 0, 0, 0, 0, 0, 54, 0, 0, 0,       // bmp file header, 14 bytes
        40,  0,   0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 1, 0, 0, 0  // bmp info header, 40 bytes
    };
    if (channel != 3 && channel != 4) {
        fprintf(stderr, "invalid BMP channel to save\n");
        return;
    }
    hd[28] = (unsigned char)(channel * 8);

    const int size = 54 + width * height * channel;  // 3channel

    hd[2] = (unsigned char)(size);
    hd[3] = (unsigned char)(size >> 8);
    hd[4] = (unsigned char)(size >> 16);
    hd[5] = (unsigned char)(size >> 24);

    hd[18] = (unsigned char)(width);
    hd[19] = (unsigned char)(width >> 8);
    hd[20] = (unsigned char)(width >> 16);
    hd[21] = (unsigned char)(width >> 24);

    hd[22] = (unsigned char)(height);
    hd[23] = (unsigned char)(height >> 8);
    hd[24] = (unsigned char)(height >> 16);
    hd[25] = (unsigned char)(height >> 24);

    if (1 != fwrite(hd, 54, 1, fp)) {
        fclose(fp);
        fprintf(stderr, "failed to write %s\n", filename);
        return;
    }

    char bmp_pad[3] = {0, 0, 0};
    int pixel_bytes = width * channel;
    int src_line_bytes = align_up(pixel_bytes, src_align);
    int line_bytes = align_up(width * channel, dst_align);
    int line_pad = line_bytes - pixel_bytes;
    // buf += line_bytes * (height - 1);

    int padding = line_bytes - pixel_bytes;
    for (int h = height - 1; h != -1; h--) {
        // if (fwrite(buf, pixel_bytes, 1, fp) == 1 && fwrite(bmp_pad, 1, line_pad, fp) == line_pad)
        // {
        //    buf -= pixel_bytes;
        //}
        // else {
        //    fprintf(stderr, "failed to save bmp file %s\n", filename);
        //    return;
        //}
        // for (int w = 0; w < width; w++) {
        //    //int i = (w + h * width)*channel;
        //    int i = h * pixel_bytes + w * channel;
        //    if (fwrite(data + i, channel, 1, fp) == 0) {
        //        fclose(fp);
        //    }
        //}
        if (fwrite(data + h * src_line_bytes, pixel_bytes, 1, fp) == 1 &&
            fwrite(bmp_pad, 1, padding, fp) == padding) {
            continue;
        } else {
            fclose(fp);
            fprintf(stderr, "write BMP padding error\n");
            return;
        }
    }
    fclose(fp);
}

}  // namespace fc