# 简化的 C 标准库实现

## 概述

这是一个极简的 C 标准库实现，直接使用 Linux 系统调用，不依赖 glibc。

**设计原则**:
- ✅ 简洁实用
- ✅ 零依赖（仅依赖 Linux 系统调用）
- ✅ 直接使用内联汇编
- ✅ 代码量极小（~300行）

## 支持的功能

### stdio.h ✨ 完整实现
- `int putchar(int c)` - 输出字符
- `int puts(const char *s)` - 输出字符串
- `int printf(const char *format, ...)` - **完整格式化输出**
  - ✅ `%d`, `%i` - 有符号整数
  - ✅ `%u` - 无符号整数
  - ✅ `%x`, `%X` - 十六进制
  - ✅ `%o` - 八进制
  - ✅ `%c` - 字符
  - ✅ `%s` - 字符串
  - ✅ `%p` - 指针
  - ✅ `%f` - 浮点数（简化版）
  - ✅ `%ld`, `%lu`, `%lld` - 长整数
  - ✅ `%%` - 百分号

### string.h
- `int strlen(const char *s)` - 计算字符串长度
- `int strcmp(const char *s1, const char *s2)` - 比较字符串
- `char *strcpy(char *dest, const char *src)` - 复制字符串
- `char *strcat(char *dest, const char *src)` - 连接字符串
- `void *memset(void *s, int c, int n)` - 设置内存
- `void *memcpy(void *dest, const void *src, int n)` - 复制内存

### stdlib.h ✨ 完整实现
- `void *malloc(int size)` - **分配内存（完整空闲链表实现）**
- `void free(void *ptr)` - **释放内存（完整实现）**
- `void exit(int status)` - 退出程序
- `int atoi(const char *s)` - **字符串转整数（完整实现）**
- `long atol(const char *s)` - 字符串转长整数
- `long strtol(const char *s, char **endptr, int base)` - 带进制的转换

## 编译使用

### 构建标准库
```bash
cd stdlib
make
```

### 使用标准库
```c
#include "stdio.h"
#include "string.h"

int main() {
    printf("Hello, World!\n");
    printf("Number: %d\n", 42);
    return 0;
}
```

编译时链接标准库：
```bash
gcc -nostdlib -Istdlib program.c stdlib/libmini.a -o program
```

或使用我们的编译器：
```bash
# 注意：需要确保生成的汇编代码与标准库兼容
./cc program.c
# 手动链接标准库
gcc program.s stdlib/libmini.a -o program
```

## 限制

1. **浮点格式化**: `%f` 简化版，精度有限
2. **仅 Linux x86-64**: 使用 Linux 特定的系统调用
3. **内存管理**: malloc/free 简单实现，不合并空闲块
4. **格式宽度**: printf 暂不完全支持宽度和对齐

## 代码统计

| 模块 | 行数 | 功能完成度 |
|------|------|------------|
| stdio.c | ~300行 | ✅ 100% |
| string.c | ~60行 | ✅ 100% |
| stdlib.c | ~250行 | ✅ 100% |
| 头文件 | ~70行 | ✅ 100% |
| **总计** | **~680行** | **100%** |

## 系统调用使用

直接使用内联汇编调用 Linux 系统调用：

```c
static long syscall1(long n, long a1) {
    long ret;
    asm volatile("syscall" 
        : "=a"(ret) 
        : "a"(n), "D"(a1) 
        : "rcx", "r11", "memory");
    return ret;
}
```

使用的系统调用：
- `SYS_read` (0) - 读取
- `SYS_write` (1) - 写入
- `SYS_brk` (12) - 内存管理
- `SYS_exit` (60) - 退出

## 示例程序

```c
#include "stdio.h"
#include "string.h"
#include "stdlib.h"

int main() {
    // 字符串操作
    char str1[20] = "Hello";
    char str2[20] = " World";
    strcat(str1, str2);
    printf("Concatenated: %s\n", str1);
    
    // 数值转换
    int num = atoi("123");
    printf("Number: %d\n", num);
    
    // 内存分配
    int *arr = (int*)malloc(10 * sizeof(int));
    arr[0] = 42;
    printf("Array[0]: %d\n", arr[0]);
    
    return 0;
}
```

## 设计说明

这个标准库设计为：
1. **教学目的**: 展示如何从零实现标准库
2. **系统编程**: 直接使用系统调用
3. **编译器测试**: 作为我们编译器的测试平台

**不适合生产环境**，仅用于学习和测试！

