#ifndef SEMANTIC_H
#define SEMANTIC_H

#include "ast.h"
#include "symbol_table.h"

// 语义分析器结构
typedef struct SemanticAnalyzer
{
    SymbolTable *symbol_table;
    int error_count;
    int warning_count;
    int loop_depth; // 当前循环嵌套深度（用于检查 break/continue）
} SemanticAnalyzer;

// 主要函数
SemanticAnalyzer *semantic_analyzer_create();
void semantic_analyzer_destroy(SemanticAnalyzer *analyzer);
int analyze_program(SemanticAnalyzer *analyzer, ASTNode *root);

// AST 遍历函数
void analyze_function(SemanticAnalyzer *analyzer, ASTNode *node);
void analyze_declaration(SemanticAnalyzer *analyzer, ASTNode *node);
void analyze_statement(SemanticAnalyzer *analyzer, ASTNode *node);
TypeInfo *analyze_expression(SemanticAnalyzer *analyzer, ASTNode *node);

// 类型推导和检查
TypeInfo *get_type_from_specifier(ASTNode *node);
void check_assignment(SemanticAnalyzer *analyzer, ASTNode *lhs, TypeInfo *rhs_type, int lineno);
TypeInfo *check_binary_operation(SemanticAnalyzer *analyzer, OperatorType op,
                                 TypeInfo *left, TypeInfo *right, int lineno);

// 错误报告
void semantic_error(SemanticAnalyzer *analyzer, int lineno, const char *format, ...);
void semantic_warning(SemanticAnalyzer *analyzer, int lineno, const char *format, ...);

#endif // SEMANTIC_H
