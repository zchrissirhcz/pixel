# ndk-hello-neon

原始代码来自 https://github.com/android/ndk-samples/tree/main/hello-neon

本目录下的代码，剔除了JNI，增加了android-arm64-v8a支持。

cmake Release模式，在小米8上运行输出：
>FIR Filter benchmark:
>C version          : 29.2183 ms
>Neon version   : 13.2515 ms (x2.20491 faster)
