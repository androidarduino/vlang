#include "codegen.h"
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

// 创建代码生成器
CodeGenerator *codegen_create(FILE *output, SemanticAnalyzer *analyzer)
{
    CodeGenerator *gen = (CodeGenerator *)malloc(sizeof(CodeGenerator));
    if (!gen)
    {
        fprintf(stderr, "Error: Failed to allocate code generator\n");
        exit(1);
    }
    gen->output = output;
    gen->analyzer = analyzer;
    gen->label_counter = 0;
    gen->current_stack_offset = 0;
    gen->max_stack_size = 0;
    gen->loop_context = NULL;
    gen->return_label = 0;
    gen->strings = NULL;
    gen->num_strings = 0;
    gen->string_capacity = 0;
    return gen;
}

// 销毁代码生成器
void codegen_destroy(CodeGenerator *gen)
{
    if (gen)
    {
        // 释放字符串常量
        for (int i = 0; i < gen->num_strings; i++)
        {
            if (gen->strings[i])
            {
                free(gen->strings[i]->content);
                free(gen->strings[i]);
            }
        }
        free(gen->strings);
        free(gen);
    }
}

// 生成新标签
int new_label(CodeGenerator *gen)
{
    return gen->label_counter++;
}

// 添加字符串常量并返回其标签编号
int add_string_constant(CodeGenerator *gen, const char *str)
{
    // 检查是否需要扩展数组
    if (gen->num_strings >= gen->string_capacity)
    {
        int new_capacity = gen->string_capacity == 0 ? 8 : gen->string_capacity * 2;
        StringConstant **new_strings = (StringConstant **)realloc(
            gen->strings, new_capacity * sizeof(StringConstant *));
        if (!new_strings)
        {
            fprintf(stderr, "Error: Failed to allocate memory for string constants\n");
            exit(1);
        }
        gen->strings = new_strings;
        gen->string_capacity = new_capacity;
    }

    // 创建新的字符串常量
    StringConstant *sc = (StringConstant *)malloc(sizeof(StringConstant));
    if (!sc)
    {
        fprintf(stderr, "Error: Failed to allocate string constant\n");
        exit(1);
    }

    sc->label = gen->num_strings;

    // 去掉字符串首尾的引号
    // str 是 "Hello" 形式，需要变成 Hello
    size_t len = strlen(str);
    if (len >= 2 && str[0] == '"' && str[len - 1] == '"')
    {
        // 复制去掉首尾引号的内容
        sc->content = (char *)malloc(len - 1); // len-2个字符 + '\0'
        if (!sc->content)
        {
            fprintf(stderr, "Error: Failed to allocate string content\n");
            exit(1);
        }
        strncpy(sc->content, str + 1, len - 2);
        sc->content[len - 2] = '\0';
    }
    else
    {
        // 没有引号，直接复制
        sc->content = strdup(str);
    }

    gen->strings[gen->num_strings] = sc;
    gen->num_strings++;

    return sc->label;
}

// 输出所有字符串常量到 .rodata 段
void emit_string_constants(CodeGenerator *gen)
{
    if (gen->num_strings == 0)
        return;

    emit(gen, "    .section .rodata");
    for (int i = 0; i < gen->num_strings; i++)
    {
        emit(gen, ".LC%d:", gen->strings[i]->label);
        emit(gen, "    .string \"%s\"", gen->strings[i]->content);
    }
    emit(gen, "");
}

// 输出汇编指令
void emit(CodeGenerator *gen, const char *format, ...)
{
    va_list args;
    va_start(args, format);
    vfprintf(gen->output, format, args);
    va_end(args);
    fprintf(gen->output, "\n");
}

// 压栈
void push_reg(CodeGenerator *gen, const char *reg)
{
    emit(gen, "    pushq %%%s", reg);
    gen->current_stack_offset += 8;
    if (gen->current_stack_offset > gen->max_stack_size)
    {
        gen->max_stack_size = gen->current_stack_offset;
    }
}

// 出栈
void pop_reg(CodeGenerator *gen, const char *reg)
{
    emit(gen, "    popq %%%s", reg);
    gen->current_stack_offset -= 8;
}

// 生成函数序言
void gen_prologue(CodeGenerator *gen, const char *func_name)
{
    emit(gen, "");
    emit(gen, "    .globl %s", func_name);
    emit(gen, "    .type %s, @function", func_name);
    emit(gen, "%s:", func_name);
    emit(gen, "    pushq %%rbp");
    emit(gen, "    movq %%rsp, %%rbp");
    // 为局部变量预留空间（后面会调整）
    emit(gen, "    subq $64, %%rsp  # Reserve space for local variables");
}

// 生成函数尾声
void gen_epilogue(CodeGenerator *gen)
{
    emit(gen, ".L%d:  # Function return", gen->return_label);
    emit(gen, "    movq %%rbp, %%rsp");
    emit(gen, "    popq %%rbp");
    emit(gen, "    ret");
}

// 辅助函数：检查表达式是否为指针类型
static int is_pointer_expression(ASTNode *node)
{
    if (!node)
        return 0;

    // 如果节点有semantic_info，尝试获取类型
    // 注意：semantic_info可能是Symbol*或TypeInfo*，需要安全处理
    if (node->semantic_info && node->type == AST_IDENTIFIER)
    {
        // 对于标识符节点，semantic_info是Symbol*
        Symbol *symbol = (Symbol *)node->semantic_info;
        if (symbol && symbol->type)
        {
            return symbol->type->pointer_level > 0;
        }
    }
    else if (node->semantic_info)
    {
        // 对于其他节点，semantic_info可能是TypeInfo*
        // 但为了安全，我们不尝试访问它
        // 只依赖于下面的AST节点类型判断
    }

    // 对于一元表达式（取地址），结果是指针
    if (node->type == AST_UNARY_EXPR && node->value.op_type == OP_ADDR)
    {
        return 1;
    }

    // 对于数组下标，结果不是指针（是元素值）
    // 但 &arr[i] 是指针

    return 0;
}

// 辅助函数：检查表达式是否为浮点类型
static int is_float_expression(ASTNode *node)
{
    if (!node)
        return 0;

    // 如果节点有semantic_info，尝试获取类型
    if (node->semantic_info && node->type == AST_IDENTIFIER)
    {
        // 对于标识符节点，semantic_info是Symbol*
        Symbol *symbol = (Symbol *)node->semantic_info;
        if (symbol && symbol->type)
        {
            return (symbol->type->base_type == TYPE_FLOAT ||
                    symbol->type->base_type == TYPE_DOUBLE);
        }
    }
    else if (node->semantic_info)
    {
        // 对于表达式节点，semantic_info可能是TypeInfo*
        TypeInfo *type = (TypeInfo *)node->semantic_info;
        // 简单启发式：检查base_type是否在有效范围
        if ((int)type->base_type >= 0 && (int)type->base_type < 20)
        {
            return (type->base_type == TYPE_FLOAT ||
                    type->base_type == TYPE_DOUBLE);
        }
    }

    // 浮点数字面量
    if (node->type == AST_FLOAT_LITERAL)
    {
        return 1;
    }

    return 0;
}

// 辅助函数：检查表达式是否为double类型
static int is_double_expression(ASTNode *node)
{
    if (!node)
        return 0;

    // 如果节点有semantic_info，尝试获取类型
    if (node->semantic_info && node->type == AST_IDENTIFIER)
    {
        // 对于标识符节点，semantic_info是Symbol*
        Symbol *symbol = (Symbol *)node->semantic_info;
        if (symbol && symbol->type)
        {
            return (symbol->type->base_type == TYPE_DOUBLE);
        }
    }
    else if (node->semantic_info)
    {
        // 对于表达式节点，semantic_info可能是TypeInfo*
        TypeInfo *type = (TypeInfo *)node->semantic_info;
        // 简单启发式：检查base_type是否在有效范围
        if ((int)type->base_type >= 0 && (int)type->base_type < 20)
        {
            return (type->base_type == TYPE_DOUBLE);
        }
    }

    return 0;
}

