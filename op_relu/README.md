# Operator RELU

```
f(x)=max(0, x)
```
将buffer中的每个元素更新：如果小于0则设定为0，否则不变。


## Result
Tested on MI8(QC845) armv8 mode

```
android-arm64/demo: 1 file pushed. 29.9 MB/s (1465104 bytes in 0.047s)
--- test_op_relu_int8 ---
[good] result match
[c naive] cost 99.0603 ms
[c fast] cost 13.0911 ms
[neon intrinsics] cost 13.1516 ms
[neon intrinsics2] cost 12.7559 ms
--- test_op_relu_float ---
[good] result match
[c naive] cost 107.635 ms
[c fast] cost 78.6077 ms
[neon intrinsics] cost 78.6709 ms
[neon intrinsics2] cost 79.0083 ms
```

## Reference

https://stackoverflow.com/a/34505048/2999096