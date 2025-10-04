#include "symbol_table.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INITIAL_SCOPE_CAPACITY 16

// 创建符号
Symbol *symbol_create(const char *name, TypeInfo *type, SymbolKind kind)
{
    Symbol *symbol = (Symbol *)malloc(sizeof(Symbol));
    if (!symbol)
    {
        fprintf(stderr, "Error: Failed to allocate memory for symbol\n");
        exit(1);
    }
    symbol->name = strdup(name);
    symbol->type = type;
    symbol->kind = kind;
    symbol->scope_level = 0;
    symbol->offset = 0;
    symbol->declaration = NULL;
    symbol->is_defined = 0;
    return symbol;
}

// 创建作用域
static Scope *scope_create(Scope *parent, int level)
{
    Scope *scope = (Scope *)malloc(sizeof(Scope));
    if (!scope)
    {
        fprintf(stderr, "Error: Failed to allocate memory for scope\n");
        exit(1);
    }
    scope->symbols = (Symbol **)malloc(INITIAL_SCOPE_CAPACITY * sizeof(Symbol *));
    if (!scope->symbols)
    {
        fprintf(stderr, "Error: Failed to allocate memory for symbols array\n");
        exit(1);
    }
    scope->num_symbols = 0;
    scope->capacity = INITIAL_SCOPE_CAPACITY;
    scope->parent = parent;
    scope->level = level;
    // 继承父作用域的偏移量，这样嵌套作用域的变量不会覆盖外层变量
    scope->next_offset = parent ? parent->next_offset : 0;
    return scope;
}

// 销毁作用域
static void scope_destroy(Scope *scope)
{
    if (!scope)
        return;

    for (int i = 0; i < scope->num_symbols; i++)
    {
        free(scope->symbols[i]->name);
        // 只释放非参数符号的类型
        // 参数类型已经在函数类型的 param_types 中，会在函数符号释放时一起释放
        if (scope->symbols[i]->kind != SYMBOL_PARAMETER)
        {
            free_type(scope->symbols[i]->type);
        }
        free(scope->symbols[i]);
    }
    free(scope->symbols);
    free(scope);
}

// 创建符号表
SymbolTable *symbol_table_create()
{
    SymbolTable *table = (SymbolTable *)malloc(sizeof(SymbolTable));
    if (!table)
    {
        fprintf(stderr, "Error: Failed to allocate memory for symbol table\n");
        exit(1);
    }

    table->global_scope = scope_create(NULL, 0);
    table->current_scope = table->global_scope;
    table->current_level = 0;
    table->has_errors = 0;

    return table;
}

// 销毁符号表
void symbol_table_destroy(SymbolTable *table)
{
    if (!table)
        return;

    // 销毁所有作用域（从当前作用域回溯到全局）
    Scope *scope = table->current_scope;
    while (scope)
    {
        Scope *parent = scope->parent;
        scope_destroy(scope);
        scope = parent;
    }

    free(table);
}

// 进入新作用域
void enter_scope(SymbolTable *table)
{
    if (!table)
        return;

    table->current_level++;
    Scope *new_scope = scope_create(table->current_scope, table->current_level);
    table->current_scope = new_scope;
}

// 退出当前作用域
void exit_scope(SymbolTable *table)
{
    if (!table || !table->current_scope)
        return;

    Scope *old_scope = table->current_scope;
    Scope *parent = old_scope->parent;

    // 更新父作用域的偏移量，反映子作用域占用的栈空间
    if (parent)
    {
        parent->next_offset = old_scope->next_offset;
    }

    table->current_scope = parent;
    table->current_level--;

    scope_destroy(old_scope);
}

// 在当前作用域查找符号
Symbol *symbol_table_lookup_current_scope(SymbolTable *table, const char *name)
{
    if (!table || !table->current_scope || !name)
        return NULL;

    Scope *scope = table->current_scope;
    for (int i = 0; i < scope->num_symbols; i++)
    {
        if (strcmp(scope->symbols[i]->name, name) == 0)
        {
            return scope->symbols[i];
        }
    }

    return NULL;
}

