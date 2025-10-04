/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "c_parser.y"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern int yylex();
extern int yyerror(const char *s);
extern char *yytext;


#line 82 "y.tab.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

/* Use api.header.include to #include this header
   instead of duplicating it here.  */
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

#line 321 "y.tab.c"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_AUTO = 3,                       /* AUTO  */
  YYSYMBOL_BREAK = 4,                      /* BREAK  */
  YYSYMBOL_CASE = 5,                       /* CASE  */
  YYSYMBOL_CHAR = 6,                       /* CHAR  */
  YYSYMBOL_CONST = 7,                      /* CONST  */
  YYSYMBOL_CONTINUE = 8,                   /* CONTINUE  */
  YYSYMBOL_DEFAULT = 9,                    /* DEFAULT  */
  YYSYMBOL_DO = 10,                        /* DO  */
  YYSYMBOL_DOUBLE = 11,                    /* DOUBLE  */
  YYSYMBOL_ELSE = 12,                      /* ELSE  */
  YYSYMBOL_ENUM = 13,                      /* ENUM  */
  YYSYMBOL_EXTERN = 14,                    /* EXTERN  */
  YYSYMBOL_FLOAT = 15,                     /* FLOAT  */
  YYSYMBOL_FOR = 16,                       /* FOR  */
  YYSYMBOL_GOTO = 17,                      /* GOTO  */
  YYSYMBOL_IF = 18,                        /* IF  */
  YYSYMBOL_INLINE = 19,                    /* INLINE  */
  YYSYMBOL_INT = 20,                       /* INT  */
  YYSYMBOL_LONG = 21,                      /* LONG  */
  YYSYMBOL_REGISTER = 22,                  /* REGISTER  */
  YYSYMBOL_RESTRICT = 23,                  /* RESTRICT  */
  YYSYMBOL_RETURN = 24,                    /* RETURN  */
  YYSYMBOL_SHORT = 25,                     /* SHORT  */
  YYSYMBOL_SIGNED = 26,                    /* SIGNED  */
  YYSYMBOL_SIZEOF = 27,                    /* SIZEOF  */
  YYSYMBOL_STATIC = 28,                    /* STATIC  */
  YYSYMBOL_STRUCT = 29,                    /* STRUCT  */
  YYSYMBOL_SWITCH = 30,                    /* SWITCH  */
  YYSYMBOL_TYPEDEF = 31,                   /* TYPEDEF  */
  YYSYMBOL_UNION = 32,                     /* UNION  */
  YYSYMBOL_UNSIGNED = 33,                  /* UNSIGNED  */
  YYSYMBOL_VOID = 34,                      /* VOID  */
  YYSYMBOL_VOLATILE = 35,                  /* VOLATILE  */
  YYSYMBOL_WHILE = 36,                     /* WHILE  */
  YYSYMBOL_BOOL = 37,                      /* BOOL  */
  YYSYMBOL_COMPLEX = 38,                   /* COMPLEX  */
  YYSYMBOL_IMAGINARY = 39,                 /* IMAGINARY  */
  YYSYMBOL_IDENTIFIER = 40,                /* IDENTIFIER  */
  YYSYMBOL_INTEGER_CONSTANT = 41,          /* INTEGER_CONSTANT  */
  YYSYMBOL_FLOATING_CONSTANT = 42,         /* FLOATING_CONSTANT  */
  YYSYMBOL_CHARACTER_CONSTANT = 43,        /* CHARACTER_CONSTANT  */
  YYSYMBOL_STRING_LITERAL = 44,            /* STRING_LITERAL  */
  YYSYMBOL_ELLIPSIS = 45,                  /* ELLIPSIS  */
  YYSYMBOL_RIGHT_ASSIGN = 46,              /* RIGHT_ASSIGN  */
  YYSYMBOL_LEFT_ASSIGN = 47,               /* LEFT_ASSIGN  */
  YYSYMBOL_ADD_ASSIGN = 48,                /* ADD_ASSIGN  */
  YYSYMBOL_SUB_ASSIGN = 49,                /* SUB_ASSIGN  */
  YYSYMBOL_MUL_ASSIGN = 50,                /* MUL_ASSIGN  */
  YYSYMBOL_DIV_ASSIGN = 51,                /* DIV_ASSIGN  */
  YYSYMBOL_MOD_ASSIGN = 52,                /* MOD_ASSIGN  */
  YYSYMBOL_AND_ASSIGN = 53,                /* AND_ASSIGN  */
  YYSYMBOL_XOR_ASSIGN = 54,                /* XOR_ASSIGN  */
  YYSYMBOL_OR_ASSIGN = 55,                 /* OR_ASSIGN  */
  YYSYMBOL_RIGHT_OP = 56,                  /* RIGHT_OP  */
  YYSYMBOL_LEFT_OP = 57,                   /* LEFT_OP  */
  YYSYMBOL_INC_OP = 58,                    /* INC_OP  */
  YYSYMBOL_DEC_OP = 59,                    /* DEC_OP  */
  YYSYMBOL_PTR_OP = 60,                    /* PTR_OP  */
  YYSYMBOL_AND_OP = 61,                    /* AND_OP  */
  YYSYMBOL_OR_OP = 62,                     /* OR_OP  */
  YYSYMBOL_LE_OP = 63,                     /* LE_OP  */
  YYSYMBOL_GE_OP = 64,                     /* GE_OP  */
  YYSYMBOL_EQ_OP = 65,                     /* EQ_OP  */
  YYSYMBOL_NE_OP = 66,                     /* NE_OP  */
  YYSYMBOL_SEMICOLON = 67,                 /* SEMICOLON  */
  YYSYMBOL_LBRACE = 68,                    /* LBRACE  */
  YYSYMBOL_RBRACE = 69,                    /* RBRACE  */
  YYSYMBOL_COMMA = 70,                     /* COMMA  */
  YYSYMBOL_COLON = 71,                     /* COLON  */
  YYSYMBOL_ASSIGN = 72,                    /* ASSIGN  */
  YYSYMBOL_LPAREN = 73,                    /* LPAREN  */
  YYSYMBOL_RPAREN = 74,                    /* RPAREN  */
  YYSYMBOL_LBRACKET = 75,                  /* LBRACKET  */
  YYSYMBOL_RBRACKET = 76,                  /* RBRACKET  */
  YYSYMBOL_DOT = 77,                       /* DOT  */
  YYSYMBOL_AMPERSAND = 78,                 /* AMPERSAND  */
  YYSYMBOL_EXCLAMATION = 79,               /* EXCLAMATION  */
  YYSYMBOL_TILDE = 80,                     /* TILDE  */
  YYSYMBOL_MINUS = 81,                     /* MINUS  */
  YYSYMBOL_PLUS = 82,                      /* PLUS  */
  YYSYMBOL_STAR = 83,                      /* STAR  */
  YYSYMBOL_SLASH = 84,                     /* SLASH  */
  YYSYMBOL_PERCENT = 85,                   /* PERCENT  */
  YYSYMBOL_LT = 86,                        /* LT  */
  YYSYMBOL_GT = 87,                        /* GT  */
  YYSYMBOL_CARET = 88,                     /* CARET  */
  YYSYMBOL_PIPE = 89,                      /* PIPE  */
  YYSYMBOL_QUESTION = 90,                  /* QUESTION  */
  YYSYMBOL_YYACCEPT = 91,                  /* $accept  */
  YYSYMBOL_program = 92,                   /* program  */
  YYSYMBOL_external_declaration_list = 93, /* external_declaration_list  */
  YYSYMBOL_external_declaration = 94,      /* external_declaration  */
  YYSYMBOL_function_definition = 95,       /* function_definition  */
  YYSYMBOL_declaration_specifiers = 96,    /* declaration_specifiers  */
  YYSYMBOL_type_specifier = 97,            /* type_specifier  */
  YYSYMBOL_declarator = 98,                /* declarator  */
  YYSYMBOL_direct_declarator = 99,         /* direct_declarator  */
  YYSYMBOL_parameter_type_list = 100,      /* parameter_type_list  */
  YYSYMBOL_parameter_list = 101,           /* parameter_list  */
  YYSYMBOL_parameter_declaration = 102,    /* parameter_declaration  */
  YYSYMBOL_abstract_declarator = 103,      /* abstract_declarator  */
  YYSYMBOL_pointer = 104,                  /* pointer  */
  YYSYMBOL_type_qualifier_list = 105,      /* type_qualifier_list  */
  YYSYMBOL_direct_abstract_declarator = 106, /* direct_abstract_declarator  */
  YYSYMBOL_declaration = 107,              /* declaration  */
  YYSYMBOL_init_declarator_list = 108,     /* init_declarator_list  */
  YYSYMBOL_init_declarator = 109,          /* init_declarator  */
  YYSYMBOL_initializer = 110,              /* initializer  */
  YYSYMBOL_initializer_list = 111,         /* initializer_list  */
  YYSYMBOL_statement = 112,                /* statement  */
  YYSYMBOL_compound_statement = 113,       /* compound_statement  */
  YYSYMBOL_block_item_list = 114,          /* block_item_list  */
  YYSYMBOL_block_item = 115,               /* block_item  */
  YYSYMBOL_expression_statement = 116,     /* expression_statement  */
  YYSYMBOL_selection_statement = 117,      /* selection_statement  */
  YYSYMBOL_iteration_statement = 118,      /* iteration_statement  */
  YYSYMBOL_jump_statement = 119,           /* jump_statement  */
  YYSYMBOL_expression = 120,               /* expression  */
  YYSYMBOL_assignment_expression = 121,    /* assignment_expression  */
  YYSYMBOL_assignment_operator = 122,      /* assignment_operator  */
  YYSYMBOL_conditional_expression = 123,   /* conditional_expression  */
  YYSYMBOL_logical_or_expression = 124,    /* logical_or_expression  */
  YYSYMBOL_logical_and_expression = 125,   /* logical_and_expression  */
  YYSYMBOL_inclusive_or_expression = 126,  /* inclusive_or_expression  */
  YYSYMBOL_exclusive_or_expression = 127,  /* exclusive_or_expression  */
  YYSYMBOL_and_expression = 128,           /* and_expression  */
  YYSYMBOL_equality_expression = 129,      /* equality_expression  */
  YYSYMBOL_relational_expression = 130,    /* relational_expression  */
  YYSYMBOL_shift_expression = 131,         /* shift_expression  */
  YYSYMBOL_additive_expression = 132,      /* additive_expression  */
  YYSYMBOL_multiplicative_expression = 133, /* multiplicative_expression  */
  YYSYMBOL_cast_expression = 134,          /* cast_expression  */
  YYSYMBOL_unary_expression = 135,         /* unary_expression  */
  YYSYMBOL_unary_operator = 136,           /* unary_operator  */
  YYSYMBOL_postfix_expression = 137,       /* postfix_expression  */
  YYSYMBOL_primary_expression = 138,       /* primary_expression  */
  YYSYMBOL_argument_expression_list = 139, /* argument_expression_list  */
  YYSYMBOL_type_name = 140,                /* type_name  */
  YYSYMBOL_specifier_qualifier_list = 141, /* specifier_qualifier_list  */
  YYSYMBOL_type_qualifier = 142            /* type_qualifier  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_int16 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if !defined yyoverflow

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* !defined yyoverflow */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  28
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   1175

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  91
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  52
/* YYNRULES -- Number of rules.  */
#define YYNRULES  178
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  291

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   345


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,    34,    34,    38,    39,    43,    44,    48,    52,    53,
      54,    55,    59,    59,    59,    59,    59,    59,    59,    59,
      59,    59,    59,    59,    60,    61,    65,    66,    70,    71,
      72,    73,    74,    75,    79,    80,    84,    85,    89,    90,
      91,    95,    96,    97,   101,   102,   103,   104,   108,   109,
     113,   114,   115,   116,   117,   118,   119,   120,   124,   128,
     129,   133,   134,   138,   139,   140,   144,   145,   149,   150,
     151,   152,   153,   157,   158,   162,   163,   167,   168,   172,
     173,   177,   178,   179,   183,   184,   185,   186,   187,   188,
     192,   193,   194,   195,   196,   200,   201,   205,   206,   210,
     210,   210,   210,   210,   210,   211,   211,   211,   211,   211,
     215,   216,   220,   221,   225,   226,   230,   231,   235,   236,
     240,   241,   245,   246,   247,   251,   252,   253,   254,   255,
     259,   260,   261,   265,   266,   267,   271,   272,   273,   274,
     278,   279,   283,   284,   285,   286,   287,   288,   292,   292,
     292,   292,   292,   292,   296,   297,   298,   299,   300,   301,
     302,   303,   307,   308,   309,   310,   311,   312,   316,   317,
     321,   322,   326,   327,   328,   329,   333,   333,   333
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "AUTO", "BREAK",
  "CASE", "CHAR", "CONST", "CONTINUE", "DEFAULT", "DO", "DOUBLE", "ELSE",
  "ENUM", "EXTERN", "FLOAT", "FOR", "GOTO", "IF", "INLINE", "INT", "LONG",
  "REGISTER", "RESTRICT", "RETURN", "SHORT", "SIGNED", "SIZEOF", "STATIC",
  "STRUCT", "SWITCH", "TYPEDEF", "UNION", "UNSIGNED", "VOID", "VOLATILE",
  "WHILE", "BOOL", "COMPLEX", "IMAGINARY", "IDENTIFIER",
  "INTEGER_CONSTANT", "FLOATING_CONSTANT", "CHARACTER_CONSTANT",
  "STRING_LITERAL", "ELLIPSIS", "RIGHT_ASSIGN", "LEFT_ASSIGN",
  "ADD_ASSIGN", "SUB_ASSIGN", "MUL_ASSIGN", "DIV_ASSIGN", "MOD_ASSIGN",
  "AND_ASSIGN", "XOR_ASSIGN", "OR_ASSIGN", "RIGHT_OP", "LEFT_OP", "INC_OP",
  "DEC_OP", "PTR_OP", "AND_OP", "OR_OP", "LE_OP", "GE_OP", "EQ_OP",
  "NE_OP", "SEMICOLON", "LBRACE", "RBRACE", "COMMA", "COLON", "ASSIGN",
  "LPAREN", "RPAREN", "LBRACKET", "RBRACKET", "DOT", "AMPERSAND",
  "EXCLAMATION", "TILDE", "MINUS", "PLUS", "STAR", "SLASH", "PERCENT",
  "LT", "GT", "CARET", "PIPE", "QUESTION", "$accept", "program",
  "external_declaration_list", "external_declaration",
  "function_definition", "declaration_specifiers", "type_specifier",
  "declarator", "direct_declarator", "parameter_type_list",
  "parameter_list", "parameter_declaration", "abstract_declarator",
  "pointer", "type_qualifier_list", "direct_abstract_declarator",
  "declaration", "init_declarator_list", "init_declarator", "initializer",
  "initializer_list", "statement", "compound_statement", "block_item_list",
  "block_item", "expression_statement", "selection_statement",
  "iteration_statement", "jump_statement", "expression",
  "assignment_expression", "assignment_operator", "conditional_expression",
  "logical_or_expression", "logical_and_expression",
  "inclusive_or_expression", "exclusive_or_expression", "and_expression",
  "equality_expression", "relational_expression", "shift_expression",
  "additive_expression", "multiplicative_expression", "cast_expression",
  "unary_expression", "unary_operator", "postfix_expression",
  "primary_expression", "argument_expression_list", "type_name",
  "specifier_qualifier_list", "type_qualifier", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-173)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    1136,  -173,  -173,  -173,   -19,  -173,  -173,  -173,  -173,  -173,
    -173,    -9,  -173,  -173,  -173,  -173,  -173,  -173,    48,  1136,
    -173,  -173,    -6,  1136,  -173,  1136,  -173,  -173,  -173,  -173,
    -173,    -6,    34,   -32,    58,    -5,    96,  -173,  -173,  -173,
     -16,  -173,    34,  -173,   231,   673,  -173,   147,   685,    58,
    -173,    -6,  -173,  -173,  -173,    16,    37,   391,    45,    15,
      52,   730,   953,   106,   110,  -173,  -173,  -173,  -173,  -173,
     997,   997,  -173,  -173,   547,  -173,  -173,  -173,  -173,  -173,
    -173,    -6,  -173,  -173,  -173,   311,  -173,  -173,  -173,  -173,
    -173,   104,  -173,  -173,   -48,   108,    98,   117,   116,    79,
      63,     9,   127,   118,  -173,   230,  1023,    64,  -173,   673,
    -173,  -173,  -173,    46,   146,   145,  -173,  -173,   148,   153,
    -173,  -173,  -173,   190,   469,   161,  1023,  -173,   125,   547,
    -173,  1023,  1023,  1023,  -173,  -173,  1136,   -50,   160,    57,
    1136,  -173,  -173,  -173,  1023,  1023,  1023,  1023,  1023,  1023,
    1023,  1023,  1023,  1023,  1023,  1023,  1023,  1023,  1023,  1023,
    1023,  1023,  1023,  1023,  -173,  -173,  -173,  -173,  -173,  -173,
    -173,  -173,  -173,  -173,  -173,  1023,  -173,  -173,  -173,  -173,
     196,   742,  1023,   200,  -173,   141,   -23,   787,  -173,  -173,
     -14,   102,  -173,  1101,  -173,   170,   813,   813,  -173,   -25,
    -173,   176,     2,     5,  -173,  -173,  1023,   -45,  -173,   131,
    -173,  -173,   108,   142,    98,   117,   116,    79,    63,    63,
       9,     9,     9,     9,   127,   127,   118,   118,  -173,  -173,
    -173,  -173,  -173,  -173,  -173,    73,   -37,  -173,  -173,   628,
    -173,   179,  -173,   183,   102,   625,   857,  -173,  -173,  1023,
     883,   927,   391,  -173,   391,   391,  -173,  1023,  1023,  -173,
    -173,  -173,  -173,  -173,  -173,  -173,   212,  -173,   211,    81,
     391,    82,   391,    91,   276,  -173,  -173,  -173,  -173,  -173,
    -173,   224,  -173,   391,  -173,   391,   391,  -173,  -173,  -173,
    -173
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,    13,   176,    18,     0,    17,    15,    16,   177,    14,
      19,     0,    20,    12,   178,    21,    22,    23,     0,     2,
       4,     5,     0,     8,     6,    10,    25,    24,     1,     3,
      28,     0,    45,    61,    26,     0,     0,    59,     9,    11,
       0,    47,    44,    48,     0,     0,     7,     0,     0,    27,
      58,     0,    29,    46,    49,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   162,   163,   164,   165,   166,
       0,     0,    80,    74,     0,   148,   153,   152,   151,   150,
     149,     0,    77,    78,    68,     0,    76,    69,    70,    71,
      72,     0,    95,    97,   110,   112,   114,   116,   118,   120,
     122,   125,   130,   133,   136,   140,     0,   142,   154,     0,
      62,    63,    31,    39,     0,    34,    36,    33,     0,    61,
      60,    92,    91,     0,     0,     0,     0,    94,     0,     0,
     146,     0,     0,     0,   143,   144,   173,     0,     0,   170,
     175,    73,    75,    79,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   106,   105,   103,   104,   100,   101,
     102,   107,   108,   109,    99,     0,   145,   140,   160,   161,
       0,     0,     0,     0,    66,     0,     0,     0,    38,    40,
      41,    43,    30,     0,    32,     0,     0,     0,    90,     0,
      93,     0,     0,     0,   172,   167,     0,     0,   171,    41,
     174,    96,   113,     0,   115,   117,   119,   121,   123,   124,
     128,   129,   126,   127,   132,   131,   135,   134,   137,   138,
     139,    98,   159,   156,   168,     0,     0,   158,    64,     0,
      51,     0,    52,     0,    42,     0,     0,    35,    37,     0,
       0,     0,     0,   147,     0,     0,   141,     0,     0,   157,
     155,    65,    67,    50,    53,    55,     0,    57,     0,     0,
       0,     0,     0,     0,    81,    83,    84,   111,   169,    54,
      56,     0,    88,     0,    86,     0,     0,    85,    89,    87,
      82
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -173,  -173,  -173,   273,  -173,     0,   -58,    -7,   -31,    49,
    -173,   100,   -86,   -29,  -173,  -172,   -12,  -173,   244,  -104,
    -173,   -56,   263,  -173,   216,  -116,  -173,  -173,  -173,   -18,
     -39,  -173,    40,  -173,   158,   159,   157,   167,   173,    66,
      35,    65,   103,   -99,   -60,  -173,  -173,  -173,  -173,   178,
      -2,   -20
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_uint8 yydefgoto[] =
{
       0,    18,    19,    20,    21,    81,    23,    40,    34,   114,
     115,   116,   241,    35,    42,   191,    24,    36,    37,   110,
     185,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      92,   175,    93,    94,    95,    96,    97,    98,    99,   100,
     101,   102,   103,   104,   105,   106,   107,   108,   235,   138,
     139,    25
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      22,   123,   130,    41,    49,   184,   111,   176,   197,   118,
     134,   135,    43,    53,   145,    33,   136,    30,   244,    22,
     144,    26,    54,    38,   205,    39,    30,   189,   207,   240,
     187,    27,    82,   144,    30,    30,    44,   244,    32,   260,
      45,     2,   146,   128,   119,   144,   177,   113,    28,   252,
     186,   240,   187,   208,   140,   125,   137,     8,    52,   186,
      32,   187,   228,   229,   230,   157,   158,    31,    31,    14,
     111,   136,   144,    82,   119,   144,   254,    32,   136,   255,
     250,   251,   136,   121,   190,   177,    30,   177,   177,   177,
     177,   177,   177,   177,   177,   177,   177,   177,   177,   177,
     177,   177,   177,   177,   122,   211,   188,   256,   199,   140,
     209,   137,   196,   202,   203,   137,   140,    32,   124,   186,
     140,   187,   178,   179,   180,   126,   153,   154,   213,    32,
     207,    47,   187,    48,   204,   262,   231,   181,   210,   182,
      32,   183,   234,   258,   151,   152,   177,   259,   243,   155,
     156,   144,   144,     1,     2,   281,   283,   190,     3,    49,
       4,   144,     5,    50,   236,   285,    51,     6,     7,   147,
       8,   143,     9,    10,   144,   245,    11,   246,   209,   131,
      12,    13,    14,   132,    15,    16,    17,   148,   220,   221,
     222,   223,   200,   113,   150,   144,   274,   177,   275,   276,
     111,   161,   162,   163,   207,   149,   187,   268,   159,   160,
     238,   239,   144,   257,   282,   193,   284,   218,   219,   278,
     192,   112,   224,   225,   194,    45,   195,   288,   198,   289,
     290,   269,   271,   273,   206,    55,   232,     1,     2,    56,
     237,    57,     3,   249,     4,   113,     5,    58,    59,    60,
     253,     6,     7,   263,     8,    61,     9,    10,    62,   264,
      11,    63,   226,   227,    12,    13,    14,    64,    15,    16,
      17,    65,    66,    67,    68,    69,   164,   165,   166,   167,
     168,   169,   170,   171,   172,   173,   279,   280,   286,    70,
      71,   287,    29,   248,   266,   120,    46,   277,    72,    44,
      73,   142,   174,   212,    74,   215,   214,   201,     0,    75,
      76,    77,    78,    79,    80,    55,   216,     1,     2,    56,
       0,    57,     3,   217,     4,     0,     5,    58,    59,    60,
       0,     6,     7,     0,     8,    61,     9,    10,    62,     0,
      11,    63,     0,     0,    12,    13,    14,    64,    15,    16,
      17,    65,    66,    67,    68,    69,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    70,
      71,     0,     0,     0,     0,     0,     0,     0,    72,    44,
     141,     0,     0,     0,    74,     0,     0,     0,     0,    75,
      76,    77,    78,    79,    80,    55,     0,     0,     0,    56,
       0,    57,     0,     0,     0,     0,     0,    58,    59,    60,
       0,     0,     0,     0,     0,    61,     0,     0,    62,     0,
       0,    63,     0,     0,     0,     0,     0,    64,     0,     0,
       0,    65,    66,    67,    68,    69,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    70,
      71,     0,     0,     0,     0,     0,     0,     0,    72,    44,
       0,     0,     0,     0,    74,     0,     0,     0,     0,    75,
      76,    77,    78,    79,    80,     1,     2,     0,     0,     0,
       3,     0,     4,     0,     5,     0,     0,     0,     0,     6,
       7,     0,     8,     0,     9,    10,    62,     0,    11,     0,
       0,     0,    12,    13,    14,     0,    15,    16,    17,    65,
      66,    67,    68,    69,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    70,    71,     0,
       0,     0,     0,     0,     0,     0,    72,     0,     0,     0,
       0,     0,    74,     0,     0,     0,     0,    75,    76,    77,
      78,    79,    80,     1,     2,     0,     0,     0,     3,     0,
       4,     0,     5,     0,     0,     0,     0,     6,     7,     0,
       8,     0,     9,    10,    62,     0,    11,     0,     0,     0,
      12,    13,    14,     0,    15,    16,    17,    65,    66,    67,
      68,    69,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    70,    71,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      74,     0,     0,     0,     0,    75,    76,    77,    78,    79,
      80,     1,     2,     0,     0,     0,     3,     0,     4,     0,
       5,     0,     0,     0,     0,     6,     7,     0,     8,     0,
       9,    10,     0,     0,    11,    62,     0,     0,    12,    13,
      14,     0,    15,    16,    17,     0,     0,     0,    65,    66,
      67,    68,    69,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    70,    71,     0,     0,
       0,     0,     0,     0,     0,     0,   109,   261,     0,   265,
      62,    74,     0,     0,     0,     0,    75,    76,    77,    78,
      79,    80,    62,    65,    66,    67,    68,    69,     0,     0,
       0,     0,     0,     0,     0,    65,    66,    67,    68,    69,
       0,    70,    71,     0,     0,     0,     0,     0,     0,     0,
       0,   109,     0,    70,    71,     0,    74,     0,     0,     0,
       0,    75,    76,    77,    78,    79,    80,    62,    74,     0,
       0,   117,     0,    75,    76,    77,    78,    79,    80,    62,
      65,    66,    67,    68,    69,     0,     0,     0,     0,     0,
       0,     0,    65,    66,    67,    68,    69,     0,    70,    71,
       0,     0,     0,     0,     0,     0,     0,   127,     0,     0,
      70,    71,     0,    74,     0,     0,     0,     0,    75,    76,
      77,    78,    79,    80,    62,    74,   233,     0,     0,     0,
      75,    76,    77,    78,    79,    80,     0,    65,    66,    67,
      68,    69,     0,     0,     0,     0,     0,     0,     0,     0,
      62,     0,     0,     0,     0,    70,    71,     0,     0,     0,
       0,     0,     0,    65,    66,    67,    68,    69,     0,     0,
      74,     0,     0,   242,     0,    75,    76,    77,    78,    79,
      80,    70,    71,     0,     0,     0,     0,     0,     0,     0,
      72,     0,     0,     0,    62,     0,    74,     0,     0,     0,
       0,    75,    76,    77,    78,    79,    80,    65,    66,    67,
      68,    69,     0,     0,     0,     0,     0,     0,     0,     0,
      62,     0,     0,     0,     0,    70,    71,     0,     0,     0,
       0,     0,     0,    65,    66,    67,    68,    69,     0,     0,
      74,     0,     0,   267,     0,    75,    76,    77,    78,    79,
      80,    70,    71,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    62,     0,    74,   270,     0,     0,
       0,    75,    76,    77,    78,    79,    80,    65,    66,    67,
      68,    69,     0,     0,     0,     0,     0,     0,     0,     0,
      62,     0,     0,     0,     0,    70,    71,     0,     0,     0,
       0,     0,     0,    65,    66,    67,    68,    69,     0,     0,
      74,   272,     0,     0,     0,    75,    76,    77,    78,    79,
      80,    70,    71,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    62,     0,   129,     0,     0,     0,
       0,    75,    76,    77,    78,    79,    80,    65,    66,    67,
      68,    69,     0,     0,     0,     0,     0,     0,     0,     0,
      62,     0,     0,     0,     0,    70,    71,     0,     0,     0,
       0,     0,     0,    65,    66,    67,    68,    69,     0,     0,
     133,     0,     0,     0,     0,    75,    76,    77,    78,    79,
      80,    70,    71,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    74,     0,     0,     0,
       0,    75,    76,    77,    78,    79,    80,     1,     2,     0,
       0,     0,     3,     0,     4,     0,     5,     0,     0,     0,
       0,     6,     7,     0,     8,     0,     9,    10,     0,     0,
      11,     0,     0,     0,    12,    13,    14,     0,    15,    16,
      17,     0,     1,     2,     0,     0,   247,     3,     0,     4,
       0,     5,     0,     0,     0,     0,     6,     7,     0,     8,
       0,     9,    10,     0,     0,    11,     0,     0,     0,    12,
      13,    14,     0,    15,    16,    17
};

