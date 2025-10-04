# C 编译器架构

## 完整的编译器流程

```
源代码 (source.c)
    ↓
┌────────────────────────────────────────────────────┐
│  阶段 1: 词法分析 (Lexer)                          │ ✅ 已完成
│  ・将源代码分解为 tokens                           │
│  ・识别关键字、标识符、字面量、运算符              │
│  输入: "int x = 10;"                               │
│  输出: [INT, IDENTIFIER, ASSIGN, INT_CONST, SEMI]  │
└────────────────────────────────────────────────────┘
    ↓
┌────────────────────────────────────────────────────┐
│  阶段 2: 语法分析 (Parser)                         │ ✅ 已完成
│  ・验证 token 序列的语法正确性                     │
│  ・构建抽象语法树 (AST)                            │
│  输入: token 流                                    │
│  输出: AST                                         │
└────────────────────────────────────────────────────┘
    ↓
┌────────────────────────────────────────────────────┐
│  阶段 3: 语义分析 (Semantic Analysis)              │ ⏳ 下一步
│  ・构建符号表                                      │
│  ・类型检查                                        │
│  ・作用域分析                                      │
│  ・检测语义错误                                    │
│  输入: AST                                         │
│  输出: 带类型信息的 AST + 符号表                   │
└────────────────────────────────────────────────────┘
    ↓
┌────────────────────────────────────────────────────┐
│  阶段 4: 中间代码生成 (IR Generation)              │ 🔜 未来
│  ・将 AST 转换为三地址码                           │
│  ・简化复杂表达式                                  │
│  ・规范化控制流                                    │
│  输入: 带类型的 AST                                │
│  输出: 中间表示 (IR)                               │
└────────────────────────────────────────────────────┘
    ↓
┌────────────────────────────────────────────────────┐
│  阶段 5: 代码优化 (Optimization)                   │ 🔜 未来
│  ・常量折叠                                        │
│  ・死代码消除                                      │
│  ・公共子表达式消除                                │
│  输入: IR                                          │
│  输出: 优化的 IR                                   │
└────────────────────────────────────────────────────┘
    ↓
┌────────────────────────────────────────────────────┐
│  阶段 6: 目标代码生成 (Code Generation)            │ 🔜 未来
│  ・指令选择                                        │
│  ・寄存器分配                                      │
│  ・生成汇编代码                                    │
│  输入: 优化的 IR                                   │
│  输出: 汇编代码 (.s)                               │
└────────────────────────────────────────────────────┘
    ↓
┌────────────────────────────────────────────────────┐
│  阶段 7: 汇编和链接                                │ 🔜 未来
│  ・调用系统汇编器 (as)                             │
│  ・调用链接器 (ld)                                 │
│  输入: 汇编代码                                    │
│  输出: 可执行文件                                  │
└────────────────────────────────────────────────────┘
    ↓
可执行文件 (a.out)
```

## 关键数据结构

### 当前已有
```c
// Token (Lexer 输出)
typedef struct {
    TokenType type;
    char *value;
    int lineno;
} Token;

// AST Node (Parser 输出)
typedef struct ASTNode {
    ASTNodeType type;
    int lineno;
    struct ASTNode **children;
    int num_children;
    union {
        int int_val;
        float float_val;
        char *string_val;
        OperatorType op_type;
    } value;
    void *semantic_info;  // 将在语义分析阶段填充
} ASTNode;
```

### 需要添加 (下一步)
```c
// 类型系统
typedef enum {
    TYPE_INT,
    TYPE_FLOAT,
    TYPE_CHAR,
    TYPE_VOID,
    TYPE_POINTER,
    TYPE_ARRAY,
    TYPE_FUNCTION
} DataType;

typedef struct TypeInfo {
    DataType base_type;
    int pointer_level;      // 指针层级
    int array_size;         // 数组大小
    struct TypeInfo *return_type;  // 函数返回类型
    struct TypeInfo **param_types; // 函数参数类型
    int num_params;
} TypeInfo;

// 符号表项
typedef struct Symbol {
    char *name;
    TypeInfo *type;
    int scope_level;
    int offset;             // 相对于基址的偏移
    int is_function;
    ASTNode *declaration;   // 指向声明节点
} Symbol;

// 作用域
typedef struct Scope {
    Symbol **symbols;
    int num_symbols;
    int capacity;
    struct Scope *parent;   // 父作用域
} Scope;

// 符号表
typedef struct SymbolTable {
    Scope *current_scope;
    Scope *global_scope;
    int current_level;
} SymbolTable;
```

