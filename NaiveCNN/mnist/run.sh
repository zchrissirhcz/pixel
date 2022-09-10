#!/bin/bash

unzip_gz_files()
{
    gzip -d train-images-idx3-ubyte.gz
    gzip -d train-labels-idx1-ubyte.gz
    gzip -d t10k-images-idx3-ubyte.gz
    gzip -d t10k-labels-idx1-ubyte.gz
}

rename_files()
{
    mv train-images-idx3-ubyte train-images.idx3-ubyte
    mv train-labels-idx1-ubyte train-labels.idx1-ubyte
    mv t10k-images-idx3-ubyte t10k-images.idx3-ubyte
    mv t10k-labels-idx1-ubyte t10k-labels.idx1-ubyte
}

rename_files