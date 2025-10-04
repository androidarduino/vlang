#include "semantic.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

// 创建语义分析器
SemanticAnalyzer *semantic_analyzer_create()
{
    SemanticAnalyzer *analyzer = (SemanticAnalyzer *)malloc(sizeof(SemanticAnalyzer));
    if (!analyzer)
    {
        fprintf(stderr, "Error: Failed to allocate memory for semantic analyzer\n");
        exit(1);
    }
    analyzer->symbol_table = symbol_table_create();
    analyzer->error_count = 0;
    analyzer->warning_count = 0;
    analyzer->loop_depth = 0;
    return analyzer;
}

// 销毁语义分析器
void semantic_analyzer_destroy(SemanticAnalyzer *analyzer)
{
    if (!analyzer)
        return;
    symbol_table_destroy(analyzer->symbol_table);
    free(analyzer);
}

// 错误报告
void semantic_error(SemanticAnalyzer *analyzer, int lineno, const char *format, ...)
{
    fprintf(stderr, "Semantic Error (line %d): ", lineno);
    va_list args;
    va_start(args, format);
    vfprintf(stderr, format, args);
    va_end(args);
    fprintf(stderr, "\n");
    analyzer->error_count++;
}

// 警告报告
void semantic_warning(SemanticAnalyzer *analyzer, int lineno, const char *format, ...)
{
    fprintf(stderr, "Warning (line %d): ", lineno);
    va_list args;
    va_start(args, format);
    vfprintf(stderr, format, args);
    va_end(args);
    fprintf(stderr, "\n");
    analyzer->warning_count++;
}

// 从类型说明符获取类型
TypeInfo *get_type_from_specifier(ASTNode *node)
{
    if (!node || node->type != AST_TYPE_SPECIFIER)
    {
        return create_type(TYPE_UNKNOWN);
    }

    const char *type_str = node->value.string_val;
    if (strcmp(type_str, "int") == 0)
    {
        return create_type(TYPE_INT);
    }
    else if (strcmp(type_str, "float") == 0)
    {
        return create_type(TYPE_FLOAT);
    }
    else if (strcmp(type_str, "char") == 0)
    {
        return create_type(TYPE_CHAR);
    }
    else if (strcmp(type_str, "void") == 0)
    {
        return create_type(TYPE_VOID);
    }

    return create_type(TYPE_UNKNOWN);
}

// 检查二元操作的类型
TypeInfo *check_binary_operation(SemanticAnalyzer *analyzer, OperatorType op,
                                 TypeInfo *left, TypeInfo *right, int lineno)
{
    if (!left || !right)
    {
        semantic_error(analyzer, lineno, "Invalid operand types");
        return create_type(TYPE_UNKNOWN);
    }

    // 算术运算符
    if (op == OP_ADD || op == OP_SUB || op == OP_MUL || op == OP_DIV || op == OP_MOD)
    {
        if (left->base_type != TYPE_INT && left->base_type != TYPE_FLOAT)
        {
            semantic_error(analyzer, lineno,
                           "Invalid left operand type for arithmetic operation: %s",
                           type_to_string(left));
            return create_type(TYPE_UNKNOWN);
        }
        if (right->base_type != TYPE_INT && right->base_type != TYPE_FLOAT)
        {
            semantic_error(analyzer, lineno,
                           "Invalid right operand type for arithmetic operation: %s",
                           type_to_string(right));
            return create_type(TYPE_UNKNOWN);
        }

        // 如果任一操作数是 float，结果是 float
        if (left->base_type == TYPE_FLOAT || right->base_type == TYPE_FLOAT)
        {
            return create_type(TYPE_FLOAT);
        }
        return create_type(TYPE_INT);
    }

    // 关系运算符
    if (op == OP_LT || op == OP_GT || op == OP_LE || op == OP_GE ||
        op == OP_EQ || op == OP_NE)
    {
        if (!types_compatible(left, right))
        {
            semantic_warning(analyzer, lineno,
                             "Comparing incompatible types: %s and %s",
                             type_to_string(left), type_to_string(right));
        }
        return create_type(TYPE_INT); // 关系运算结果是 int (0 或 1)
    }

    // 逻辑运算符
    if (op == OP_AND || op == OP_OR)
    {
        // 逻辑运算符可以接受任何类型，结果是 int (0 或 1)
        return create_type(TYPE_INT);
    }

    semantic_error(analyzer, lineno, "Unknown binary operator");
    return create_type(TYPE_UNKNOWN);
}

