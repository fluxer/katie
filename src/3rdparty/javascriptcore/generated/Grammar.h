/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

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

#ifndef YY_JSCYY_GRAMMAR_HPP_INCLUDED
# define YY_JSCYY_GRAMMAR_HPP_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int jscyydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    NULLTOKEN = 258,
    TRUETOKEN = 259,
    FALSETOKEN = 260,
    BREAK = 261,
    CASE = 262,
    DEFAULT = 263,
    FOR = 264,
    NEW = 265,
    VAR = 266,
    CONSTTOKEN = 267,
    CONTINUE = 268,
    FUNCTION = 269,
    RETURN = 270,
    VOIDTOKEN = 271,
    DELETETOKEN = 272,
    IF = 273,
    THISTOKEN = 274,
    DO = 275,
    WHILE = 276,
    INTOKEN = 277,
    INSTANCEOF = 278,
    TYPEOF = 279,
    SWITCH = 280,
    WITH = 281,
    RESERVED = 282,
    THROW = 283,
    TRY = 284,
    CATCH = 285,
    FINALLY = 286,
    DEBUGGER = 287,
    IF_WITHOUT_ELSE = 288,
    ELSE = 289,
    EQEQ = 290,
    NE = 291,
    STREQ = 292,
    STRNEQ = 293,
    LE = 294,
    GE = 295,
    OR = 296,
    AND = 297,
    PLUSPLUS = 298,
    MINUSMINUS = 299,
    LSHIFT = 300,
    RSHIFT = 301,
    URSHIFT = 302,
    PLUSEQUAL = 303,
    MINUSEQUAL = 304,
    MULTEQUAL = 305,
    DIVEQUAL = 306,
    LSHIFTEQUAL = 307,
    RSHIFTEQUAL = 308,
    URSHIFTEQUAL = 309,
    ANDEQUAL = 310,
    MODEQUAL = 311,
    XOREQUAL = 312,
    OREQUAL = 313,
    OPENBRACE = 314,
    CLOSEBRACE = 315,
    NUMBER = 316,
    IDENT = 317,
    STRING = 318,
    AUTOPLUSPLUS = 319,
    AUTOMINUSMINUS = 320
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 157 "../parser/Grammar.y" /* yacc.c:1909  */

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

#line 152 "Grammar.hpp" /* yacc.c:1909  */
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
