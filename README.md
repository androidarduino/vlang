# C 语言编译器

一个功能完整的 C 语言编译器，能够将 C 代码编译成 x86-64 可执行文件。

## 🌟 特性

- ✅ **完整的编译流程**：词法分析 → 语法分析 → 语义分析 → 代码生成
- ✅ **x86-64 代码生成**：生成真正可执行的机器码
- ✅ **类型检查**：完整的类型系统和语义分析
- ✅ **错误报告**：友好的错误和警告信息
- ✅ **控制流**：支持 if-else、while、for 循环，break/continue
- ✅ **函数**：函数参数、调用、递归 🆕
- ✅ **表达式**：算术、关系、逻辑、赋值表达式

## 📋 支持的 C 语言特性

### 数据类型
- `int`, `float`, `char`, `void`

### 函数 🆕
- 函数定义（带参数）
- 函数调用（最多6个参数）
- 递归函数
- 多函数程序

### 语句
- 变量声明和初始化
- 赋值语句
- `if-else` 条件语句
- `while` 循环
- `for` 循环（传统和C99风格）
- `break` 和 `continue` 语句 🆕
- `return` 语句
- 复合语句（代码块）

### 运算符
- 算术：`+`, `-`, `*`, `/`, `%`
- 关系：`<`, `>`, `<=`, `>=`, `==`, `!=`
- 逻辑：`&&`, `||`, `!`
- 赋值：`=`

## 🚀 快速开始

### 前置要求

确保您的系统已安装以下工具：

```bash
# Ubuntu/Debian
sudo apt-get install build-essential flex bison

# macOS
brew install flex bison

# Fedora/RHEL
sudo dnf install gcc flex bison
```

### 构建编译器

```bash
# 1. 克隆或下载项目
cd compiler

# 2. 构建编译器
make

# 3. 验证构建
./cc --help
```

构建成功后，会生成可执行文件 `cc`。

## 📖 使用方法

### 基本用法

```bash
# 编译 C 程序
./cc program.c

# 运行生成的程序
./output

# 查看退出码
echo $?
```

### 命令行选项

```bash
./cc [options] <input.c>

选项：
  -o <file>    指定输出汇编文件（默认：output.s）
  --debug      启用调试输出（显示 AST 和符号表）
  -h, --help   显示帮助信息
```

### 示例

```bash
# 示例 1: 基本编译
./cc examples/simple.c

# 示例 2: 指定输出文件
./cc -o test.s examples/simple.c

# 示例 3: 调试模式
./cc --debug examples/simple.c
```

## 📝 示例程序

### Hello World (返回值版本)

```c
// examples/hello.c
int main() {
    return 42;
}
```

编译并运行：

```bash
./cc examples/hello.c
./output
echo $?  # 输出: 42
```

### 变量和算术运算

```c
// examples/arithmetic.c
int main() {
    int x;
    int y;
    int result;
    
    x = 10;
    y = 5;
    result = x + y;
    
    return result;
}
```

### 条件语句

```c
// examples/condition.c
int main() {
    int x;
    x = 5;
    
    if (x < 10) {
        x = 20;
    }
    
    return x;
}
```

### 循环

```c
// examples/loop.c
int main() {
    int sum;
    int n;
    
    sum = 0;
    n = 1;
    
    while (n < 11) {
        sum = sum + n;
        n = n + 1;
    }
    
    return sum;  // 返回 55 (1+2+...+10)
}
```

### 函数调用和递归 🆕

```c
// 递归函数
int fibonacci(int n) {
    if (n < 2) {
        return n;
    }
    return fibonacci(n - 1) + fibonacci(n - 2);
}

// 带参数的函数
int add(int a, int b) {
    return a + b;
}

int main() {
    int result;
    result = fibonacci(7);     // 递归调用
    result = result + add(3, 5);  // 函数调用
    return result;  // 返回 21 (13 + 8)
}
```

### Break 和 Continue 🆕

```c
int main() {
    int sum = 0;
    
    for (int i = 1; i < 20; i = i + 1) {
        if (i == 5) {
            continue;  // 跳过 5
        }
        if (i > 10) {
            break;     // 在 10 后停止
        }
        sum = sum + i;
    }
    
    return sum;  // 返回 50
}
```

## 🔧 项目结构