// 检查赋值
void check_assignment(SemanticAnalyzer *analyzer, ASTNode *lhs, TypeInfo *rhs_type, int lineno)
{
    TypeInfo *lhs_type = NULL;

    // 支持多种左值类型
    if (lhs->type == AST_IDENTIFIER)
    {
        const char *var_name = lhs->value.string_val;
        Symbol *symbol = symbol_table_lookup(analyzer->symbol_table, var_name);

        if (!symbol)
        {
            semantic_error(analyzer, lineno, "Undeclared variable: %s", var_name);
            return;
        }

        // 存储符号信息到 AST 节点
        lhs->semantic_info = (void *)symbol;
        lhs_type = symbol->type;
    }
    else if (lhs->type == AST_ARRAY_SUBSCRIPT)
    {
        // 数组下标访问可以作为左值
        lhs_type = analyze_expression(analyzer, lhs);
    }
    else if (lhs->type == AST_UNARY_EXPR && lhs->value.op_type == OP_DEREF)
    {
        // 解引用可以作为左值 (*p = value)
        lhs_type = analyze_expression(analyzer, lhs);
    }
    else if (lhs->type == AST_MEMBER_ACCESS)
    {
        // 结构体成员访问可以作为左值 (s.x = value)
        lhs_type = analyze_expression(analyzer, lhs);
    }
    else
    {
        semantic_error(analyzer, lineno, "Invalid left side of assignment");
        return;
    }

    if (lhs_type && !types_compatible(lhs_type, rhs_type))
    {
        semantic_warning(analyzer, lineno,
                         "Type mismatch in assignment: %s = %s",
                         type_to_string(lhs_type), type_to_string(rhs_type));
    }
}