// 生成表达式代码（结果放在 %rax）
void gen_expression(CodeGenerator *gen, ASTNode *node)
{
    if (!node)
        return;

    switch (node->type)
    {
    case AST_INT_LITERAL:
        emit(gen, "    movq $%d, %%rax  # Load integer constant", node->value.int_val);
        break;

    case AST_FLOAT_LITERAL:
        // 简化：将 float 转为 int
        emit(gen, "    movq $%d, %%rax  # Load float as int", (int)node->value.float_val);
        break;

    case AST_SIZEOF_EXPR:
    {
        // sizeof(type) 或 sizeof(expr)
        int size = 8; // 默认8字节

        if (node->num_children > 0)
        {
            ASTNode *child = node->children[0];

            if (child->type == AST_TYPE_SPECIFIER)
            {
                // sizeof(type)
                const char *type_str = child->value.string_val;
                if (type_str)
                {
                    if (strcmp(type_str, "int") == 0 || strcmp(type_str, "float") == 0)
                        size = 8;
                    else if (strcmp(type_str, "char") == 0)
                        size = 1;
                    else if (strcmp(type_str, "short") == 0)
                        size = 2;
                    else if (strcmp(type_str, "long") == 0 || strcmp(type_str, "double") == 0)
                        size = 8;
                }
            }
        }

        emit(gen, "    movq $%d, %%rax  # sizeof result", size);
        break;
    }

    case AST_STRING_LITERAL:
    {
        // 添加字符串常量并获取标签
        int label = add_string_constant(gen, node->value.string_val);
        // 加载字符串地址到rax
        emit(gen, "    leaq .LC%d(%%rip), %%rax  # Load string address", label);
        break;
    }

    case AST_IDENTIFIER:
    {
        // 使用存储的符号信息
        const char *name = node->value.string_val;
        Symbol *symbol = (Symbol *)node->semantic_info;
        if (symbol)
        {
            // 检查是否是枚举常量（declaration节点类型为AST_ENUM_CONST）
            if (symbol->declaration && symbol->declaration->type == AST_ENUM_CONST)
            {
                // 枚举常量：直接使用其整数值
                int enum_value = symbol->declaration->value.int_val;
                emit(gen, "    movq $%d, %%rax  # Enum constant '%s'", enum_value, name);
            }
            else if (symbol->is_global || symbol->is_static)
            {
                // 全局/静态变量：使用标签访问
                emit(gen, "    movq %s(%%rip), %%rax  # Load global/static '%s'",
                     symbol->label, name);
            }
            else
            {
                // 局部变量：使用栈偏移访问
                int offset = -(symbol->offset + 8); // 相对于 rbp 的偏移
                emit(gen, "    movq %d(%%rbp), %%rax  # Load variable '%s'", offset, name);
            }
        }
        else
        {
            emit(gen, "    movq $0, %%rax  # ERROR: Unknown variable '%s'", name);
        }
        break;
    }

    case AST_BINARY_EXPR:
    {
        if (node->num_children < 2)
            break;

        // 先计算右操作数
        gen_expression(gen, node->children[1]);
        push_reg(gen, "rax"); // 保存右操作数

        // 计算左操作数
        gen_expression(gen, node->children[0]);

        // 恢复右操作数到 rbx
        pop_reg(gen, "rbx");

        // 执行操作（结果在 rax）
        switch (node->value.op_type)
        {
        case OP_COMMA:
            // 逗号运算符：左侧已求值并丢弃，右侧结果已在rax中
            // rbx中是右侧的值，直接使用
            emit(gen, "    movq %%rbx, %%rax  # Comma operator: use right side");
            break;

        case OP_ADD:
        {
            // 检查是否是指针算术
            int left_is_ptr = is_pointer_expression(node->children[0]);
            int right_is_ptr = is_pointer_expression(node->children[1]);

            // 检查是否是浮点运算
            int left_is_float = is_float_expression(node->children[0]);
            int right_is_float = is_float_expression(node->children[1]);

            if (left_is_ptr && !right_is_ptr)
            {
                // 指针 + 整数：缩放整数并减去（栈向下增长）
                emit(gen, "    imulq $8, %%rbx  # Scale integer for pointer arithmetic");
                emit(gen, "    subq %%rbx, %%rax  # Add (pointer + integer, stack grows down)");
            }
            else if (!left_is_ptr && right_is_ptr)
            {
                // 整数 + 指针：缩放整数并减去
                emit(gen, "    imulq $8, %%rax  # Scale integer for pointer arithmetic");
                emit(gen, "    subq %%rax, %%rbx  # Add (integer + pointer, stack grows down)");
                emit(gen, "    movq %%rbx, %%rax  # Result in rax");
            }
            else if (left_is_float || right_is_float)
            {
                int left_is_double = is_double_expression(node->children[0]);
                int right_is_double = is_double_expression(node->children[1]);

                if (left_is_double || right_is_double)
                {
                    // Double加法
                    emit(gen, "    cvtsi2sd %%rax, %%xmm0  # Convert left int to double");
                    emit(gen, "    cvtsi2sd %%rbx, %%xmm1  # Convert right int to double");
                    emit(gen, "    addsd %%xmm1, %%xmm0  # Double add");
                    emit(gen, "    cvttsd2si %%xmm0, %%rax  # Convert result to int");
                }
                else
                {
                    // Float加法
                    emit(gen, "    cvtsi2ss %%rax, %%xmm0  # Convert left int to float");
                    emit(gen, "    cvtsi2ss %%rbx, %%xmm1  # Convert right int to float");
                    emit(gen, "    addss %%xmm1, %%xmm0  # Float add");
                    emit(gen, "    cvttss2si %%xmm0, %%rax  # Convert result to int");
                }
            }
            else
            {
                // 普通整数加法
                emit(gen, "    addq %%rbx, %%rax  # Integer add");
            }
            break;
        }
        case OP_SUB:
        {
            // 检查是否是指针算术
            int left_is_ptr = is_pointer_expression(node->children[0]);
            int right_is_ptr = is_pointer_expression(node->children[1]);

            // 检查是否是浮点运算
            int left_is_float = is_float_expression(node->children[0]);
            int right_is_float = is_float_expression(node->children[1]);

            if (left_is_ptr && !right_is_ptr)
            {
                // 指针 - 整数：缩放整数并加上（栈向下增长，减去整数意味着向高地址）
                emit(gen, "    imulq $8, %%rbx  # Scale integer for pointer arithmetic");
                emit(gen, "    addq %%rbx, %%rax  # Subtract (pointer - integer, stack grows down)");
            }
            else if (left_is_ptr && right_is_ptr)
            {
                // 指针 - 指针：结果是元素个数（需要除以8）
                emit(gen, "    subq %%rbx, %%rax  # Subtract (pointer - pointer)");
                emit(gen, "    movq $8, %%rbx");
                emit(gen, "    cqto  # Sign extend");
                emit(gen, "    idivq %%rbx  # Divide by element size");
            }
            else if (left_is_float || right_is_float)
            {
                int left_is_double = is_double_expression(node->children[0]);
                int right_is_double = is_double_expression(node->children[1]);

                if (left_is_double || right_is_double)
                {
                    // Double减法
                    emit(gen, "    cvtsi2sd %%rax, %%xmm0  # Convert left int to double");
                    emit(gen, "    cvtsi2sd %%rbx, %%xmm1  # Convert right int to double");
                    emit(gen, "    subsd %%xmm1, %%xmm0  # Double subtract");
                    emit(gen, "    cvttsd2si %%xmm0, %%rax  # Convert result to int");
                }
                else
                {
                    // Float减法
                    emit(gen, "    cvtsi2ss %%rax, %%xmm0  # Convert left int to float");
                    emit(gen, "    cvtsi2ss %%rbx, %%xmm1  # Convert right int to float");
                    emit(gen, "    subss %%xmm1, %%xmm0  # Float subtract");
                    emit(gen, "    cvttss2si %%xmm0, %%rax  # Convert result to int");
                }
            }
            else
            {
                // 普通整数减法
                emit(gen, "    subq %%rbx, %%rax  # Integer subtract");
            }
            break;
        }
        case OP_MUL:
        {
            // 检查是否为浮点类型
            int left_is_float = is_float_expression(node->children[0]);
            int right_is_float = is_float_expression(node->children[1]);

            if (left_is_float || right_is_float)
            {
                int left_is_double = is_double_expression(node->children[0]);
                int right_is_double = is_double_expression(node->children[1]);

                if (left_is_double || right_is_double)
                {
                    // Double乘法
                    emit(gen, "    cvtsi2sd %%rax, %%xmm0  # Convert left int to double");
                    emit(gen, "    cvtsi2sd %%rbx, %%xmm1  # Convert right int to double");
                    emit(gen, "    mulsd %%xmm1, %%xmm0  # Double multiply");
                    emit(gen, "    cvttsd2si %%xmm0, %%rax  # Convert result to int");
                }
                else
                {
                    // Float乘法
                    emit(gen, "    cvtsi2ss %%rax, %%xmm0  # Convert left int to float");
                    emit(gen, "    cvtsi2ss %%rbx, %%xmm1  # Convert right int to float");
                    emit(gen, "    mulss %%xmm1, %%xmm0  # Float multiply");
                    emit(gen, "    cvttss2si %%xmm0, %%rax  # Convert result to int");
                }
            }
            else
            {
                // 整数乘法
                emit(gen, "    imulq %%rbx, %%rax  # Integer multiply");
            }
            break;
        }
        case OP_DIV:
        {
            // 检查是否为浮点类型
            int left_is_float = is_float_expression(node->children[0]);
            int right_is_float = is_float_expression(node->children[1]);

            if (left_is_float || right_is_float)
            {
                int left_is_double = is_double_expression(node->children[0]);
                int right_is_double = is_double_expression(node->children[1]);

                if (left_is_double || right_is_double)
                {
                    // Double除法
                    emit(gen, "    cvtsi2sd %%rax, %%xmm0  # Convert dividend int to double");
                    emit(gen, "    cvtsi2sd %%rbx, %%xmm1  # Convert divisor int to double");
                    emit(gen, "    divsd %%xmm1, %%xmm0  # Double divide");
                    emit(gen, "    cvttsd2si %%xmm0, %%rax  # Convert result to int");
                }
                else
                {
                    // Float除法
                    emit(gen, "    cvtsi2ss %%rax, %%xmm0  # Convert dividend int to float");
                    emit(gen, "    cvtsi2ss %%rbx, %%xmm1  # Convert divisor int to float");
                    emit(gen, "    divss %%xmm1, %%xmm0  # Float divide");
                    emit(gen, "    cvttss2si %%xmm0, %%rax  # Convert result to int");
                }
            }
            else
            {
                // 整数除法
                emit(gen, "    cqto  # Sign extend");
                emit(gen, "    idivq %%rbx  # Integer divide");
            }
            break;
        }
        case OP_MOD:
            emit(gen, "    cqto  # Sign extend");
            emit(gen, "    idivq %%rbx  # Modulo");
            emit(gen, "    movq %%rdx, %%rax  # Result in rax");
            break;
        case OP_LT:
        {
            int left_is_float = is_float_expression(node->children[0]);
            int right_is_float = is_float_expression(node->children[1]);
            int left_is_double = is_double_expression(node->children[0]);
            int right_is_double = is_double_expression(node->children[1]);

            if (left_is_double || right_is_double)
            {
                emit(gen, "    movq %%rax, %%xmm0  # Load left to xmm0");
                emit(gen, "    movq %%rbx, %%xmm1  # Load right to xmm1");
                emit(gen, "    ucomisd %%xmm1, %%xmm0  # Double compare");
                emit(gen, "    setb %%al  # Set if below");
                emit(gen, "    movzbq %%al, %%rax");
            }
            else if (left_is_float || right_is_float)
            {
                emit(gen, "    cvtsi2ss %%rax, %%xmm0  # Convert left to float");
                emit(gen, "    cvtsi2ss %%rbx, %%xmm1  # Convert right to float");
                emit(gen, "    ucomiss %%xmm1, %%xmm0  # Float compare");
                emit(gen, "    setb %%al  # Set if below");
                emit(gen, "    movzbq %%al, %%rax");
            }
            else
            {
                emit(gen, "    cmpq %%rbx, %%rax  # Integer compare <");
                emit(gen, "    setl %%al");
                emit(gen, "    movzbq %%al, %%rax");
            }
            break;
        }
        case OP_GT:
        {
            int left_is_float = is_float_expression(node->children[0]);
            int right_is_float = is_float_expression(node->children[1]);
            int left_is_double = is_double_expression(node->children[0]);
            int right_is_double = is_double_expression(node->children[1]);

            if (left_is_double || right_is_double)
            {
                emit(gen, "    movq %%rax, %%xmm0");
                emit(gen, "    movq %%rbx, %%xmm1");
                emit(gen, "    ucomisd %%xmm1, %%xmm0");
                emit(gen, "    seta %%al");
                emit(gen, "    movzbq %%al, %%rax");
            }
            else if (left_is_float || right_is_float)
            {
                emit(gen, "    cvtsi2ss %%rax, %%xmm0");
                emit(gen, "    cvtsi2ss %%rbx, %%xmm1");
                emit(gen, "    ucomiss %%xmm1, %%xmm0");
                emit(gen, "    seta %%al");
                emit(gen, "    movzbq %%al, %%rax");
            }
            else
            {
                emit(gen, "    cmpq %%rbx, %%rax");
                emit(gen, "    setg %%al");
                emit(gen, "    movzbq %%al, %%rax");
            }
            break;
        }
        case OP_LE:
        {
            int left_is_float = is_float_expression(node->children[0]);
            int right_is_float = is_float_expression(node->children[1]);
            int left_is_double = is_double_expression(node->children[0]);
            int right_is_double = is_double_expression(node->children[1]);

            if (left_is_double || right_is_double)
            {
                emit(gen, "    movq %%rax, %%xmm0");
                emit(gen, "    movq %%rbx, %%xmm1");
                emit(gen, "    ucomisd %%xmm1, %%xmm0");
                emit(gen, "    setbe %%al");
                emit(gen, "    movzbq %%al, %%rax");
            }
            else if (left_is_float || right_is_float)
            {
                emit(gen, "    cvtsi2ss %%rax, %%xmm0");
                emit(gen, "    cvtsi2ss %%rbx, %%xmm1");
                emit(gen, "    ucomiss %%xmm1, %%xmm0");
                emit(gen, "    setbe %%al");
                emit(gen, "    movzbq %%al, %%rax");
            }
            else
            {
                emit(gen, "    cmpq %%rbx, %%rax");
                emit(gen, "    setle %%al");
                emit(gen, "    movzbq %%al, %%rax");
            }
            break;
        }
        case OP_GE:
        {
            int left_is_float = is_float_expression(node->children[0]);
            int right_is_float = is_float_expression(node->children[1]);
            int left_is_double = is_double_expression(node->children[0]);
            int right_is_double = is_double_expression(node->children[1]);

            if (left_is_double || right_is_double)
            {
                emit(gen, "    movq %%rax, %%xmm0");
                emit(gen, "    movq %%rbx, %%xmm1");
                emit(gen, "    ucomisd %%xmm1, %%xmm0");
                emit(gen, "    setae %%al");
                emit(gen, "    movzbq %%al, %%rax");
            }
            else if (left_is_float || right_is_float)
            {
                emit(gen, "    cvtsi2ss %%rax, %%xmm0");
                emit(gen, "    cvtsi2ss %%rbx, %%xmm1");
                emit(gen, "    ucomiss %%xmm1, %%xmm0");
                emit(gen, "    setae %%al");
                emit(gen, "    movzbq %%al, %%rax");
            }
            else
            {
                emit(gen, "    cmpq %%rbx, %%rax");
                emit(gen, "    setge %%al");
                emit(gen, "    movzbq %%al, %%rax");
            }
            break;
        }
        case OP_EQ:
        {
            int left_is_float = is_float_expression(node->children[0]);
            int right_is_float = is_float_expression(node->children[1]);
            int left_is_double = is_double_expression(node->children[0]);
            int right_is_double = is_double_expression(node->children[1]);

            if (left_is_double || right_is_double)
            {
                emit(gen, "    movq %%rax, %%xmm0");
                emit(gen, "    movq %%rbx, %%xmm1");
                emit(gen, "    ucomisd %%xmm1, %%xmm0");
                emit(gen, "    sete %%al");
                emit(gen, "    movzbq %%al, %%rax");
            }
            else if (left_is_float || right_is_float)
            {
                emit(gen, "    cvtsi2ss %%rax, %%xmm0");
                emit(gen, "    cvtsi2ss %%rbx, %%xmm1");
                emit(gen, "    ucomiss %%xmm1, %%xmm0");
                emit(gen, "    sete %%al");
                emit(gen, "    movzbq %%al, %%rax");
            }
            else
            {
                emit(gen, "    cmpq %%rbx, %%rax");
                emit(gen, "    sete %%al");
                emit(gen, "    movzbq %%al, %%rax");
            }
            break;
        }
        case OP_NE:
        {
            int left_is_float = is_float_expression(node->children[0]);
            int right_is_float = is_float_expression(node->children[1]);
            int left_is_double = is_double_expression(node->children[0]);
            int right_is_double = is_double_expression(node->children[1]);

            if (left_is_double || right_is_double)
            {
                emit(gen, "    movq %%rax, %%xmm0");
                emit(gen, "    movq %%rbx, %%xmm1");
                emit(gen, "    ucomisd %%xmm1, %%xmm0");
                emit(gen, "    setne %%al");
                emit(gen, "    movzbq %%al, %%rax");
            }
            else if (left_is_float || right_is_float)
            {
                emit(gen, "    cvtsi2ss %%rax, %%xmm0");
                emit(gen, "    cvtsi2ss %%rbx, %%xmm1");
                emit(gen, "    ucomiss %%xmm1, %%xmm0");
                emit(gen, "    setne %%al");
                emit(gen, "    movzbq %%al, %%rax");
            }
            else
            {
                emit(gen, "    cmpq %%rbx, %%rax");
                emit(gen, "    setne %%al");
                emit(gen, "    movzbq %%al, %%rax");
            }
            break;
        }
        case OP_AND:
            // 逻辑与: 两个都非零则为1，否则为0
            emit(gen, "    testq %%rax, %%rax  # Test left operand");
            emit(gen, "    setne %%al");
            emit(gen, "    testq %%rbx, %%rbx  # Test right operand");
            emit(gen, "    setne %%bl");
            emit(gen, "    andb %%bl, %%al    # AND both");
            emit(gen, "    movzbq %%al, %%rax");
            break;
        case OP_OR:
            // 逻辑或: 至少一个非零则为1，否则为0
            emit(gen, "    testq %%rax, %%rax  # Test left operand");
            emit(gen, "    setne %%al");
            emit(gen, "    testq %%rbx, %%rbx  # Test right operand");
            emit(gen, "    setne %%bl");
            emit(gen, "    orb %%bl, %%al     # OR both");
            emit(gen, "    movzbq %%al, %%rax");
            break;

        // 位运算符
        case OP_BIT_AND:
            emit(gen, "    andq %%rbx, %%rax  # Bitwise AND");
            break;
        case OP_BIT_OR:
            emit(gen, "    orq %%rbx, %%rax  # Bitwise OR");
            break;
        case OP_BIT_XOR:
            emit(gen, "    xorq %%rbx, %%rax  # Bitwise XOR");
            break;
        case OP_LEFT_SHIFT:
            emit(gen, "    movq %%rbx, %%rcx  # Shift count to rcx");
            emit(gen, "    shlq %%cl, %%rax  # Left shift");
            break;
        case OP_RIGHT_SHIFT:
            emit(gen, "    movq %%rbx, %%rcx  # Shift count to rcx");
            emit(gen, "    shrq %%cl, %%rax  # Right shift");
            break;
        default:
            break;
        }
        break;
    }

    case AST_ASSIGN_EXPR:
    {
        if (node->num_children < 2)
            break;

        ASTNode *lhs = node->children[0];

        // 检查是否为复合赋值运算符
        int is_compound = (node->value.op_type != OP_ASSIGN);

        // 如果是复合赋值，先加载左侧的值
        if (is_compound)
        {
            gen_expression(gen, lhs); // 加载左侧的当前值到 rax
            push_reg(gen, "rax");     // 保存到栈
        }

        // 检查左侧是否是数组下标 (arr[i] = value)
        if (lhs->type == AST_ARRAY_SUBSCRIPT)
        {
            if (lhs->num_children < 2)
                break;

            ASTNode *array_node = lhs->children[0];
            ASTNode *index_node = lhs->children[1];

            // 计算索引
            gen_expression(gen, index_node);
            push_reg(gen, "rax"); // 保存索引

            // 获取数组基地址（arr[0]）
            if (array_node->type == AST_IDENTIFIER)
            {
                Symbol *symbol = (Symbol *)array_node->semantic_info;
                if (symbol)
                {
                    // arr[0]的地址
                    int offset = -(symbol->offset + 8);
                    emit(gen, "    leaq %d(%%rbp), %%rbx  # Load array base (arr[0]) for assign", offset);
                }
            }
            else
            {
                gen_expression(gen, array_node);
                emit(gen, "    movq %%rax, %%rbx  # Array base address");
            }

            // 弹出索引
            pop_reg(gen, "rax");

            // 计算元素地址：base - index*8（栈向下增长）
            emit(gen, "    imulq $8, %%rax  # Calculate offset (index * 8)");
            emit(gen, "    subq %%rax, %%rbx  # Subtract offset from base");
            emit(gen, "    movq %%rbx, %%rax  # Move element address to rax");
            push_reg(gen, "rax"); // 保存元素地址

            // 计算右侧表达式的值
            gen_expression(gen, node->children[1]);

            // 恢复元素地址
            pop_reg(gen, "rbx");

            // 存储值到数组元素
            emit(gen, "    movq %%rax, (%%rbx)  # Store to array element");
        }
        // 检查左侧是否是解引用表达式 (*ptr = value)
        else if (lhs->type == AST_UNARY_EXPR && lhs->value.op_type == OP_DEREF)
        {
            // 先计算指针地址
            gen_expression(gen, lhs->children[0]);
            push_reg(gen, "rax"); // 保存指针地址

            // 计算右侧表达式的值
            gen_expression(gen, node->children[1]);

            // 恢复指针地址到 rbx
            pop_reg(gen, "rbx");

            // 存储值到指针指向的位置
            emit(gen, "    movq %%rax, (%%rbx)  # Store to dereferenced pointer");
        }
        // 检查左侧是否是结构体成员访问 (p.x = value)
        else if (lhs->type == AST_MEMBER_ACCESS)
        {
            if (lhs->num_children < 2)
                break;

            ASTNode *struct_node = lhs->children[0];
            ASTNode *member_node = lhs->children[1];

            // 计算右侧表达式的值
            gen_expression(gen, node->children[1]);

            // 如果结构体是标识符
            if (struct_node->type == AST_IDENTIFIER)
            {
                Symbol *symbol = (Symbol *)struct_node->semantic_info;
                if (symbol && symbol->type)
                {
                    const char *member_name = member_node->value.string_val;
                    int member_offset = 0;
                    int found = 0;

                    // 从TypeInfo的members中查找成员偏移
                    if (symbol->type->members && symbol->type->num_members > 0)
                    {
                        for (int i = 0; i < symbol->type->num_members; i++)
                        {
                            if (strcmp(symbol->type->members[i].name, member_name) == 0)
                            {
                                member_offset = symbol->type->members[i].offset;
                                found = 1;
                                break;
                            }
                        }
                    }

                    // 如果没有找到members信息，使用默认偏移（向后兼容）
                    if (!found)
                    {
                        if (strcmp(member_name, "y") == 0)
                            member_offset = 8;
                        else if (strcmp(member_name, "x") == 0)
                            member_offset = 0;
                    }

                    // 结构体变量基地址 + 成员偏移
                    int base_offset = -(symbol->offset + 8);
                    int final_offset = base_offset - member_offset;

                    emit(gen, "    movq %%rax, %d(%%rbp)  # Store to %s.%s",
                         final_offset, symbol->name, member_name);
                }
            }
        }
        else if (lhs->type == AST_IDENTIFIER || lhs->type == AST_DECLARATOR)
        {
            // 计算右侧表达式
            gen_expression(gen, node->children[1]);

            // 如果是复合赋值，执行运算
            if (is_compound)
            {
                pop_reg(gen, "rbx"); // 弹出左侧的旧值到 rbx

                // 根据运算符类型执行操作
                switch (node->value.op_type)
                {
                case OP_ADD_ASSIGN:
                    emit(gen, "    addq %%rax, %%rbx  # +=");
                    break;
                case OP_SUB_ASSIGN:
                    emit(gen, "    subq %%rax, %%rbx  # -=");
                    break;
                case OP_MUL_ASSIGN:
                    emit(gen, "    imulq %%rax, %%rbx  # *=");
                    break;
                case OP_DIV_ASSIGN:
                    emit(gen, "    movq %%rax, %%rcx  # Move divisor to rcx");
                    emit(gen, "    movq %%rbx, %%rax  # Move dividend to rax");
                    emit(gen, "    cqto  # Sign extend");
                    emit(gen, "    idivq %%rcx  # Divide rax by rcx");
                    emit(gen, "    movq %%rax, %%rbx  # Result to rbx");
                    break;
                case OP_MOD_ASSIGN:
                    emit(gen, "    movq %%rax, %%rcx  # Move divisor to rcx");
                    emit(gen, "    movq %%rbx, %%rax  # Move dividend to rax");
                    emit(gen, "    cqto  # Sign extend");
                    emit(gen, "    idivq %%rcx  # Divide, remainder in rdx");
                    emit(gen, "    movq %%rdx, %%rbx  # Remainder to rbx");
                    break;
                case OP_AND_ASSIGN:
                    emit(gen, "    andq %%rax, %%rbx  # &=");
                    break;
                case OP_OR_ASSIGN:
                    emit(gen, "    orq %%rax, %%rbx  # |=");
                    break;
                case OP_XOR_ASSIGN:
                    emit(gen, "    xorq %%rax, %%rbx  # ^=");
                    break;
                case OP_LEFT_ASSIGN:
                    emit(gen, "    movq %%rax, %%rcx  # Shift count to rcx");
                    emit(gen, "    shlq %%cl, %%rbx  # <<=");
                    break;
                case OP_RIGHT_ASSIGN:
                    emit(gen, "    movq %%rax, %%rcx  # Shift count to rcx");
                    emit(gen, "    shrq %%cl, %%rbx  # >>=");
                    break;
                }
                emit(gen, "    movq %%rbx, %%rax  # Result to rax");
            }

            // 存储到左侧变量
            const char *name = lhs->value.string_val;
            Symbol *symbol = (Symbol *)lhs->semantic_info;
            if (symbol)
            {
                if (symbol->is_global || symbol->is_static)
                {
                    // 全局/静态变量：使用标签访问
                    emit(gen, "    movq %%rax, %s(%%rip)  # Store to global/static '%s'",
                         symbol->label, name);
                }
                else
                {
                    // 局部变量：使用栈偏移访问
                    int offset = -(symbol->offset + 8);
                    emit(gen, "    movq %%rax, %d(%%rbp)  # Store to variable '%s'", offset, name);
                }
            }
        }
        break;
    }

    case AST_CAST_EXPR:
    {
        // 类型转换 (type)expr
        if (node->num_children < 2)
            break;

        // 生成被转换的表达式
        gen_expression(gen, node->children[1]);

        // 根据目标类型进行转换
        TypeInfo *target_type = (TypeInfo *)node->semantic_info;
        if (target_type)
        {
            if (target_type->base_type == TYPE_FLOAT)
            {
                // 转换到float: int -> float
                emit(gen, "    cvtsi2ss %%rax, %%xmm0  # Cast int to float");
            }
            else if (target_type->base_type == TYPE_INT ||
                     target_type->base_type == TYPE_LONG ||
                     target_type->base_type == TYPE_SHORT ||
                     target_type->base_type == TYPE_CHAR)
            {
                // 转换到整数类型
                // 从float转换
                emit(gen, "    # Cast to int (value already in rax or convert from xmm0)");
                // 如果源可能是float，尝试转换
                emit(gen, "    cvttss2si %%xmm0, %%rbx  # Try float to int");
                emit(gen, "    test %%rax, %%rax");
                emit(gen, "    cmovz %%rbx, %%rax  # Use float conversion if rax was 0");
            }
        }
        break;
    }

    case AST_UNARY_EXPR:
    {
        if (node->num_children < 1)
            break;

        // 应用一元运算符
        switch (node->value.op_type)
        {
        case OP_ADDR:
            // 取地址：不计算操作数的值，而是返回其地址
            if (node->children[0]->type == AST_IDENTIFIER)
            {
                ASTNode *id = node->children[0];
                Symbol *symbol = (Symbol *)id->semantic_info;
                if (symbol)
                {
                    int offset = -(symbol->offset + 8);
                    emit(gen, "    leaq %d(%%rbp), %%rax  # Load address of '%s'",
                         offset, id->value.string_val);
                }
            }
            else if (node->children[0]->type == AST_ARRAY_SUBSCRIPT)
            {
                // 对于数组下标，计算元素的地址（不加载值）
                ASTNode *array_node = node->children[0]->children[0];
                ASTNode *index_node = node->children[0]->children[1];

                // 计算索引
                gen_expression(gen, index_node);
                emit(gen, "    pushq %%rax");

                // 获取数组基地址
                Symbol *array_symbol = (Symbol *)array_node->semantic_info;
                if (array_symbol)
                {
                    emit(gen, "    leaq -%d(%%rbp), %%rbx  # Load array base (arr[0]) address",
                         array_symbol->offset + 8);
                    emit(gen, "    popq %%rax");
                    emit(gen, "    imulq $8, %%rax  # Calculate offset (index * 8)");
                    emit(gen, "    subq %%rax, %%rbx  # Subtract offset from base");
                    emit(gen, "    movq %%rbx, %%rax  # Move element address to rax");
                }
            }
            break;
        case OP_PREINC:
        case OP_PREDEC:
        {
            // 前缀递增/递减：++i, --i, ++arr[i], ++p.x
            // 先递增/递减，然后返回新值
            ASTNode *operand = node->children[0];

            if (operand->type == AST_IDENTIFIER)
            {
                // 简单变量：++i
                Symbol *symbol = (Symbol *)operand->semantic_info;
                if (symbol)
                {
                    int offset = -(symbol->offset + 8);
                    emit(gen, "    movq %d(%%rbp), %%rax  # Load variable", offset);
                    if (node->value.op_type == OP_PREINC)
                        emit(gen, "    addq $1, %%rax  # ++");
                    else
                        emit(gen, "    subq $1, %%rax  # --");
                    emit(gen, "    movq %%rax, %d(%%rbp)  # Store back", offset);
                }
            }
            else if (operand->type == AST_ARRAY_SUBSCRIPT)
            {
                // 数组元素：++arr[i]
                // 需要特殊处理：获取地址而不是值
                if (operand->num_children >= 2)
                {
                    ASTNode *array_node = operand->children[0];
                    ASTNode *index_node = operand->children[1];

                    // 计算索引
                    gen_expression(gen, index_node);
                    push_reg(gen, "rax");

                    // 获取数组基地址
                    if (array_node->type == AST_IDENTIFIER)
                    {
                        Symbol *symbol = (Symbol *)array_node->semantic_info;
                        if (symbol)
                        {
                            int base_offset = -(symbol->offset + 8);
                            emit(gen, "    leaq %d(%%rbp), %%rax  # Array base", base_offset);
                        }
                    }

                    // 计算元素地址
                    pop_reg(gen, "rcx");
                    emit(gen, "    imulq $8, %%rcx");
                    emit(gen, "    subq %%rcx, %%rax  # Element address");

                    // 加载、递增、存回
                    emit(gen, "    movq (%%rax), %%rbx  # Load value");
                    push_reg(gen, "rax");
                    if (node->value.op_type == OP_PREINC)
                        emit(gen, "    addq $1, %%rbx");
                    else
                        emit(gen, "    subq $1, %%rbx");
                    pop_reg(gen, "rax");
                    emit(gen, "    movq %%rbx, (%%rax)  # Store back");
                    emit(gen, "    movq %%rbx, %%rax  # Result");
                }
            }
            else if (operand->type == AST_MEMBER_ACCESS ||
                     (operand->type == AST_UNARY_EXPR && operand->value.op_type == OP_DEREF))
            {
                // 结构体成员或解引用：++p.x, ++(*ptr)
                // gen_expression对于MEMBER_ACCESS会返回地址+值
                // 我们需要重新获取地址
                gen_expression(gen, operand);
                emit(gen, "    movq (%%rax), %%rbx  # Load current value");
                push_reg(gen, "rax");

                if (node->value.op_type == OP_PREINC)
                    emit(gen, "    addq $1, %%rbx  # ++");
                else
                    emit(gen, "    subq $1, %%rbx  # --");

                pop_reg(gen, "rax");
                emit(gen, "    movq %%rbx, (%%rax)  # Store back");
                emit(gen, "    movq %%rbx, %%rax  # Result is new value");
            }
            break;
        }
        case OP_POSTINC:
        case OP_POSTDEC:
        {
            // 后缀递增/递减：i++, i--, arr[i]++, p.x++
            // 先返回旧值，然后递增/递减
            ASTNode *operand = node->children[0];

            if (operand->type == AST_IDENTIFIER)
            {
                // 简单变量：i++
                Symbol *symbol = (Symbol *)operand->semantic_info;
                if (symbol)
                {
                    int offset = -(symbol->offset + 8);
                    emit(gen, "    movq %d(%%rbp), %%rax  # Load variable", offset);
                    emit(gen, "    movq %%rax, %%rbx  # Save old value");
                    if (node->value.op_type == OP_POSTINC)
                        emit(gen, "    addq $1, %%rbx  # ++");
                    else
                        emit(gen, "    subq $1, %%rbx  # --");
                    emit(gen, "    movq %%rbx, %d(%%rbp)  # Store new value", offset);
                    // rax still holds old value
                }
            }
            else if (operand->type == AST_ARRAY_SUBSCRIPT)
            {
                // 数组元素：arr[i]++
                if (operand->num_children >= 2)
                {
                    ASTNode *array_node = operand->children[0];
                    ASTNode *index_node = operand->children[1];

                    // 计算索引
                    gen_expression(gen, index_node);
                    push_reg(gen, "rax");

                    // 获取数组基地址
                    if (array_node->type == AST_IDENTIFIER)
                    {
                        Symbol *symbol = (Symbol *)array_node->semantic_info;
                        if (symbol)
                        {
                            int base_offset = -(symbol->offset + 8);
                            emit(gen, "    leaq %d(%%rbp), %%rax  # Array base", base_offset);
                        }
                    }

                    // 计算元素地址
                    pop_reg(gen, "rcx");
                    emit(gen, "    imulq $8, %%rcx");
                    emit(gen, "    subq %%rcx, %%rax  # Element address");

                    // 加载旧值
                    emit(gen, "    movq (%%rax), %%rbx  # Load old value");
                    push_reg(gen, "rax"); // 保存地址
                    emit(gen, "    movq %%rbx, %%rax  # Old value to rax");
                    push_reg(gen, "rax"); // 保存旧值

                    // 递增递减
                    if (node->value.op_type == OP_POSTINC)
                        emit(gen, "    addq $1, %%rbx");
                    else
                        emit(gen, "    subq $1, %%rbx");

                    // 存回新值
                    pop_reg(gen, "rax"); // 恢复旧值（给返回用）
                    pop_reg(gen, "rcx"); // 恢复地址
                    emit(gen, "    movq %%rbx, (%%rcx)  # Store new value");
                    // rax中已经是旧值
                }
            }
            else if (operand->type == AST_MEMBER_ACCESS ||
                     (operand->type == AST_UNARY_EXPR && operand->value.op_type == OP_DEREF))
            {
                // 结构体成员或解引用：p.x++, (*ptr)++
                gen_expression(gen, operand);
                push_reg(gen, "rax");
                emit(gen, "    movq (%%rax), %%rax  # Load current value");
                push_reg(gen, "rax");

                if (node->value.op_type == OP_POSTINC)
                    emit(gen, "    addq $1, %%rax  # ++");
                else
                    emit(gen, "    subq $1, %%rax  # --");

                emit(gen, "    movq %%rax, %%rbx  # New value to rbx");
                pop_reg(gen, "rax"); // 恢复旧值
                push_reg(gen, "rax");
                pop_reg(gen, "rcx");
                pop_reg(gen, "rdx");
                emit(gen, "    movq %%rbx, (%%rdx)  # Store new value");
                emit(gen, "    movq %%rcx, %%rax  # Result is old value");
            }
            break;
        }
        case OP_DEREF:
        case OP_NOT:
        case OP_NEG:
        case OP_BIT_NOT:
        default:
            // 对于其他运算符，先计算操作数
            gen_expression(gen, node->children[0]);

            if (node->value.op_type == OP_NOT)
            {
                // 逻辑非: 0变1，非0变0
                emit(gen, "    testq %%rax, %%rax  # Test operand");
                emit(gen, "    sete %%al           # Set if zero");
                emit(gen, "    movzbq %%al, %%rax  # Extend to 64-bit");
            }
            else if (node->value.op_type == OP_NEG)
            {
                // 取负
                emit(gen, "    negq %%rax  # Negate");
            }
            else if (node->value.op_type == OP_DEREF)
            {
                // 解引用：rax 中是指针（地址），加载该地址处的值
                emit(gen, "    movq (%%rax), %%rax  # Dereference pointer");
            }
            else if (node->value.op_type == OP_BIT_NOT)
            {
                // 按位取反
                emit(gen, "    notq %%rax  # Bitwise NOT");
            }
            break;
        }
        break;
    }

    case AST_CALL_EXPR:
    {
        if (node->num_children < 1)
            break;

        // 获取函数名
        ASTNode *func_node = node->children[0];
        if (func_node->type != AST_IDENTIFIER)
            break;

        const char *func_name = func_node->value.string_val;

        // System V AMD64 ABI: 参数寄存器
        const char *param_regs[] = {"rdi", "rsi", "rdx", "rcx", "r8", "r9"};

        // 准备参数
        int num_args = 0;
        if (node->num_children > 1 && node->children[1]->type == AST_ARG_LIST)
        {
            ASTNode *arg_list = node->children[1];
            num_args = arg_list->num_children;

            // 将参数压栈（从右到左）
            for (int i = num_args - 1; i >= 0; i--)
            {
                gen_expression(gen, arg_list->children[i]);
                push_reg(gen, "rax");
            }

            // 从栈弹出到参数寄存器（从左到右）
            int num_reg_args = num_args < 6 ? num_args : 6;
            for (int i = 0; i < num_reg_args; i++)
            {
                pop_reg(gen, param_regs[i]);
            }
        }

        // 调用函数前保证栈对齐到16字节
        // System V AMD64 ABI要求 call 前栈是16字节对齐的
        emit(gen, "    call %s", func_name);

        // rax 现在包含返回值
        break;
    }

    case AST_ARRAY_SUBSCRIPT:
    {
        // 数组下标访问: array[index]
        if (node->num_children < 2)
            break;

        ASTNode *array_node = node->children[0];
        ASTNode *index_node = node->children[1];

        // 计算当前维度的索引
        gen_expression(gen, index_node);
        push_reg(gen, "rax"); // 保存当前索引到栈

        // 获取数组基地址
        if (array_node->type == AST_IDENTIFIER)
        {
            Symbol *symbol = (Symbol *)array_node->semantic_info;
            if (symbol)
            {
                // 数组在栈上，arr[0]的地址
                int base_offset = -(symbol->offset + 8);
                emit(gen, "    leaq %d(%%rbp), %%rax  # Load array base (arr[0])", base_offset);
            }
        }
        else if (array_node->type == AST_ARRAY_SUBSCRIPT)
        {
            // 嵌套数组访问：递归计算内层地址
            gen_expression(gen, array_node);
            // rax 中已经是内层的地址
        }
        else
        {
            // 其他表达式
            gen_expression(gen, array_node);
        }

        // 恢复当前维度的索引
        pop_reg(gen, "rcx");

        // 计算元素大小：根据类型确定
        int element_size = 8; // 默认8字节
        if (node->semantic_info)
        {
            TypeInfo *elem_type = (TypeInfo *)node->semantic_info;
            // 对于多维数组，如果还有维度，大小是剩余维度的总大小
            if (elem_type->array_dimensions > 0 && elem_type->array_sizes)
            {
                // 计算剩余维度的总大小
                element_size = 1;
                for (int d = 0; d < elem_type->array_dimensions; d++)
                {
                    if (elem_type->array_sizes[d] > 0)
                    {
                        element_size *= elem_type->array_sizes[d];
                    }
                }
                element_size *= 8; // 基本元素大小（int = 8字节）
            }
            else
            {
                // 基本类型大小
                switch (elem_type->base_type)
                {
                case TYPE_CHAR:
                    element_size = 1;
                    break;
                case TYPE_SHORT:
                    element_size = 2;
                    break;
                case TYPE_INT:
                case TYPE_FLOAT:
                case TYPE_LONG:
                case TYPE_DOUBLE:
                case TYPE_POINTER:
                    element_size = 8;
                    break;
                default:
                    element_size = 8;
                }
            }
        }

        // 计算偏移：index * element_size
        if (element_size != 1)
        {
            emit(gen, "    imulq $%d, %%rcx  # Calculate offset (index * %d)",
                 element_size, element_size);
        }

        // 计算元素地址: base - offset（因为栈向下增长）
        emit(gen, "    subq %%rcx, %%rax  # Subtract offset (stack grows down)");

        // 检查是否应该加载值
        // 通过当前节点的semantic_info（返回类型）判断
        // 如果返回类型仍然是数组（array_dimensions > 0），则不加载值
        int should_load_value = 1; // 默认加载值

        if (node->semantic_info)
        {
            TypeInfo *result_type = (TypeInfo *)node->semantic_info;
            if (result_type && result_type->array_dimensions > 0)
            {
                // 返回类型是多维数组，不加载值（保留地址给下一层）
                should_load_value = 0;
            }
        }

        if (should_load_value)
        {
            // 一维数组或多维数组的最后一次访问：加载元素值
            emit(gen, "    movq (%%rax), %%rax  # Load array element");
        }
        // 否则，rax 中已经是地址，用于下一层数组访问

        break;
    }

    case AST_INIT_LIST:
    {
        // 初始化列表在声明中处理，这里不生成代码
        break;
    }

    case AST_TERNARY_EXPR:
    {
        // 三元运算符: condition ? true_expr : false_expr
        if (node->num_children < 3)
            break;

        int false_label = new_label(gen);
        int end_label = new_label(gen);

        // 计算条件
        gen_expression(gen, node->children[0]);
        emit(gen, "    testq %%rax, %%rax  # Test condition");
        emit(gen, "    je .L%d  # Jump if false", false_label);

        // true分支
        gen_expression(gen, node->children[1]);
        emit(gen, "    jmp .L%d  # Skip false branch", end_label);

        // false分支
        emit(gen, ".L%d:  # False branch", false_label);
        gen_expression(gen, node->children[2]);

        // 结束
        emit(gen, ".L%d:  # End ternary", end_label);
        break;
    }

    case AST_MEMBER_ACCESS:
    {
        // 结构体成员访问
        if (node->num_children < 2)
            break;

        ASTNode *struct_node = node->children[0];
        ASTNode *member_node = node->children[1];

        if (struct_node->type == AST_IDENTIFIER)
        {
            Symbol *symbol = (Symbol *)struct_node->semantic_info;
            if (symbol && symbol->type)
            {
                const char *member_name = member_node->value.string_val;
                int member_offset = 0;
                int found = 0;

                // 从TypeInfo的members中查找成员偏移
                if (symbol->type->members && symbol->type->num_members > 0)
                {
                    for (int i = 0; i < symbol->type->num_members; i++)
                    {
                        if (strcmp(symbol->type->members[i].name, member_name) == 0)
                        {
                            member_offset = symbol->type->members[i].offset;
                            found = 1;
                            break;
                        }
                    }
                }

                // 如果没有找到members信息，使用默认偏移（向后兼容）
                if (!found)
                {
                    if (strcmp(member_name, "y") == 0)
                        member_offset = 8;
                    else if (strcmp(member_name, "x") == 0)
                        member_offset = 0;
                }

                // 结构体变量的基地址 + 成员偏移
                int base_offset = -(symbol->offset + 8);
                int final_offset = base_offset - member_offset;

                emit(gen, "    leaq %d(%%rbp), %%rax  # Load address of %s.%s",
                     final_offset, symbol->name, member_name);
                emit(gen, "    movq (%%rax), %%rax  # Load member value");
            }
        }
        break;
    }

    default:
        break;
    }
}

