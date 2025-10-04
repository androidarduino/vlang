# C语言编译器 (C-to-x86-64)

一个用C语言编写的教学型C编译器，将C代码编译为x86-64汇编。

**版本**: 0.3-alpha  
**状态**: 🚧 积极开发中  
**最后更新**: 2025-10-04

---

## 🎯 项目目标

构建一个功能完整的C语言编译器，支持：
- ✅ 完整的词法和语法分析
- ✅ 语义分析和类型检查
- ✅ x86-64汇编代码生成
- ✅ 基础的C语言特性

---

## ⚡ 快速开始

### 编译编译器
```bash
make
```

### 编译C程序
```bash
./cc your_program.c
./output
echo $?  # 查看返回值
```

### 示例程序
```c
// example.c
int main() {
    int x = 10;
    int y = 20;
    return x + y;  // 返回 30
}
```

```bash
./cc example.c
./output
# 输出: (退出码 30)
```

---

## ✅ 当前支持的功能 (39%)

### 基础类型
- ✅ int, float, char, void

### 运算符
- ✅ 算术: `+`, `-`, `*`, `/`, `%`
- ✅ 比较: `==`, `!=`, `<`, `>`, `<=`, `>=`
- ✅ 逻辑: `&&`, `||`, `!`
- ✅ 一元: `-` (取负)

### 控制流
- ✅ `if` / `if-else`
- ✅ `while` 循环
- ✅ `for` 循环
- ✅ `break` 语句 ✨
- ✅ `continue` 语句 ✨

### 函数
- ✅ 函数定义和调用
- ✅ 参数传递
- ✅ 返回值

### 数组
- ✅ 一维数组（完整支持）
- ✅ 多维数组声明
- ✅ 数组初始化列表 `int arr[] = {1, 2, 3};`

### 结构体
- ✅ 结构体定义
- ✅ 变量声明
- ✅ 成员访问 `p.x = 10;`

### 字符串
- ✅ 字符串字面量 `"Hello"`

---

## ❌ 主要未实现功能

### 高优先级（核心缺失）
- ❌ **指针变量声明** `int *p;`
- ❌ 多维数组元素访问 `matrix[i][j]`
- ❌ 指针运算 `p + 1`

### 中优先级（常用功能）
- ❌ `do-while` 循环
- ❌ `switch-case` 语句
- ❌ 三元运算符 `? :`
- ❌ 递增/递减 `++` / `--`
- ❌ 复合赋值 `+=`, `-=`, etc.

### 低优先级（高级功能）
- ❌ 位运算符 `&`, `|`, `^`, `~`, `<<`, `>>`
- ❌ `typedef`, `enum`, `union`
- ❌ 预处理器 `#include`, `#define`
- ❌ 标准库函数

完整列表请查看 [IMPLEMENTATION_STATUS.md](IMPLEMENTATION_STATUS.md)

---

## 📊 项目统计

| 指标 | 数值 |
|------|------|
| 总代码行数 | 3,604 行 |
| 已实现特性 | 37/95 (39%) |
| 部分实现 | 4 (4%) |
| 未实现 | 54 (57%) |
| 开发时间 | ~80 小时 |

---

## 🏗️ 架构

### 编译流程
```
C源代码
   ↓
[1. 词法分析] (Lexer)
   ↓
Token流
   ↓
[2. 语法分析] (Parser)
   ↓
抽象语法树 (AST)
   ↓
[3. 语义分析] (Semantic Analyzer)
   ↓
带类型信息的AST
   ↓
[4. 代码生成] (Code Generator)
   ↓
x86-64汇编
   ↓
[5. 汇编链接] (as + ld)
   ↓
可执行文件
```

### 目录结构
```
compiler/
├── src/
│   ├── lexer/          # 词法分析器
│   ├── parser/         # 语法分析器
│   ├── ast/            # AST节点定义
│   ├── semantic/       # 语义分析
│   │   ├── types.c     # 类型系统
│   │   └── symbol_table.c  # 符号表
│   └── codegen/        # 代码生成
├── include/            # 头文件
├── examples/           # 示例程序
├── build/              # 编译产物
└── Makefile
```

