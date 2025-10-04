#ifndef CODEGEN_H
#define CODEGEN_H

#include "ast.h"
#include "semantic.h"
#include <stdio.h>

// 循环上下文（用于 break/continue）
typedef struct LoopContext
{
    int start_label;            // 循环开始标签
    int end_label;              // 循环结束标签
    int continue_label;         // continue 跳转标签
    struct LoopContext *parent; // 父循环上下文
} LoopContext;

// 字符串常量结构
typedef struct StringConstant
{
    int label;     // 字符串标签编号 (.LC0, .LC1, ...)
    char *content; // 字符串内容
} StringConstant;

// 代码生成器结构
typedef struct CodeGenerator
{
    FILE *output;               // 输出文件
    SemanticAnalyzer *analyzer; // 语义分析器（用于符号表）
    int label_counter;          // 标签计数器
    int current_stack_offset;   // 当前栈偏移
    int max_stack_size;         // 最大栈大小
    LoopContext *loop_context;  // 当前循环上下文（用于 break/continue）
    int return_label;           // 当前函数的返回标签
    StringConstant **strings;   // 字符串常量数组
    int num_strings;            // 字符串常量数量
    int string_capacity;        // 字符串数组容量
} CodeGenerator;

// 主要函数
CodeGenerator *codegen_create(FILE *output, SemanticAnalyzer *analyzer);
void codegen_destroy(CodeGenerator *gen);
void generate_code(CodeGenerator *gen, ASTNode *root);

// 代码生成函数
void gen_function(CodeGenerator *gen, ASTNode *node);
void gen_statement(CodeGenerator *gen, ASTNode *node);
void gen_expression(CodeGenerator *gen, ASTNode *node);

// 辅助函数
void gen_prologue(CodeGenerator *gen, const char *func_name);
void gen_epilogue(CodeGenerator *gen);
int new_label(CodeGenerator *gen);
void emit(CodeGenerator *gen, const char *format, ...);

// 栈管理
void push_reg(CodeGenerator *gen, const char *reg);
void pop_reg(CodeGenerator *gen, const char *reg);

#endif // CODEGEN_H
