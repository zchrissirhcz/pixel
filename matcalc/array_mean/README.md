# array mean

用ARM NEON，加速计算数组均值



old output:
```
mean_naive cost 225 ms
mean_neon cost 37 ms
mean_naive returns -0.00729697
mean_neon  returns -0.00729697
```

new output:
```
pixel/Debug | main.cpp, line 227 | mean_naive cost 223.938073 ms
pixel/Debug | main.cpp, line 238 | mean_neon cost 33.861823 ms
pixel/Debug | main.cpp, line 241 | mean_naive returns 0.001056
pixel/Debug | main.cpp, line 242 | mean_neon  returns 0.001056
```

old executable size: about 4.5 MB
new executable size: 13KB


## Refs

https://stackoverflow.com/a/6932695/2999096