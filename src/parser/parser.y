%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"

extern int yylex();
extern int yyerror(const char *s);
extern char *yytext;
extern int yylineno;

ASTNode *ast_root = NULL;
%}

%union {
    int int_val;
    float float_val;
    char *string_val;
    ASTNode *node;
}

%token <string_val> IDENTIFIER
%token <int_val> INTEGER_CONSTANT
%token <float_val> FLOATING_CONSTANT
%token <string_val> CHARACTER_CONSTANT STRING_LITERAL

%token INT FLOAT CHAR VOID STRUCT RETURN IF ELSE WHILE FOR BREAK CONTINUE
%token SEMICOLON LBRACE RBRACE COMMA LPAREN RPAREN LBRACKET RBRACKET
%token ASSIGN PLUS MINUS STAR SLASH PERCENT AMPERSAND
%token LT GT LE_OP GE_OP EQ_OP NE_OP
%token AND_OP OR_OP NOT_OP DOT ARROW
%token INC_OP DEC_OP QUESTION COLON

%type <node> program external_declaration function_definition declaration_specifiers
%type <node> declarator compound_statement statement_list statement
%type <node> declaration init_declarator parameter_list parameter_declaration
%type <node> expression_statement selection_statement iteration_statement
%type <node> jump_statement expression assignment_expression conditional_expression
%type <node> logical_or_expression logical_and_expression
%type <node> equality_expression relational_expression
%type <node> additive_expression multiplicative_expression
%type <node> unary_expression postfix_expression primary_expression
%type <node> argument_expression_list
%type <node> initializer initializer_list
%type <node> struct_specifier struct_declaration_list struct_declaration

%start program

%%

program:
    external_declaration {
        ast_root = create_ast_node(AST_PROGRAM, yylineno);
        add_child(ast_root, $1);
        $$ = ast_root;
    }
    | program external_declaration {
        add_child($1, $2);
        $$ = $1;
    }
    ;

external_declaration:
    function_definition { $$ = $1; }
    | struct_specifier SEMICOLON { $$ = $1; }
    ;

function_definition:
    declaration_specifiers declarator compound_statement {
        $$ = create_ast_node(AST_FUNCTION_DEF, yylineno);
        add_child($$, $1);
        add_child($$, $2);
        add_child($$, $3);
    }
    ;

declaration_specifiers:
    INT { $$ = create_string_node("int", yylineno); $$->type = AST_TYPE_SPECIFIER; }
    | FLOAT { $$ = create_string_node("float", yylineno); $$->type = AST_TYPE_SPECIFIER; }
    | CHAR { $$ = create_string_node("char", yylineno); $$->type = AST_TYPE_SPECIFIER; }
    | VOID { $$ = create_string_node("void", yylineno); $$->type = AST_TYPE_SPECIFIER; }
    | struct_specifier { $$ = $1; }
    ;

declarator:
    IDENTIFIER {
        $$ = create_identifier_node($1, yylineno);
        $$->type = AST_DECLARATOR;
        free($1);
    }
    | STAR declarator {
        // 创建一个包装节点来表示指针声明
        ASTNode *ptr_node = create_ast_node(AST_DECLARATOR, yylineno);
        ptr_node->value.int_val = -1;  // 特殊标记：-1表示指针声明
        add_child(ptr_node, $2);  // 子节点是被指向的声明
        $$ = ptr_node;
    }
    | declarator LPAREN RPAREN {
        $$ = $1;
    }
    | declarator LPAREN parameter_list RPAREN {
        $$ = $1;
        add_child($$, $3);  // Add parameter list as child
    }
    | declarator LBRACKET INTEGER_CONSTANT RBRACKET {
        ASTNode *array_decl = create_ast_node(AST_DECLARATOR, yylineno);
        array_decl->value.int_val = $3;  // Store array size
        add_child(array_decl, $1);
        $$ = array_decl;
    }
    | declarator LBRACKET RBRACKET {
        ASTNode *array_decl = create_ast_node(AST_DECLARATOR, yylineno);
        array_decl->value.int_val = -1;  // Unsized array
        add_child(array_decl, $1);
        $$ = array_decl;
    }
    ;

