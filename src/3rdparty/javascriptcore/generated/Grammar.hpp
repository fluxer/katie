/* A Bison parser, made by GNU Bison 3.7.6.  */

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

#ifndef YY_JSCYY_GRAMMAR_HPP_INCLUDED
# define YY_JSCYY_GRAMMAR_HPP_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int jscyydebug;
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
    NULLTOKEN = 258,               /* NULLTOKEN  */
    TRUETOKEN = 259,               /* TRUETOKEN  */
    FALSETOKEN = 260,              /* FALSETOKEN  */
    BREAK = 261,                   /* BREAK  */
    CASE = 262,                    /* CASE  */
    DEFAULT = 263,                 /* DEFAULT  */
    FOR = 264,                     /* FOR  */
    NEW = 265,                     /* NEW  */
    VAR = 266,                     /* VAR  */
    CONSTTOKEN = 267,              /* CONSTTOKEN  */
    CONTINUE = 268,                /* CONTINUE  */
    FUNCTION = 269,                /* FUNCTION  */
    RETURN = 270,                  /* RETURN  */
    VOIDTOKEN = 271,               /* VOIDTOKEN  */
    DELETETOKEN = 272,             /* DELETETOKEN  */
    IF = 273,                      /* IF  */
    THISTOKEN = 274,               /* THISTOKEN  */
    DO = 275,                      /* DO  */
    WHILE = 276,                   /* WHILE  */
    INTOKEN = 277,                 /* INTOKEN  */
    INSTANCEOF = 278,              /* INSTANCEOF  */
    TYPEOF = 279,                  /* TYPEOF  */
    SWITCH = 280,                  /* SWITCH  */
    WITH = 281,                    /* WITH  */
    RESERVED = 282,                /* RESERVED  */
    THROW = 283,                   /* THROW  */
    TRY = 284,                     /* TRY  */
    CATCH = 285,                   /* CATCH  */
    FINALLY = 286,                 /* FINALLY  */
    DEBUGGER = 287,                /* DEBUGGER  */
    IF_WITHOUT_ELSE = 288,         /* IF_WITHOUT_ELSE  */
    ELSE = 289,                    /* ELSE  */
    EQEQ = 290,                    /* EQEQ  */
    NE = 291,                      /* NE  */
    STREQ = 292,                   /* STREQ  */
    STRNEQ = 293,                  /* STRNEQ  */
    LE = 294,                      /* LE  */
    GE = 295,                      /* GE  */
    OR = 296,                      /* OR  */
    AND = 297,                     /* AND  */
    PLUSPLUS = 298,                /* PLUSPLUS  */
    MINUSMINUS = 299,              /* MINUSMINUS  */
    LSHIFT = 300,                  /* LSHIFT  */
    RSHIFT = 301,                  /* RSHIFT  */
    URSHIFT = 302,                 /* URSHIFT  */
    PLUSEQUAL = 303,               /* PLUSEQUAL  */
    MINUSEQUAL = 304,              /* MINUSEQUAL  */
    MULTEQUAL = 305,               /* MULTEQUAL  */
    DIVEQUAL = 306,                /* DIVEQUAL  */
    LSHIFTEQUAL = 307,             /* LSHIFTEQUAL  */
    RSHIFTEQUAL = 308,             /* RSHIFTEQUAL  */
    URSHIFTEQUAL = 309,            /* URSHIFTEQUAL  */
    ANDEQUAL = 310,                /* ANDEQUAL  */
    MODEQUAL = 311,                /* MODEQUAL  */
    XOREQUAL = 312,                /* XOREQUAL  */
    OREQUAL = 313,                 /* OREQUAL  */
    OPENBRACE = 314,               /* OPENBRACE  */
    CLOSEBRACE = 315,              /* CLOSEBRACE  */
    NUMBER = 316,                  /* NUMBER  */
    IDENT = 317,                   /* IDENT  */
    STRING = 318,                  /* STRING  */
    AUTOPLUSPLUS = 319,            /* AUTOPLUSPLUS  */
    AUTOMINUSMINUS = 320           /* AUTOMINUSMINUS  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{

    int                 intValue;
    double              doubleValue;
    const Identifier*   ident;

    // expression subtrees
    ExpressionNodeInfo  expressionNode;
    FuncDeclNodeInfo    funcDeclNode;
    PropertyNodeInfo    propertyNode;
    ArgumentsNodeInfo   argumentsNode;
    ConstDeclNodeInfo   constDeclNode;
    CaseBlockNodeInfo   caseBlockNode;
    CaseClauseNodeInfo  caseClauseNode;
    FuncExprNodeInfo    funcExprNode;

    // statement nodes
    StatementNodeInfo   statementNode;
    FunctionBodyNode*   functionBodyNode;
    ProgramNode*        programNode;

    SourceElementsInfo  sourceElements;
    PropertyListInfo    propertyList;
    ArgumentListInfo    argumentList;
    VarDeclListInfo     varDeclList;
    ConstDeclListInfo   constDeclList;
    ClauseListInfo      clauseList;
    ElementListInfo     elementList;
    ParameterListInfo   parameterList;

    Operator            op;


};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif

/* Location type.  */
#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE YYLTYPE;
struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
};
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif



int jscyyparse (void *globalPtr);

#endif /* !YY_JSCYY_GRAMMAR_HPP_INCLUDED  */
