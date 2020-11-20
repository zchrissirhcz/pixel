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
android-arm64/demo: 1 file pushed. 28.2 MB/s (1452016 bytes in 0.049s)
[good] result match
[c naive] cost 62.6631 ms
[c fast] cost 7.37915 ms
[neon intrinsics] cost 7.43677 ms
[neon intrinsics2] cost 7.39136 ms
```

## Reference

https://stackoverflow.com/a/34505048/2999096