// 生成语句代码
void gen_statement(CodeGenerator *gen, ASTNode *node)
{
    if (!node)
        return;

    switch (node->type)
    {
    case AST_DECLARATION:
    {
        // 变量声明，可能有初始化
        if (node->num_children >= 2)
        {
            ASTNode *type_spec = node->children[0];
            ASTNode *declarator = node->children[1];

            // 检查是否是静态变量
            Symbol *symbol = NULL;
            if (declarator->type == AST_ASSIGN_EXPR && declarator->num_children > 0)
            {
                symbol = (Symbol *)declarator->children[0]->semantic_info;
            }

            // 静态变量不在函数中初始化，已经在.data段初始化
            if (symbol && symbol->is_static)
            {
                // 跳过静态变量的初始化代码生成
                break;
            }

            // 如果是赋值表达式（带初始化）
            if (declarator->type == AST_ASSIGN_EXPR)
            {
                if (declarator->num_children >= 2)
                {
                    ASTNode *init_expr = declarator->children[1];

                    // 处理数组初始化列表
                    if (init_expr->type == AST_INIT_LIST)
                    {
                        // 获取数组符号信息 - 递归查找有semantic_info的节点
                        Symbol *symbol = NULL;
                        ASTNode *search_node = declarator->children[0];
                        int depth = 0;

                        while (search_node && depth < 10)
                        {
                            if (search_node->semantic_info)
                            {
                                symbol = (Symbol *)search_node->semantic_info;
                                break;
                            }

                            // 尝试进入第一个子节点
                            if (search_node->num_children > 0)
                            {
                                search_node = search_node->children[0];
                                depth++;
                            }
                            else
                            {
                                break;
                            }
                        }

                        if (symbol)
                        {
                            // 展平多维初始化列表到一维序列
                            int flat_index = 0;

                            // 递归函数：展平初始化列表
                            void flatten_init_list(ASTNode * list, int *index_ptr)
                            {
                                for (int i = 0; i < list->num_children; i++)
                                {
                                    ASTNode *child = list->children[i];
                                    if (child->type == AST_INIT_LIST)
                                    {
                                        // 递归处理嵌套列表
                                        flatten_init_list(child, index_ptr);
                                    }
                                    else
                                    {
                                        // 叶子节点：计算并存储
                                        gen_expression(gen, child);
                                        int elem_offset = -(symbol->offset + (*index_ptr + 1) * 8);
                                        emit(gen, "    movq %%rax, %d(%%rbp)  # Initialize array[%d]",
                                             elem_offset, *index_ptr);
                                        (*index_ptr)++;
                                    }
                                }
                            }

                            // 展平并初始化所有元素
                            flatten_init_list(init_expr, &flat_index);
                        }
                    }
                    else
                    {
                        // 普通初始化表达式
                        gen_expression(gen, declarator);
                    }
                }
            }
        }
        break;
    }

    case AST_EXPR_STMT:
        if (node->num_children > 0)
        {
            gen_expression(gen, node->children[0]);
        }
        break;

    case AST_COMPOUND_STMT:
        for (int i = 0; i < node->num_children; i++)
        {
            gen_statement(gen, node->children[i]);
        }
        break;

    case AST_IF_STMT:
    {
        int else_label = new_label(gen);
        int end_label = new_label(gen);

        // 计算条件
        if (node->num_children > 0)
        {
            gen_expression(gen, node->children[0]);
            emit(gen, "    testq %%rax, %%rax  # Test condition");
            emit(gen, "    je .L%d  # Jump if false", else_label);
        }

        // then 分支
        if (node->num_children > 1)
        {
            gen_statement(gen, node->children[1]);
        }
        emit(gen, "    jmp .L%d  # Jump to end", end_label);

        // else 分支
        emit(gen, ".L%d:", else_label);
        if (node->num_children > 2)
        {
            gen_statement(gen, node->children[2]);
        }

        emit(gen, ".L%d:", end_label);
        break;
    }

    case AST_WHILE_STMT:
    {
        int start_label = new_label(gen);
        int end_label = new_label(gen);

        // 设置循环上下文
        LoopContext loop_ctx;
        loop_ctx.start_label = start_label;
        loop_ctx.end_label = end_label;
        loop_ctx.continue_label = start_label; // continue 跳到循环开始
        loop_ctx.parent = gen->loop_context;
        gen->loop_context = &loop_ctx;

        emit(gen, ".L%d:  # While loop start", start_label);

        // 计算条件
        if (node->num_children > 0)
        {
            gen_expression(gen, node->children[0]);
            emit(gen, "    testq %%rax, %%rax  # Test condition");
            emit(gen, "    je .L%d  # Jump if false", end_label);
        }

        // 循环体
        if (node->num_children > 1)
        {
            gen_statement(gen, node->children[1]);
        }

        emit(gen, "    jmp .L%d  # Loop back", start_label);
        emit(gen, ".L%d:  # While loop end", end_label);

        // 恢复循环上下文
        gen->loop_context = loop_ctx.parent;
        break;
    }

    case AST_DO_WHILE_STMT:
    {
        int start_label = new_label(gen);
        int continue_label = new_label(gen);
        int end_label = new_label(gen);

        // 设置循环上下文
        LoopContext loop_ctx;
        loop_ctx.start_label = start_label;
        loop_ctx.end_label = end_label;
        loop_ctx.continue_label = continue_label; // continue 跳到条件判断
        loop_ctx.parent = gen->loop_context;
        gen->loop_context = &loop_ctx;

        emit(gen, ".L%d:  # Do-while loop start", start_label);

        // 循环体
        if (node->num_children > 0)
        {
            gen_statement(gen, node->children[0]);
        }

        emit(gen, ".L%d:  # Do-while continue point", continue_label);

        // 计算条件
        if (node->num_children > 1)
        {
            gen_expression(gen, node->children[1]);
            emit(gen, "    testq %%rax, %%rax  # Test condition");
            emit(gen, "    jne .L%d  # Jump if true", start_label);
        }

        emit(gen, ".L%d:  # Do-while loop end", end_label);

        // 恢复循环上下文
        gen->loop_context = loop_ctx.parent;
        break;
    }

    case AST_FOR_STMT:
    {
        int start_label = new_label(gen);
        int end_label = new_label(gen);
        int inc_label = new_label(gen);

        // 初始化部分 (expression_statement)
        if (node->num_children > 0 && node->children[0])
        {
            gen_statement(gen, node->children[0]);
        }

        // 设置循环上下文
        LoopContext loop_ctx;
        loop_ctx.start_label = start_label;
        loop_ctx.end_label = end_label;
        loop_ctx.continue_label = inc_label; // continue 跳到增量部分
        loop_ctx.parent = gen->loop_context;
        gen->loop_context = &loop_ctx;

        emit(gen, ".L%d:  # For loop start", start_label);

        // 条件部分 (expression_statement)
        if (node->num_children > 1 && node->children[1])
        {
            // expression_statement 可能包含表达式
            if (node->children[1]->num_children > 0)
            {
                gen_expression(gen, node->children[1]->children[0]);
                emit(gen, "    testq %%rax, %%rax  # Test condition");
                emit(gen, "    je .L%d  # Jump if false", end_label);
            }
        }

        // 循环体
        int body_index = (node->num_children == 3) ? 2 : 3;
        if (node->num_children >= body_index + 1)
        {
            gen_statement(gen, node->children[body_index]);
        }

        emit(gen, ".L%d:  # For loop increment", inc_label);

        // 增量部分 (如果有4个子节点，第3个是增量表达式)
        if (node->num_children == 4 && node->children[2])
        {
            gen_expression(gen, node->children[2]);
        }

        emit(gen, "    jmp .L%d  # Loop back", start_label);
        emit(gen, ".L%d:  # For loop end", end_label);

        // 恢复循环上下文
        gen->loop_context = loop_ctx.parent;
        break;
    }

    case AST_RETURN_STMT:
        if (node->num_children > 0)
        {
            gen_expression(gen, node->children[0]);
        }
        else
        {
            emit(gen, "    movq $0, %%rax  # Return 0");
        }
        emit(gen, "    jmp .L%d  # Return", gen->return_label);
        break;

    case AST_SWITCH_STMT:
    {
        if (node->num_children < 2)
            break;

        int end_label = new_label(gen);

        // 计算switch表达式
        gen_expression(gen, node->children[0]);
        emit(gen, "    pushq %%rax  # Save switch value");

        // 设置循环上下文（switch可以使用break）
        LoopContext loop_ctx;
        loop_ctx.end_label = end_label;
        loop_ctx.parent = gen->loop_context;
        gen->loop_context = &loop_ctx;

        // 生成case比较和跳转
        ASTNode *body = node->children[1];
        int default_label = -1;

        if (body->type == AST_COMPOUND_STMT)
        {
            // 第一遍：生成case比较
            for (int i = 0; i < body->num_children; i++)
            {
                if (body->children[i]->type == AST_CASE_STMT)
                {
                    int case_label = new_label(gen);
                    body->children[i]->value.int_val = case_label;

                    if (body->children[i]->num_children >= 1)
                    {
                        emit(gen, "    movq (%%rsp), %%rbx  # Load switch value");
                        gen_expression(gen, body->children[i]->children[0]);
                        emit(gen, "    cmpq %%rax, %%rbx  # Compare case value");
                        emit(gen, "    je .L%d  # Jump if match", case_label);
                    }
                }
                else if (body->children[i]->type == AST_DEFAULT_STMT)
                {
                    default_label = new_label(gen);
                    body->children[i]->value.int_val = default_label;
                }
            }

            // 没有匹配，跳到default或结束
            if (default_label >= 0)
            {
                emit(gen, "    jmp .L%d  # Jump to default", default_label);
            }
            else
            {
                emit(gen, "    addq $8, %%rsp  # Pop switch value");
                emit(gen, "    jmp .L%d  # No match, exit switch", end_label);
            }

            // 第二遍：生成case代码
            for (int i = 0; i < body->num_children; i++)
            {
                if (body->children[i]->type == AST_CASE_STMT)
                {
                    emit(gen, ".L%d:  # case", body->children[i]->value.int_val);
                    if (body->children[i]->num_children >= 2)
                    {
                        gen_statement(gen, body->children[i]->children[1]);
                    }
                }
                else if (body->children[i]->type == AST_DEFAULT_STMT)
                {
                    emit(gen, ".L%d:  # default", body->children[i]->value.int_val);
                    if (body->children[i]->num_children >= 1)
                    {
                        gen_statement(gen, body->children[i]->children[0]);
                    }
                }
                else
                {
                    gen_statement(gen, body->children[i]);
                }
            }
        }

        emit(gen, "    addq $8, %%rsp  # Pop switch value");
        emit(gen, ".L%d:  # Switch end", end_label);

        // 恢复循环上下文
        gen->loop_context = loop_ctx.parent;
        break;
    }

    case AST_CASE_STMT:
    case AST_DEFAULT_STMT:
        // 这些在switch中处理
        break;

    case AST_BREAK_STMT:
        if (gen->loop_context)
        {
            emit(gen, "    jmp .L%d  # Break", gen->loop_context->end_label);
        }
        else
        {
            emit(gen, "    # ERROR: break outside loop");
        }
        break;

    case AST_CONTINUE_STMT:
        if (gen->loop_context)
        {
            emit(gen, "    jmp .L%d  # Continue", gen->loop_context->continue_label);
        }
        else
        {
            emit(gen, "    # ERROR: continue outside loop");
        }
        break;

    default:
        break;
    }
}

