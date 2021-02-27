# Binarize

对uchar类型的数组做单向二值化：大于阈值thresh的更新为255。

## Build
```
cd build
./android-arm64-build.sh
./android-arm64-run.sh
```

## Result
Tested on MI8(QC845) armv8 mode

```
pixel/Debug | main.cpp, line 120 | [good] result match
pixel/Debug | main.cpp, line 122 | [c naive] cost 1.49156 ms
pixel/Debug | main.cpp, line 123 | [c fast] cost 0.113437 ms
pixel/Debug | main.cpp, line 124 | [neon intrinsics] cost 0.11224 ms
```

## Reference

https://stackoverflow.com/a/34505048/2999096