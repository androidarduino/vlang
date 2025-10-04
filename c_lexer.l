%{
#include <stdio.h>
#include "y.tab.h" // Assuming you will generate y.tab.h from a yacc/bison grammar
%}

%%

"auto"          return AUTO;
"break"         return BREAK;
"case"          return CASE;
"char"          return CHAR;
"const"         return CONST;
"continue"      return CONTINUE;
"default"       return DEFAULT;
"do"            return DO;
"double"        return DOUBLE;
"else"          return ELSE;
"enum"          return ENUM;
"extern"        return EXTERN;
"float"         return FLOAT;
"for"           return FOR;
"goto"          return GOTO;
"if"            return IF;
"inline"        return INLINE;
"int"           return INT;
"long"          return LONG;
"register"      return REGISTER;
"restrict"      return RESTRICT;
"return"        return RETURN;
"short"         return SHORT;
"signed"        return SIGNED;
"sizeof"        return SIZEOF;
"static"        return STATIC;
"struct"        return STRUCT;
"switch"        return SWITCH;
"typedef"       return TYPEDEF;
"union"         return UNION;
"unsigned"      return UNSIGNED;
"void"          return VOID;
"volatile"      return VOLATILE;
"while"         return WHILE;
"_Bool"         return BOOL;
"_Complex"      return COMPLEX;
"_Imaginary"    return IMAGINARY;

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
\.[0-9]+([Ee][+-]?[0-9]+)?[fFlL]?         {
                                            yylval.float_val = atof(yytext);
                                            return FLOATING_CONSTANT;
                                            }
[0-9]+[Ee][+-]?[0-9]+[fFlL]?              {
                                            yylval.float_val = atof(yytext);
                                            return FLOATING_CONSTANT;
                                            }

'([^\\']|\\.)+'   {
                    yylval.string_val = strdup(yytext);
                    return CHARACTER_CONSTANT;
                    }
\"([^\\"]|\\.)*\" {
                    yylval.string_val = strdup(yytext);
                    return STRING_LITERAL;
                    }

"..."           return ELLIPSIS;
">>="           return RIGHT_ASSIGN;
"<<="           return LEFT_ASSIGN;
"+="            return ADD_ASSIGN;
"-="            return SUB_ASSIGN;
"*="            return MUL_ASSIGN;
"/="            return DIV_ASSIGN;
"%="            return MOD_ASSIGN;
"&="            return AND_ASSIGN;
"^="            return XOR_ASSIGN;
"|="            return OR_ASSIGN;
">>"            return RIGHT_OP;
"<<"            return LEFT_OP;
"++"            return INC_OP;
"--"            return DEC_OP;
"->"            return PTR_OP;
"&&"            return AND_OP;
"||"            return OR_OP;
"<="            return LE_OP;
">="            return GE_OP;
"=="            return EQ_OP;
"!="            return NE_OP;
";"             return SEMICOLON;
("{"|"<%")      return LBRACE;
("}"|"%>")      return RBRACE;
","             return COMMA;
":"             return COLON;
"="             return ASSIGN;
"("             return LPAREN;
")"             return RPAREN;
"["             return LBRACKET;
"]"             return RBRACKET;
"."             return DOT;
"&"             return AMPERSAND;
"!"             return EXCLAMATION;
"~"             return TILDE;
"-"             return MINUS;
"+"             return PLUS;
"*"             return STAR;
"/"             return SLASH;
"%"             return PERCENT;
"<"             return LT;
">"             return GT;
"^"             return CARET;
"|"             return PIPE;
"?"             return QUESTION;

"//".*          /* C++ style comments, ignore */
"/*"([^*]|\*+[^*/])*\*+"/" /* C style comments, ignore */

[ \t\n]+        /* Whitespace, ignore */

.               { fprintf(stderr, "Unknown character: %s\n", yytext); return yytext[0]; }

%%

int yywrap(void) {
    return 1;
}