parameter_list:
    parameter_declaration {
        $$ = create_ast_node(AST_PARAM_LIST, yylineno);
        add_child($$, $1);
    }
    | parameter_list COMMA parameter_declaration {
        $$ = $1;
        add_child($$, $3);
    }
    ;

parameter_declaration:
    declaration_specifiers declarator {
        $$ = create_ast_node(AST_DECLARATION, yylineno);
        add_child($$, $1);
        add_child($$, $2);
    }
    | declaration_specifiers IDENTIFIER {
        $$ = create_ast_node(AST_DECLARATION, yylineno);
        add_child($$, $1);
        ASTNode *id = create_identifier_node($2, yylineno);
        id->type = AST_DECLARATOR;
        add_child($$, id);
        free($2);
    }
    ;

declaration:
    declaration_specifiers init_declarator SEMICOLON {
        $$ = create_ast_node(AST_DECLARATION, yylineno);
        add_child($$, $1);
        add_child($$, $2);
    }
    ;

init_declarator:
    declarator {
        $$ = $1;
    }
    | declarator ASSIGN initializer {
        $$ = create_binary_expr_node(OP_ASSIGN, $1, $3, yylineno);
        $$->type = AST_ASSIGN_EXPR;
    }
    ;

initializer:
    assignment_expression { $$ = $1; }
    | LBRACE initializer_list RBRACE { $$ = $2; }
    | LBRACE initializer_list COMMA RBRACE { $$ = $2; }
    ;

initializer_list:
    initializer {
        $$ = create_ast_node(AST_INIT_LIST, yylineno);
        add_child($$, $1);
    }
    | initializer_list COMMA initializer {
        $$ = $1;
        add_child($$, $3);
    }
    ;

compound_statement:
    LBRACE RBRACE {
        $$ = create_ast_node(AST_COMPOUND_STMT, yylineno);
    }
    | LBRACE statement_list RBRACE {
        $$ = $2;
    }
    ;

statement_list:
    statement {
        $$ = create_ast_node(AST_COMPOUND_STMT, yylineno);
        add_child($$, $1);
    }
    | statement_list statement {
        add_child($1, $2);
        $$ = $1;
    }
    ;

statement:
    declaration { $$ = $1; }
    | expression_statement { $$ = $1; }
    | compound_statement { $$ = $1; }
    | selection_statement { $$ = $1; }
    | iteration_statement { $$ = $1; }
    | jump_statement { $$ = $1; }
    ;

expression_statement:
    SEMICOLON {
        $$ = create_ast_node(AST_EXPR_STMT, yylineno);
    }
    | expression SEMICOLON {
        $$ = create_ast_node(AST_EXPR_STMT, yylineno);
        add_child($$, $1);
    }
    ;

selection_statement:
    IF LPAREN expression RPAREN statement {
        $$ = create_ast_node(AST_IF_STMT, yylineno);
        add_child($$, $3);
        add_child($$, $5);
    }
    | IF LPAREN expression RPAREN statement ELSE statement {
        $$ = create_ast_node(AST_IF_STMT, yylineno);
        add_child($$, $3);
        add_child($$, $5);
        add_child($$, $7);
    }
    ;

iteration_statement:
    WHILE LPAREN expression RPAREN statement {
        $$ = create_ast_node(AST_WHILE_STMT, yylineno);
        add_child($$, $3);
        add_child($$, $5);
    }
    | FOR LPAREN expression_statement expression_statement RPAREN statement {
        $$ = create_ast_node(AST_FOR_STMT, yylineno);
        add_child($$, $3);  // init
        add_child($$, $4);  // condition
        add_child($$, $6);  // body
    }
    | FOR LPAREN expression_statement expression_statement expression RPAREN statement {
        $$ = create_ast_node(AST_FOR_STMT, yylineno);
        add_child($$, $3);  // init
        add_child($$, $4);  // condition
        add_child($$, $5);  // increment
        add_child($$, $7);  // body
    }
    | FOR LPAREN declaration expression_statement RPAREN statement {
        $$ = create_ast_node(AST_FOR_STMT, yylineno);
        add_child($$, $3);  // init (declaration)
        add_child($$, $4);  // condition
        add_child($$, $6);  // body
    }
    | FOR LPAREN declaration expression_statement expression RPAREN statement {
        $$ = create_ast_node(AST_FOR_STMT, yylineno);
        add_child($$, $3);  // init (declaration)
        add_child($$, $4);  // condition
        add_child($$, $5);  // increment
        add_child($$, $7);  // body
    }
    ;

