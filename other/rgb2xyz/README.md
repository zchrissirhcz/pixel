## 原理
https://www.jianshu.com/p/c34a313e12eb

![](rgb2xyz.png)


## 评测
Android ARMv8, QCOM805, OpenCV 4.5.0
```
image info: height=512, width=512
rgb2xyz, naive impl,    time=2.8918 ms, x=0.3197, y=0.3351, z=0.3452
rgb2xyz, fused double,  time=0.6353 ms, x=0.3196, y=0.3350, z=0.3454
rgb2xyz, fused float,   time=0.6214 ms, x=0.3196, y=0.3350, z=0.3454
image info: height=4032, width=3024
rgb2xyz, naive impl,    time=121.3239 ms, x=0.3010, y=0.3184, z=0.3806
rgb2xyz, fused double,  time=33.6198 ms, x=0.2988, y=0.3162, z=0.3849
rgb2xyz, fused float,   time=32.6428 ms, x=0.2988, y=0.3162, z=0.3849
```