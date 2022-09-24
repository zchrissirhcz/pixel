#include "px_ppm_pgm.h"
#include "px_assert.h"
#include "px_log.h"

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

static void write_ppm_pgm(const char* filename, uint8_t* data, int height, int width, int channel)
{
    FILE* fp = fopen(filename, "wb");
    if (!fp)
    {
        PX_LOGE("failed to open file %s for write\n", filename);
        return;
    }

    if (channel == 3) // ppm
    {
        fprintf(fp, "P6\n%d %d\n255\n", width, height);
    }
    else if (channel == 1) // pgm
    {
        fprintf(fp, "P5\n%d %d\n255\n", width, height);
    }
    const int bufsize = height * width * channel;
    if (fwrite(data, bufsize, 1, fp) != 1)
    {
        PX_LOGE("fwrite() failed in %s\n", __FUNCTION__);
    }

    fclose(fp);
}

void px_write_ppm(const char* filename, uint8_t* data, int height, int width)
{
    write_ppm_pgm(filename, data, height, width, 3);
}

void px_write_pgm(const char* filename, uint8_t* data, int height, int width)
{
    write_ppm_pgm(filename, data, height, width, 1);
}

static uint8_t* read_ppm_pgm(const char* filename, int* height, int* width, int channel)
{
    FILE* fp = fopen(filename, "rb");
    if (!fp)
    {
        PX_LOGE("failed to open file %s for read\n", filename);
        return NULL;
    }

    char magic[3];
    int nscan = fscanf(fp, "%2s\n%d %d\n255\n", magic, width, height);
    if (nscan != 3)
    {
        PX_LOGE("fscanf() failed in %s\n", __FUNCTION__);
        return NULL;
    }
    if (magic[0] == 'P' && ( (channel == 3 && magic[1] == '6') || (channel == 1 && magic[1] == '5') ))
    {
        const int bufsize = (*width) * (*height) * channel * sizeof(uint8_t);
        uint8_t* buf = (uint8_t*)malloc(bufsize);
        if (fread(buf, bufsize, 1, fp) != 1)
        {
            PX_LOGE("fread failed in %s\n", __FUNCTION__);
            return NULL;
        }
        return buf;
    }
    
    return NULL;
}

uint8_t* px_read_ppm(const char* filename, int* height, int* width)
{
    return read_ppm_pgm(filename, height, width, 3);
}

uint8_t* px_read_pgm(const char* filename, int* height, int* width)
{
    return read_ppm_pgm(filename, height, width, 1);
}