// 分析表达式
TypeInfo *analyze_expression(SemanticAnalyzer *analyzer, ASTNode *node)
{
    if (!node)
        return create_type(TYPE_UNKNOWN);

    switch (node->type)
    {
    case AST_INT_LITERAL:
        return create_type(TYPE_INT);

    case AST_FLOAT_LITERAL:
        return create_type(TYPE_FLOAT);

    case AST_STRING_LITERAL:
        return create_type(TYPE_CHAR); // 简化：字符串视为 char

    case AST_IDENTIFIER:
    {
        const char *name = node->value.string_val;
        Symbol *symbol = symbol_table_lookup(analyzer->symbol_table, name);
        if (!symbol)
        {
            semantic_error(analyzer, node->lineno, "Undeclared variable: %s", name);
            return create_type(TYPE_UNKNOWN);
        }
        // 存储符号信息到节点
        node->semantic_info = (void *)symbol;
        return symbol->type;
    }

    case AST_BINARY_EXPR:
    {
        if (node->num_children < 2)
        {
            semantic_error(analyzer, node->lineno, "Binary expression missing operands");
            return create_type(TYPE_UNKNOWN);
        }
        TypeInfo *left = analyze_expression(analyzer, node->children[0]);
        TypeInfo *right = analyze_expression(analyzer, node->children[1]);
        return check_binary_operation(analyzer, node->value.op_type, left, right, node->lineno);
    }

    case AST_ASSIGN_EXPR:
    {
        if (node->num_children < 2)
        {
            semantic_error(analyzer, node->lineno, "Assignment missing operands");
            return create_type(TYPE_UNKNOWN);
        }
        TypeInfo *rhs_type = analyze_expression(analyzer, node->children[1]);
        check_assignment(analyzer, node->children[0], rhs_type, node->lineno);
        return rhs_type;
    }

    case AST_UNARY_EXPR:
    {
        if (node->num_children < 1)
        {
            semantic_error(analyzer, node->lineno, "Unary expression missing operand");
            return create_type(TYPE_UNKNOWN);
        }
        TypeInfo *operand_type = analyze_expression(analyzer, node->children[0]);

        // 处理不同的一元运算符
        if (node->value.op_type == OP_NOT)
        {
            return create_type(TYPE_INT); // 逻辑非返回 int (0 或 1)
        }
        else if (node->value.op_type == OP_NEG)
        {
            return operand_type; // 取负返回相同类型
        }
        else if (node->value.op_type == OP_ADDR)
        {
            // 取地址：返回指向操作数类型的指针
            TypeInfo *ptr_type = create_pointer_type(operand_type);
            return ptr_type;
        }
        else if (node->value.op_type == OP_DEREF)
        {
            // 解引用：返回指针指向的类型
            if (operand_type->pointer_level == 0)
            {
                semantic_error(analyzer, node->lineno,
                               "Cannot dereference non-pointer type");
                return create_type(TYPE_UNKNOWN);
            }
            // 创建一个指针级别减1的类型
            TypeInfo *deref_type = create_type(operand_type->base_type);
            deref_type->pointer_level = operand_type->pointer_level - 1;
            return deref_type;
        }

        return operand_type;
    }

    case AST_CALL_EXPR:
    {
        if (node->num_children < 1)
        {
            semantic_error(analyzer, node->lineno, "Function call missing function name");
            return create_type(TYPE_UNKNOWN);
        }

        // 获取函数名
        ASTNode *func_node = node->children[0];
        if (func_node->type != AST_IDENTIFIER)
        {
            semantic_error(analyzer, node->lineno, "Function name must be an identifier");
            return create_type(TYPE_UNKNOWN);
        }

        const char *func_name = func_node->value.string_val;
        Symbol *func_symbol = symbol_table_lookup(analyzer->symbol_table, func_name);

        if (!func_symbol)
        {
            semantic_error(analyzer, node->lineno, "Undeclared function: %s", func_name);
            return create_type(TYPE_UNKNOWN);
        }

        if (func_symbol->kind != SYMBOL_FUNCTION)
        {
            semantic_error(analyzer, node->lineno, "'%s' is not a function", func_name);
            return create_type(TYPE_UNKNOWN);
        }

        // 存储函数符号到 AST
        func_node->semantic_info = (void *)func_symbol;

        // 检查参数数量和类型
        TypeInfo *func_type = func_symbol->type;
        int expected_params = func_type->num_params;
        int actual_params = 0;

        if (node->num_children > 1 && node->children[1]->type == AST_ARG_LIST)
        {
            actual_params = node->children[1]->num_children;

            // 检查每个参数的类型
            for (int i = 0; i < actual_params && i < expected_params; i++)
            {
                TypeInfo *arg_type = analyze_expression(analyzer, node->children[1]->children[i]);
                TypeInfo *param_type = func_type->param_types[i];

                if (!types_compatible(arg_type, param_type))
                {
                    semantic_warning(analyzer, node->lineno,
                                     "Argument %d type mismatch: expected %s, got %s",
                                     i + 1, type_to_string(param_type), type_to_string(arg_type));
                }
            }
        }

        if (actual_params != expected_params)
        {
            semantic_error(analyzer, node->lineno,
                           "Function '%s' expects %d arguments, but %d were provided",
                           func_name, expected_params, actual_params);
        }

        // 返回函数的返回类型
        return func_type->return_type;
    }

    case AST_ARRAY_SUBSCRIPT:
    {
        if (node->num_children < 2)
        {
            semantic_error(analyzer, node->lineno, "Array subscript missing operands");
            return create_type(TYPE_UNKNOWN);
        }

        TypeInfo *array_type = analyze_expression(analyzer, node->children[0]);
        TypeInfo *index_type = analyze_expression(analyzer, node->children[1]);

        // 检查索引是否为整数类型
        if (index_type->base_type != TYPE_INT)
        {
            semantic_error(analyzer, node->lineno,
                           "Array subscript must be of integer type, got %s",
                           type_to_string(index_type));
        }

        // 数组访问返回元素类型
        // 如果是指针，则返回指针指向的类型
        if (array_type->pointer_level > 0)
        {
            TypeInfo *elem_type = create_type(array_type->base_type);
            elem_type->pointer_level = array_type->pointer_level - 1;
            elem_type->array_size = array_type->array_size;
            return elem_type;
        }
        else if (array_type->array_size >= 0)
        {
            // 数组类型，返回元素类型
            TypeInfo *elem_type = create_type(array_type->base_type);
            elem_type->pointer_level = array_type->pointer_level;
            return elem_type;
        }
        else
        {
            semantic_error(analyzer, node->lineno,
                           "Cannot subscript non-array type: %s",
                           type_to_string(array_type));
            return create_type(TYPE_UNKNOWN);
        }
    }

    case AST_MEMBER_ACCESS:
    {
        if (node->num_children < 2)
        {
            semantic_error(analyzer, node->lineno, "Member access missing operands");
            return create_type(TYPE_UNKNOWN);
        }

        TypeInfo *struct_type = analyze_expression(analyzer, node->children[0]);
        ASTNode *member_node = node->children[1];

        if (member_node->type != AST_IDENTIFIER)
        {
            semantic_error(analyzer, node->lineno, "Member name must be an identifier");
            return create_type(TYPE_UNKNOWN);
        }

        const char *member_name = member_node->value.string_val;

        // 处理箭头运算符：需要自动解引用
        if (node->value.op_type == OP_ARROW)
        {
            if (struct_type->pointer_level == 0)
            {
                semantic_error(analyzer, node->lineno,
                               "Cannot use '->' on non-pointer type: %s",
                               type_to_string(struct_type));
                return create_type(TYPE_UNKNOWN);
            }
            // 解引用
            struct_type->pointer_level--;
        }

        // 检查是否为结构体类型
        if (struct_type->base_type != TYPE_STRUCT)
        {
            semantic_error(analyzer, node->lineno,
                           "Member access requires struct type, got: %s",
                           type_to_string(struct_type));
            return create_type(TYPE_UNKNOWN);
        }

        // 查找成员
        for (int i = 0; i < struct_type->num_members; i++)
        {
            if (strcmp(struct_type->members[i].name, member_name) == 0)
            {
                return struct_type->members[i].type;
            }
        }

        semantic_error(analyzer, node->lineno,
                       "Struct has no member named '%s'", member_name);
        return create_type(TYPE_UNKNOWN);
    }

    case AST_INIT_LIST:
    {
        // 初始化列表：返回未知类型，由上下文确定
        return create_type(TYPE_UNKNOWN);
    }

    default:
        return create_type(TYPE_UNKNOWN);
    }
}