### 未来需要 (中间代码)
```c
// 三地址码指令
typedef enum {
    IR_ADD, IR_SUB, IR_MUL, IR_DIV,
    IR_ASSIGN, IR_LABEL, IR_GOTO,
    IR_IF_FALSE, IR_PARAM, IR_CALL, IR_RETURN
} IROpcode;

typedef struct IRInstruction {
    IROpcode op;
    char *result;
    char *arg1;
    char *arg2;
    int label;
} IRInstruction;

// IR 程序
typedef struct IRProgram {
    IRInstruction **instructions;
    int num_instructions;
    int next_temp;    // 临时变量计数器
    int next_label;   // 标签计数器
} IRProgram;
```

## 下一步实施计划

### 立即开始：符号表实现

#### 第 1 天：基础设施
```c
1. 创建 symbol_table.h 和 symbol_table.c
2. 实现类型系统 (TypeInfo)
3. 实现符号表基本操作：
   - symbol_table_init()
   - symbol_table_insert()
   - symbol_table_lookup()
   - enter_scope() / exit_scope()
```

#### 第 2 天：AST 遍历
```c
1. 创建 semantic.h 和 semantic.c
2. 实现 AST 遍历函数：
   - analyze_program(AST)
   - analyze_function(AST)
   - analyze_declaration(AST)
   - analyze_statement(AST)
   - analyze_expression(AST)
```

#### 第 3 天：类型检查
```c
1. 为每个 AST 节点推导类型
2. 检查类型兼容性：
   - check_binary_expr_types()
   - check_assignment_types()
   - check_function_call_types()
```

#### 第 4 天：错误报告
```c
1. 实现友好的错误消息
2. 集成到编译器主流程
3. 测试各种错误情况
```

### 示例：符号表的使用

```c
// 输入程序
int x;
int main() {
    float y;
    x = 10;      // OK: x 是全局 int
    y = 3.14;    // OK: y 是局部 float
    z = 5;       // ERROR: z 未声明
    y = x + 1;   // OK: 类型兼容
}

// 符号表内容
Global Scope:
  - x: int, offset=0
  - main: function()->int
    Local Scope (main):
      - y: float, offset=-4

// 错误检测
Error at line 5: Undeclared variable 'z'
```

## 工具和文件组织

### 当前文件结构
```
compiler/
├── ast.h / ast.c              ✅ AST 定义
├── c_lexer_ast.l              ✅ Lexer
├── c_parser_ast.y             ✅ Parser
└── c_compiler_ast             ✅ 编译器可执行文件
```

### 下一步添加
```
compiler/
├── ast.h / ast.c
├── c_lexer_ast.l
├── c_parser_ast.y
├── symbol_table.h / symbol_table.c  ⏳ 新增
├── semantic.h / semantic.c          ⏳ 新增
├── types.h                          ⏳ 新增
└── error.h / error.c                ⏳ 新增
```

### 未来添加
```
compiler/
├── ... (上述文件)
├── ir_gen.h / ir_gen.c       🔜 中间代码生成
├── optimizer.h / optimizer.c  🔜 优化器
├── codegen.h / codegen.c      🔜 代码生成
└── Makefile                   🔜 构建系统
```

## 测试驱动开发

### 测试用例 1: 简单声明
```c
int x;
int main() {
    x = 10;
    return 0;
}
// 预期：符号表包含 x 和 main，无错误
```

### 测试用例 2: 未声明变量
```c
int main() {
    x = 10;  // 错误
    return 0;
}
// 预期：错误 "Undeclared variable 'x'"
```

### 测试用例 3: 类型不匹配
```c
int main() {
    int x;
    x = 3.14;  // 警告或错误
    return 0;
}
// 预期：警告 "Type mismatch in assignment"
```

---

## 准备好开始了吗？

我可以立即开始实现符号表和语义分析。这将使您的编译器能够：

✅ 检测未声明的变量
✅ 检测类型错误
✅ 管理作用域
✅ 为代码生成准备类型信息

需要我现在开始实现吗？
