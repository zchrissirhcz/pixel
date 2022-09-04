# coding: utf-8

import cv2

im = cv2.imread('river_bank2.png')
gray = cv2.cvtColor(im, cv2.COLOR_BGR2GRAY)

size = gray.shape
height = size[0]
width = size[1]

total = 0
for i in range(height):
    for j in range(width):
        total += gray[i, j]

print('sum is: ', total)
print('avg is: ', total*1.0/(height*width))

#print(size)
#height=3024, width=4032