#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include "types.h"
#include "ast.h"

// 符号类型
typedef enum {
    SYMBOL_VARIABLE,
    SYMBOL_FUNCTION,
    SYMBOL_PARAMETER
} SymbolKind;

// 符号表项
typedef struct Symbol {
    char *name;
    TypeInfo *type;
    SymbolKind kind;
    int scope_level;
    int offset;                 // 相对于栈帧的偏移
    ASTNode *declaration;       // 指向声明节点
    int is_defined;             // 函数是否已定义
} Symbol;

// 作用域
typedef struct Scope {
    Symbol **symbols;
    int num_symbols;
    int capacity;
    struct Scope *parent;       // 父作用域
    int level;                  // 作用域层级
    int next_offset;            // 下一个可用的偏移量
} Scope;

// 符号表
typedef struct SymbolTable {
    Scope *current_scope;
    Scope *global_scope;
    int current_level;
    int has_errors;             // 是否有错误
} SymbolTable;

// 函数声明
SymbolTable *symbol_table_create();
void symbol_table_destroy(SymbolTable *table);

// 作用域管理
void enter_scope(SymbolTable *table);
void exit_scope(SymbolTable *table);

// 符号操作
Symbol *symbol_create(const char *name, TypeInfo *type, SymbolKind kind);
int symbol_table_insert(SymbolTable *table, Symbol *symbol);
Symbol *symbol_table_lookup(SymbolTable *table, const char *name);
Symbol *symbol_table_lookup_current_scope(SymbolTable *table, const char *name);

// 调试函数
void print_symbol_table(SymbolTable *table);
void print_scope(Scope *scope, int indent);

#endif // SYMBOL_TABLE_H
