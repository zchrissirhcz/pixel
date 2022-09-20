#include "zcnn_cnn.h"
#include <iostream>
#include <unordered_map>
#include <algorithm>
using namespace std;

Blob::Blob() {
    w = 0;
    h = 0;
    c = 0;
    n = 0;
    data = nullptr;
}

Blob::Blob(int _w, int _h, int _c) {
    w = _w;
    h = _h;
    c = _c;
    n = 0;
    data = nullptr;
}

Blob::Blob(int _w, int _h, int _c, int _n) {
    w = _w;
    h = _h;
    c = _c;
    n = _n;
    data = nullptr;
}

//Blob::Blob(const Blob& m) {
//	w = m.w;
//	h = m.h;
//	c = m.c;
//	data = m.data;
//}
//
//Blob& Blob::operator=(const Blob& m) {
//	if (this == &m) {
//		return *this;
//	}
//
//	w = m.w;
//	h = m.h;
//	c = m.c;
//
//	return *this;
//}

Blob* BlobFactory(unordered_map<string, Blob*> blob_map, const string& blob_name) {
    if (blob_map.count(blob_name)==0) {
        blob_map[blob_name] = new Blob();
        blob_map[blob_name]->name = blob_name;
    }
    return blob_map[blob_name];
}


InputLayer::InputLayer(const string& _name) {
    name = _name;
    type = kINPUT;
}

void InputLayer::Forward() {
    Blob* bottom = bottom_blobs[0];
    Blob* top = top_blobs[0];
    for (int i = 0; i < bottom->total(); i++) {
        top->data[i] = bottom->data[i];
    }
}

ConvolutionLayer::ConvolutionLayer(const string& _name) {
    name = _name;
    type = kCONVOLUTION;
    stride.h = 0;
    stride.w = 0;
    filters = nullptr;
}

void ConvolutionLayer::Forward() {
    Blob* bottom = bottom_blobs[0];
    Blob* top = top_blobs[0];
    //TODO: do convolution here
    convolution_pure(bottom, filters, &top, &stride);
}

ConvolutionLayer::~ConvolutionLayer() {
    if(filters) {
        delete filters;
        filters = nullptr;
    }
}

ReluLayer::ReluLayer(const string& _name) {
    name = _name;
    type = kRELU;
}

void ReluLayer::Forward() {
    Blob* bottom = bottom_blobs[0];
    Blob* top = top_blobs[0];
    for (int i = 0; i < bottom->total(); i++) {
        top->data[i] = std::max(0.f, bottom->data[i]);
    }
}

// enum type as key of unordered_map requires this
struct MyEnumClassHash
{
    template <typename T>
    std::size_t operator()(T t) const
    {
        return static_cast<std::size_t>(t);
    }
};

string getLayerTypeStr(LayerType type)
{
    unordered_map<LayerType, string, MyEnumClassHash> type_map;
    type_map[kINPUT] = "Input";
    type_map[kCONVOLUTION] = "Convolution";
    type_map[kRELU] = "ReLU";

    if (type_map.count(type))
    {
        return type_map[type];
    }
    else
    {
        return "unknown";
    }
}
