# 基于ARM NEON Intrinsics 的 RGB BGR 转换加速


Tested on MI8 (QC845) for 1001 trials, image size 353 x 500
```
-- loaded image 000001.jpg, height=500, width=353
no neon1 impl time cost 229.106927 ms
no neon2 impl time cost 114.606823 ms
neon intrinsics impl time cost 48.314219 ms
```

## Ref links

- https://community.arm.com/developer/ip-products/processors/b/processors-ip-blog/posts/coding-for-neon---part-1-load-and-stores

- https://stackoverflow.com/a/39519421/2999096

- https://blog.csdn.net/wohenfanjian/article/details/103407259

- https://stackoverflow.com/a/11684331/2999096

- https://stackoverflow.com/questions/65056624/confusion-about-different-clobber-description-for-arm-inline-assembly