static const yytype_int16 yycheck[] =
{
       0,    57,    62,    32,    35,   109,    45,   106,   124,    48,
      70,    71,    32,    42,    62,    22,    74,    40,   190,    19,
      70,    40,    42,    23,    74,    25,    40,   113,    73,    74,
      75,    40,    44,    70,    40,    40,    68,   209,    83,    76,
      72,     7,    90,    61,    51,    70,   106,    47,     0,    74,
      73,    74,    75,   139,    74,    40,    74,    23,    74,    73,
      83,    75,   161,   162,   163,    56,    57,    73,    73,    35,
     109,   129,    70,    85,    81,    70,    74,    83,   136,    74,
     196,   197,   140,    67,   113,   145,    40,   147,   148,   149,
     150,   151,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,    67,   144,   113,   206,   126,   129,
     139,   129,   124,   131,   132,   133,   136,    83,    73,    73,
     140,    75,    58,    59,    60,    73,    63,    64,   146,    83,
      73,    73,    75,    75,   136,   239,   175,    73,   140,    75,
      83,    77,   181,    70,    65,    66,   206,    74,   187,    86,
      87,    70,    70,     6,     7,    74,    74,   186,    11,   190,
      13,    70,    15,    67,   182,    74,    70,    20,    21,    61,
      23,    67,    25,    26,    70,    73,    29,    75,   207,    73,
      33,    34,    35,    73,    37,    38,    39,    89,   153,   154,
     155,   156,    67,   193,    78,    70,   252,   257,   254,   255,
     239,    83,    84,    85,    73,    88,    75,   246,    81,    82,
      69,    70,    70,    71,   270,    70,   272,   151,   152,   258,
      74,    74,   157,   158,    76,    72,    36,   283,    67,   285,
     286,   249,   250,   251,    74,     4,    40,     6,     7,     8,
      40,    10,    11,    73,    13,   245,    15,    16,    17,    18,
      74,    20,    21,    74,    23,    24,    25,    26,    27,    76,
      29,    30,   159,   160,    33,    34,    35,    36,    37,    38,
      39,    40,    41,    42,    43,    44,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    74,    76,    12,    58,
      59,    67,    19,   193,   245,    51,    33,   257,    67,    68,
      69,    85,    72,   145,    73,   148,   147,   129,    -1,    78,
      79,    80,    81,    82,    83,     4,   149,     6,     7,     8,
      -1,    10,    11,   150,    13,    -1,    15,    16,    17,    18,
      -1,    20,    21,    -1,    23,    24,    25,    26,    27,    -1,
      29,    30,    -1,    -1,    33,    34,    35,    36,    37,    38,
      39,    40,    41,    42,    43,    44,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    58,
      59,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    67,    68,
      69,    -1,    -1,    -1,    73,    -1,    -1,    -1,    -1,    78,
      79,    80,    81,    82,    83,     4,    -1,    -1,    -1,     8,
      -1,    10,    -1,    -1,    -1,    -1,    -1,    16,    17,    18,
      -1,    -1,    -1,    -1,    -1,    24,    -1,    -1,    27,    -1,
      -1,    30,    -1,    -1,    -1,    -1,    -1,    36,    -1,    -1,
      -1,    40,    41,    42,    43,    44,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    58,
      59,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    67,    68,
      -1,    -1,    -1,    -1,    73,    -1,    -1,    -1,    -1,    78,
      79,    80,    81,    82,    83,     6,     7,    -1,    -1,    -1,
      11,    -1,    13,    -1,    15,    -1,    -1,    -1,    -1,    20,
      21,    -1,    23,    -1,    25,    26,    27,    -1,    29,    -1,
      -1,    -1,    33,    34,    35,    -1,    37,    38,    39,    40,
      41,    42,    43,    44,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    58,    59,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    67,    -1,    -1,    -1,
      -1,    -1,    73,    -1,    -1,    -1,    -1,    78,    79,    80,
      81,    82,    83,     6,     7,    -1,    -1,    -1,    11,    -1,
      13,    -1,    15,    -1,    -1,    -1,    -1,    20,    21,    -1,
      23,    -1,    25,    26,    27,    -1,    29,    -1,    -1,    -1,
      33,    34,    35,    -1,    37,    38,    39,    40,    41,    42,
      43,    44,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    58,    59,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      73,    -1,    -1,    -1,    -1,    78,    79,    80,    81,    82,
      83,     6,     7,    -1,    -1,    -1,    11,    -1,    13,    -1,
      15,    -1,    -1,    -1,    -1,    20,    21,    -1,    23,    -1,
      25,    26,    -1,    -1,    29,    27,    -1,    -1,    33,    34,
      35,    -1,    37,    38,    39,    -1,    -1,    -1,    40,    41,
      42,    43,    44,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    58,    59,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    68,    69,    -1,    74,
      27,    73,    -1,    -1,    -1,    -1,    78,    79,    80,    81,
      82,    83,    27,    40,    41,    42,    43,    44,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    40,    41,    42,    43,    44,
      -1,    58,    59,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    68,    -1,    58,    59,    -1,    73,    -1,    -1,    -1,
      -1,    78,    79,    80,    81,    82,    83,    27,    73,    -1,
      -1,    76,    -1,    78,    79,    80,    81,    82,    83,    27,
      40,    41,    42,    43,    44,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    40,    41,    42,    43,    44,    -1,    58,    59,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    67,    -1,    -1,
      58,    59,    -1,    73,    -1,    -1,    -1,    -1,    78,    79,
      80,    81,    82,    83,    27,    73,    74,    -1,    -1,    -1,
      78,    79,    80,    81,    82,    83,    -1,    40,    41,    42,
      43,    44,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      27,    -1,    -1,    -1,    -1,    58,    59,    -1,    -1,    -1,
      -1,    -1,    -1,    40,    41,    42,    43,    44,    -1,    -1,
      73,    -1,    -1,    76,    -1,    78,    79,    80,    81,    82,
      83,    58,    59,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      67,    -1,    -1,    -1,    27,    -1,    73,    -1,    -1,    -1,
      -1,    78,    79,    80,    81,    82,    83,    40,    41,    42,
      43,    44,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      27,    -1,    -1,    -1,    -1,    58,    59,    -1,    -1,    -1,
      -1,    -1,    -1,    40,    41,    42,    43,    44,    -1,    -1,
      73,    -1,    -1,    76,    -1,    78,    79,    80,    81,    82,
      83,    58,    59,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    27,    -1,    73,    74,    -1,    -1,
      -1,    78,    79,    80,    81,    82,    83,    40,    41,    42,
      43,    44,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      27,    -1,    -1,    -1,    -1,    58,    59,    -1,    -1,    -1,
      -1,    -1,    -1,    40,    41,    42,    43,    44,    -1,    -1,
      73,    74,    -1,    -1,    -1,    78,    79,    80,    81,    82,
      83,    58,    59,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    27,    -1,    73,    -1,    -1,    -1,
      -1,    78,    79,    80,    81,    82,    83,    40,    41,    42,
      43,    44,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      27,    -1,    -1,    -1,    -1,    58,    59,    -1,    -1,    -1,
      -1,    -1,    -1,    40,    41,    42,    43,    44,    -1,    -1,
      73,    -1,    -1,    -1,    -1,    78,    79,    80,    81,    82,
      83,    58,    59,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    73,    -1,    -1,    -1,
      -1,    78,    79,    80,    81,    82,    83,     6,     7,    -1,
      -1,    -1,    11,    -1,    13,    -1,    15,    -1,    -1,    -1,
      -1,    20,    21,    -1,    23,    -1,    25,    26,    -1,    -1,
      29,    -1,    -1,    -1,    33,    34,    35,    -1,    37,    38,
      39,    -1,     6,     7,    -1,    -1,    45,    11,    -1,    13,
      -1,    15,    -1,    -1,    -1,    -1,    20,    21,    -1,    23,
      -1,    25,    26,    -1,    -1,    29,    -1,    -1,    -1,    33,
      34,    35,    -1,    37,    38,    39
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     6,     7,    11,    13,    15,    20,    21,    23,    25,
      26,    29,    33,    34,    35,    37,    38,    39,    92,    93,
      94,    95,    96,    97,   107,   142,    40,    40,     0,    94,
      40,    73,    83,    98,    99,   104,   108,   109,    96,    96,
      98,   104,   105,   142,    68,    72,   113,    73,    75,    99,
      67,    70,    74,   104,   142,     4,     8,    10,    16,    17,
      18,    24,    27,    30,    36,    40,    41,    42,    43,    44,
      58,    59,    67,    69,    73,    78,    79,    80,    81,    82,
      83,    96,   107,   112,   113,   114,   115,   116,   117,   118,
     119,   120,   121,   123,   124,   125,   126,   127,   128,   129,
     130,   131,   132,   133,   134,   135,   136,   137,   138,    68,
     110,   121,    74,    96,   100,   101,   102,    76,   121,    98,
     109,    67,    67,   112,    73,    40,    73,    67,   120,    73,
     135,    73,    73,    73,   135,   135,    97,   120,   140,   141,
     142,    69,   115,    67,    70,    62,    90,    61,    89,    88,
      78,    65,    66,    63,    64,    86,    87,    56,    57,    81,
      82,    83,    84,    85,    46,    47,    48,    49,    50,    51,
      52,    53,    54,    55,    72,   122,   134,   135,    58,    59,
      60,    73,    75,    77,   110,   111,    73,    75,    98,   103,
     104,   106,    74,    70,    76,    36,   107,   116,    67,   120,
      67,   140,   120,   120,   141,    74,    74,    73,   103,   104,
     141,   121,   125,   120,   126,   127,   128,   129,   130,   130,
     131,   131,   131,   131,   132,   132,   133,   133,   134,   134,
     134,   121,    40,    74,   121,   139,   120,    40,    69,    70,
      74,   103,    76,   121,   106,    73,    75,    45,   102,    73,
     116,   116,    74,    74,    74,    74,   134,    71,    70,    74,
      76,    69,   110,    74,    76,    74,   100,    76,   121,   120,
      74,   120,    74,   120,   112,   112,   112,   123,   121,    74,
      76,    74,   112,    74,   112,    74,    12,    67,   112,   112,
     112
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_uint8 yyr1[] =
{
       0,    91,    92,    93,    93,    94,    94,    95,    96,    96,
      96,    96,    97,    97,    97,    97,    97,    97,    97,    97,
      97,    97,    97,    97,    97,    97,    98,    98,    99,    99,
      99,    99,    99,    99,   100,   100,   101,   101,   102,   102,
     102,   103,   103,   103,   104,   104,   104,   104,   105,   105,
     106,   106,   106,   106,   106,   106,   106,   106,   107,   108,
     108,   109,   109,   110,   110,   110,   111,   111,   112,   112,
     112,   112,   112,   113,   113,   114,   114,   115,   115,   116,
     116,   117,   117,   117,   118,   118,   118,   118,   118,   118,
     119,   119,   119,   119,   119,   120,   120,   121,   121,   122,
     122,   122,   122,   122,   122,   122,   122,   122,   122,   122,
     123,   123,   124,   124,   125,   125,   126,   126,   127,   127,
     128,   128,   129,   129,   129,   130,   130,   130,   130,   130,
     131,   131,   131,   132,   132,   132,   133,   133,   133,   133,
     134,   134,   135,   135,   135,   135,   135,   135,   136,   136,
     136,   136,   136,   136,   137,   137,   137,   137,   137,   137,
     137,   137,   138,   138,   138,   138,   138,   138,   139,   139,
     140,   140,   141,   141,   141,   141,   142,   142,   142
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     2,     1,     1,     1,     3,     1,     2,
       1,     2,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     2,     2,     1,     2,     1,     3,
       4,     3,     4,     3,     1,     3,     1,     3,     2,     1,
       2,     1,     2,     1,     2,     1,     3,     2,     1,     2,
       3,     2,     2,     3,     4,     3,     4,     3,     3,     1,
       3,     1,     3,     1,     3,     4,     1,     3,     1,     1,
       1,     1,     1,     3,     2,     2,     1,     1,     1,     2,
       1,     5,     7,     5,     5,     7,     6,     7,     6,     7,
       3,     2,     2,     3,     2,     1,     3,     1,     3,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     5,     1,     3,     1,     3,     1,     3,     1,     3,
       1,     3,     1,     3,     3,     1,     3,     3,     3,     3,
       1,     3,     3,     1,     3,     3,     1,     3,     3,     3,
       1,     4,     1,     2,     2,     2,     2,     4,     1,     1,
       1,     1,     1,     1,     1,     4,     3,     4,     3,     3,
       2,     2,     1,     1,     1,     1,     1,     3,     1,     3,
       1,     2,     2,     1,     2,     1,     1,     1,     1
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)




# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)]);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep)
{
  YY_USE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      goto yyerrlab1;
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {

#line 1847 "y.tab.c"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (YY_("syntax error"));
    }

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;
  ++yynerrs;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 336 "c_parser.y"


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