// 分析声明
void analyze_declaration(SemanticAnalyzer *analyzer, ASTNode *node)
{
    if (!node || node->type != AST_DECLARATION)
        return;
    if (node->num_children < 2)
        return;

    // 获取基本类型
    TypeInfo *base_type = get_type_from_specifier(node->children[0]);

    // 获取声明符（可能包含初始化）
    ASTNode *declarator = node->children[1];

    // 处理指针类型和数组类型：根据 declarator 中的信息调整类型
    TypeInfo *var_type = base_type;
    int pointer_level = 0;
    int array_size = -1;
    const char *var_name = NULL;

    // 从 declarator 获取变量名、指针级别和数组大小
    ASTNode *id_node_temp = declarator;
    if (declarator->type == AST_ASSIGN_EXPR && declarator->num_children > 0)
    {
        id_node_temp = declarator->children[0];
    }

    // 简化的declarator处理逻辑
    ASTNode *current = id_node_temp;

    // 如果是标识符，直接获取名字
    if (current->type == AST_IDENTIFIER)
    {
        var_name = current->value.string_val;
        // AST_IDENTIFIER 不使用 int_val，pointer_level 保持为 0
    }
    // 如果是AST_DECLARATOR，可能是数组或指针
    else if (current->type == AST_DECLARATOR)
    {
        // 检查是否有子节点（数组声明）
        if (current->num_children > 0)
        {
            // value.int_val可能存储数组大小（>= 0）或其他信息
            if (current->value.int_val > 0)
            {
                // 这是数组声明
                array_size = current->value.int_val;
            }

            // 获取子节点（可能是IDENTIFIER或另一个DECLARATOR）
            ASTNode *child = current->children[0];
            if (child)
            {
                if (child->type == AST_IDENTIFIER)
                {
                    var_name = child->value.string_val;
                }
                else if (child->type == AST_DECLARATOR)
                {
                    // 子节点也是DECLARATOR（来自parser规则中的类型覆盖）
                    var_name = child->value.string_val;
                }
            }
        }
        else
        {
            // 没有子节点，可能直接使用string_val（旧代码兼容性）
            if (current->value.string_val)
            {
                var_name = current->value.string_val;
            }
            // 只有在明确是指针时才使用int_val
            if (current->value.int_val > 0)
            {
                pointer_level = current->value.int_val;
            }
        }
    }

    // 创建数组类型
    if (array_size >= 0)
    {
        var_type = create_array_type(var_type, array_size);
    }

    // 创建指针类型
    for (int i = 0; i < pointer_level; i++)
    {
        var_type = create_pointer_type(var_type);
    }

    // 如果还没有变量名，尝试从原始 declarator 获取
    if (!var_name)
    {
        if (declarator->type == AST_IDENTIFIER)
        {
            var_name = declarator->value.string_val;
        }
        else if (declarator->type == AST_DECLARATOR)
        {
            var_name = declarator->value.string_val;
        }
    }

    // 确保有变量名
    if (!var_name)
    {
        semantic_error(analyzer, node->lineno, "Declaration missing variable name");
        return;
    }

    // 如果是赋值表达式（带初始化）
    if (declarator->type == AST_ASSIGN_EXPR)
    {
        if (declarator->num_children < 2)
            return;

        // 创建并插入符号
        Symbol *symbol = symbol_create(var_name, var_type, SYMBOL_VARIABLE);
        symbol->declaration = node;

        if (!symbol_table_insert(analyzer->symbol_table, symbol))
        {
            semantic_error(analyzer, node->lineno,
                           "Variable '%s' already declared", var_name);
        }

        // 存储符号信息到 AST 节点
        declarator->children[0]->semantic_info = (void *)symbol;

        // 检查初始化表达式的类型
        ASTNode *init_expr = declarator->children[1];

        // 对于数组，检查初始化列表
        if (init_expr->type == AST_INIT_LIST)
        {
            if (array_size >= 0 && init_expr->num_children > array_size)
            {
                semantic_warning(analyzer, node->lineno,
                                 "Too many initializers for array of size %d", array_size);
            }
            // 检查每个初始化元素的类型
            for (int i = 0; i < init_expr->num_children; i++)
            {
                TypeInfo *elem_type = analyze_expression(analyzer, init_expr->children[i]);
                if (!types_compatible(base_type, elem_type))
                {
                    semantic_warning(analyzer, node->lineno,
                                     "Array element %d type mismatch: expected %s, got %s",
                                     i, type_to_string(base_type), type_to_string(elem_type));
                }
            }
        }
        else
        {
            TypeInfo *init_type = analyze_expression(analyzer, init_expr);
            if (!types_compatible(var_type, init_type))
            {
                semantic_warning(analyzer, node->lineno,
                                 "Type mismatch in initialization: %s = %s",
                                 type_to_string(var_type), type_to_string(init_type));
            }
        }
    }
    else if (declarator->type == AST_DECLARATOR || declarator->type == AST_IDENTIFIER)
    {
        // 仅声明，无初始化
        Symbol *symbol = symbol_create(var_name, var_type, SYMBOL_VARIABLE);
        symbol->declaration = node;

        if (!symbol_table_insert(analyzer->symbol_table, symbol))
        {
            semantic_error(analyzer, node->lineno,
                           "Variable '%s' already declared", var_name);
        }

        // 存储符号信息到 AST 节点
        declarator->semantic_info = (void *)symbol;
    }
}

