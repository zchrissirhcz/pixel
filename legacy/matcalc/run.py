#!/usr/bin/env python
#coding: utf-8


import numpy as np

mat = np.array(
  [
    [1, 2, 3, 4, 5],
    [0, 1, 4, 1, 2],
    [5, 6, 0, 1, 0],
    [2, 4, 1, 1, 2],
    [1, 3, 5, 2, 4]
  ]
)

print(mat)

mat_inv = np.linalg.inv(mat)
print(mat_inv)
