#!/bin/bash

echo "Script executed from: ${PWD}"

BASEDIR=$(dirname $0)
echo "Script location: ${BASEDIR}"

# download each zip file.
# -N : if file already exist, skip.
# -P : specifies the saving directory ("prefix")
download()
{
    # You may also download mnist data from [BaiduYun](https://pan.baidu.com/s/1TqVk_0un_HaKN38262X2Jg), password: 990m
    wget -N http://yann.lecun.com/exdb/mnist/train-images-idx3-ubyte.gz -P ${BASEDIR}
    wget -N http://yann.lecun.com/exdb/mnist/train-labels-idx1-ubyte.gz -P ${BASEDIR}
    wget -N http://yann.lecun.com/exdb/mnist/t10k-images-idx3-ubyte.gz -P ${BASEDIR}
    wget -N http://yann.lecun.com/exdb/mnist/t10k-labels-idx1-ubyte.gz -P ${BASEDIR}
}

# unzip each .gz file to specified output file, keep the original .gz file, without temporary file
unzip_gz_files()
{
    zcat ${BASEDIR}/train-images-idx3-ubyte.gz > ${BASEDIR}/train-images.idx3-ubyte
    zcat ${BASEDIR}/train-labels-idx1-ubyte.gz > ${BASEDIR}/train-labels.idx1-ubyte
    zcat ${BASEDIR}/t10k-images-idx3-ubyte.gz > ${BASEDIR}/t10k-images.idx3-ubyte
    zcat ${BASEDIR}/t10k-labels-idx1-ubyte.gz > ${BASEDIR}/t10k-labels.idx1-ubyte
}

download
unzip_gz_files