---

## 🧪 测试

### 运行示例
```bash
# 基础算术
./cc examples/arithmetic.c && ./output

# 数组操作
./cc examples/array_test.c && ./output

# 控制流
./cc examples/control_flow.c && ./output
```

### 测试覆盖
- ✅ 一维数组：完整测试
- ✅ 结构体：基础功能测试
- ✅ 控制流：if/while/for测试
- ✅ 函数：参数和返回值测试

---

## 🐛 已知问题

### 当前限制
1. ⚠️ 结构体成员类型硬编码为 int
2. ⚠️ 结构体成员偏移硬编码
3. ⚠️ 复杂结构体表达式可能超时
4. ⚠️ 多维数组只能声明不能访问
5. ⚠️ 无法声明指针变量

### 技术债务
- 需要重构 declarator 处理逻辑
- 类型系统需要扩展
- 错误处理机制需要改进
- 缺少警告系统

详见 [IMPLEMENTATION_STATUS.md](IMPLEMENTATION_STATUS.md)

---

## 🔄 最近更新

### v0.3-alpha (2025-10-04)
- ✅ **修复**: 结构体变量声明超时
- ✅ **修复**: 结构体成员访问无限递归
- ✅ **实现**: 结构体成员访问代码生成
- ✅ **修复**: 多维数组段错误
- ✅ **改进**: 多维数组完整声明支持

### v0.2-alpha
- ✅ 一维数组完整实现
- ✅ 数组初始化列表
- ✅ 字符串字面量支持

### v0.1-alpha
- ✅ 基础编译流程
- ✅ 基础类型和运算符
- ✅ 控制流语句
- ✅ 函数定义和调用

详见 [ALL_FIXES_COMPLETE.md](ALL_FIXES_COMPLETE.md)

---

## 🚀 开发路线图

### 第一阶段：完善基础功能 ✅ (接近完成)
- [x] 结构体基础功能
- [x] 多维数组声明
- [x] break / continue ✨ (新发现：已实现)
- [ ] 指针变量声明 🔴 (下一步)
- [ ] 多维数组元素访问

### 第二阶段：增强表达式
- [ ] do-while 循环
- [ ] 三元运算符
- [ ] 递增/递减运算符
- [ ] 复合赋值运算符
- [ ] 位运算符

### 第三阶段：高级类型
- [ ] switch-case
- [ ] typedef
- [ ] enum / union
- [ ] 全局变量
- [ ] 静态变量

### 第四阶段：预处理和库
- [ ] 预处理器
- [ ] 基础标准库
- [ ] 多文件编译
- [ ] 链接器集成

---

## 📚 文档

- [完整实现状态](IMPLEMENTATION_STATUS.md) - 所有功能的详细列表
- [修复报告](ALL_FIXES_COMPLETE.md) - 最近的bug修复
- [结构体实现](STRUCT_ARRAY_FIX_REPORT.md) - 结构体功能详情
- [多维数组修复](MULTIDIM_ARRAY_FIX.md) - 多维数组实现

---

## 🤝 贡献

欢迎贡献！特别需要：
- 🐛 Bug报告和修复
- ✨ 新功能实现
- 📝 文档改进
- 🧪 测试用例

---

## 📄 许可

MIT License

---

## 🎓 学习资源

### 推荐阅读
- "Compilers: Principles, Techniques, and Tools" (龙书)
- "Engineering a Compiler" (鲸书)
- "Modern Compiler Implementation in C" (虎书)

### 相关技术
- x86-64 汇编 (System V AMD64 ABI)
- Flex/Bison (词法/语法分析)
- 编译器优化技术

---

## 📞 联系

- 项目：C-to-x86-64 Compiler
- 状态：积极开发中
- 版本：0.3-alpha

---

**⚠️ 警告**: 这是一个教学项目，不建议用于生产环境。

**🎯 目标**: 通过实现编译器深入理解编译原理和代码生成技术。

---

Made with ❤️ by the Compiler Team
