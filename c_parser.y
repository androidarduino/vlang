%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern int yylex();
extern int yyerror(const char *s);
extern char *yytext;

%}

%union {
    int int_val;
    float float_val;
    char *string_val;
};

%token AUTO BREAK CASE CHAR CONST CONTINUE DEFAULT DO DOUBLE ELSE ENUM EXTERN FLOAT FOR GOTO IF INLINE INT LONG REGISTER RESTRICT RETURN SHORT SIGNED SIZEOF STATIC STRUCT SWITCH TYPEDEF UNION UNSIGNED VOID VOLATILE WHILE BOOL COMPLEX IMAGINARY
%token IDENTIFIER INTEGER_CONSTANT FLOATING_CONSTANT CHARACTER_CONSTANT STRING_LITERAL
%token ELLIPSIS RIGHT_ASSIGN LEFT_ASSIGN ADD_ASSIGN SUB_ASSIGN MUL_ASSIGN DIV_ASSIGN MOD_ASSIGN AND_ASSIGN XOR_ASSIGN OR_ASSIGN RIGHT_OP LEFT_OP INC_OP DEC_OP PTR_OP AND_OP OR_OP LE_OP GE_OP EQ_OP NE_OP
%token SEMICOLON LBRACE RBRACE COMMA COLON ASSIGN LPAREN RPAREN LBRACKET RBRACKET DOT AMPERSAND EXCLAMATION TILDE MINUS PLUS STAR SLASH PERCENT LT GT CARET PIPE QUESTION

%type <int_val> INTEGER_CONSTANT
%type <float_val> FLOATING_CONSTANT
%type <string_val> IDENTIFIER CHARACTER_CONSTANT STRING_LITERAL

%expect 1

%start program

%%

program:
    external_declaration_list
    ;

external_declaration_list:
    external_declaration_list external_declaration
    | external_declaration
    ;

external_declaration:
    function_definition
    | declaration
    ;

function_definition:
    declaration_specifiers declarator compound_statement
    ;

declaration_specifiers:
    type_specifier
    | type_specifier declaration_specifiers
    | type_qualifier
    | type_qualifier declaration_specifiers
    ;

type_specifier:
    VOID | CHAR | SHORT | INT | LONG | FLOAT | DOUBLE | SIGNED | UNSIGNED | BOOL | COMPLEX | IMAGINARY
    | STRUCT IDENTIFIER
    | ENUM IDENTIFIER
    ;

declarator:
    direct_declarator
    | pointer direct_declarator
    ;

direct_declarator:
    IDENTIFIER
    | LPAREN declarator RPAREN
    | direct_declarator LPAREN parameter_type_list RPAREN
    | direct_declarator LPAREN RPAREN
    | direct_declarator LBRACKET assignment_expression RBRACKET
    | direct_declarator LBRACKET RBRACKET
    ;

parameter_type_list:
    parameter_list
    | parameter_list COMMA ELLIPSIS
    ;

parameter_list:
    parameter_declaration
    | parameter_list COMMA parameter_declaration
    ;

parameter_declaration:
    declaration_specifiers declarator
    | declaration_specifiers
    | declaration_specifiers abstract_declarator
    ;

abstract_declarator:
    pointer
    | pointer direct_abstract_declarator
    | direct_abstract_declarator
    ;

pointer:
    STAR type_qualifier_list
    | STAR
    | STAR type_qualifier_list pointer
    | STAR pointer
    ;

type_qualifier_list:
    type_qualifier
    | type_qualifier_list type_qualifier
    ;

direct_abstract_declarator:
    LPAREN abstract_declarator RPAREN
    | LPAREN RPAREN
    | LBRACKET RBRACKET
    | LBRACKET assignment_expression RBRACKET
    | direct_abstract_declarator LPAREN parameter_type_list RPAREN
    | direct_abstract_declarator LPAREN RPAREN
    | direct_abstract_declarator LBRACKET assignment_expression RBRACKET
    | direct_abstract_declarator LBRACKET RBRACKET
    ;

declaration:
    declaration_specifiers init_declarator_list SEMICOLON
    ;

init_declarator_list:
    init_declarator
    | init_declarator_list COMMA init_declarator
    ;

init_declarator:
    declarator
    | declarator ASSIGN initializer
    ;

initializer:
    assignment_expression
    | LBRACE initializer_list RBRACE
    | LBRACE initializer_list COMMA RBRACE
    ;

initializer_list:
    initializer
    | initializer_list COMMA initializer
    ;

statement:
    compound_statement
    | expression_statement
    | selection_statement
    | iteration_statement
    | jump_statement
    ;

compound_statement:
    LBRACE block_item_list RBRACE
    | LBRACE RBRACE
    ;

block_item_list:
    block_item_list block_item
    | block_item
    ;

block_item:
    declaration
    | statement
    ;

