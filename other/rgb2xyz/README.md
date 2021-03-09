## 原理
https://www.jianshu.com/p/c34a313e12eb

![](rgb2xyz.png)


## 评测
Android ARMv8, QCOM805, OpenCV 4.5.0
```
image info: height=512, width=512
rgb2xyz, naive impl,    time=4.1622 ms, x=0.3197, y=0.3351, z=0.3452
rgb2xyz, fused double,  time=1.0258 ms, x=0.3196, y=0.3350, z=0.3454
rgb2xyz, fused float,   time=0.7645 ms, x=0.3196, y=0.3350, z=0.3454
rgb2xyz, fused asimd,   time=0.3381 ms, x=0.3196, y=0.3350, z=0.3454
rgb2xyz, fused asimd2,  time=0.2897 ms, x=0.3196, y=0.3350, z=0.3454
image info: height=4032, width=3024
rgb2xyz, naive impl,    time=120.1666 ms, x=0.3010, y=0.3184, z=0.3806
rgb2xyz, fused double,  time=33.3066 ms, x=0.2988, y=0.3162, z=0.3849
rgb2xyz, fused float,   time=33.0811 ms, x=0.2988, y=0.3162, z=0.3849
rgb2xyz, fused asimd,   time=16.3362 ms, x=0.2988, y=0.3162, z=0.3850
rgb2xyz, fused asimd2,  time=13.1094 ms, x=0.2988, y=0.3162, z=0.3849
```