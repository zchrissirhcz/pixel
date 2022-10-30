#pragma once

#include <string>
#include <vector>

enum class LayerType
{
    INPUT,
    CONVOLUTION,
    RELU
};

class Blob
{
public:
    Blob();
    Blob(int w, int h, int c);
    Blob(int w, int h, int c, int n);
    //Blob(const Blob& m);
    //Blob& operator=(const Blob& m);
    int w, h, c, n;
    int cstep() {
        return w*h;
    }
    float* data;
    int total() {
        return w * h * c * n;
    }
    std::string name;
};

class Layer
{
public:
    virtual void Forward()=0;
    virtual ~Layer() = 0;

public:
    std::vector<Blob*> bottom_blobs;
    std::vector<Blob*> top_blobs;
    std::string name;
    LayerType type;
};

typedef struct Stride
{
    int w;
    int h;
} Stride;

class InputLayer: public Layer
{
public:
    InputLayer(const std::string& name);
    void Forward();
};

class ConvolutionLayer : public Layer
{
public:
    ConvolutionLayer(const std::string& name);
    ~ConvolutionLayer();
    void Forward();
    Stride stride;
    Blob* filters;
};

class ReluLayer: public Layer
{
public:
    ReluLayer(const std::string& name);
    void Forward();
};

void convolution_pure(Blob* padded_bottom_blob, Blob* filters, Blob** _top_blob, Stride* stride);