jump_statement:
    RETURN SEMICOLON {
        $$ = create_ast_node(AST_RETURN_STMT, yylineno);
    }
    | RETURN expression SEMICOLON {
        $$ = create_ast_node(AST_RETURN_STMT, yylineno);
        add_child($$, $2);
    }
    | BREAK SEMICOLON {
        $$ = create_ast_node(AST_BREAK_STMT, yylineno);
    }
    | CONTINUE SEMICOLON {
        $$ = create_ast_node(AST_CONTINUE_STMT, yylineno);
    }
    ;

expression:
    assignment_expression { $$ = $1; }
    ;

assignment_expression:
    conditional_expression { $$ = $1; }
    | unary_expression ASSIGN assignment_expression {
        $$ = create_binary_expr_node(OP_ASSIGN, $1, $3, yylineno);
        $$->type = AST_ASSIGN_EXPR;
    }
    ;

conditional_expression:
    logical_or_expression { $$ = $1; }
    | logical_or_expression QUESTION expression COLON conditional_expression {
        $$ = create_ast_node(AST_TERNARY_EXPR, yylineno);
        add_child($$, $1);  // condition
        add_child($$, $3);  // true_expr
        add_child($$, $5);  // false_expr
    }
    ;

logical_or_expression:
    logical_and_expression { $$ = $1; }
    | logical_or_expression OR_OP logical_and_expression {
        $$ = create_binary_expr_node(OP_OR, $1, $3, yylineno);
    }
    ;

logical_and_expression:
    equality_expression { $$ = $1; }
    | logical_and_expression AND_OP equality_expression {
        $$ = create_binary_expr_node(OP_AND, $1, $3, yylineno);
    }
    ;

equality_expression:
    relational_expression { $$ = $1; }
    | equality_expression EQ_OP relational_expression {
        $$ = create_binary_expr_node(OP_EQ, $1, $3, yylineno);
    }
    | equality_expression NE_OP relational_expression {
        $$ = create_binary_expr_node(OP_NE, $1, $3, yylineno);
    }
    ;

relational_expression:
    additive_expression { $$ = $1; }
    | relational_expression LT additive_expression {
        $$ = create_binary_expr_node(OP_LT, $1, $3, yylineno);
    }
    | relational_expression GT additive_expression {
        $$ = create_binary_expr_node(OP_GT, $1, $3, yylineno);
    }
    | relational_expression LE_OP additive_expression {
        $$ = create_binary_expr_node(OP_LE, $1, $3, yylineno);
    }
    | relational_expression GE_OP additive_expression {
        $$ = create_binary_expr_node(OP_GE, $1, $3, yylineno);
    }
    ;

additive_expression:
    multiplicative_expression { $$ = $1; }
    | additive_expression PLUS multiplicative_expression {
        $$ = create_binary_expr_node(OP_ADD, $1, $3, yylineno);
    }
    | additive_expression MINUS multiplicative_expression {
        $$ = create_binary_expr_node(OP_SUB, $1, $3, yylineno);
    }
    ;

multiplicative_expression:
    unary_expression { $$ = $1; }
    | multiplicative_expression STAR unary_expression {
        $$ = create_binary_expr_node(OP_MUL, $1, $3, yylineno);
    }
    | multiplicative_expression SLASH unary_expression {
        $$ = create_binary_expr_node(OP_DIV, $1, $3, yylineno);
    }
    | multiplicative_expression PERCENT unary_expression {
        $$ = create_binary_expr_node(OP_MOD, $1, $3, yylineno);
    }
    ;

