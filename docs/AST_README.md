# C 编译器 - AST 构建完成

## 项目概述

我们已经成功为 C 语言编译器添加了抽象语法树 (AST) 构建功能。编译器现在可以：

1. ✅ 词法分析 (Lexer) - 将源代码转换为 token 流
2. ✅ 语法分析 (Parser) - 验证语法正确性
3. ✅ **AST 构建 - 生成程序的结构化表示**

## 文件结构

### AST 相关文件
- `ast.h` - AST 节点类型和函数声明
- `ast.c` - AST 节点创建、管理和打印功能

### 编译器文件
- `c_parser_ast.y` - 带 AST 构建的 Bison parser
- `c_lexer_ast.l` - 简化的 Flex lexer
- `c_compiler_ast` - 编译后的可执行文件

### 测试文件
- `test_ast.c` - 简单测试程序
- `test_complex.c` - 复杂测试程序（包含 if/while）

## 支持的语言特性

当前 AST 构建器支持以下 C 语言特性：

### 类型系统
- `int`, `float`, `char`, `void`

### 声明
- 变量声明：`int x;`
- 变量初始化：`int x = 10;`

### 表达式
- 算术运算：`+`, `-`, `*`, `/`, `%`
- 关系运算：`<`, `>`, `<=`, `>=`, `==`, `!=`
- 赋值：`=`
- 标识符和字面量

### 语句
- 表达式语句
- 复合语句 `{ ... }`
- if-else 语句
- while 循环
- return 语句

### 函数
- 函数定义：`int main() { ... }`

## 使用方法

### 编译编译器
```bash
bison -y -d c_parser_ast.y
flex c_lexer_ast.l
gcc y.tab.c lex.yy.c ast.c -o c_compiler_ast -lfl
```

### 运行编译器
```bash
./c_compiler_ast < your_program.c
```

### 示例输出

输入程序:
```c
int main() {
    int a;
    a = 10 + 5;
    return 0;
}
```

AST 输出:
```
PROGRAM
  FUNCTION_DEF
    TYPE_SPECIFIER
    DECLARATOR
    COMPOUND_STMT
      DECLARATION
        TYPE_SPECIFIER
        DECLARATOR
      EXPR_STMT
        ASSIGN_EXPR
          IDENTIFIER: a
          BINARY_EXPR: +
            INT_LITERAL: 10
            INT_LITERAL: 5
      RETURN_STMT
        INT_LITERAL: 0
```

## AST 节点类型

| 节点类型 | 描述 |
|---------|------|
| `AST_PROGRAM` | 程序根节点 |
| `AST_FUNCTION_DEF` | 函数定义 |
| `AST_DECLARATION` | 变量声明 |
| `AST_COMPOUND_STMT` | 复合语句 {} |
| `AST_EXPR_STMT` | 表达式语句 |
| `AST_IF_STMT` | if 语句 |
| `AST_WHILE_STMT` | while 循环 |
| `AST_RETURN_STMT` | return 语句 |
| `AST_BINARY_EXPR` | 二元表达式 |
| `AST_ASSIGN_EXPR` | 赋值表达式 |
| `AST_IDENTIFIER` | 标识符 |
| `AST_INT_LITERAL` | 整数字面量 |
| `AST_FLOAT_LITERAL` | 浮点数字面量 |
| `AST_STRING_LITERAL` | 字符串字面量 |
| `AST_TYPE_SPECIFIER` | 类型说明符 |
| `AST_DECLARATOR` | 声明符 |

## 下一步工作

编译器的后续阶段可以包括：

1. **语义分析** 
   - 符号表管理
   - 类型检查
   - 作用域分析

2. **中间代码生成**
   - 三地址码 (TAC)
   - 控制流图 (CFG)

3. **代码优化**
   - 常量折叠
   - 死代码消除
   - 公共子表达式消除

4. **目标代码生成**
   - 汇编代码生成
   - 寄存器分配

## 注意事项

- 这是一个简化的 C 编译器，不支持完整的 C 语言特性
- 当前版本专注于核心语法结构的 AST 构建
- 可以逐步扩展以支持更多 C 语言特性

## 测试通过 ✓

- ✅ 简单程序解析
- ✅ 变量声明和赋值
- ✅ 算术表达式
- ✅ 控制流语句 (if/while)
- ✅ 函数定义
- ✅ AST 正确生成和打印

---

编译器前端（Lexer + Parser + AST）已完成！🎉