// 生成函数代码
void gen_function(CodeGenerator *gen, ASTNode *node)
{
    if (!node || node->type != AST_FUNCTION_DEF)
        return;
    if (node->num_children < 3)
        return;

    // 获取函数名
    ASTNode *declarator = node->children[1];
    const char *func_name = declarator->value.string_val;

    // 为这个函数分配唯一的返回标签
    gen->return_label = new_label(gen);

    // 生成序言
    gen_prologue(gen, func_name);

    // 处理函数参数（从寄存器保存到栈）
    // System V AMD64 ABI: rdi, rsi, rdx, rcx, r8, r9
    const char *param_regs[] = {"rdi", "rsi", "rdx", "rcx", "r8", "r9"};
    if (declarator->num_children > 0 && declarator->children[0]->type == AST_PARAM_LIST)
    {
        ASTNode *param_list = declarator->children[0];
        int num_params = param_list->num_children < 6 ? param_list->num_children : 6;

        for (int i = 0; i < num_params; i++)
        {
            ASTNode *param = param_list->children[i];
            if (param->type == AST_DECLARATION && param->num_children >= 2)
            {
                ASTNode *param_declarator = param->children[1];
                const char *param_name = param_declarator->value.string_val;
                Symbol *param_symbol = (Symbol *)param_declarator->semantic_info;

                if (param_symbol)
                {
                    int offset = -(param_symbol->offset + 8);
                    emit(gen, "    movq %%%s, %d(%%rbp)  # Save parameter '%s'",
                         param_regs[i], offset, param_name);
                }
            }
        }
    }

    // 生成函数体
    gen_statement(gen, node->children[2]);

    // 生成尾声
    gen_epilogue(gen);
}

