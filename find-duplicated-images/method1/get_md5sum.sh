#!/bin/bash

#DATA_DIR=~/data/traffic_sign/JPEGImages
DATA_DIR=~/data/traffic_sign/downsample4x4
for f in `ls $DATA_DIR`
do
    fullpath=$DATA_DIR/$f
    md5sum=`md5sum $fullpath`
    echo $md5sum
done

