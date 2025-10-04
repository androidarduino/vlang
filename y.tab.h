/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    AUTO = 258,                    /* AUTO  */
    BREAK = 259,                   /* BREAK  */
    CASE = 260,                    /* CASE  */
    CHAR = 261,                    /* CHAR  */
    CONST = 262,                   /* CONST  */
    CONTINUE = 263,                /* CONTINUE  */
    DEFAULT = 264,                 /* DEFAULT  */
    DO = 265,                      /* DO  */
    DOUBLE = 266,                  /* DOUBLE  */
    ELSE = 267,                    /* ELSE  */
    ENUM = 268,                    /* ENUM  */
    EXTERN = 269,                  /* EXTERN  */
    FLOAT = 270,                   /* FLOAT  */
    FOR = 271,                     /* FOR  */
    GOTO = 272,                    /* GOTO  */
    IF = 273,                      /* IF  */
    INLINE = 274,                  /* INLINE  */
    INT = 275,                     /* INT  */
    LONG = 276,                    /* LONG  */
    REGISTER = 277,                /* REGISTER  */
    RESTRICT = 278,                /* RESTRICT  */
    RETURN = 279,                  /* RETURN  */
    SHORT = 280,                   /* SHORT  */
    SIGNED = 281,                  /* SIGNED  */
    SIZEOF = 282,                  /* SIZEOF  */
    STATIC = 283,                  /* STATIC  */
    STRUCT = 284,                  /* STRUCT  */
    SWITCH = 285,                  /* SWITCH  */
    TYPEDEF = 286,                 /* TYPEDEF  */
    UNION = 287,                   /* UNION  */
    UNSIGNED = 288,                /* UNSIGNED  */
    VOID = 289,                    /* VOID  */
    VOLATILE = 290,                /* VOLATILE  */
    WHILE = 291,                   /* WHILE  */
    BOOL = 292,                    /* BOOL  */
    COMPLEX = 293,                 /* COMPLEX  */
    IMAGINARY = 294,               /* IMAGINARY  */
    IDENTIFIER = 295,              /* IDENTIFIER  */
    INTEGER_CONSTANT = 296,        /* INTEGER_CONSTANT  */
    FLOATING_CONSTANT = 297,       /* FLOATING_CONSTANT  */
    CHARACTER_CONSTANT = 298,      /* CHARACTER_CONSTANT  */
    STRING_LITERAL = 299,          /* STRING_LITERAL  */
    ELLIPSIS = 300,                /* ELLIPSIS  */
    RIGHT_ASSIGN = 301,            /* RIGHT_ASSIGN  */
    LEFT_ASSIGN = 302,             /* LEFT_ASSIGN  */
    ADD_ASSIGN = 303,              /* ADD_ASSIGN  */
    SUB_ASSIGN = 304,              /* SUB_ASSIGN  */
    MUL_ASSIGN = 305,              /* MUL_ASSIGN  */
    DIV_ASSIGN = 306,              /* DIV_ASSIGN  */
    MOD_ASSIGN = 307,              /* MOD_ASSIGN  */
    AND_ASSIGN = 308,              /* AND_ASSIGN  */
    XOR_ASSIGN = 309,              /* XOR_ASSIGN  */
    OR_ASSIGN = 310,               /* OR_ASSIGN  */
    RIGHT_OP = 311,                /* RIGHT_OP  */
    LEFT_OP = 312,                 /* LEFT_OP  */
    INC_OP = 313,                  /* INC_OP  */
    DEC_OP = 314,                  /* DEC_OP  */
    PTR_OP = 315,                  /* PTR_OP  */
    AND_OP = 316,                  /* AND_OP  */
    OR_OP = 317,                   /* OR_OP  */
    LE_OP = 318,                   /* LE_OP  */
    GE_OP = 319,                   /* GE_OP  */
    EQ_OP = 320,                   /* EQ_OP  */
    NE_OP = 321,                   /* NE_OP  */
    SEMICOLON = 322,               /* SEMICOLON  */
    LBRACE = 323,                  /* LBRACE  */
    RBRACE = 324,                  /* RBRACE  */
    COMMA = 325,                   /* COMMA  */
    COLON = 326,                   /* COLON  */
    ASSIGN = 327,                  /* ASSIGN  */
    LPAREN = 328,                  /* LPAREN  */
    RPAREN = 329,                  /* RPAREN  */
    LBRACKET = 330,                /* LBRACKET  */
    RBRACKET = 331,                /* RBRACKET  */
    DOT = 332,                     /* DOT  */
    AMPERSAND = 333,               /* AMPERSAND  */
    EXCLAMATION = 334,             /* EXCLAMATION  */
    TILDE = 335,                   /* TILDE  */
    MINUS = 336,                   /* MINUS  */
    PLUS = 337,                    /* PLUS  */
    STAR = 338,                    /* STAR  */
    SLASH = 339,                   /* SLASH  */
    PERCENT = 340,                 /* PERCENT  */
    LT = 341,                      /* LT  */
    GT = 342,                      /* GT  */
    CARET = 343,                   /* CARET  */
    PIPE = 344,                    /* PIPE  */
    QUESTION = 345                 /* QUESTION  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif
/* Token kinds.  */
#define YYEMPTY -2
#define YYEOF 0
#define YYerror 256
#define YYUNDEF 257
#define AUTO 258
#define BREAK 259
#define CASE 260
#define CHAR 261
#define CONST 262
#define CONTINUE 263
#define DEFAULT 264
#define DO 265
#define DOUBLE 266
#define ELSE 267
#define ENUM 268
#define EXTERN 269
#define FLOAT 270
#define FOR 271
#define GOTO 272
#define IF 273
#define INLINE 274
#define INT 275
#define LONG 276
#define REGISTER 277
#define RESTRICT 278
#define RETURN 279
#define SHORT 280
#define SIGNED 281
#define SIZEOF 282
#define STATIC 283
#define STRUCT 284
#define SWITCH 285
#define TYPEDEF 286
#define UNION 287
#define UNSIGNED 288
#define VOID 289
#define VOLATILE 290
#define WHILE 291
#define BOOL 292
#define COMPLEX 293
#define IMAGINARY 294
#define IDENTIFIER 295
#define INTEGER_CONSTANT 296
#define FLOATING_CONSTANT 297
#define CHARACTER_CONSTANT 298
#define STRING_LITERAL 299
#define ELLIPSIS 300
#define RIGHT_ASSIGN 301
#define LEFT_ASSIGN 302
#define ADD_ASSIGN 303
#define SUB_ASSIGN 304
#define MUL_ASSIGN 305
#define DIV_ASSIGN 306
#define MOD_ASSIGN 307
#define AND_ASSIGN 308
#define XOR_ASSIGN 309
#define OR_ASSIGN 310
#define RIGHT_OP 311
#define LEFT_OP 312
#define INC_OP 313
#define DEC_OP 314
#define PTR_OP 315
#define AND_OP 316
#define OR_OP 317
#define LE_OP 318
#define GE_OP 319
#define EQ_OP 320
#define NE_OP 321
#define SEMICOLON 322
#define LBRACE 323
#define RBRACE 324
#define COMMA 325
#define COLON 326
#define ASSIGN 327
#define LPAREN 328
#define RPAREN 329
#define LBRACKET 330
#define RBRACKET 331
#define DOT 332
#define AMPERSAND 333
#define EXCLAMATION 334
#define TILDE 335
#define MINUS 336
#define PLUS 337
#define STAR 338
#define SLASH 339
#define PERCENT 340
#define LT 341
#define GT 342
#define CARET 343
#define PIPE 344
#define QUESTION 345

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 12 "c_parser.y"

    int int_val;
    float float_val;
    char *string_val;

#line 253 "y.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
