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

%token INT FLOAT CHAR VOID RETURN IF ELSE WHILE FOR
%token SEMICOLON LBRACE RBRACE COMMA LPAREN RPAREN LBRACKET RBRACKET
%token ASSIGN PLUS MINUS STAR SLASH PERCENT
%token LT GT LE_OP GE_OP EQ_OP NE_OP

%type <node> program function_definition declaration_specifiers
%type <node> declarator compound_statement statement_list statement
%type <node> declaration init_declarator
%type <node> expression_statement selection_statement iteration_statement
%type <node> jump_statement expression assignment_expression
%type <node> equality_expression relational_expression
%type <node> additive_expression multiplicative_expression
%type <node> primary_expression

%start program

%%

program:
    function_definition {
        ast_root = create_ast_node(AST_PROGRAM, yylineno);
        add_child(ast_root, $1);
        $$ = ast_root;
    }
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
    ;

declarator:
    IDENTIFIER {
        $$ = create_identifier_node($1, yylineno);
        $$->type = AST_DECLARATOR;
        free($1);
    }
    | declarator LPAREN RPAREN {
        $$ = $1;
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
    | declarator ASSIGN expression {
        $$ = create_binary_expr_node(OP_ASSIGN, $1, $3, yylineno);
        $$->type = AST_ASSIGN_EXPR;
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
    ;

jump_statement:
    RETURN SEMICOLON {
        $$ = create_ast_node(AST_RETURN_STMT, yylineno);
    }
    | RETURN expression SEMICOLON {
        $$ = create_ast_node(AST_RETURN_STMT, yylineno);
        add_child($$, $2);
    }
    ;

expression:
    assignment_expression { $$ = $1; }
    ;

assignment_expression:
    equality_expression { $$ = $1; }
    | IDENTIFIER ASSIGN assignment_expression {
        ASTNode *id = create_identifier_node($1, yylineno);
        $$ = create_binary_expr_node(OP_ASSIGN, id, $3, yylineno);
        $$->type = AST_ASSIGN_EXPR;
        free($1);
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
    primary_expression { $$ = $1; }
    | multiplicative_expression STAR primary_expression {
        $$ = create_binary_expr_node(OP_MUL, $1, $3, yylineno);
    }
    | multiplicative_expression SLASH primary_expression {
        $$ = create_binary_expr_node(OP_DIV, $1, $3, yylineno);
    }
    | multiplicative_expression PERCENT primary_expression {
        $$ = create_binary_expr_node(OP_MOD, $1, $3, yylineno);
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

%%

int yyerror(const char *s) {
    fprintf(stderr, "Parse Error at line %d: %s near '%s'\n", yylineno, s, yytext);
    return 0;
}
