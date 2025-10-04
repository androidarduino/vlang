%{
#include <stdio.h>
#include <string.h>
#include "ast.h"
#include "y.tab.h"

extern int yylineno;
%}

%option yylineno

%%

"int"           return INT;
"float"         return FLOAT;
"char"          return CHAR;
"void"          return VOID;
"short"         return SHORT;
"long"          return LONG;
"double"        return DOUBLE;
"unsigned"      return UNSIGNED;
"struct"        return STRUCT;
"static"        return STATIC;
"return"        return RETURN;
"if"            return IF;
"else"          return ELSE;
"while"         return WHILE;
"do"            return DO;
"for"           return FOR;
"switch"        return SWITCH;
"case"          return CASE;
"default"       return DEFAULT;
"break"         return BREAK;
"continue"      return CONTINUE;

[a-zA-Z_][a-zA-Z0-9_]*  {
                        yylval.string_val = strdup(yytext);
                        return IDENTIFIER;
                        }

0|[1-9][0-9]*   {
                yylval.int_val = atoi(yytext);
                return INTEGER_CONSTANT;
                }

[0-9]+\.[0-9]*([Ee][+-]?[0-9]+)?[fFlL]?   {
                                            yylval.float_val = atof(yytext);
                                            return FLOATING_CONSTANT;
                                            }

\"([^\\"]|\\.)*\" {
                    yylval.string_val = strdup(yytext);
                    return STRING_LITERAL;
                    }

";"             return SEMICOLON;
"{"             return LBRACE;
"}"             return RBRACE;
","             return COMMA;
"("             return LPAREN;
")"             return RPAREN;
"["             return LBRACKET;
"]"             return RBRACKET;

"="             return ASSIGN;
"+="            return ADD_ASSIGN;
"-="            return SUB_ASSIGN;
"*="            return MUL_ASSIGN;
"/="            return DIV_ASSIGN;
"%="            return MOD_ASSIGN;
"&="            return AND_ASSIGN;
"|="            return OR_ASSIGN;
"^="            return XOR_ASSIGN;
"<<="           return LEFT_ASSIGN;
">>="           return RIGHT_ASSIGN;
"<<"            return LEFT_SHIFT;
">>"            return RIGHT_SHIFT;
"+"             return PLUS;
"-"             return MINUS;
"*"             return STAR;
"/"             return SLASH;
"%"             return PERCENT;
"++"            return INC_OP;
"--"            return DEC_OP;

"<"             return LT;
">"             return GT;
"<="            return LE_OP;
">="            return GE_OP;
"=="            return EQ_OP;
"!="            return NE_OP;

"&&"            return AND_OP;
"||"            return OR_OP;
"!"             return NOT_OP;
"&"             return AMPERSAND;
"|"             return PIPE;
"^"             return XOR;
"~"             return TILDE;
"->"            return ARROW;
"."             return DOT;
"?"             return QUESTION;
":"             return COLON;

[ \t\n]+        /* Whitespace, ignore */

.               { fprintf(stderr, "Unknown character: %s\n", yytext); }

%%

int yywrap(void) {
    return 1;
}