// 分析语句
void analyze_statement(SemanticAnalyzer *analyzer, ASTNode *node)
{
    if (!node)
        return;

    switch (node->type)
    {
    case AST_DECLARATION:
        analyze_declaration(analyzer, node);
        break;

    case AST_EXPR_STMT:
        if (node->num_children > 0)
        {
            analyze_expression(analyzer, node->children[0]);
        }
        break;

    case AST_COMPOUND_STMT:
        enter_scope(analyzer->symbol_table);
        for (int i = 0; i < node->num_children; i++)
        {
            analyze_statement(analyzer, node->children[i]);
        }
        exit_scope(analyzer->symbol_table);
        break;

    case AST_IF_STMT:
        if (node->num_children > 0)
        {
            analyze_expression(analyzer, node->children[0]); // condition
        }
        if (node->num_children > 1)
        {
            analyze_statement(analyzer, node->children[1]); // then
        }
        if (node->num_children > 2)
        {
            analyze_statement(analyzer, node->children[2]); // else
        }
        break;

    case AST_WHILE_STMT:
        if (node->num_children > 0)
        {
            analyze_expression(analyzer, node->children[0]); // condition
        }
        analyzer->loop_depth++;
        if (node->num_children > 1)
        {
            analyze_statement(analyzer, node->children[1]); // body
        }
        analyzer->loop_depth--;
        break;

    case AST_FOR_STMT:
    {
        // for (init; condition; increment) body
        // C99风格的for循环（带声明）需要新的作用域
        int has_init_decl = (node->num_children > 0 &&
                             node->children[0]->type == AST_DECLARATION);

        if (has_init_decl)
        {
            enter_scope(analyzer->symbol_table);
        }

        // 初始化
        if (node->num_children > 0)
        {
            analyze_statement(analyzer, node->children[0]);
        }
        // 条件
        if (node->num_children > 1 && node->children[1]->num_children > 0)
        {
            analyze_expression(analyzer, node->children[1]->children[0]);
        }
        // 增量 (如果有4个子节点)
        if (node->num_children == 4 && node->children[2])
        {
            analyze_expression(analyzer, node->children[2]);
        }
        // 循环体
        analyzer->loop_depth++;
        int body_index = (node->num_children == 3) ? 2 : 3;
        if (node->num_children >= body_index + 1)
        {
            analyze_statement(analyzer, node->children[body_index]);
        }
        analyzer->loop_depth--;

        // 注意：对于 C99 for 循环，我们不立即退出作用域
        // 因为代码生成阶段还需要访问循环变量的符号信息
        // 作用域会在函数结束时统一清理
        // if (has_init_decl)
        // {
        //     exit_scope(analyzer->symbol_table);
        // }
        break;
    }

    case AST_RETURN_STMT:
        if (node->num_children > 0)
        {
            analyze_expression(analyzer, node->children[0]);
        }
        break;

    case AST_BREAK_STMT:
        if (analyzer->loop_depth == 0)
        {
            semantic_error(analyzer, node->lineno, "break statement outside loop");
        }
        break;

    case AST_CONTINUE_STMT:
        if (analyzer->loop_depth == 0)
        {
            semantic_error(analyzer, node->lineno, "continue statement outside loop");
        }
        break;

    default:
        break;
    }
}

