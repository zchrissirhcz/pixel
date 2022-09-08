# 找出相同或相似图像

尝试找出重复或近似图像。在 BDCI Traffic Sign 数据集上实验。基本思路：图像->特征，距离相同或接近判定为相似。

## method1 基于md5sum

找出二进制文件完全相同的文件。

duplicated ones:
000520.png
000521.png


## method2 哈希感知

实现了最简单的 phash 算法：先 resize 到 8x8，再转灰度图，算出灰度像素均值，大于均值为1小于为0，得到长度64的特征。

两个phash特征的汉明距离<2时判定相似，构造两两特征是否相似的矩阵M，用并查集对M标记（类似聚类），挑选同一group内两张图比较：

![](method2/simplest-phash-dist2-example.jpg)