// 在所有作用域中查找符号（从内到外）
Symbol *symbol_table_lookup(SymbolTable *table, const char *name)
{
    if (!table || !name)
        return NULL;

    Scope *scope = table->current_scope;
    while (scope)
    {
        for (int i = 0; i < scope->num_symbols; i++)
        {
            if (strcmp(scope->symbols[i]->name, name) == 0)
            {
                return scope->symbols[i];
            }
        }
        scope = scope->parent;
    }

    return NULL;
}

// 插入符号到当前作用域
int symbol_table_insert(SymbolTable *table, Symbol *symbol)
{
    if (!table || !symbol)
        return 0;

    // 检查当前作用域是否已存在同名符号
    if (symbol_table_lookup_current_scope(table, symbol->name))
    {
        fprintf(stderr, "Error: Symbol '%s' already declared in current scope\n",
                symbol->name);
        table->has_errors = 1;
        return 0;
    }

    Scope *scope = table->current_scope;

    // 扩展数组容量（如果需要）
    if (scope->num_symbols >= scope->capacity)
    {
        scope->capacity *= 2;
        scope->symbols = (Symbol **)realloc(scope->symbols,
                                            scope->capacity * sizeof(Symbol *));
        if (!scope->symbols)
        {
            fprintf(stderr, "Error: Failed to reallocate memory for symbols\n");
            exit(1);
        }
    }

    // 设置符号属性
    symbol->scope_level = scope->level;

    // 为变量分配偏移量
    if (symbol->kind == SYMBOL_VARIABLE || symbol->kind == SYMBOL_PARAMETER)
    {
        symbol->offset = scope->next_offset;

        // 计算变量所需的空间
        int var_size = 8; // 默认 8 字节（int, float, 指针）

        if (symbol->type)
        {
            // 如果是数组，分配 array_size * element_size
            if (symbol->type->array_size > 0)
            {
                int element_size = 8; // 假设元素大小为 8 字节
                var_size = symbol->type->array_size * element_size;
            }
            // 如果是结构体，使用结构体大小
            else if (symbol->type->base_type == TYPE_STRUCT && symbol->type->struct_size > 0)
            {
                var_size = symbol->type->struct_size;
            }
        }

        scope->next_offset += var_size;
    }

    // 添加到作用域
    scope->symbols[scope->num_symbols++] = symbol;

    return 1;
}

// 打印作用域（用于调试）
void print_scope(Scope *scope, int indent)
{
    if (!scope)
        return;

    for (int i = 0; i < indent; i++)
        printf("  ");
    printf("Scope (level %d, %d symbols):\n", scope->level, scope->num_symbols);

    for (int i = 0; i < scope->num_symbols; i++)
    {
        Symbol *sym = scope->symbols[i];
        for (int j = 0; j < indent + 1; j++)
            printf("  ");

        const char *kind_str = "";
        switch (sym->kind)
        {
        case SYMBOL_VARIABLE:
            kind_str = "var";
            break;
        case SYMBOL_FUNCTION:
            kind_str = "func";
            break;
        case SYMBOL_PARAMETER:
            kind_str = "param";
            break;
        }

        printf("- %s: %s (%s), offset=%d\n",
               sym->name, type_to_string(sym->type), kind_str, sym->offset);
    }
}

// 打印符号表（用于调试）
void print_symbol_table(SymbolTable *table)
{
    if (!table)
        return;

    printf("\n=== Symbol Table ===\n");

    // 收集所有作用域（从全局到当前）
    Scope *scopes[100]; // 假设最多100层嵌套
    int num_scopes = 0;

    Scope *scope = table->current_scope;
    while (scope)
    {
        scopes[num_scopes++] = scope;
        scope = scope->parent;
    }

    // 反向打印（从全局到内层）
    for (int i = num_scopes - 1; i >= 0; i--)
    {
        print_scope(scopes[i], num_scopes - 1 - i);
    }

    printf("===================\n\n");
}
