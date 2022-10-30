# resize by two
gray image, linear image resize by two.


https://stackoverflow.com/questions/17815959/resize-8-bit-image-by-2-with-arm-neon/18103004#18103004

## benchmark on Android
Device: XiaoMi 11 (bind to little core, A55)

| method  | time cost   |
| ------- | ----------- |
| naive   | 0.18 ms |
| neon    | 0.15 ms |
| neon_v2 | 0.21 ms |

Results not stable due to time cost abs is less than 1 ms.

## benchmark on Apple M1

| method  | time cost   |
| ------- | ----------- |
| naive   | 0.18 ms |
| neon    | 0.11 ms |
| neon_v2 | 0.11 ms |