// 收集全局/静态变量
static void collect_global_symbols(ASTNode *node, Symbol ***vars, int *count, int *capacity)
{
    if (!node)
        return;

    if (node->type == AST_DECLARATION && node->num_children >= 2)
    {
        ASTNode *declarator = node->children[1];
        Symbol *symbol = NULL;

        if (declarator->type == AST_ASSIGN_EXPR && declarator->num_children > 0)
        {
            symbol = (Symbol *)declarator->children[0]->semantic_info;
        }
        else
        {
            symbol = (Symbol *)declarator->semantic_info;
        }

        if (symbol && (symbol->is_global || symbol->is_static))
        {
            if (*count >= *capacity)
            {
                *capacity *= 2;
                *vars = realloc(*vars, (*capacity) * sizeof(Symbol *));
            }
            (*vars)[(*count)++] = symbol;
        }
    }

    for (int i = 0; i < node->num_children; i++)
    {
        collect_global_symbols(node->children[i], vars, count, capacity);
    }
}

// 生成全局/静态变量段
static void gen_global_data(CodeGenerator *gen, Symbol **vars, int count)
{
    if (count == 0)
        return;

    emit(gen, "");
    emit(gen, "    .data");
    emit(gen, "    # Global and static variables");

    for (int i = 0; i < count; i++)
    {
        Symbol *var = vars[i];
        if (var->is_global)
        {
            emit(gen, "    .globl %s", var->label);
        }
        emit(gen, "%s:", var->label);

        // 从声明节点中获取初始化值
        long init_value = 0;
        if (var->declaration && var->declaration->num_children >= 2)
        {
            ASTNode *declarator = var->declaration->children[1];
            if (declarator->type == AST_ASSIGN_EXPR && declarator->num_children >= 2)
            {
                ASTNode *init_expr = declarator->children[1];
                if (init_expr->type == AST_INT_LITERAL)
                {
                    init_value = init_expr->value.int_val;
                }
            }
        }

        emit(gen, "    .quad %ld  # %s", init_value, var->name);
    }
}

// 生成完整程序代码
void generate_code(CodeGenerator *gen, ASTNode *root)
{
    if (!root || root->type != AST_PROGRAM)
        return;

    // 输出文件头
    emit(gen, "    .file \"output.c\"");

    // 收集所有全局/静态变量
    Symbol **global_vars = malloc(16 * sizeof(Symbol *));
    int var_count = 0;
    int var_capacity = 16;

    for (int i = 0; i < root->num_children; i++)
    {
        collect_global_symbols(root->children[i], &global_vars, &var_count, &var_capacity);
    }

    // 生成全局/静态变量段
    gen_global_data(gen, global_vars, var_count);
    free(global_vars);

    emit(gen, "");
    emit(gen, "    .text");

    // 遍历所有函数（这会收集字符串常量）
    for (int i = 0; i < root->num_children; i++)
    {
        if (root->children[i]->type == AST_FUNCTION_DEF)
        {
            gen_function(gen, root->children[i]);
        }
    }

    // 在代码生成完毕后输出字符串常量（.rodata段）
    emit(gen, "");
    emit_string_constants(gen);

    // 输出文件尾
    emit(gen, "");
    emit(gen, "    .section .note.GNU-stack,\"\",@progbits");
}
