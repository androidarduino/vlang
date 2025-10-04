#include "ast.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INITIAL_CHILDREN_CAPACITY 4

// 创建通用 AST 节点
ASTNode *create_ast_node(ASTNodeType type, int lineno)
{
    ASTNode *node = (ASTNode *)malloc(sizeof(ASTNode));
    if (!node)
    {
        fprintf(stderr, "Error: Failed to allocate memory for AST node\n");
        exit(1);
    }

    node->type = type;
    node->lineno = lineno;
    node->children = NULL;
    node->num_children = 0;
    node->children_capacity = 0;
    node->semantic_info = NULL;
    memset(&node->value, 0, sizeof(node->value));

    return node;
}

// 创建整数字面量节点
ASTNode *create_int_node(int value, int lineno)
{
    ASTNode *node = create_ast_node(AST_INT_LITERAL, lineno);
    node->value.int_val = value;
    return node;
}

// 创建浮点数字面量节点
ASTNode *create_float_node(float value, int lineno)
{
    ASTNode *node = create_ast_node(AST_FLOAT_LITERAL, lineno);
    node->value.float_val = value;
    return node;
}

// 创建字符串字面量节点
ASTNode *create_string_node(const char *value, int lineno)
{
    ASTNode *node = create_ast_node(AST_STRING_LITERAL, lineno);
    node->value.string_val = strdup(value);
    return node;
}

// 创建标识符节点
ASTNode *create_identifier_node(const char *name, int lineno)
{
    ASTNode *node = create_ast_node(AST_IDENTIFIER, lineno);
    node->value.string_val = strdup(name);
    return node;
}

// 创建二元表达式节点
ASTNode *create_binary_expr_node(OperatorType op, ASTNode *left, ASTNode *right, int lineno)
{
    ASTNode *node = create_ast_node(AST_BINARY_EXPR, lineno);
    node->value.op_type = op;
    add_child(node, left);
    add_child(node, right);
    return node;
}

// 创建一元表达式节点
ASTNode *create_unary_expr_node(OperatorType op, ASTNode *operand, int lineno)
{
    ASTNode *node = create_ast_node(AST_UNARY_EXPR, lineno);
    node->value.op_type = op;
    add_child(node, operand);
    return node;
}

// 添加子节点
void add_child(ASTNode *parent, ASTNode *child)
{
    if (!parent || !child)
        return;

    if (parent->num_children >= parent->children_capacity)
    {
        int new_capacity = parent->children_capacity == 0 ? INITIAL_CHILDREN_CAPACITY : parent->children_capacity * 2;
        ASTNode **new_children = (ASTNode **)realloc(
            parent->children,
            new_capacity * sizeof(ASTNode *));
        if (!new_children)
        {
            fprintf(stderr, "Error: Failed to reallocate memory for children\n");
            exit(1);
        }
        parent->children = new_children;
        parent->children_capacity = new_capacity;
    }

    parent->children[parent->num_children++] = child;
}

// 打印 AST（用于调试）
static const char *node_type_to_string(ASTNodeType type)
{
    switch (type)
    {
    case AST_PROGRAM:
        return "PROGRAM";
    case AST_FUNCTION_DEF:
        return "FUNCTION_DEF";
    case AST_DECLARATION:
        return "DECLARATION";
    case AST_COMPOUND_STMT:
        return "COMPOUND_STMT";
    case AST_EXPR_STMT:
        return "EXPR_STMT";
    case AST_IF_STMT:
        return "IF_STMT";
    case AST_WHILE_STMT:
        return "WHILE_STMT";
    case AST_FOR_STMT:
        return "FOR_STMT";
    case AST_RETURN_STMT:
        return "RETURN_STMT";
    case AST_BREAK_STMT:
        return "BREAK_STMT";
    case AST_CONTINUE_STMT:
        return "CONTINUE_STMT";
    case AST_BINARY_EXPR:
        return "BINARY_EXPR";
    case AST_UNARY_EXPR:
        return "UNARY_EXPR";
    case AST_ASSIGN_EXPR:
        return "ASSIGN_EXPR";
    case AST_CALL_EXPR:
        return "CALL_EXPR";
    case AST_IDENTIFIER:
        return "IDENTIFIER";
    case AST_INT_LITERAL:
        return "INT_LITERAL";
    case AST_FLOAT_LITERAL:
        return "FLOAT_LITERAL";
    case AST_STRING_LITERAL:
        return "STRING_LITERAL";
    case AST_CHAR_LITERAL:
        return "CHAR_LITERAL";
    case AST_TYPE_SPECIFIER:
        return "TYPE_SPECIFIER";
    case AST_DECLARATOR:
        return "DECLARATOR";
    case AST_PARAM_LIST:
        return "PARAM_LIST";
    case AST_ARG_LIST:
        return "ARG_LIST";
    case AST_ARRAY_SUBSCRIPT:
        return "ARRAY_SUBSCRIPT";
    case AST_INIT_LIST:
        return "INIT_LIST";
    case AST_STRUCT_DEF:
        return "STRUCT_DEF";
    case AST_MEMBER_ACCESS:
        return "MEMBER_ACCESS";
    default:
        return "UNKNOWN";
    }
}

static const char *op_type_to_string(OperatorType op)
{
    switch (op)
    {
    case OP_ADD:
        return "+";
    case OP_SUB:
        return "-";
    case OP_MUL:
        return "*";
    case OP_DIV:
        return "/";
    case OP_MOD:
        return "%";
    case OP_LT:
        return "<";
    case OP_GT:
        return ">";
    case OP_LE:
        return "<=";
    case OP_GE:
        return ">=";
    case OP_EQ:
        return "==";
    case OP_NE:
        return "!=";
    case OP_AND:
        return "&&";
    case OP_OR:
        return "||";
    case OP_ASSIGN:
        return "=";
    case OP_NEG:
        return "-";
    case OP_NOT:
        return "!";
    case OP_ADDR:
        return "&";
    case OP_DEREF:
        return "*";
    case OP_MEMBER:
        return ".";
    case OP_ARROW:
        return "->";
    default:
        return "?";
    }
}

void print_ast(ASTNode *node, int indent)
{
    if (!node)
        return;

    for (int i = 0; i < indent; i++)
        printf("  ");

    printf("%s", node_type_to_string(node->type));

    switch (node->type)
    {
    case AST_INT_LITERAL:
        printf(": %d", node->value.int_val);
        break;
    case AST_FLOAT_LITERAL:
        printf(": %f", node->value.float_val);
        break;
    case AST_IDENTIFIER:
    case AST_STRING_LITERAL:
    case AST_CHAR_LITERAL:
        if (node->value.string_val)
            printf(": %s", node->value.string_val);
        break;
    case AST_BINARY_EXPR:
    case AST_UNARY_EXPR:
        printf(": %s", op_type_to_string(node->value.op_type));
        break;
    default:
        break;
    }

    printf("\n");

    for (int i = 0; i < node->num_children; i++)
    {
        print_ast(node->children[i], indent + 1);
    }
}

// 释放 AST
void free_ast(ASTNode *node)
{
    if (!node)
        return;

    for (int i = 0; i < node->num_children; i++)
    {
        free_ast(node->children[i]);
    }

    if (node->children)
    {
        free(node->children);
    }

    if ((node->type == AST_IDENTIFIER ||
         node->type == AST_STRING_LITERAL ||
         node->type == AST_CHAR_LITERAL) &&
        node->value.string_val)
    {
        free(node->value.string_val);
    }

    free(node);
}
