#pragma once

#ifdef __cplusplus
extern "C" {
#endif

// 对 nxn 尺寸的矩阵，求它的逆矩阵
void square_matrix_inverse(float* src, uint32_t n, float* dst);

#ifdef __cplusplus
}
#endif
