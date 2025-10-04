%{
#include <stdio.h>
%}

%start program
%%
program: /* empty */ ;
%%

int yyerror(const char *s) {
    fprintf(stderr, "Error: %s\n", s);
    return 0;
}

int yylex() {
    return EOF;
}

int main() {
    yyparse();
    return 0;
}