expression_statement:
    expression SEMICOLON
    | SEMICOLON
    ;

selection_statement:
    IF LPAREN expression RPAREN statement
    | IF LPAREN expression RPAREN statement ELSE statement
    | SWITCH LPAREN expression RPAREN statement
    ;

iteration_statement:
    WHILE LPAREN expression RPAREN statement
    | DO statement WHILE LPAREN expression RPAREN SEMICOLON
    | FOR LPAREN expression_statement expression_statement RPAREN statement
    | FOR LPAREN expression_statement expression_statement expression RPAREN statement
    | FOR LPAREN declaration expression_statement RPAREN statement
    | FOR LPAREN declaration expression_statement expression RPAREN statement
    ;

jump_statement:
    GOTO IDENTIFIER SEMICOLON
    | CONTINUE SEMICOLON
    | BREAK SEMICOLON
    | RETURN expression SEMICOLON
    | RETURN SEMICOLON
    ;

expression:
    assignment_expression
    | expression COMMA assignment_expression
    ;

assignment_expression:
    conditional_expression
    | unary_expression assignment_operator assignment_expression
    ;

assignment_operator:
    ASSIGN | MUL_ASSIGN | DIV_ASSIGN | MOD_ASSIGN | ADD_ASSIGN | SUB_ASSIGN
    | LEFT_ASSIGN | RIGHT_ASSIGN | AND_ASSIGN | XOR_ASSIGN | OR_ASSIGN
    ;

conditional_expression:
    logical_or_expression
    | logical_or_expression QUESTION expression COLON conditional_expression
    ;

logical_or_expression:
    logical_and_expression
    | logical_or_expression OR_OP logical_and_expression
    ;

logical_and_expression:
    inclusive_or_expression
    | logical_and_expression AND_OP inclusive_or_expression
    ;

inclusive_or_expression:
    exclusive_or_expression
    | inclusive_or_expression PIPE exclusive_or_expression
    ;

exclusive_or_expression:
    and_expression
    | exclusive_or_expression CARET and_expression
    ;

and_expression:
    equality_expression
    | and_expression AMPERSAND equality_expression
    ;

equality_expression:
    relational_expression
    | equality_expression EQ_OP relational_expression
    | equality_expression NE_OP relational_expression
    ;

relational_expression:
    shift_expression
    | relational_expression LT shift_expression
    | relational_expression GT shift_expression
    | relational_expression LE_OP shift_expression
    | relational_expression GE_OP shift_expression
    ;

shift_expression:
    additive_expression
    | shift_expression LEFT_OP additive_expression
    | shift_expression RIGHT_OP additive_expression
    ;

additive_expression:
    multiplicative_expression
    | additive_expression PLUS multiplicative_expression
    | additive_expression MINUS multiplicative_expression
    ;

multiplicative_expression:
    cast_expression
    | multiplicative_expression STAR cast_expression
    | multiplicative_expression SLASH cast_expression
    | multiplicative_expression PERCENT cast_expression
    ;

cast_expression:
    unary_expression
    | LPAREN type_name RPAREN cast_expression
    ;

unary_expression:
    postfix_expression
    | INC_OP unary_expression
    | DEC_OP unary_expression
    | unary_operator cast_expression
    | SIZEOF unary_expression
    | SIZEOF LPAREN type_name RPAREN
    ;

unary_operator:
    AMPERSAND | STAR | PLUS | MINUS | TILDE | EXCLAMATION
    ;

postfix_expression:
    primary_expression
    | postfix_expression LBRACKET expression RBRACKET
    | postfix_expression LPAREN RPAREN
    | postfix_expression LPAREN argument_expression_list RPAREN
    | postfix_expression DOT IDENTIFIER
    | postfix_expression PTR_OP IDENTIFIER
    | postfix_expression INC_OP
    | postfix_expression DEC_OP
    ;

primary_expression:
    IDENTIFIER
    | INTEGER_CONSTANT
    | FLOATING_CONSTANT
    | CHARACTER_CONSTANT
    | STRING_LITERAL
    | LPAREN expression RPAREN
    ;

argument_expression_list:
    assignment_expression
    | argument_expression_list COMMA assignment_expression
    ;

type_name:
    specifier_qualifier_list
    | specifier_qualifier_list abstract_declarator
;

specifier_qualifier_list:
    type_specifier specifier_qualifier_list
    | type_specifier
    | type_qualifier specifier_qualifier_list
    | type_qualifier
    ;

type_qualifier:
    CONST | RESTRICT | VOLATILE
    ;

%%

int yyerror(const char *s) {
    fprintf(stderr, "Error: %s at token \"%s\"\n", s, yytext);
    return 0;
}

int main(int argc, char **argv) {
    if (yyparse() == 0) {
        printf("Parsing successful!\n");
    } else {
        printf("Parsing failed.\n");
    }
    return 0;
}