```
compiler/
├── Makefile              # 构建脚本
├── README.md             # 本文档
│
├── include/              # 头文件
│   ├── ast.h
│   ├── types.h
│   ├── symbol_table.h
│   ├── semantic.h
│   └── codegen.h
│
├── src/                  # 源代码
│   ├── lexer/
│   │   └── lexer.l       # 词法分析器
│   ├── parser/
│   │   └── parser.y      # 语法分析器
│   ├── ast/
│   │   └── ast.c         # AST 实现
│   ├── semantic/
│   │   ├── types.c       # 类型系统
│   │   ├── symbol_table.c # 符号表
│   │   └── semantic.c    # 语义分析
│   ├── codegen/
│   │   └── codegen.c     # 代码生成
│   └── main.c            # 主程序
│
├── examples/             # 示例程序
│   ├── simple.c
│   └── ...
│
├── docs/                 # 文档
│   ├── COMPILER_ARCHITECTURE.md
│   ├── COMPILER_ROADMAP.md
│   └── ...
│
├── tests/                # 测试文件
└── build/                # 构建输出（自动生成）
```

## 🛠️ 开发

### 构建命令

```bash
# 构建编译器
make

# 清理构建文件
make clean

# 重新构建
make rebuild

# 运行测试
make test

# 查看帮助
make help
```

### 调试

```bash
# 启用调试模式查看 AST 和符号表
./cc --debug program.c

# 查看生成的汇编代码
cat output.s

# 使用 GDB 调试生成的程序
gdb ./output
```

## 📊 编译流程

```
源代码 (*.c)
    ↓
┌─────────────────────────┐
│ Phase 1: 词法分析        │  Lexer (Flex)
│ 将源代码转换为 token 流  │
└─────────────────────────┘
    ↓ tokens
┌─────────────────────────┐
│ Phase 2: 语法分析        │  Parser (Bison)
│ 构建抽象语法树 (AST)     │
└─────────────────────────┘
    ↓ AST
┌─────────────────────────┐
│ Phase 3: 语义分析        │  Semantic Analyzer
│ 类型检查、符号表管理     │
└─────────────────────────┘
    ↓ 类型化的 AST
┌─────────────────────────┐
│ Phase 4: 代码生成        │  Code Generator
│ 生成 x86-64 汇编代码     │
└─────────────────────────┘
    ↓ Assembly (.s)
┌─────────────────────────┐
│ Phase 5: 汇编和链接      │  GCC Assembler & Linker
│ 生成可执行文件           │
└─────────────────────────┘
    ↓
可执行文件 (output)
```

## 🧪 测试

运行自动化测试：

```bash
make test
```

手动测试示例：

```bash
# 测试 1: 简单返回
echo 'int main() { return 42; }' | ./cc /dev/stdin
./output; echo $?  # 应该输出 42

# 测试 2: 算术运算
./cc examples/arithmetic.c
./output; echo $?  # 应该输出 15

# 测试 3: 循环
./cc examples/loop.c
./output; echo $?  # 应该输出 55
```

## 📚 文档

详细文档请参阅 `docs/` 目录：

- [编译器架构](docs/COMPILER_ARCHITECTURE.md) - 详细的架构说明
- [开发路线图](docs/COMPILER_ROADMAP.md) - 开发规划和进度
- [AST 文档](docs/AST_README.md) - 抽象语法树详解
- [最终总结](docs/FINAL_SUMMARY.md) - 完整项目总结

## ⚠️ 限制

当前版本的限制：

- 数组、指针、结构体有语法支持但代码生成不完整（运行时可能出错）
- 不支持自增自减运算符 (`++`, `--`)
- 不支持函数前向声明（需要先定义后使用）
- 不支持预处理器指令 (`#include`, `#define` 等)
- 不支持标准库函数 (`printf`, `scanf` 等)
- 只支持单文件编译

详细的未支持特性请参阅 `UNSUPPORTED_FEATURES.md`

## 🔮 未来计划

- [x] ~~支持函数参数和多参数函数~~ ✅ v1.3
- [x] ~~支持函数调用~~ ✅ v1.3
- [x] ~~支持 for 循环~~ ✅ v1.1
- [x] ~~支持 break/continue~~ ✅ v1.3
- [ ] 完善数组和指针支持
- [ ] 完善结构体支持
- [ ] 添加自增自减运算符
- [ ] 支持函数前向声明
- [ ] 添加代码优化
- [ ] 支持多文件编译
- [ ] 添加预处理器

## 🎓 技术栈

- **Flex** - 词法分析器生成器
- **Bison** - 语法分析器生成器
- **C** - 编译器实现语言
- **x86-64 Assembly** - 目标代码
- **GCC** - 汇编器和链接器

## 📄 许可证

本项目为教育目的开发。

## 👨‍💻 作者

George (with AI assistance)

## 🙏 致谢

本项目参考了以下资源：

- "Compilers: Principles, Techniques, and Tools" (Dragon Book)
- "Modern Compiler Implementation in C"
- Intel x86-64 Architecture Manual
- System V AMD64 ABI

---

**开始使用：**

```bash
make            # 构建编译器
./cc test.c     # 编译你的第一个程序
./output        # 运行程序
```

如有问题或建议，欢迎反馈！
