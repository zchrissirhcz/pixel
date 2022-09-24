#pragma once


typedef struct array_t
{
    int len;
    float* data;
} array_t;

typedef struct matrix_t
{
    int height;
    int width;
    float** data;
} matrix_t;

typedef struct cube_t
{
    int channel;
    int height;
    int width;
    float*** data;
} cube_t;

typedef struct tensor_t
{
    int batch;
    int channel;
    int height;
    int width;
    float**** data;
} tensor_t;