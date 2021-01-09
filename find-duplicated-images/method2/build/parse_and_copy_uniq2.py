#!/usr/bin/env python
#coding: utf-8

import shutil

#-----------------------
# get duplicated images
#-----------------------
duplicated_images = []

fin = open("duplicated_results.txt")
for line in fin.readlines():
    line = line.strip()
    if ( '.png' not in line ):
        continue
    if ( 'speaker' not in line ):
        duplicated_images.append(line)
fin.close()


#-----------------------
# get all images
#-----------------------
all_images = []

fin = open('/Users/chris/data/traffic_sign/dir.txt')
for line in fin.readlines():
    line = line.strip()
    all_images.append(line)
fin.close()

#-----------------------
# get unique images
#-----------------------
uniq_images = []

src_dir = '/Users/chris/data/traffic_sign/JPEGImages'
dst_dir = '/Users/chris/data/traffic_sign/uniq2'
for im_name in all_images:
    if im_name not in duplicated_images:
        uniq_images.append(im_name)
        src_file = src_dir + '/' + im_name
        dst_file = dst_dir + '/' + im_name
        shutil.copy(src_file, dst_file)
