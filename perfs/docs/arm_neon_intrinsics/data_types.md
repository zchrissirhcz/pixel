# arm neon 的数据类型

## 基本数据类型

有64位、128位两种长度的类型，分别对应到 D寄存器、 Q寄存器。

| 64-bit 类型(D寄存器) | 128-bit 类型（Q寄存器） |
| -------------------- | ----------------------- |
| `int8x8_t`           | `int8x16_t`             |
| `int16x4_t`          | `int16x8_t`             |
| `int32x2_t`          | `int32x4_t`             |
| `int64x1_t`          | `int64x2_t`             |
| `uint8x8_t`          | `uint8x16_t`            |
| `uint16x4_t`         | `uint16x8_t`            |
| `uint32x2_t`         | `uint32x4_t`            |
| `uint64x1_t`         | `uint64x2_t`            |
| `float16x4_t`        | `float16x8_t`           |
| `float32x2_t`        | `float32x4_t`           |
| `poly8x8_t`          | `poly8x16_t`            |
| `poly16x4_t`         | `poly16x8_t`            |


`poly8_t`和`poly16_t`这样的类型，常规计算用不到，它们是[为加密算法和CRC哈希校验准备的](https://stackoverflow.com/questions/22224282/arm-neon-and-poly8-t-and-poly16-t)。

## 数据类型“数组”

如何使用 `float32x4x4_t` 这样的“长”类型：
把它当做是数组，随后访问`.val[i]`字段：

```c++
typedef struct float32x4x4_t {
    float32x4_t val[4];
} float32x4x4_t;
```

这样的“长类型”， 也就是“数组类型”， 数组元素为 2、3 或 4 个。

也就是说， 前面 “基础类型” 表格中累出的类型， 都有对应的 `...x2_t`, `...x3_t`, `...x4_t` 数组类型。

## 类型转换： 64bit 类型 <=> 128 bit 类型

### 64bit => 128bit
- `vmovl_type`, `l` for longer， 更宽的数据类型宽度（2x）

### 128bit => 64bit
- `vmovn_type`, `n` for narrow,  更窄的数据类型宽度（2x）,用旧vector创建一个新vector，新vector的元素bit位是旧vector的一半。新vector元素只保留旧vector元素的低半部分。
- `vqmovn_type`, 用旧vector创建一个新vector，新vector的元素bit位是旧vector的一半。如果旧vector元素的值超过新vector元素的最大值，则新vector元素就取最大值。否则新vector元素就等于旧vector元素的值。
- `vqmovun_type`, 作用与vqmovn_type类似，但它输入的是有符号vector，输出的是无符号vector。

## fp16/bf16
TODO