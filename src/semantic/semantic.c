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
    if (!node)
    {
        return create_type(TYPE_UNKNOWN);
    }

    // 处理基本类型
    if (node->type == AST_TYPE_SPECIFIER)
    {
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
        else if (strcmp(type_str, "short") == 0)
        {
            return create_type(TYPE_SHORT);
        }
        else if (strcmp(type_str, "long") == 0)
        {
            return create_type(TYPE_LONG);
        }
        else if (strcmp(type_str, "double") == 0)
        {
            return create_type(TYPE_DOUBLE);
        }
        else if (strcmp(type_str, "unsigned") == 0)
        {
            return create_type(TYPE_UNSIGNED);
        }
        return create_type(TYPE_UNKNOWN);
    }

    // 处理结构体类型
    if (node->type == AST_STRUCT_DEF)
    {
        TypeInfo *type = create_type(TYPE_STRUCT);

        // 如果有结构体名称（第一个子节点是标识符）
        if (node->num_children > 0 && node->children[0]->type == AST_IDENTIFIER)
        {
            type->struct_name = strdup(node->children[0]->value.string_val);
        }

        // 暂时不解析结构体成员，避免复杂性
        // TODO: 完整实现结构体成员解析
        type->struct_size = 16; // 固定大小
        type->num_members = 0;  // 暂时没有成员信息

        return type;
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
        // 指针算术：pointer + integer 或 integer + pointer
        if (op == OP_ADD)
        {
            // 指针 + 整数
            if (left->pointer_level > 0 && right->base_type == TYPE_INT)
            {
                return left; // 返回指针类型
            }
            // 整数 + 指针
            if (left->base_type == TYPE_INT && right->pointer_level > 0)
            {
                return right; // 返回指针类型
            }
        }

        // 指针算术：pointer - integer 或 pointer - pointer
        if (op == OP_SUB)
        {
            // 指针 - 整数
            if (left->pointer_level > 0 && right->base_type == TYPE_INT)
            {
                return left; // 返回指针类型
            }
            // 指针 - 指针（返回整数，表示元素个数）
            if (left->pointer_level > 0 && right->pointer_level > 0)
            {
                if (!types_compatible(left, right))
                {
                    semantic_warning(analyzer, lineno,
                                     "Subtracting incompatible pointer types");
                }
                return create_type(TYPE_INT); // 返回差值
            }
        }

        // 普通算术运算
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
        // 允许指针比较
        if (left->pointer_level > 0 && right->pointer_level > 0)
        {
            if (!types_compatible(left, right))
            {
                semantic_warning(analyzer, lineno,
                                 "Comparing incompatible pointer types: %s and %s",
                                 type_to_string(left), type_to_string(right));
            }
            return create_type(TYPE_INT); // 比较结果是 int (0 或 1)
        }

        // 允许指针与整数0（NULL）比较
        if ((left->pointer_level > 0 && right->base_type == TYPE_INT) ||
            (left->base_type == TYPE_INT && right->pointer_level > 0))
        {
            return create_type(TYPE_INT);
        }

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

    // 位运算符
    if (op == OP_BIT_AND || op == OP_BIT_OR || op == OP_BIT_XOR ||
        op == OP_LEFT_SHIFT || op == OP_RIGHT_SHIFT)
    {
        // 位运算要求两个操作数都是整数类型
        if (left->base_type != TYPE_INT || left->pointer_level > 0)
        {
            semantic_error(analyzer, lineno,
                           "Left operand of bitwise operation must be integer");
            return create_type(TYPE_UNKNOWN);
        }
        if (right->base_type != TYPE_INT || right->pointer_level > 0)
        {
            semantic_error(analyzer, lineno,
                           "Right operand of bitwise operation must be integer");
            return create_type(TYPE_UNKNOWN);
        }
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
        // 避免递归：直接处理，不调用 analyze_expression
        if (lhs->num_children >= 2)
        {
            ASTNode *struct_node = lhs->children[0];
            if (struct_node->type == AST_IDENTIFIER)
            {
                Symbol *symbol = symbol_table_lookup(analyzer->symbol_table,
                                                     struct_node->value.string_val);
                if (symbol && symbol->type->base_type == TYPE_STRUCT)
                {
                    struct_node->semantic_info = (void *)symbol;
                    lhs_type = create_type(TYPE_INT); // 简化：成员类型为int
                }
                else
                {
                    semantic_error(analyzer, lineno, "Variable is not a struct");
                    return;
                }
            }
        }
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

        TypeInfo *lhs_type = analyze_expression(analyzer, node->children[0]);
        TypeInfo *rhs_type = analyze_expression(analyzer, node->children[1]);

        // 检查是否为复合赋值运算符
        if (node->value.op_type != OP_ASSIGN)
        {
            // 复合赋值运算符：a += b 等价于 a = a + b
            // 先检查操作是否合法
            check_binary_operation(analyzer, node->value.op_type - (OP_ADD_ASSIGN - OP_ADD),
                                   lhs_type, rhs_type, node->lineno);
        }

        check_assignment(analyzer, node->children[0], rhs_type, node->lineno);
        return lhs_type;
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
        else if (node->value.op_type == OP_PREINC || node->value.op_type == OP_PREDEC ||
                 node->value.op_type == OP_POSTINC || node->value.op_type == OP_POSTDEC)
        {
            // 递增递减运算符：检查操作数是否为左值（变量、数组元素等）
            // 返回操作数的类型
            if (operand_type->base_type != TYPE_INT && operand_type->pointer_level == 0)
            {
                semantic_warning(analyzer, node->lineno,
                                 "Increment/decrement on non-integer/pointer type");
            }
            return operand_type;
        }
        else if (node->value.op_type == OP_BIT_NOT)
        {
            // 按位取反：检查操作数是否为整数类型
            if (operand_type->base_type != TYPE_INT && operand_type->pointer_level == 0)
            {
                semantic_error(analyzer, node->lineno,
                               "Bitwise NOT requires integer type");
            }
            return create_type(TYPE_INT);
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
        TypeInfo *result_type = NULL;

        if (array_type->pointer_level > 0)
        {
            result_type = create_type(array_type->base_type);
            result_type->pointer_level = array_type->pointer_level - 1;
            result_type->array_size = array_type->array_size;
        }
        else if (array_type->array_size >= 0 || array_type->array_dimensions > 0)
        {
            // 数组类型，检查是否为多维数组
            if (array_type->array_dimensions > 1)
            {
                // 多维数组：返回降低一维的数组类型
                result_type = create_type(array_type->base_type);
                result_type->pointer_level = array_type->pointer_level;
                result_type->array_dimensions = array_type->array_dimensions - 1;

                // 复制剩余维度的大小
                if (result_type->array_dimensions > 0)
                {
                    result_type->array_sizes = (int *)malloc(result_type->array_dimensions * sizeof(int));
                    for (int i = 0; i < result_type->array_dimensions; i++)
                    {
                        result_type->array_sizes[i] = array_type->array_sizes[i + 1];
                    }
                    // 设置第一维大小（用于代码生成）
                    result_type->array_size = result_type->array_sizes[0];
                }
                else
                {
                    result_type->array_size = -1;
                }
            }
            else
            {
                // 一维数组：返回元素类型
                result_type = create_type(array_type->base_type);
                result_type->pointer_level = array_type->pointer_level;
            }
        }
        else
        {
            semantic_error(analyzer, node->lineno,
                           "Cannot subscript non-array type: %s",
                           type_to_string(array_type));
            return create_type(TYPE_UNKNOWN);
        }

        // 将返回类型附加到节点，供代码生成使用
        node->semantic_info = (void *)result_type;
        return result_type;
    }

    case AST_TERNARY_EXPR:
    {
        // 三元运算符: condition ? true_expr : false_expr
        if (node->num_children < 3)
        {
            semantic_error(analyzer, node->lineno, "Ternary expression missing operands");
            return create_type(TYPE_UNKNOWN);
        }

        TypeInfo *cond_type = analyze_expression(analyzer, node->children[0]);
        TypeInfo *true_type = analyze_expression(analyzer, node->children[1]);
        TypeInfo *false_type = analyze_expression(analyzer, node->children[2]);

        // 条件应该是整数或指针类型
        if (cond_type->base_type != TYPE_INT && cond_type->pointer_level == 0)
        {
            semantic_warning(analyzer, node->lineno,
                             "Ternary condition should be int or pointer type");
        }

        // 返回类型：如果两个分支类型相同，返回该类型；否则返回第一个分支的类型
        if (types_compatible(true_type, false_type))
        {
            return true_type;
        }
        else
        {
            semantic_warning(analyzer, node->lineno,
                             "Type mismatch in ternary branches: %s vs %s",
                             type_to_string(true_type), type_to_string(false_type));
            return true_type;
        }
    }

    case AST_MEMBER_ACCESS:
    {
        if (node->num_children < 2)
        {
            semantic_error(analyzer, node->lineno, "Member access missing operands");
            return create_type(TYPE_UNKNOWN);
        }

        // 简化：暂时跳过完整的类型检查，直接返回int类型
        // 这避免了可能的无限递归
        // TODO: 实现完整的结构体成员类型检查

        ASTNode *struct_node = node->children[0];
        ASTNode *member_node = node->children[1];

        if (member_node->type != AST_IDENTIFIER)
        {
            semantic_error(analyzer, node->lineno, "Member name must be an identifier");
            return create_type(TYPE_UNKNOWN);
        }

        // 对于点运算符，检查左侧是否是标识符（结构体变量）
        if (node->value.op_type == OP_MEMBER && struct_node->type == AST_IDENTIFIER)
        {
            // 查找结构体变量
            Symbol *symbol = symbol_table_lookup(analyzer->symbol_table, struct_node->value.string_val);
            if (!symbol)
            {
                semantic_error(analyzer, node->lineno, "Undeclared variable: %s",
                               struct_node->value.string_val);
                return create_type(TYPE_UNKNOWN);
            }

            // 存储符号信息
            struct_node->semantic_info = (void *)symbol;

            // 检查是否为结构体类型
            if (symbol->type->base_type != TYPE_STRUCT)
            {
                semantic_error(analyzer, node->lineno,
                               "Member access requires struct type");
                return create_type(TYPE_UNKNOWN);
            }

            // 简化：暂时所有成员都返回int类型
            // TODO: 从结构体定义中查找真实的成员类型
            return create_type(TYPE_INT);
        }

        // 对于箭头运算符或其他复杂情况，暂时返回int
        return create_type(TYPE_INT);
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

    // 处理多维数组和指针：遍历嵌套的DECLARATOR节点
    // 收集所有数组维度和指针级别，直到找到变量名
    int *array_dimensions = NULL;
    int num_dimensions = 0;
    int dim_capacity = 4;
    array_dimensions = (int *)malloc(dim_capacity * sizeof(int));

    ASTNode *current = id_node_temp;

    // 遍历DECLARATOR链，收集数组维度和指针级别
    while (current && current->type == AST_DECLARATOR)
    {
        if (current->num_children > 0)
        {
            // 如果int_val == -1，这是一个指针声明
            if (current->value.int_val == -1)
            {
                pointer_level++;
                // 移动到子节点
                current = current->children[0];
            }
            // 如果int_val > 0，这是一个数组维度
            else if (current->value.int_val > 0)
            {
                // 确保有足够空间
                if (num_dimensions >= dim_capacity)
                {
                    dim_capacity *= 2;
                    array_dimensions = (int *)realloc(array_dimensions, dim_capacity * sizeof(int));
                }
                array_dimensions[num_dimensions++] = current->value.int_val;
                // 移动到子节点
                current = current->children[0];
            }
            else
            {
                // 其他情况（函数参数等），继续移动
                current = current->children[0];
            }
        }
        else
        {
            // 没有子节点，检查是否有string_val（变量名）
            if (current->value.string_val)
            {
                var_name = current->value.string_val;
            }
            break;
        }
    }

    // 如果遍历到了IDENTIFIER
    if (current && current->type == AST_IDENTIFIER)
    {
        var_name = current->value.string_val;
    }

    // 先处理指针类型
    if (pointer_level > 0)
    {
        for (int i = 0; i < pointer_level; i++)
        {
            var_type = create_pointer_type(var_type);
        }
    }

    // 然后处理多维数组（如果有）
    if (num_dimensions > 0)
    {
        // 从最内层开始构建类型（反向遍历维度）
        for (int i = num_dimensions - 1; i >= 0; i--)
        {
            var_type = create_array_type(var_type, array_dimensions[i]);
        }

        // 存储多维数组信息到类型中
        var_type->array_dimensions = num_dimensions;
        var_type->array_sizes = array_dimensions; // 转移所有权
        array_dimensions = NULL;                  // 防止下面free
    }

    if (array_dimensions)
    {
        free(array_dimensions);
    }

    // 注意：多维数组的类型已经在上面创建了
    // 这里不需要再创建 array_size（它是旧的单维数组逻辑）

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

    case AST_DO_WHILE_STMT:
        // do body while (condition);
        analyzer->loop_depth++;
        if (node->num_children > 0)
        {
            analyze_statement(analyzer, node->children[0]); // body
        }
        analyzer->loop_depth--;
        if (node->num_children > 1)
        {
            analyze_expression(analyzer, node->children[1]); // condition
        }
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

    case AST_SWITCH_STMT:
    {
        if (node->num_children >= 1)
        {
            TypeInfo *switch_type = analyze_expression(analyzer, node->children[0]);
            if (switch_type->base_type != TYPE_INT && switch_type->base_type != TYPE_CHAR)
            {
                semantic_warning(analyzer, node->lineno,
                                 "Switch expression should be integer type");
            }

            analyzer->loop_depth++;
            if (node->num_children >= 2)
            {
                analyze_statement(analyzer, node->children[1]);
            }
            analyzer->loop_depth--;
        }
        break;
    }

    case AST_CASE_STMT:
    {
        if (node->num_children >= 1)
        {
            analyze_expression(analyzer, node->children[0]);
        }
        if (node->num_children >= 2)
        {
            analyze_statement(analyzer, node->children[1]);
        }
        break;
    }

    case AST_DEFAULT_STMT:
    {
        if (node->num_children >= 1)
        {
            analyze_statement(analyzer, node->children[0]);
        }
        break;
    }

    case AST_BREAK_STMT:
        if (analyzer->loop_depth == 0)
        {
            semantic_error(analyzer, node->lineno, "break statement outside loop or switch");
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
        else if (child->type == AST_STRUCT_DEF)
        {
            // 结构体定义 - 暂时跳过，不进行语义分析
            // TODO: 实现完整的结构体类型检查
        }
        else
        {
            semantic_error(analyzer, child->lineno, "Unexpected top-level declaration");
        }
    }

    return analyzer->error_count == 0;
}
