# 快速开始指南

## ⚡ 5分钟快速上手

### 1. 构建编译器（1分钟）

```bash
cd compiler
make
```

### 2. 编写第一个程序（1分钟）

创建 `hello.c`：

```c
int main() {
    return 42;
}
```

### 3. 编译并运行（1分钟）

```bash
./cc hello.c
./output
echo $?  # 输出: 42
```

### 4. 查看生成的汇编代码（1分钟）

```bash
cat output.s
```

### 5. 尝试更多示例（1分钟）

```bash
./cc examples/arithmetic.c && ./output; echo $?  # 输出: 15
./cc examples/condition.c && ./output; echo $?   # 输出: 20
./cc examples/loop.c && ./output; echo $?        # 输出: 55
```

---

## 📝 常用命令

### 构建相关

```bash
make           # 构建编译器
make clean     # 清理构建文件
make rebuild   # 重新构建
make test      # 运行测试
make help      # 显示帮助
```

### 编译器使用

```bash
./cc program.c              # 基本编译
./cc -o test.s program.c    # 指定输出文件
./cc --debug program.c      # 调试模式
./cc --help                 # 显示帮助
```

### 查看输出

```bash
cat output.s               # 查看汇编代码
objdump -d output          # 反汇编可执行文件
./output; echo $?          # 运行并查看退出码
```

---

## 🎓 示例程序

### 示例 1：变量和算术

```c
int main() {
    int a;
    int b;
    a = 10;
    b = 5;
    return a + b;  // 返回 15
}
```

### 示例 2：条件判断

```c
int main() {
    int x;
    x = 5;
    
    if (x < 10) {
        x = 20;
    }
    
    return x;  // 返回 20
}
```

### 示例 3：循环计算

```c
int main() {
    int sum;
    int i;
    
    sum = 0;
    i = 1;
    
    while (i < 11) {
        sum = sum + i;
        i = i + 1;
    }
    
    return sum;  // 返回 55 (1+2+...+10)
}
```

### 示例 4：综合应用

```c
int main() {
    int n;
    int result;
    
    n = 10;
    result = 1;
    
    while (n > 0) {
        if (n > 5) {
            result = result * 2;
        }
        n = n - 1;
    }
    
    return result;  // 返回 32 (2^5)
}
```

---

## ⚠️ 常见问题

### Q: 编译失败怎么办？

A: 检查以下内容：
- 是否安装了 `flex` 和 `bison`？
- 是否有语法错误？
- 使用 `--debug` 模式查看详细信息

### Q: 如何查看中间结果？

A: 使用调试模式：

```bash
./cc --debug program.c
```

这会显示 AST 和符号表。

### Q: 支持哪些 C 特性？

A: 当前支持：
- 基本类型：int, float, char, void
- 运算符：+, -, *, /, %, <, >, <=, >=, ==, !=, =
- 控制流：if-else, while
- 变量和表达式

不支持：
- 函数参数
- 数组和指针
- 结构体
- 函数调用
- 预处理器

### Q: 如何贡献？

A: 
1. Fork 项目
2. 创建功能分支
3. 提交代码
4. 创建 Pull Request

---

## 📚 深入学习

- 📖 [README.md](README.md) - 完整用户文档
- 📖 [PROJECT_OVERVIEW.md](PROJECT_OVERVIEW.md) - 项目概览
- 📖 [docs/COMPILER_ARCHITECTURE.md](docs/COMPILER_ARCHITECTURE.md) - 架构详解
- 📖 [docs/FINAL_SUMMARY.md](docs/FINAL_SUMMARY.md) - 项目总结

---

## 🎯 下一步

1. ✅ 成功构建编译器
2. ✅ 运行示例程序
3. ⏭️ 尝试编写自己的程序
4. ⏭️ 阅读详细文档
5. ⏭️ 贡献代码或提出建议

---

**需要帮助？** 查看 README.md 或提交 issue。

**想贡献？** 查看 PROJECT_OVERVIEW.md 了解项目结构。