// 分析函数
void analyze_function(SemanticAnalyzer *analyzer, ASTNode *node)
{
    if (!node || node->type != AST_FUNCTION_DEF)
        return;
    if (node->num_children < 3)
        return;

    // 获取返回类型
    TypeInfo *return_type = get_type_from_specifier(node->children[0]);

    // 获取函数名
    ASTNode *declarator = node->children[1];
    const char *func_name = declarator->value.string_val;

    // 创建函数类型
    TypeInfo *func_type = create_function_type(return_type);

    // 创建函数符号
    Symbol *func_symbol = symbol_create(func_name, func_type, SYMBOL_FUNCTION);
    func_symbol->declaration = node;
    func_symbol->is_defined = 1;

    // 插入到全局作用域
    if (!symbol_table_insert(analyzer->symbol_table, func_symbol))
    {
        semantic_error(analyzer, node->lineno, "Function '%s' already declared", func_name);
    }

    // 进入函数作用域
    enter_scope(analyzer->symbol_table);

    // 处理函数参数
    if (declarator->num_children > 0 && declarator->children[0]->type == AST_PARAM_LIST)
    {
        ASTNode *param_list = declarator->children[0];
        for (int i = 0; i < param_list->num_children; i++)
        {
            ASTNode *param = param_list->children[i];
            if (param->type == AST_DECLARATION && param->num_children >= 2)
            {
                TypeInfo *param_type = get_type_from_specifier(param->children[0]);
                ASTNode *param_declarator = param->children[1];
                const char *param_name = param_declarator->value.string_val;

                // 创建参数符号
                Symbol *param_symbol = symbol_create(param_name, param_type, SYMBOL_PARAMETER);
                param_symbol->declaration = param;

                if (!symbol_table_insert(analyzer->symbol_table, param_symbol))
                {
                    semantic_error(analyzer, param->lineno,
                                   "Parameter '%s' already declared", param_name);
                }

                // 存储符号信息到 AST
                param_declarator->semantic_info = (void *)param_symbol;

                // 添加参数类型到函数类型
                add_param_type(func_type, param_type);
            }
        }
    }

    // 分析函数体（compound_statement）
    // 注意：不要退出作用域，直到代码生成完成
    if (node->num_children > 2)
    {
        ASTNode *body = node->children[2];
        // 直接分析 compound_statement 的内容，不进入新作用域
        if (body->type == AST_COMPOUND_STMT)
        {
            for (int i = 0; i < body->num_children; i++)
            {
                analyze_statement(analyzer, body->children[i]);
            }
        }
    }

    // 不要退出作用域 - 让符号在代码生成时可用
    // exit_scope(analyzer->symbol_table);
}

// 分析程序
int analyze_program(SemanticAnalyzer *analyzer, ASTNode *root)
{
    if (!analyzer || !root)
        return 0;

    if (root->type != AST_PROGRAM)
    {
        semantic_error(analyzer, 0, "Root node is not a program");
        return 0;
    }

    // 遍历所有顶层声明
    for (int i = 0; i < root->num_children; i++)
    {
        ASTNode *child = root->children[i];
        if (child->type == AST_FUNCTION_DEF)
        {
            analyze_function(analyzer, child);
        }
        else
        {
            semantic_error(analyzer, child->lineno, "Unexpected top-level declaration");
        }
    }

    return analyzer->error_count == 0;
}
