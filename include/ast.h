#ifndef AST_H
#define AST_H

#include <stdlib.h>

// AST 节点类型
typedef enum
{
    AST_PROGRAM,
    AST_FUNCTION_DEF,
    AST_DECLARATION,
    AST_COMPOUND_STMT,
    AST_EXPR_STMT,
    AST_IF_STMT,
    AST_WHILE_STMT,
    AST_FOR_STMT,
    AST_RETURN_STMT,
    AST_BREAK_STMT,
    AST_CONTINUE_STMT,
    AST_BINARY_EXPR,
    AST_UNARY_EXPR,
    AST_ASSIGN_EXPR,
    AST_CALL_EXPR,
    AST_IDENTIFIER,
    AST_INT_LITERAL,
    AST_FLOAT_LITERAL,
    AST_STRING_LITERAL,
    AST_CHAR_LITERAL,
    AST_TYPE_SPECIFIER,
    AST_DECLARATOR,
    AST_PARAM_LIST,
    AST_ARG_LIST,
    AST_ARRAY_SUBSCRIPT, // 数组下标访问 a[i]
    AST_INIT_LIST,       // 初始化列表 {1, 2, 3}
    AST_STRUCT_DEF,      // 结构体定义
    AST_MEMBER_ACCESS,   // 成员访问 a.b 或 a->b
    AST_TERNARY_EXPR     // 三元运算符 a ? b : c
} ASTNodeType;

// 运算符类型
typedef enum
{
    OP_ADD,
    OP_SUB,
    OP_MUL,
    OP_DIV,
    OP_MOD,
    OP_LT,
    OP_GT,
    OP_LE,
    OP_GE,
    OP_EQ,
    OP_NE,
    OP_AND,
    OP_OR,
    OP_ASSIGN,
    OP_NEG,
    OP_NOT,
    OP_ADDR,    // 取地址 &
    OP_DEREF,   // 解引用 *
    OP_MEMBER,  // 成员访问 .
    OP_ARROW,   // 指针成员访问 ->
    OP_PREINC,  // 前缀递增 ++i
    OP_PREDEC,  // 前缀递减 --i
    OP_POSTINC, // 后缀递增 i++
    OP_POSTDEC  // 后缀递减 i--
} OperatorType;

// 前向声明
struct ASTNode;

// AST 节点结构
typedef struct ASTNode
{
    ASTNodeType type;
    int lineno;

    // 子节点
    struct ASTNode **children;
    int num_children;
    int children_capacity;

    // 节点特定数据
    union
    {
        int int_val;
        float float_val;
        char *string_val;
        OperatorType op_type;
    } value;

    // 语义信息（类型检查等）
    void *semantic_info;
} ASTNode;

// 函数声明
ASTNode *create_ast_node(ASTNodeType type, int lineno);
ASTNode *create_int_node(int value, int lineno);
ASTNode *create_float_node(float value, int lineno);
ASTNode *create_string_node(const char *value, int lineno);
ASTNode *create_identifier_node(const char *name, int lineno);
ASTNode *create_binary_expr_node(OperatorType op, ASTNode *left, ASTNode *right, int lineno);
ASTNode *create_unary_expr_node(OperatorType op, ASTNode *operand, int lineno);

void add_child(ASTNode *parent, ASTNode *child);
void print_ast(ASTNode *node, int indent);
void free_ast(ASTNode *node);

#endif // AST_H