unary_expression:
    postfix_expression { $$ = $1; }
    | INC_OP unary_expression {
        $$ = create_unary_expr_node(OP_PREINC, $2, yylineno);
    }
    | DEC_OP unary_expression {
        $$ = create_unary_expr_node(OP_PREDEC, $2, yylineno);
    }
    | NOT_OP unary_expression {
        $$ = create_unary_expr_node(OP_NOT, $2, yylineno);
    }
    | AMPERSAND unary_expression {
        $$ = create_unary_expr_node(OP_ADDR, $2, yylineno);
    }
    | STAR unary_expression {
        $$ = create_unary_expr_node(OP_DEREF, $2, yylineno);
    }
    ;

postfix_expression:
    primary_expression { $$ = $1; }
    | postfix_expression LPAREN RPAREN {
        $$ = create_ast_node(AST_CALL_EXPR, yylineno);
        add_child($$, $1);  // function name/expression
    }
    | postfix_expression LPAREN argument_expression_list RPAREN {
        $$ = create_ast_node(AST_CALL_EXPR, yylineno);
        add_child($$, $1);  // function name/expression
        add_child($$, $3);  // argument list
    }
    | postfix_expression LBRACKET expression RBRACKET {
        $$ = create_ast_node(AST_ARRAY_SUBSCRIPT, yylineno);
        add_child($$, $1);  // array expression
        add_child($$, $3);  // subscript expression
    }
    | postfix_expression DOT IDENTIFIER {
        $$ = create_binary_expr_node(OP_MEMBER, $1, create_identifier_node($3, yylineno), yylineno);
        $$->type = AST_MEMBER_ACCESS;
        free($3);
    }
    | postfix_expression ARROW IDENTIFIER {
        $$ = create_binary_expr_node(OP_ARROW, $1, create_identifier_node($3, yylineno), yylineno);
        $$->type = AST_MEMBER_ACCESS;
        free($3);
    }
    | postfix_expression INC_OP {
        $$ = create_unary_expr_node(OP_POSTINC, $1, yylineno);
    }
    | postfix_expression DEC_OP {
        $$ = create_unary_expr_node(OP_POSTDEC, $1, yylineno);
    }
    ;

argument_expression_list:
    assignment_expression {
        $$ = create_ast_node(AST_ARG_LIST, yylineno);
        add_child($$, $1);
    }
    | argument_expression_list COMMA assignment_expression {
        $$ = $1;
        add_child($$, $3);
    }
    ;

primary_expression:
    IDENTIFIER {
        $$ = create_identifier_node($1, yylineno);
        free($1);
    }
    | INTEGER_CONSTANT {
        $$ = create_int_node($1, yylineno);
    }
    | FLOATING_CONSTANT {
        $$ = create_float_node($1, yylineno);
    }
    | STRING_LITERAL {
        $$ = create_string_node($1, yylineno);
        free($1);
    }
    | LPAREN expression RPAREN {
        $$ = $2;
    }
    ;

struct_specifier:
    STRUCT IDENTIFIER LBRACE struct_declaration_list RBRACE {
        $$ = create_ast_node(AST_STRUCT_DEF, yylineno);
        ASTNode *name = create_identifier_node($2, yylineno);
        add_child($$, name);
        add_child($$, $4);
        free($2);
    }
    | STRUCT LBRACE struct_declaration_list RBRACE {
        $$ = create_ast_node(AST_STRUCT_DEF, yylineno);
        add_child($$, $3);
    }
    | STRUCT IDENTIFIER {
        $$ = create_ast_node(AST_STRUCT_DEF, yylineno);
        ASTNode *name = create_identifier_node($2, yylineno);
        add_child($$, name);
        free($2);
    }
    ;

struct_declaration_list:
    struct_declaration {
        $$ = create_ast_node(AST_COMPOUND_STMT, yylineno);
        add_child($$, $1);
    }
    | struct_declaration_list struct_declaration {
        $$ = $1;
        add_child($$, $2);
    }
    ;

struct_declaration:
    declaration_specifiers declarator SEMICOLON {
        $$ = create_ast_node(AST_DECLARATION, yylineno);
        add_child($$, $1);
        add_child($$, $2);
    }
    ;

%%

int yyerror(const char *s) {
    fprintf(stderr, "Parse Error at line %d: %s near '%s'\n", yylineno, s, yytext);
    return 0;
}
