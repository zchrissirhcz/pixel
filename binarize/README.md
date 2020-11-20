# Binarize

快速把uchar类型矩阵中非0元素变为FF，0元素变为0

## Build
```
cd build
.\build-android-arm64.bat
.\run-android-arm64.bat
```

## Result
Tested on MI8(QC845) armv8 mode

```
android-arm64/demo: 1 file pushed, 0 skipped. 20.6 MB/s (13040 bytes in 0.001s)
[good] result match
c impl cost 62.8491 ms
neon impl cost 7.48022 ms
neon impl2 cost 7.45947 ms
```

## Reference

https://stackoverflow.com/a/34505048/2999096