#!/usr/bin/env python
#coding: utf-8

import shutil

src_im_dir = "/Users/chris/data/traffic_sign/JPEGImages"
dst_im_dir = "/Users/chris/data/traffic_sign/uniq"
fin = open("uniq.txt")
for line in fin.readlines():
    im_name = line.strip()
    if ( len(im_name) == 0 ):
        continue
    src_pth = src_im_dir + '/' + im_name
    dst_pth = dst_im_dir + '/' + im_name
    shutil.copy(src_pth, dst_pth)


fin.close()
