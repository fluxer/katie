/* A Bison parser, made by GNU Bison 3.7.4.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2020 Free Software Foundation,
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
#define YYBISON 30704

/* Bison version string.  */
#define YYBISON_VERSION "3.7.4"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 1

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1


/* Substitute the variable and function names.  */
#define yyparse         jscyyparse
#define yylex           jscyylex
#define yyerror         jscyyerror
#define yydebug         jscyydebug
#define yynerrs         jscyynerrs

/* First part of user prologue.  */


/*
 *  Copyright (C) 1999-2000 Harri Porten (porten@kde.org)
 *  Copyright (C) 2006, 2007, 2008, 2009 Apple Inc. All rights reserved.
 *  Copyright (C) 2007 Eric Seidel <eric@webkit.org>
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

#include "Platform.h"

#include "JSObject.h"
#include "JSString.h"
#include "Lexer.h"
#include "NodeConstructors.h"
#include "NodeInfo.h"
#include <stdlib.h>
#include <string.h>

#define YYMALLOC fastMalloc
#define YYFREE fastFree

#define YYMAXDEPTH 10000
#define YYENABLE_NLS 0

// Default values for bison.
#define YYDEBUG 0 // Set to 1 to debug a parse error.
#define jscyydebug 0 // Set to 1 to debug a parse error.
// Avoid triggering warnings in older bison by not setting this on the Darwin platform.
// FIXME: Is this still needed?
#define YYERROR_VERBOSE

// Called by yyparse on error.
int yyerror(void *globalPtr)
{
    return 1;
}

int yyerror(void *globalPtr, void*&_scanner, const char *string)
{
    return 1;
}

static inline bool allowAutomaticSemicolon(JSC::Lexer&, int);

#define GLOBAL_DATA static_cast<JSGlobalData*>(globalPtr)
#define AUTO_SEMICOLON do { if (!allowAutomaticSemicolon(*GLOBAL_DATA->lexer, yychar)) YYABORT; } while (0)

using namespace JSC;
using namespace std;

static ExpressionNode* makeAssignNode(JSGlobalData*, ExpressionNode* left, Operator, ExpressionNode* right, bool leftHasAssignments, bool rightHasAssignments, int start, int divot, int end);
static ExpressionNode* makePrefixNode(JSGlobalData*, ExpressionNode*, Operator, int start, int divot, int end);
static ExpressionNode* makePostfixNode(JSGlobalData*, ExpressionNode*, Operator, int start, int divot, int end);
static PropertyNode* makeGetterOrSetterPropertyNode(JSGlobalData*, const Identifier& getOrSet, const Identifier& name, ParameterNode*, FunctionBodyNode*, const SourceCode&);
static ExpressionNodeInfo makeFunctionCallNode(JSGlobalData*, ExpressionNodeInfo function, ArgumentsNodeInfo, int start, int divot, int end);
static ExpressionNode* makeTypeOfNode(JSGlobalData*, ExpressionNode*);
static ExpressionNode* makeDeleteNode(JSGlobalData*, ExpressionNode*, int start, int divot, int end);
static ExpressionNode* makeNegateNode(JSGlobalData*, ExpressionNode*);
static NumberNode* makeNumberNode(JSGlobalData*, double);
static ExpressionNode* makeBitwiseNotNode(JSGlobalData*, ExpressionNode*);
static ExpressionNode* makeMultNode(JSGlobalData*, ExpressionNode* left, ExpressionNode* right, bool rightHasAssignments);
static ExpressionNode* makeDivNode(JSGlobalData*, ExpressionNode* left, ExpressionNode* right, bool rightHasAssignments);
static ExpressionNode* makeAddNode(JSGlobalData*, ExpressionNode* left, ExpressionNode* right, bool rightHasAssignments);
static ExpressionNode* makeSubNode(JSGlobalData*, ExpressionNode* left, ExpressionNode* right, bool rightHasAssignments);
static ExpressionNode* makeLeftShiftNode(JSGlobalData*, ExpressionNode* left, ExpressionNode* right, bool rightHasAssignments);
static ExpressionNode* makeRightShiftNode(JSGlobalData*, ExpressionNode* left, ExpressionNode* right, bool rightHasAssignments);
static StatementNode* makeVarStatementNode(JSGlobalData*, ExpressionNode*);
static ExpressionNode* combineCommaNodes(JSGlobalData*, ExpressionNode* list, ExpressionNode* init);

#define YYPARSE_PARAM globalPtr
#define YYLEX_PARAM globalPtr

template <typename T> inline NodeDeclarationInfo<T> createNodeDeclarationInfo(T node,
    ParserArenaData<DeclarationStacks::VarStack>* varDecls,
    ParserArenaData<DeclarationStacks::FunctionStack>* funcDecls,
    CodeFeatures info, int numConstants) 
{
    Q_ASSERT((info & ~AllFeatures) == 0);
    NodeDeclarationInfo<T> result = { node, varDecls, funcDecls, info, numConstants };
    return result;
}

template <typename T> inline NodeInfo<T> createNodeInfo(T node, CodeFeatures info, int numConstants)
{
    Q_ASSERT((info & ~AllFeatures) == 0);
    NodeInfo<T> result = { node, info, numConstants };
    return result;
}

template <typename T> inline T mergeDeclarationLists(T decls1, T decls2) 
{
    // decls1 or both are null
    if (!decls1)
        return decls2;
    // only decls1 is non-null
    if (!decls2)
        return decls1;

    // Both are non-null
    decls1->data.append(decls2->data);
    
    // Manually release as much as possible from the now-defunct declaration lists
    // to avoid accumulating so many unused heap allocated vectors.
    decls2->data.clear();

    return decls1;
}

static inline void appendToVarDeclarationList(JSGlobalData* globalData, ParserArenaData<DeclarationStacks::VarStack>*& varDecls, const Identifier& ident, unsigned attrs)
{
    if (!varDecls)
        varDecls = new (globalData) ParserArenaData<DeclarationStacks::VarStack>;

    varDecls->data.append(std::make_pair(&ident, attrs));
}

static inline void appendToVarDeclarationList(JSGlobalData* globalData, ParserArenaData<DeclarationStacks::VarStack>*& varDecls, ConstDeclNode* decl)
{
    unsigned attrs = DeclarationStacks::IsConstant;
    if (decl->hasInitializer())
        attrs |= DeclarationStacks::HasInitializer;        
    appendToVarDeclarationList(globalData, varDecls, decl->ident(), attrs);
}



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

#include "Grammar.hpp"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_NULLTOKEN = 3,                  /* NULLTOKEN  */
  YYSYMBOL_TRUETOKEN = 4,                  /* TRUETOKEN  */
  YYSYMBOL_FALSETOKEN = 5,                 /* FALSETOKEN  */
  YYSYMBOL_BREAK = 6,                      /* BREAK  */
  YYSYMBOL_CASE = 7,                       /* CASE  */
  YYSYMBOL_DEFAULT = 8,                    /* DEFAULT  */
  YYSYMBOL_FOR = 9,                        /* FOR  */
  YYSYMBOL_NEW = 10,                       /* NEW  */
  YYSYMBOL_VAR = 11,                       /* VAR  */
  YYSYMBOL_CONSTTOKEN = 12,                /* CONSTTOKEN  */
  YYSYMBOL_CONTINUE = 13,                  /* CONTINUE  */
  YYSYMBOL_FUNCTION = 14,                  /* FUNCTION  */
  YYSYMBOL_RETURN = 15,                    /* RETURN  */
  YYSYMBOL_VOIDTOKEN = 16,                 /* VOIDTOKEN  */
  YYSYMBOL_DELETETOKEN = 17,               /* DELETETOKEN  */
  YYSYMBOL_IF = 18,                        /* IF  */
  YYSYMBOL_THISTOKEN = 19,                 /* THISTOKEN  */
  YYSYMBOL_DO = 20,                        /* DO  */
  YYSYMBOL_WHILE = 21,                     /* WHILE  */
  YYSYMBOL_INTOKEN = 22,                   /* INTOKEN  */
  YYSYMBOL_INSTANCEOF = 23,                /* INSTANCEOF  */
  YYSYMBOL_TYPEOF = 24,                    /* TYPEOF  */
  YYSYMBOL_SWITCH = 25,                    /* SWITCH  */
  YYSYMBOL_WITH = 26,                      /* WITH  */
  YYSYMBOL_RESERVED = 27,                  /* RESERVED  */
  YYSYMBOL_THROW = 28,                     /* THROW  */
  YYSYMBOL_TRY = 29,                       /* TRY  */
  YYSYMBOL_CATCH = 30,                     /* CATCH  */
  YYSYMBOL_FINALLY = 31,                   /* FINALLY  */
  YYSYMBOL_DEBUGGER = 32,                  /* DEBUGGER  */
  YYSYMBOL_IF_WITHOUT_ELSE = 33,           /* IF_WITHOUT_ELSE  */
  YYSYMBOL_ELSE = 34,                      /* ELSE  */
  YYSYMBOL_EQEQ = 35,                      /* EQEQ  */
  YYSYMBOL_NE = 36,                        /* NE  */
  YYSYMBOL_STREQ = 37,                     /* STREQ  */
  YYSYMBOL_STRNEQ = 38,                    /* STRNEQ  */
  YYSYMBOL_LE = 39,                        /* LE  */
  YYSYMBOL_GE = 40,                        /* GE  */
  YYSYMBOL_OR = 41,                        /* OR  */
  YYSYMBOL_AND = 42,                       /* AND  */
  YYSYMBOL_PLUSPLUS = 43,                  /* PLUSPLUS  */
  YYSYMBOL_MINUSMINUS = 44,                /* MINUSMINUS  */
  YYSYMBOL_LSHIFT = 45,                    /* LSHIFT  */
  YYSYMBOL_RSHIFT = 46,                    /* RSHIFT  */
  YYSYMBOL_URSHIFT = 47,                   /* URSHIFT  */
  YYSYMBOL_PLUSEQUAL = 48,                 /* PLUSEQUAL  */
  YYSYMBOL_MINUSEQUAL = 49,                /* MINUSEQUAL  */
  YYSYMBOL_MULTEQUAL = 50,                 /* MULTEQUAL  */
  YYSYMBOL_DIVEQUAL = 51,                  /* DIVEQUAL  */
  YYSYMBOL_LSHIFTEQUAL = 52,               /* LSHIFTEQUAL  */
  YYSYMBOL_RSHIFTEQUAL = 53,               /* RSHIFTEQUAL  */
  YYSYMBOL_URSHIFTEQUAL = 54,              /* URSHIFTEQUAL  */
  YYSYMBOL_ANDEQUAL = 55,                  /* ANDEQUAL  */
  YYSYMBOL_MODEQUAL = 56,                  /* MODEQUAL  */
  YYSYMBOL_XOREQUAL = 57,                  /* XOREQUAL  */
  YYSYMBOL_OREQUAL = 58,                   /* OREQUAL  */
  YYSYMBOL_OPENBRACE = 59,                 /* OPENBRACE  */
  YYSYMBOL_CLOSEBRACE = 60,                /* CLOSEBRACE  */
  YYSYMBOL_NUMBER = 61,                    /* NUMBER  */
  YYSYMBOL_IDENT = 62,                     /* IDENT  */
  YYSYMBOL_STRING = 63,                    /* STRING  */
  YYSYMBOL_AUTOPLUSPLUS = 64,              /* AUTOPLUSPLUS  */
  YYSYMBOL_AUTOMINUSMINUS = 65,            /* AUTOMINUSMINUS  */
  YYSYMBOL_66_ = 66,                       /* '/'  */
  YYSYMBOL_67_ = 67,                       /* ':'  */
  YYSYMBOL_68_ = 68,                       /* '('  */
  YYSYMBOL_69_ = 69,                       /* ')'  */
  YYSYMBOL_70_ = 70,                       /* ','  */
  YYSYMBOL_71_ = 71,                       /* '['  */
  YYSYMBOL_72_ = 72,                       /* ']'  */
  YYSYMBOL_73_ = 73,                       /* '.'  */
  YYSYMBOL_74_ = 74,                       /* '+'  */
  YYSYMBOL_75_ = 75,                       /* '-'  */
  YYSYMBOL_76_ = 76,                       /* '~'  */
  YYSYMBOL_77_ = 77,                       /* '!'  */
  YYSYMBOL_78_ = 78,                       /* '*'  */
  YYSYMBOL_79_ = 79,                       /* '%'  */
  YYSYMBOL_80_ = 80,                       /* '<'  */
  YYSYMBOL_81_ = 81,                       /* '>'  */
  YYSYMBOL_82_ = 82,                       /* '&'  */
  YYSYMBOL_83_ = 83,                       /* '^'  */
  YYSYMBOL_84_ = 84,                       /* '|'  */
  YYSYMBOL_85_ = 85,                       /* '?'  */
  YYSYMBOL_86_ = 86,                       /* '='  */
  YYSYMBOL_87_ = 87,                       /* ';'  */
  YYSYMBOL_YYACCEPT = 88,                  /* $accept  */
  YYSYMBOL_Literal = 89,                   /* Literal  */
  YYSYMBOL_Property = 90,                  /* Property  */
  YYSYMBOL_PropertyList = 91,              /* PropertyList  */
  YYSYMBOL_PrimaryExpr = 92,               /* PrimaryExpr  */
  YYSYMBOL_PrimaryExprNoBrace = 93,        /* PrimaryExprNoBrace  */
  YYSYMBOL_ArrayLiteral = 94,              /* ArrayLiteral  */
  YYSYMBOL_ElementList = 95,               /* ElementList  */
  YYSYMBOL_ElisionOpt = 96,                /* ElisionOpt  */
  YYSYMBOL_Elision = 97,                   /* Elision  */
  YYSYMBOL_MemberExpr = 98,                /* MemberExpr  */
  YYSYMBOL_MemberExprNoBF = 99,            /* MemberExprNoBF  */
  YYSYMBOL_NewExpr = 100,                  /* NewExpr  */
  YYSYMBOL_NewExprNoBF = 101,              /* NewExprNoBF  */
  YYSYMBOL_CallExpr = 102,                 /* CallExpr  */
  YYSYMBOL_CallExprNoBF = 103,             /* CallExprNoBF  */
  YYSYMBOL_Arguments = 104,                /* Arguments  */
  YYSYMBOL_ArgumentList = 105,             /* ArgumentList  */
  YYSYMBOL_LeftHandSideExpr = 106,         /* LeftHandSideExpr  */
  YYSYMBOL_LeftHandSideExprNoBF = 107,     /* LeftHandSideExprNoBF  */
  YYSYMBOL_PostfixExpr = 108,              /* PostfixExpr  */
  YYSYMBOL_PostfixExprNoBF = 109,          /* PostfixExprNoBF  */
  YYSYMBOL_UnaryExprCommon = 110,          /* UnaryExprCommon  */
  YYSYMBOL_UnaryExpr = 111,                /* UnaryExpr  */
  YYSYMBOL_UnaryExprNoBF = 112,            /* UnaryExprNoBF  */
  YYSYMBOL_MultiplicativeExpr = 113,       /* MultiplicativeExpr  */
  YYSYMBOL_MultiplicativeExprNoBF = 114,   /* MultiplicativeExprNoBF  */
  YYSYMBOL_AdditiveExpr = 115,             /* AdditiveExpr  */
  YYSYMBOL_AdditiveExprNoBF = 116,         /* AdditiveExprNoBF  */
  YYSYMBOL_ShiftExpr = 117,                /* ShiftExpr  */
  YYSYMBOL_ShiftExprNoBF = 118,            /* ShiftExprNoBF  */
  YYSYMBOL_RelationalExpr = 119,           /* RelationalExpr  */
  YYSYMBOL_RelationalExprNoIn = 120,       /* RelationalExprNoIn  */
  YYSYMBOL_RelationalExprNoBF = 121,       /* RelationalExprNoBF  */
  YYSYMBOL_EqualityExpr = 122,             /* EqualityExpr  */
  YYSYMBOL_EqualityExprNoIn = 123,         /* EqualityExprNoIn  */
  YYSYMBOL_EqualityExprNoBF = 124,         /* EqualityExprNoBF  */
  YYSYMBOL_BitwiseANDExpr = 125,           /* BitwiseANDExpr  */
  YYSYMBOL_BitwiseANDExprNoIn = 126,       /* BitwiseANDExprNoIn  */
  YYSYMBOL_BitwiseANDExprNoBF = 127,       /* BitwiseANDExprNoBF  */
  YYSYMBOL_BitwiseXORExpr = 128,           /* BitwiseXORExpr  */
  YYSYMBOL_BitwiseXORExprNoIn = 129,       /* BitwiseXORExprNoIn  */
  YYSYMBOL_BitwiseXORExprNoBF = 130,       /* BitwiseXORExprNoBF  */
  YYSYMBOL_BitwiseORExpr = 131,            /* BitwiseORExpr  */
  YYSYMBOL_BitwiseORExprNoIn = 132,        /* BitwiseORExprNoIn  */
  YYSYMBOL_BitwiseORExprNoBF = 133,        /* BitwiseORExprNoBF  */
  YYSYMBOL_LogicalANDExpr = 134,           /* LogicalANDExpr  */
  YYSYMBOL_LogicalANDExprNoIn = 135,       /* LogicalANDExprNoIn  */
  YYSYMBOL_LogicalANDExprNoBF = 136,       /* LogicalANDExprNoBF  */
  YYSYMBOL_LogicalORExpr = 137,            /* LogicalORExpr  */
  YYSYMBOL_LogicalORExprNoIn = 138,        /* LogicalORExprNoIn  */
  YYSYMBOL_LogicalORExprNoBF = 139,        /* LogicalORExprNoBF  */
  YYSYMBOL_ConditionalExpr = 140,          /* ConditionalExpr  */
  YYSYMBOL_ConditionalExprNoIn = 141,      /* ConditionalExprNoIn  */
  YYSYMBOL_ConditionalExprNoBF = 142,      /* ConditionalExprNoBF  */
  YYSYMBOL_AssignmentExpr = 143,           /* AssignmentExpr  */
  YYSYMBOL_AssignmentExprNoIn = 144,       /* AssignmentExprNoIn  */
  YYSYMBOL_AssignmentExprNoBF = 145,       /* AssignmentExprNoBF  */
  YYSYMBOL_AssignmentOperator = 146,       /* AssignmentOperator  */
  YYSYMBOL_Expr = 147,                     /* Expr  */
  YYSYMBOL_ExprNoIn = 148,                 /* ExprNoIn  */
  YYSYMBOL_ExprNoBF = 149,                 /* ExprNoBF  */
  YYSYMBOL_Statement = 150,                /* Statement  */
  YYSYMBOL_Block = 151,                    /* Block  */
  YYSYMBOL_VariableStatement = 152,        /* VariableStatement  */
  YYSYMBOL_VariableDeclarationList = 153,  /* VariableDeclarationList  */
  YYSYMBOL_VariableDeclarationListNoIn = 154, /* VariableDeclarationListNoIn  */
  YYSYMBOL_ConstStatement = 155,           /* ConstStatement  */
  YYSYMBOL_ConstDeclarationList = 156,     /* ConstDeclarationList  */
  YYSYMBOL_ConstDeclaration = 157,         /* ConstDeclaration  */
  YYSYMBOL_Initializer = 158,              /* Initializer  */
  YYSYMBOL_InitializerNoIn = 159,          /* InitializerNoIn  */
  YYSYMBOL_EmptyStatement = 160,           /* EmptyStatement  */
  YYSYMBOL_ExprStatement = 161,            /* ExprStatement  */
  YYSYMBOL_IfStatement = 162,              /* IfStatement  */
  YYSYMBOL_IterationStatement = 163,       /* IterationStatement  */
  YYSYMBOL_ExprOpt = 164,                  /* ExprOpt  */
  YYSYMBOL_ExprNoInOpt = 165,              /* ExprNoInOpt  */
  YYSYMBOL_ContinueStatement = 166,        /* ContinueStatement  */
  YYSYMBOL_BreakStatement = 167,           /* BreakStatement  */
  YYSYMBOL_ReturnStatement = 168,          /* ReturnStatement  */
  YYSYMBOL_WithStatement = 169,            /* WithStatement  */
  YYSYMBOL_SwitchStatement = 170,          /* SwitchStatement  */
  YYSYMBOL_CaseBlock = 171,                /* CaseBlock  */
  YYSYMBOL_CaseClausesOpt = 172,           /* CaseClausesOpt  */
  YYSYMBOL_CaseClauses = 173,              /* CaseClauses  */
  YYSYMBOL_CaseClause = 174,               /* CaseClause  */
  YYSYMBOL_DefaultClause = 175,            /* DefaultClause  */
  YYSYMBOL_LabelledStatement = 176,        /* LabelledStatement  */
  YYSYMBOL_ThrowStatement = 177,           /* ThrowStatement  */
  YYSYMBOL_TryStatement = 178,             /* TryStatement  */
  YYSYMBOL_DebuggerStatement = 179,        /* DebuggerStatement  */
  YYSYMBOL_FunctionDeclaration = 180,      /* FunctionDeclaration  */
  YYSYMBOL_FunctionExpr = 181,             /* FunctionExpr  */
  YYSYMBOL_FormalParameterList = 182,      /* FormalParameterList  */
  YYSYMBOL_FunctionBody = 183,             /* FunctionBody  */
  YYSYMBOL_Program = 184,                  /* Program  */
  YYSYMBOL_SourceElements = 185,           /* SourceElements  */
  YYSYMBOL_Literal_NoNode = 186,           /* Literal_NoNode  */
  YYSYMBOL_Property_NoNode = 187,          /* Property_NoNode  */
  YYSYMBOL_PropertyList_NoNode = 188,      /* PropertyList_NoNode  */
  YYSYMBOL_PrimaryExpr_NoNode = 189,       /* PrimaryExpr_NoNode  */
  YYSYMBOL_PrimaryExprNoBrace_NoNode = 190, /* PrimaryExprNoBrace_NoNode  */
  YYSYMBOL_ArrayLiteral_NoNode = 191,      /* ArrayLiteral_NoNode  */
  YYSYMBOL_ElementList_NoNode = 192,       /* ElementList_NoNode  */
  YYSYMBOL_ElisionOpt_NoNode = 193,        /* ElisionOpt_NoNode  */
  YYSYMBOL_Elision_NoNode = 194,           /* Elision_NoNode  */
  YYSYMBOL_MemberExpr_NoNode = 195,        /* MemberExpr_NoNode  */
  YYSYMBOL_MemberExprNoBF_NoNode = 196,    /* MemberExprNoBF_NoNode  */
  YYSYMBOL_NewExpr_NoNode = 197,           /* NewExpr_NoNode  */
  YYSYMBOL_NewExprNoBF_NoNode = 198,       /* NewExprNoBF_NoNode  */
  YYSYMBOL_CallExpr_NoNode = 199,          /* CallExpr_NoNode  */
  YYSYMBOL_CallExprNoBF_NoNode = 200,      /* CallExprNoBF_NoNode  */
  YYSYMBOL_Arguments_NoNode = 201,         /* Arguments_NoNode  */
  YYSYMBOL_ArgumentList_NoNode = 202,      /* ArgumentList_NoNode  */
  YYSYMBOL_LeftHandSideExpr_NoNode = 203,  /* LeftHandSideExpr_NoNode  */
  YYSYMBOL_LeftHandSideExprNoBF_NoNode = 204, /* LeftHandSideExprNoBF_NoNode  */
  YYSYMBOL_PostfixExpr_NoNode = 205,       /* PostfixExpr_NoNode  */
  YYSYMBOL_PostfixExprNoBF_NoNode = 206,   /* PostfixExprNoBF_NoNode  */
  YYSYMBOL_UnaryExprCommon_NoNode = 207,   /* UnaryExprCommon_NoNode  */
  YYSYMBOL_UnaryExpr_NoNode = 208,         /* UnaryExpr_NoNode  */
  YYSYMBOL_UnaryExprNoBF_NoNode = 209,     /* UnaryExprNoBF_NoNode  */
  YYSYMBOL_MultiplicativeExpr_NoNode = 210, /* MultiplicativeExpr_NoNode  */
  YYSYMBOL_MultiplicativeExprNoBF_NoNode = 211, /* MultiplicativeExprNoBF_NoNode  */
  YYSYMBOL_AdditiveExpr_NoNode = 212,      /* AdditiveExpr_NoNode  */
  YYSYMBOL_AdditiveExprNoBF_NoNode = 213,  /* AdditiveExprNoBF_NoNode  */
  YYSYMBOL_ShiftExpr_NoNode = 214,         /* ShiftExpr_NoNode  */
  YYSYMBOL_ShiftExprNoBF_NoNode = 215,     /* ShiftExprNoBF_NoNode  */
  YYSYMBOL_RelationalExpr_NoNode = 216,    /* RelationalExpr_NoNode  */
  YYSYMBOL_RelationalExprNoIn_NoNode = 217, /* RelationalExprNoIn_NoNode  */
  YYSYMBOL_RelationalExprNoBF_NoNode = 218, /* RelationalExprNoBF_NoNode  */
  YYSYMBOL_EqualityExpr_NoNode = 219,      /* EqualityExpr_NoNode  */
  YYSYMBOL_EqualityExprNoIn_NoNode = 220,  /* EqualityExprNoIn_NoNode  */
  YYSYMBOL_EqualityExprNoBF_NoNode = 221,  /* EqualityExprNoBF_NoNode  */
  YYSYMBOL_BitwiseANDExpr_NoNode = 222,    /* BitwiseANDExpr_NoNode  */
  YYSYMBOL_BitwiseANDExprNoIn_NoNode = 223, /* BitwiseANDExprNoIn_NoNode  */
  YYSYMBOL_BitwiseANDExprNoBF_NoNode = 224, /* BitwiseANDExprNoBF_NoNode  */
  YYSYMBOL_BitwiseXORExpr_NoNode = 225,    /* BitwiseXORExpr_NoNode  */
  YYSYMBOL_BitwiseXORExprNoIn_NoNode = 226, /* BitwiseXORExprNoIn_NoNode  */
  YYSYMBOL_BitwiseXORExprNoBF_NoNode = 227, /* BitwiseXORExprNoBF_NoNode  */
  YYSYMBOL_BitwiseORExpr_NoNode = 228,     /* BitwiseORExpr_NoNode  */
  YYSYMBOL_BitwiseORExprNoIn_NoNode = 229, /* BitwiseORExprNoIn_NoNode  */
  YYSYMBOL_BitwiseORExprNoBF_NoNode = 230, /* BitwiseORExprNoBF_NoNode  */
  YYSYMBOL_LogicalANDExpr_NoNode = 231,    /* LogicalANDExpr_NoNode  */
  YYSYMBOL_LogicalANDExprNoIn_NoNode = 232, /* LogicalANDExprNoIn_NoNode  */
  YYSYMBOL_LogicalANDExprNoBF_NoNode = 233, /* LogicalANDExprNoBF_NoNode  */
  YYSYMBOL_LogicalORExpr_NoNode = 234,     /* LogicalORExpr_NoNode  */
  YYSYMBOL_LogicalORExprNoIn_NoNode = 235, /* LogicalORExprNoIn_NoNode  */
  YYSYMBOL_LogicalORExprNoBF_NoNode = 236, /* LogicalORExprNoBF_NoNode  */
  YYSYMBOL_ConditionalExpr_NoNode = 237,   /* ConditionalExpr_NoNode  */
  YYSYMBOL_ConditionalExprNoIn_NoNode = 238, /* ConditionalExprNoIn_NoNode  */
  YYSYMBOL_ConditionalExprNoBF_NoNode = 239, /* ConditionalExprNoBF_NoNode  */
  YYSYMBOL_AssignmentExpr_NoNode = 240,    /* AssignmentExpr_NoNode  */
  YYSYMBOL_AssignmentExprNoIn_NoNode = 241, /* AssignmentExprNoIn_NoNode  */
  YYSYMBOL_AssignmentExprNoBF_NoNode = 242, /* AssignmentExprNoBF_NoNode  */
  YYSYMBOL_AssignmentOperator_NoNode = 243, /* AssignmentOperator_NoNode  */
  YYSYMBOL_Expr_NoNode = 244,              /* Expr_NoNode  */
  YYSYMBOL_ExprNoIn_NoNode = 245,          /* ExprNoIn_NoNode  */
  YYSYMBOL_ExprNoBF_NoNode = 246,          /* ExprNoBF_NoNode  */
  YYSYMBOL_Statement_NoNode = 247,         /* Statement_NoNode  */
  YYSYMBOL_Block_NoNode = 248,             /* Block_NoNode  */
  YYSYMBOL_VariableStatement_NoNode = 249, /* VariableStatement_NoNode  */
  YYSYMBOL_VariableDeclarationList_NoNode = 250, /* VariableDeclarationList_NoNode  */
  YYSYMBOL_VariableDeclarationListNoIn_NoNode = 251, /* VariableDeclarationListNoIn_NoNode  */
  YYSYMBOL_ConstStatement_NoNode = 252,    /* ConstStatement_NoNode  */
  YYSYMBOL_ConstDeclarationList_NoNode = 253, /* ConstDeclarationList_NoNode  */
  YYSYMBOL_ConstDeclaration_NoNode = 254,  /* ConstDeclaration_NoNode  */
  YYSYMBOL_Initializer_NoNode = 255,       /* Initializer_NoNode  */
  YYSYMBOL_InitializerNoIn_NoNode = 256,   /* InitializerNoIn_NoNode  */
  YYSYMBOL_EmptyStatement_NoNode = 257,    /* EmptyStatement_NoNode  */
  YYSYMBOL_ExprStatement_NoNode = 258,     /* ExprStatement_NoNode  */
  YYSYMBOL_IfStatement_NoNode = 259,       /* IfStatement_NoNode  */
  YYSYMBOL_IterationStatement_NoNode = 260, /* IterationStatement_NoNode  */
  YYSYMBOL_ExprOpt_NoNode = 261,           /* ExprOpt_NoNode  */
  YYSYMBOL_ExprNoInOpt_NoNode = 262,       /* ExprNoInOpt_NoNode  */
  YYSYMBOL_ContinueStatement_NoNode = 263, /* ContinueStatement_NoNode  */
  YYSYMBOL_BreakStatement_NoNode = 264,    /* BreakStatement_NoNode  */
  YYSYMBOL_ReturnStatement_NoNode = 265,   /* ReturnStatement_NoNode  */
  YYSYMBOL_WithStatement_NoNode = 266,     /* WithStatement_NoNode  */
  YYSYMBOL_SwitchStatement_NoNode = 267,   /* SwitchStatement_NoNode  */
  YYSYMBOL_CaseBlock_NoNode = 268,         /* CaseBlock_NoNode  */
  YYSYMBOL_CaseClausesOpt_NoNode = 269,    /* CaseClausesOpt_NoNode  */
  YYSYMBOL_CaseClauses_NoNode = 270,       /* CaseClauses_NoNode  */
  YYSYMBOL_CaseClause_NoNode = 271,        /* CaseClause_NoNode  */
  YYSYMBOL_DefaultClause_NoNode = 272,     /* DefaultClause_NoNode  */
  YYSYMBOL_LabelledStatement_NoNode = 273, /* LabelledStatement_NoNode  */
  YYSYMBOL_ThrowStatement_NoNode = 274,    /* ThrowStatement_NoNode  */
  YYSYMBOL_TryStatement_NoNode = 275,      /* TryStatement_NoNode  */
  YYSYMBOL_DebuggerStatement_NoNode = 276, /* DebuggerStatement_NoNode  */
  YYSYMBOL_FunctionDeclaration_NoNode = 277, /* FunctionDeclaration_NoNode  */
  YYSYMBOL_FunctionExpr_NoNode = 278,      /* FunctionExpr_NoNode  */
  YYSYMBOL_FormalParameterList_NoNode = 279, /* FormalParameterList_NoNode  */
  YYSYMBOL_FunctionBody_NoNode = 280,      /* FunctionBody_NoNode  */
  YYSYMBOL_SourceElements_NoNode = 281     /* SourceElements_NoNode  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;


/* Second part of user prologue.  */


template <typename T> inline void setStatementLocation(StatementNode* statement, const T& start, const T& end)
{
    statement->setLoc(start.first_line, end.last_line);
}

static inline void setExceptionLocation(ThrowableExpressionData* node, unsigned start, unsigned divot, unsigned end)
{
    node->setExceptionSourceCode(divot, divot - start, end - divot);
}




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
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && ! defined __ICC && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                            \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
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
         || (defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL \
             && defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
  YYLTYPE yyls_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE) \
             + YYSIZEOF (YYLTYPE)) \
      + 2 * YYSTACK_GAP_MAXIMUM)

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
#define YYFINAL  206
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   2349

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  88
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  194
/* YYNRULES -- Number of rules.  */
#define YYNRULES  597
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1082

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   320


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
       2,     2,     2,    77,     2,     2,     2,    79,    82,     2,
      68,    69,    78,    74,    70,    75,    73,    66,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    67,    87,
      80,    86,    81,    85,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    71,     2,    72,    83,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,    84,     2,    76,     2,     2,     2,
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
      65
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   293,   293,   294,   295,   296,   297,   298,   309,   323,
     324,   325,   326,   327,   339,   343,   350,   351,   352,   354,
     358,   359,   360,   361,   362,   366,   367,   368,   372,   376,
     384,   385,   389,   390,   394,   395,   396,   400,   404,   411,
     412,   416,   420,   427,   428,   435,   436,   443,   444,   445,
     449,   455,   456,   457,   461,   468,   469,   473,   477,   484,
     485,   489,   490,   494,   495,   496,   500,   501,   502,   506,
     507,   508,   509,   510,   511,   512,   513,   514,   515,   516,
     519,   520,   524,   525,   529,   530,   531,   532,   536,   537,
     539,   541,   546,   547,   548,   552,   553,   555,   560,   561,
     562,   563,   567,   568,   569,   570,   574,   575,   576,   577,
     578,   579,   582,   588,   589,   590,   591,   592,   593,   600,
     601,   602,   603,   604,   605,   609,   616,   617,   618,   619,
     620,   624,   625,   627,   629,   631,   636,   637,   639,   640,
     642,   647,   648,   652,   653,   658,   659,   663,   664,   668,
     669,   674,   675,   680,   681,   685,   686,   691,   692,   697,
     698,   702,   703,   708,   709,   714,   715,   719,   720,   725,
     726,   730,   731,   736,   737,   742,   743,   748,   749,   756,
     757,   764,   765,   772,   773,   774,   775,   776,   777,   778,
     779,   780,   781,   782,   783,   787,   788,   792,   793,   797,
     798,   802,   803,   804,   805,   806,   807,   808,   809,   810,
     811,   812,   813,   814,   815,   816,   817,   818,   822,   824,
     829,   831,   837,   844,   853,   861,   874,   881,   890,   898,
     911,   913,   919,   927,   939,   940,   944,   948,   952,   956,
     958,   963,   966,   976,   978,   980,   982,   988,   995,  1004,
    1010,  1021,  1022,  1026,  1027,  1031,  1035,  1039,  1043,  1050,
    1053,  1056,  1059,  1065,  1068,  1071,  1074,  1080,  1086,  1092,
    1093,  1102,  1103,  1107,  1113,  1123,  1124,  1128,  1129,  1133,
    1139,  1143,  1150,  1156,  1162,  1172,  1174,  1179,  1180,  1191,
    1192,  1199,  1200,  1210,  1213,  1219,  1220,  1224,  1225,  1230,
    1237,  1248,  1249,  1250,  1251,  1252,  1253,  1254,  1258,  1259,
    1260,  1261,  1262,  1266,  1267,  1271,  1272,  1273,  1275,  1279,
    1280,  1281,  1282,  1283,  1287,  1288,  1289,  1293,  1294,  1297,
    1299,  1303,  1304,  1308,  1309,  1310,  1311,  1312,  1316,  1317,
    1318,  1319,  1323,  1324,  1328,  1329,  1333,  1334,  1335,  1336,
    1340,  1341,  1342,  1343,  1347,  1348,  1352,  1353,  1357,  1358,
    1362,  1363,  1367,  1368,  1369,  1373,  1374,  1375,  1379,  1380,
    1381,  1382,  1383,  1384,  1385,  1386,  1387,  1388,  1389,  1392,
    1393,  1397,  1398,  1402,  1403,  1404,  1405,  1409,  1410,  1411,
    1412,  1416,  1417,  1418,  1422,  1423,  1424,  1428,  1429,  1430,
    1431,  1435,  1436,  1437,  1438,  1442,  1443,  1444,  1445,  1446,
    1447,  1448,  1452,  1453,  1454,  1455,  1456,  1457,  1461,  1462,
    1463,  1464,  1465,  1466,  1467,  1471,  1472,  1473,  1474,  1475,
    1479,  1480,  1481,  1482,  1483,  1487,  1488,  1489,  1490,  1491,
    1495,  1496,  1500,  1501,  1505,  1506,  1510,  1511,  1515,  1516,
    1520,  1521,  1525,  1526,  1530,  1531,  1535,  1536,  1540,  1541,
    1545,  1546,  1550,  1551,  1555,  1556,  1560,  1561,  1565,  1566,
    1570,  1571,  1575,  1576,  1580,  1581,  1585,  1586,  1590,  1591,
    1595,  1596,  1600,  1601,  1602,  1603,  1604,  1605,  1606,  1607,
    1608,  1609,  1610,  1611,  1615,  1616,  1620,  1621,  1625,  1626,
    1630,  1631,  1632,  1633,  1634,  1635,  1636,  1637,  1638,  1639,
    1640,  1641,  1642,  1643,  1644,  1645,  1646,  1650,  1651,  1655,
    1656,  1660,  1661,  1662,  1663,  1667,  1668,  1669,  1670,  1674,
    1675,  1679,  1680,  1684,  1685,  1689,  1693,  1697,  1701,  1702,
    1706,  1707,  1711,  1712,  1713,  1714,  1715,  1716,  1717,  1718,
    1721,  1723,  1726,  1728,  1732,  1733,  1734,  1735,  1739,  1740,
    1741,  1742,  1746,  1747,  1748,  1749,  1753,  1757,  1761,  1762,
    1765,  1767,  1771,  1772,  1776,  1777,  1781,  1782,  1786,  1790,
    1791,  1795,  1796,  1797,  1801,  1802,  1806,  1807,  1811,  1812,
    1813,  1814,  1818,  1819,  1822,  1824,  1828,  1829
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
  "\"end of file\"", "error", "\"invalid token\"", "NULLTOKEN",
  "TRUETOKEN", "FALSETOKEN", "BREAK", "CASE", "DEFAULT", "FOR", "NEW",
  "VAR", "CONSTTOKEN", "CONTINUE", "FUNCTION", "RETURN", "VOIDTOKEN",
  "DELETETOKEN", "IF", "THISTOKEN", "DO", "WHILE", "INTOKEN", "INSTANCEOF",
  "TYPEOF", "SWITCH", "WITH", "RESERVED", "THROW", "TRY", "CATCH",
  "FINALLY", "DEBUGGER", "IF_WITHOUT_ELSE", "ELSE", "EQEQ", "NE", "STREQ",
  "STRNEQ", "LE", "GE", "OR", "AND", "PLUSPLUS", "MINUSMINUS", "LSHIFT",
  "RSHIFT", "URSHIFT", "PLUSEQUAL", "MINUSEQUAL", "MULTEQUAL", "DIVEQUAL",
  "LSHIFTEQUAL", "RSHIFTEQUAL", "URSHIFTEQUAL", "ANDEQUAL", "MODEQUAL",
  "XOREQUAL", "OREQUAL", "OPENBRACE", "CLOSEBRACE", "NUMBER", "IDENT",
  "STRING", "AUTOPLUSPLUS", "AUTOMINUSMINUS", "'/'", "':'", "'('", "')'",
  "','", "'['", "']'", "'.'", "'+'", "'-'", "'~'", "'!'", "'*'", "'%'",
  "'<'", "'>'", "'&'", "'^'", "'|'", "'?'", "'='", "';'", "$accept",
  "Literal", "Property", "PropertyList", "PrimaryExpr",
  "PrimaryExprNoBrace", "ArrayLiteral", "ElementList", "ElisionOpt",
  "Elision", "MemberExpr", "MemberExprNoBF", "NewExpr", "NewExprNoBF",
  "CallExpr", "CallExprNoBF", "Arguments", "ArgumentList",
  "LeftHandSideExpr", "LeftHandSideExprNoBF", "PostfixExpr",
  "PostfixExprNoBF", "UnaryExprCommon", "UnaryExpr", "UnaryExprNoBF",
  "MultiplicativeExpr", "MultiplicativeExprNoBF", "AdditiveExpr",
  "AdditiveExprNoBF", "ShiftExpr", "ShiftExprNoBF", "RelationalExpr",
  "RelationalExprNoIn", "RelationalExprNoBF", "EqualityExpr",
  "EqualityExprNoIn", "EqualityExprNoBF", "BitwiseANDExpr",
  "BitwiseANDExprNoIn", "BitwiseANDExprNoBF", "BitwiseXORExpr",
  "BitwiseXORExprNoIn", "BitwiseXORExprNoBF", "BitwiseORExpr",
  "BitwiseORExprNoIn", "BitwiseORExprNoBF", "LogicalANDExpr",
  "LogicalANDExprNoIn", "LogicalANDExprNoBF", "LogicalORExpr",
  "LogicalORExprNoIn", "LogicalORExprNoBF", "ConditionalExpr",
  "ConditionalExprNoIn", "ConditionalExprNoBF", "AssignmentExpr",
  "AssignmentExprNoIn", "AssignmentExprNoBF", "AssignmentOperator", "Expr",
  "ExprNoIn", "ExprNoBF", "Statement", "Block", "VariableStatement",
  "VariableDeclarationList", "VariableDeclarationListNoIn",
  "ConstStatement", "ConstDeclarationList", "ConstDeclaration",
  "Initializer", "InitializerNoIn", "EmptyStatement", "ExprStatement",
  "IfStatement", "IterationStatement", "ExprOpt", "ExprNoInOpt",
  "ContinueStatement", "BreakStatement", "ReturnStatement",
  "WithStatement", "SwitchStatement", "CaseBlock", "CaseClausesOpt",
  "CaseClauses", "CaseClause", "DefaultClause", "LabelledStatement",
  "ThrowStatement", "TryStatement", "DebuggerStatement",
  "FunctionDeclaration", "FunctionExpr", "FormalParameterList",
  "FunctionBody", "Program", "SourceElements", "Literal_NoNode",
  "Property_NoNode", "PropertyList_NoNode", "PrimaryExpr_NoNode",
  "PrimaryExprNoBrace_NoNode", "ArrayLiteral_NoNode", "ElementList_NoNode",
  "ElisionOpt_NoNode", "Elision_NoNode", "MemberExpr_NoNode",
  "MemberExprNoBF_NoNode", "NewExpr_NoNode", "NewExprNoBF_NoNode",
  "CallExpr_NoNode", "CallExprNoBF_NoNode", "Arguments_NoNode",
  "ArgumentList_NoNode", "LeftHandSideExpr_NoNode",
  "LeftHandSideExprNoBF_NoNode", "PostfixExpr_NoNode",
  "PostfixExprNoBF_NoNode", "UnaryExprCommon_NoNode", "UnaryExpr_NoNode",
  "UnaryExprNoBF_NoNode", "MultiplicativeExpr_NoNode",
  "MultiplicativeExprNoBF_NoNode", "AdditiveExpr_NoNode",
  "AdditiveExprNoBF_NoNode", "ShiftExpr_NoNode", "ShiftExprNoBF_NoNode",
  "RelationalExpr_NoNode", "RelationalExprNoIn_NoNode",
  "RelationalExprNoBF_NoNode", "EqualityExpr_NoNode",
  "EqualityExprNoIn_NoNode", "EqualityExprNoBF_NoNode",
  "BitwiseANDExpr_NoNode", "BitwiseANDExprNoIn_NoNode",
  "BitwiseANDExprNoBF_NoNode", "BitwiseXORExpr_NoNode",
  "BitwiseXORExprNoIn_NoNode", "BitwiseXORExprNoBF_NoNode",
  "BitwiseORExpr_NoNode", "BitwiseORExprNoIn_NoNode",
  "BitwiseORExprNoBF_NoNode", "LogicalANDExpr_NoNode",
  "LogicalANDExprNoIn_NoNode", "LogicalANDExprNoBF_NoNode",
  "LogicalORExpr_NoNode", "LogicalORExprNoIn_NoNode",
  "LogicalORExprNoBF_NoNode", "ConditionalExpr_NoNode",
  "ConditionalExprNoIn_NoNode", "ConditionalExprNoBF_NoNode",
  "AssignmentExpr_NoNode", "AssignmentExprNoIn_NoNode",
  "AssignmentExprNoBF_NoNode", "AssignmentOperator_NoNode", "Expr_NoNode",
  "ExprNoIn_NoNode", "ExprNoBF_NoNode", "Statement_NoNode", "Block_NoNode",
  "VariableStatement_NoNode", "VariableDeclarationList_NoNode",
  "VariableDeclarationListNoIn_NoNode", "ConstStatement_NoNode",
  "ConstDeclarationList_NoNode", "ConstDeclaration_NoNode",
  "Initializer_NoNode", "InitializerNoIn_NoNode", "EmptyStatement_NoNode",
  "ExprStatement_NoNode", "IfStatement_NoNode",
  "IterationStatement_NoNode", "ExprOpt_NoNode", "ExprNoInOpt_NoNode",
  "ContinueStatement_NoNode", "BreakStatement_NoNode",
  "ReturnStatement_NoNode", "WithStatement_NoNode",
  "SwitchStatement_NoNode", "CaseBlock_NoNode", "CaseClausesOpt_NoNode",
  "CaseClauses_NoNode", "CaseClause_NoNode", "DefaultClause_NoNode",
  "LabelledStatement_NoNode", "ThrowStatement_NoNode",
  "TryStatement_NoNode", "DebuggerStatement_NoNode",
  "FunctionDeclaration_NoNode", "FunctionExpr_NoNode",
  "FormalParameterList_NoNode", "FunctionBody_NoNode",
  "SourceElements_NoNode", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_int16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,    47,    58,    40,    41,
      44,    91,    93,    46,    43,    45,   126,    33,    42,    37,
      60,    62,    38,    94,   124,    63,    61,    59
};
#endif

#define YYPACT_NINF (-941)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    1516,  -941,  -941,  -941,    44,    -2,   839,    26,   178,    73,
     192,   954,  2114,  2114,   189,  -941,  1516,   207,  2114,   245,
     275,  2114,   226,    47,  2114,  2114,  -941,  1200,  -941,   280,
    -941,  2114,  2114,  -941,  2114,    20,  2114,  2114,  2114,  2114,
    -941,  -941,  -941,  -941,   350,  -941,   361,  2201,  -941,  -941,
    -941,     6,   -21,   437,   446,   264,   269,   315,   306,   364,
       9,  -941,  -941,    69,  -941,  -941,  -941,  -941,  -941,  -941,
    -941,  -941,  -941,  -941,  -941,  -941,  -941,  -941,  -941,  -941,
    -941,  -941,   417,  1516,  -941,    88,  -941,  1670,   839,    25,
     435,  -941,  -941,  -941,   390,  -941,  -941,   338,    96,   338,
     151,  -941,  -941,    90,  -941,   365,  -941,  -941,   390,  -941,
     394,  2224,  -941,  -941,  -941,   215,   255,   483,   509,   504,
     374,   377,   380,   424,    14,  -941,  -941,   163,   445,  -941,
    -941,  2114,   452,  2114,  -941,  2114,  2114,   164,   486,  -941,
    -941,  -941,  -941,  -941,  1279,  1516,  -941,  -941,   495,  -941,
     311,  1706,   400,  -941,  -941,  -941,  -941,  1781,  2114,   418,
    -941,  2114,   432,  -941,  -941,  -941,  -941,  -941,  -941,  -941,
    -941,  -941,  -941,  -941,  -941,  -941,  -941,  -941,  2114,  2114,
    2114,  2114,  2114,  2114,  2114,  2114,  2114,  2114,  2114,  2114,
    2114,  2114,  2114,  2114,  2114,  2114,  2114,  2114,  2114,  2114,
    2114,  2114,  2114,  -941,  2114,  -941,  -941,  -941,  -941,  -941,
     442,   737,   483,   355,   583,   428,   440,   453,   491,    17,
    -941,  -941,   481,   469,   390,  -941,   505,   187,  -941,   513,
      -5,   521,  -941,   177,  2114,   539,  -941,  2114,  -941,  -941,
     545,  -941,  -941,  -941,   178,  -941,  -941,  -941,   236,  -941,
    2114,   547,  -941,  -941,  -941,  2114,  2114,  2114,  2114,  2114,
    2114,  2114,  2114,  2114,  2114,  2114,  2114,  2114,  2114,  2114,
    2114,  2114,  2114,  2114,  2114,  2114,  2114,  2114,  2114,  2114,
    -941,  2114,  -941,   499,   548,   559,   582,   617,  -941,  -941,
     556,   226,  -941,  -941,  -941,    20,  -941,  -941,  -941,  -941,
    -941,   628,  -941,   314,  -941,   329,  -941,  -941,  -941,  -941,
    -941,   215,   215,   255,   255,   255,   483,   483,   483,   483,
     483,   483,   509,   509,   509,   509,   504,   374,   377,   380,
     424,   546,  -941,    29,   -11,  2114,  2114,  2114,  2114,  2114,
    2114,  2114,  2114,  2114,  2114,  2114,  2114,  2114,  2114,  2114,
    2114,  2114,  2114,  2114,  -941,   256,  -941,   567,   632,  2114,
     563,  2114,  2114,  -941,   586,   358,  -941,  -941,   338,  -941,
     574,   635,   436,  -941,  -941,  -941,  -941,  -941,   215,   215,
     255,   255,   255,   483,   483,   483,   483,   483,   483,   509,
     509,   509,   509,   504,   374,   377,   380,   424,   571,  -941,
    1516,  2114,  1516,   584,  1516,   591,  -941,  1817,  -941,  2114,
    -941,  -941,  2114,  2114,  2114,   656,   598,  2114,   648,  2224,
    -941,   483,   483,   483,   483,   483,   355,   355,   355,   355,
     583,   428,   440,   453,   491,   639,  -941,   614,   608,   649,
     662,  1595,   651,   650,  -941,   283,  -941,  -941,  -941,  -941,
    -941,  -941,  1595,   660,  -941,  2114,   681,   670,  -941,   716,
    -941,  -941,   657,  -941,  -941,  -941,  -941,   680,  -941,  2114,
     647,   654,  1516,  2114,  2114,  1595,   677,  -941,  -941,  -941,
     141,   688,  1122,   707,   712,   179,   717,  1087,  2150,  2150,
     728,  -941,  1595,   730,  2150,   732,   743,  2150,   754,    91,
    2150,  2150,  -941,  1358,  -941,   755,  -941,  2150,  2150,  -941,
    2150,   714,  2150,  2150,  2150,  2150,  -941,   756,  -941,  -941,
    -941,   403,  -941,   434,  2240,  -941,  -941,  -941,   257,   581,
     498,   619,   630,   747,   769,   753,   828,    23,  -941,  -941,
     185,  -941,  -941,  -941,  -941,  -941,  -941,  -941,  -941,  -941,
    -941,  -941,  -941,  -941,  -941,  -941,  -941,  -941,  -941,  1595,
    1595,  -941,   819,   685,   821,  1595,  -941,  1516,   171,  2114,
     219,   716,  -941,   226,  1516,   692,  -941,  2114,  -941,  -941,
     810,   822,  1595,  -941,   183,  -941,  1892,  1122,   305,   609,
    -941,  -941,  -941,   441,  -941,  -941,   797,   195,   797,   203,
    -941,  -941,   197,  -941,   816,  -941,  -941,   441,  -941,   447,
    2263,  -941,  -941,  -941,   262,   698,   515,   640,   638,   812,
     802,   811,   845,    28,  -941,  -941,   208,   739,  -941,  -941,
    2150,   868,  2150,  -941,  2150,  2150,   216,   777,  -941,  -941,
    -941,  -941,  -941,  1437,  1595,  -941,  -941,   740,  -941,   449,
    1928,   827,  -941,  -941,  -941,  -941,  -941,  2003,  2150,   837,
    -941,  2150,   841,  -941,  -941,  -941,  -941,  -941,  -941,  -941,
    -941,  -941,  -941,  -941,  -941,  -941,  -941,  -941,  2150,  2150,
    2150,  2150,  2150,  2150,  2150,  2150,  2150,  2150,  2150,  2150,
    2150,  2150,  2150,  2150,  2150,  2150,  2150,  2150,  2150,  2150,
    2150,  2150,  2150,  -941,  2150,  -941,  -941,   844,  1595,   847,
    -941,   848,  -941,  -941,  -941,     8,   842,  -941,   716,  -941,
     880,  -941,  1516,   849,  1516,  -941,   859,  -941,  -941,   860,
    2185,   515,   357,   655,   843,   838,   840,   884,   150,  -941,
    -941,   857,   846,   441,  -941,   861,   299,  -941,   863,   181,
     870,  -941,   284,  2150,   866,  -941,  2150,  -941,  -941,   873,
    -941,  -941,  -941,   712,  -941,  -941,  -941,   301,  -941,  2150,
     876,  -941,  -941,  -941,  2150,  2150,  2150,  2150,  2150,  2150,
    2150,  2150,  2150,  2150,  2150,  2150,  2150,  2150,  2150,  2150,
    2150,  2150,  2150,  2150,  2150,  2150,  2150,  2150,  2150,  -941,
    2150,  -941,   749,   871,   751,   757,   766,  -941,  -941,   872,
     754,  -941,  -941,  -941,   714,  -941,  -941,  -941,  -941,  -941,
     778,  -941,   464,  -941,   511,  -941,  -941,  -941,  -941,  -941,
     262,   262,   698,   698,   698,   515,   515,   515,   515,   515,
     515,   640,   640,   640,   640,   638,   812,   802,   811,   845,
     878,  -941,  -941,   891,  1595,  -941,  1516,  1516,   894,   226,
    -941,  1516,  -941,  -941,    39,    -7,  2150,  2150,  2150,  2150,
    2150,  2150,  2150,  2150,  2150,  2150,  2150,  2150,  2150,  2150,
    2150,  2150,  2150,  2150,  2150,  -941,   307,  -941,   897,   781,
    2150,   892,  2150,  2150,  -941,   683,   522,  -941,  -941,   797,
    -941,   902,   785,   525,  -941,  -941,  -941,  -941,  -941,   262,
     262,   698,   698,   698,   515,   515,   515,   515,   515,   515,
     640,   640,   640,   640,   638,   812,   802,   811,   845,   895,
    -941,  1595,  2150,  1595,   904,  1595,   907,  -941,  2039,  -941,
    2150,  -941,  -941,  2150,  -941,   906,  1516,  1516,  -941,  -941,
    -941,  2150,  2150,   950,   912,  2150,   793,  2263,  -941,   515,
     515,   515,   515,   515,   357,   357,   357,   357,   655,   843,
     838,   840,   884,   908,  -941,   909,   889,   918,   796,  1595,
     921,   919,  -941,   313,  -941,  -941,  -941,  -941,  -941,  -941,
    1595,   923,  -941,  2150,   949,   798,  -941,   977,  -941,  -941,
     916,  -941,  -941,  -941,  -941,  -941,   803,  -941,  2150,   900,
     901,  1595,  2150,  2150,  1595,   928,   935,  1595,  1595,  -941,
     937,   805,   939,  1595,  -941,  1595,   217,  2150,   237,   977,
    -941,   754,  1595,   807,  -941,  2150,  -941,  -941,   931,   941,
    1595,  -941,   942,  1595,   944,  -941,   946,  -941,  -941,  -941,
      37,   940,  -941,   977,  -941,   973,  -941,  1595,   943,  1595,
    -941,   948,  -941,   951,  1595,  -941,  1595,  1595,   961,   754,
    -941,  1595,  -941,  -941,  -941,   963,  1595,  1595,  -941,  -941,
    -941,  -941
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_int16 yydefact[] =
{
     297,     2,     3,     4,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    20,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     8,     0,     5,    23,
       6,     0,     0,     7,     0,    30,     0,     0,     0,     0,
     238,    21,    39,    22,    45,    61,    62,    66,    82,    83,
      88,    95,   102,   119,   136,   145,   151,   157,   163,   169,
     175,   181,   199,     0,   299,   201,   202,   203,   205,   206,
     207,   208,   209,   210,   211,   212,   213,   214,   215,   216,
     217,   204,     0,   298,   260,     0,   259,   253,     0,     0,
       0,    23,    34,    16,    43,    46,    35,   222,     0,   234,
       0,   232,   256,     0,   255,     0,   264,   263,    43,    59,
      60,    63,    80,    81,    84,    92,    98,   106,   126,   141,
     147,   153,   159,   165,   171,   177,   195,     0,    63,    70,
      69,     0,     0,     0,    71,     0,     0,     0,     0,   286,
     285,    72,    74,   218,     0,     0,    73,    75,     0,    32,
       0,     0,    31,    76,    77,    78,    79,     0,     0,     0,
      51,     0,     0,    52,    67,    68,   184,   185,   186,   187,
     188,   189,   190,   191,   194,   192,   193,   183,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   240,     0,   239,     1,   300,   262,   261,
       0,    63,   113,   131,   143,   149,   155,   161,   167,   173,
     179,   197,   254,     0,    43,    44,     0,     0,    17,     0,
       0,     0,    14,     0,     0,     0,    42,     0,   223,   221,
       0,   220,   235,   231,     0,   230,   258,   257,     0,    47,
       0,     0,    48,    64,    65,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     266,     0,   265,     0,     0,     0,     0,     0,   281,   280,
       0,     0,   219,   279,    24,    30,    26,    25,    28,    33,
      55,     0,    57,     0,    41,     0,    54,   182,    90,    89,
      91,    96,    97,   103,   104,   105,   125,   124,   122,   123,
     120,   121,   137,   138,   139,   140,   146,   152,   158,   164,
     170,     0,   200,   226,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   251,    38,     0,   293,     0,     0,     0,
       0,     0,     0,    18,     0,     0,    37,   236,   224,   233,
       0,     0,     0,    50,   178,    86,    85,    87,    93,    94,
      99,   100,   101,   112,   111,   109,   110,   107,   108,   127,
     128,   129,   130,   142,   148,   154,   160,   166,     0,   196,
       0,     0,     0,     0,     0,     0,   282,     0,    56,     0,
      40,    53,     0,     0,     0,   227,     0,   251,     0,    63,
     180,   118,   116,   117,   114,   115,   132,   133,   134,   135,
     144,   150,   156,   162,   168,     0,   198,   252,     0,     0,
       0,   295,     0,     0,    11,     0,     9,    10,    19,    15,
      36,   225,   295,     0,    49,     0,   241,     0,   245,   271,
     268,   267,     0,    27,    29,    58,   176,     0,   237,     0,
     228,     0,     0,     0,   251,   295,     0,   301,   302,   303,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   319,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   307,     0,   304,   322,   305,     0,     0,   306,
       0,   329,     0,     0,     0,     0,   537,     0,   320,   338,
     321,   344,   360,   361,   365,   381,   382,   387,   394,   401,
     418,   435,   444,   450,   456,   462,   468,   474,   480,   498,
       0,   596,   500,   501,   502,   504,   505,   506,   507,   508,
     509,   510,   511,   512,   513,   514,   515,   516,   503,   296,
     295,   294,     0,     0,     0,   295,   172,     0,     0,     0,
       0,   272,   273,     0,     0,     0,   229,   251,   248,   174,
       0,     0,   295,   559,     0,   558,   552,     0,     0,     0,
     322,   333,   315,   342,   345,   334,   521,     0,   533,     0,
     531,   555,     0,   554,     0,   563,   562,   342,   358,   359,
     362,   379,   380,   383,   391,   397,   405,   425,   440,   446,
     452,   458,   464,   470,   476,   494,     0,   362,   369,   368,
       0,     0,     0,   370,     0,     0,     0,     0,   585,   584,
     371,   373,   517,     0,     0,   372,   374,     0,   331,     0,
       0,   330,   375,   376,   377,   378,   289,     0,     0,     0,
     350,     0,     0,   351,   366,   367,   483,   484,   485,   486,
     487,   488,   489,   490,   493,   491,   492,   482,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   539,     0,   538,   597,     0,   295,     0,
     287,     0,   242,   244,   243,     0,     0,   269,   271,   274,
     283,   249,     0,     0,     0,   291,     0,   561,   560,     0,
     362,   412,   430,   442,   448,   454,   460,   466,   472,   478,
     496,   553,     0,   342,   343,     0,     0,   316,     0,     0,
       0,   313,     0,     0,     0,   341,     0,   522,   520,     0,
     519,   534,   530,     0,   529,   557,   556,     0,   346,     0,
       0,   347,   363,   364,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   565,
       0,   564,     0,     0,     0,     0,     0,   580,   579,     0,
       0,   518,   578,   323,   329,   325,   324,   327,   332,   354,
       0,   356,     0,   340,     0,   353,   481,   389,   388,   390,
     395,   396,   402,   403,   404,   424,   423,   421,   422,   419,
     420,   436,   437,   438,   439,   445,   451,   457,   463,   469,
       0,   499,   290,     0,   295,   288,   275,   277,     0,     0,
     250,     0,   246,   292,   525,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   550,   337,     0,   592,     0,     0,
       0,     0,     0,     0,   317,     0,     0,   336,   535,   523,
     532,     0,     0,     0,   349,   477,   385,   384,   386,   392,
     393,   398,   399,   400,   411,   410,   408,   409,   406,   407,
     426,   427,   428,   429,   441,   447,   453,   459,   465,     0,
     495,     0,     0,     0,     0,     0,     0,   581,     0,   355,
       0,   339,   352,     0,    12,     0,   276,   278,   270,   284,
     247,     0,     0,   526,     0,   550,     0,   362,   479,   417,
     415,   416,   413,   414,   431,   432,   433,   434,   443,   449,
     455,   461,   467,     0,   497,   551,     0,     0,     0,   594,
       0,     0,   310,     0,   308,   309,   318,   314,   335,   524,
     594,     0,   348,     0,   540,     0,   544,   570,   567,   566,
       0,   326,   328,   357,   475,    13,     0,   536,     0,   527,
       0,     0,     0,   550,   594,     0,     0,   595,   594,   593,
       0,     0,     0,   594,   471,     0,     0,     0,     0,   571,
     572,     0,     0,     0,   528,   550,   547,   473,     0,     0,
     594,   588,     0,   594,     0,   586,     0,   541,   543,   542,
       0,     0,   568,   570,   573,   582,   548,     0,     0,     0,
     590,     0,   589,     0,   594,   587,   574,   576,     0,     0,
     549,     0,   545,   591,   311,     0,   575,   577,   569,   583,
     546,   312
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -941,  -941,   645,  -941,  -941,     0,  -941,  -941,   715,  -941,
      22,  -941,   186,  -941,  -941,  -941,   -29,  -941,   479,  -941,
    -941,  -941,     3,   169,  -941,   105,  -941,   230,  -941,   725,
    -941,   138,   423,  -941,  -174,   668,  -941,    31,   679,  -941,
      40,   676,  -941,    42,   678,  -941,    68,   682,  -941,  -941,
    -941,  -941,  -941,  -941,  -941,   -35,  -305,  -941,   172,    18,
    -941,  -941,   -15,   -20,  -941,  -941,  -941,  -941,  -941,   791,
     -91,   566,  -941,  -941,  -941,  -941,  -407,  -941,  -941,  -941,
    -941,  -941,  -941,  -941,   319,  -941,   471,  -941,  -941,  -941,
    -941,  -941,  -941,  -941,  -235,  -441,  -941,   -23,  -941,   148,
    -941,  -941,  -432,  -941,  -941,   231,  -941,  -450,  -941,  -449,
    -941,  -941,  -941,  -511,  -941,   167,  -941,  -941,  -941,  -329,
     263,  -941,  -661,  -941,  -460,  -941,  -428,  -941,  -480,   -70,
    -941,  -679,   170,  -941,  -673,   166,  -941,  -663,   173,  -941,
    -660,   174,  -941,  -657,   165,  -941,  -941,  -941,  -941,  -941,
    -941,  -941,  -601,  -841,  -941,  -302,  -473,  -941,  -941,  -454,
    -493,  -941,  -941,  -941,  -941,  -941,   290,  -592,    46,  -941,
    -941,  -941,  -941,  -940,  -941,  -941,  -941,  -941,  -941,  -941,
    -941,     5,  -941,    49,  -941,  -941,  -941,  -941,  -941,  -941,
    -941,  -760,  -652,  -468
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    41,   232,   233,    92,    93,    43,   150,   151,   152,
     108,    44,   109,    45,   110,    46,   160,   301,   128,    47,
     112,    48,   113,   114,    50,   115,    51,   116,    52,   117,
      53,   118,   213,    54,   119,   214,    55,   120,   215,    56,
     121,   216,    57,   122,   217,    58,   123,   218,    59,   124,
     219,    60,   125,   220,    61,   126,   221,    62,   336,   437,
     222,    63,    64,    65,    66,    98,   334,    67,   100,   101,
     238,   415,    68,    69,    70,    71,   438,   223,    72,    73,
      74,    75,    76,   460,   570,   571,   572,   718,    77,    78,
      79,    80,    81,    96,   358,   517,    82,    83,   518,   751,
     752,   591,   592,   520,   649,   650,   651,   607,   521,   608,
     522,   609,   523,   660,   820,   627,   524,   611,   525,   612,
     613,   527,   614,   528,   615,   529,   616,   530,   617,   732,
     531,   618,   733,   532,   619,   734,   533,   620,   735,   534,
     621,   736,   535,   622,   737,   536,   623,   738,   537,   624,
     739,   538,   625,   740,   539,   867,   975,   741,   540,   541,
     542,   543,   597,   865,   544,   599,   600,   757,   953,   545,
     546,   547,   548,   976,   742,   549,   550,   551,   552,   553,
     998,  1028,  1029,  1030,  1053,   554,   555,   556,   557,   558,
     595,   889,  1016,   559
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      42,   132,   138,    49,   144,   637,   761,   902,   242,   519,
     471,   564,   663,   371,   626,  1010,    42,   163,   845,    49,
     519,   830,   831,   326,   636,   846,   958,    42,    94,   127,
      49,   420,   593,   594,   581,   643,   847,   647,   631,   137,
     848,   973,   974,   519,   849,    84,   435,   436,   139,   817,
     201,   413,   148,   182,   183,   278,   821,   360,   350,   416,
     519,   951,   361,   954,   701,   236,    87,   580,   207,   797,
     203,   519,   179,  1038,   102,   856,   417,   826,   281,   249,
     955,   252,   755,    42,   180,   181,    49,   226,    97,   208,
     149,   246,   638,   227,   202,  1058,   768,   239,   771,   279,
     393,   850,   351,   851,  1066,   706,    85,   800,   702,   468,
     224,  1007,   526,   798,   924,   414,   298,   909,   910,   707,
     440,   925,   302,   526,   711,   952,   978,   519,   519,   207,
     293,    86,   926,   519,   140,   103,   927,   743,   744,   204,
     928,   726,   583,   307,    42,    42,   526,    49,    49,   283,
     519,   285,   243,   286,   287,   898,   205,   802,   731,   804,
     104,   805,   806,   526,   280,   288,   240,   331,   579,   332,
     723,  1037,   713,   905,   526,   209,   303,   247,   639,   305,
     601,   129,   130,   241,   727,   822,   703,   134,   824,   706,
     812,   881,    95,   141,   142,   354,   758,   929,   765,   930,
     146,   147,   367,   584,   762,   153,   154,   155,   156,   799,
     563,   519,   519,   841,   842,   843,   844,   807,  1048,   178,
     374,   244,   678,  1021,   832,   833,   834,   716,   585,   327,
     526,   526,   885,   281,   281,   882,   526,   363,   245,   328,
      99,   602,   329,   891,   398,  1051,   399,   364,   892,   356,
     282,   289,   365,   526,   105,   704,   357,   131,   714,   835,
     836,   837,   838,   839,   840,   759,   603,   853,   372,   330,
     728,   406,   705,   763,   225,   133,   519,   451,   800,   717,
     896,   256,   760,   255,   766,    27,   800,   311,   312,   982,
     764,   984,   985,   257,   258,   801,   903,  1052,   356,   193,
     194,   195,   196,   808,  1049,   370,   394,   989,   774,   920,
     921,   922,   923,   135,   526,   526,   395,   937,   356,   396,
     911,   912,   913,   679,   444,   439,   446,   447,   775,   259,
     260,   322,   323,   324,   325,   680,   681,  1002,  1022,  1003,
     776,   777,  1004,   136,   894,   356,   397,   145,   308,   309,
     310,   197,   562,   418,   895,   914,   915,   916,   917,   918,
     919,   887,  1039,   887,   378,   379,  1042,   745,   888,   887,
     901,  1046,   464,   746,   465,   887,   977,   466,   337,   526,
     868,   295,  1020,   296,   281,   456,   410,   458,  1061,   461,
     199,  1063,  1024,   956,   338,   339,   869,   870,   198,   281,
      42,   411,    42,    49,    42,    49,   200,    49,   389,   390,
     391,   392,  1075,   945,   313,   314,   315,   206,   157,   457,
     566,   158,   519,   159,   237,   375,   376,   377,   281,   157,
     450,   467,   161,   248,   162,   340,   341,   871,   872,   731,
     959,   960,   961,   962,   963,   731,   731,   731,   731,   731,
     731,   731,   731,   731,   731,   731,   274,   578,   157,   995,
     275,   234,   157,   235,   276,   250,   277,   251,   187,   188,
     299,   657,    42,   284,   658,    49,   659,   994,  1006,   996,
     304,   999,   184,   185,   186,   189,   190,   575,   253,   254,
     111,   380,   381,   382,   306,   228,   229,   230,   231,   519,
     111,   519,   657,   519,   333,   661,   281,   662,   454,   657,
     346,  1017,   753,   111,   754,   657,   290,   291,   769,   814,
     770,   815,  1017,   347,   731,   526,   191,   192,   261,   262,
     263,   264,   265,   349,   800,  1033,   941,   348,  1055,   270,
     271,   272,   273,   684,   685,   686,  1017,   519,   266,   267,
    1017,   352,   712,   720,  1050,  1017,   353,  1036,   519,   721,
     780,   781,   782,   706,   294,   281,   211,    42,   400,   281,
      49,  1047,  1017,   355,    42,  1017,  1079,    49,  1056,   519,
     359,   800,   519,   942,   731,   519,   519,   715,   362,   268,
     269,   519,   800,   519,   988,   800,  1017,   992,  1076,  1077,
     519,   366,   526,  1070,   526,  1072,   526,   368,   519,   373,
     111,   519,   111,   412,   111,   111,   401,  1080,   342,   343,
     344,   345,   706,   706,   405,   519,   441,   519,   402,   281,
     111,   445,   519,   452,   519,   519,   111,   111,   455,   519,
     111,   687,   688,   459,   519,   519,   448,   229,   230,   231,
     526,   403,   281,   462,   610,   682,   683,   111,   689,   690,
     470,   526,   783,   784,   610,   693,   694,   695,   696,   747,
     748,   749,   750,   789,   790,   791,   792,   610,   469,   785,
     786,   111,   526,   111,   281,   526,   404,   281,   526,   526,
     873,   874,   875,   876,   526,   474,   526,   408,   409,   691,
     692,   442,   443,   526,   453,   443,   473,   860,   475,   862,
     560,   526,   561,   111,   526,   567,   111,   472,   281,   565,
     787,   788,    42,   569,    42,    49,   573,    49,   526,   111,
     526,   476,   443,   414,   111,   526,   582,   526,   526,   568,
     281,   577,   526,   986,   748,   749,   750,   526,   526,   574,
     281,   628,   629,   730,   709,   443,   586,   633,   111,   335,
     111,   722,   281,   640,   641,   426,   427,   428,   429,   596,
     645,   646,   778,   779,   598,   652,   653,   654,   655,   604,
     253,   254,   772,   773,   648,   166,   167,   168,   169,   170,
     171,   172,   173,   174,   175,   176,   630,   610,   632,   610,
     634,   610,   610,   964,   965,   966,   967,   809,   810,   813,
     800,   635,   212,   503,   111,   419,   656,   610,   931,   800,
     933,   800,   644,   177,   610,   610,   934,   800,   610,   697,
     419,   419,   111,   946,   947,   935,   800,   699,   111,   949,
     111,   111,     1,     2,     3,   610,   950,   939,   940,    88,
     980,   981,   698,    89,   991,   981,    42,    42,    15,    49,
      49,    42,  1011,   800,    49,  1015,   981,  1026,   800,   610,
     700,   610,  1032,   800,  1044,   981,  1057,   800,   708,   724,
     111,   710,   725,   756,   767,   794,   111,   796,   111,   803,
      26,   111,   111,   419,   793,   795,   111,   818,    90,   823,
      28,    91,    30,   825,   852,    33,   854,    34,   855,   857,
      35,   859,   316,   317,   318,   319,   320,   321,   861,   863,
     610,   878,   864,   610,   879,   877,   880,   883,   897,   886,
     890,   207,   207,   884,   111,   899,   610,   893,   904,   932,
     936,   610,   827,   828,   829,   943,    42,    42,   111,    49,
      49,   944,   419,   111,   948,   106,   979,     1,     2,     3,
     983,   990,   993,   997,    88,   610,  1005,   610,    89,  1000,
      12,    13,  1008,    15,  1009,  1012,  1013,  1014,    18,   800,
    1018,  1019,  1023,  1025,  1027,  1031,   952,  1040,  1035,   383,
     384,   385,   386,   387,   388,  1041,  1043,    24,    25,  1045,
    1059,  1060,  1062,  1064,  1069,    26,  1065,  1067,  1073,   449,
     407,  1074,  1071,    90,   430,    28,    91,    30,    31,    32,
      33,  1078,    34,  1081,   432,    35,   431,   433,    36,    37,
      38,    39,   434,   610,   957,   369,   576,   858,   906,   907,
     908,   107,   719,   987,   969,   938,   972,   968,   111,   957,
     957,   610,   970,   900,   971,  1034,   111,   610,  1068,   610,
     610,   212,   421,   422,   423,   424,   425,   212,   212,   212,
     212,   212,   212,   212,   212,   212,   212,   212,  1054,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   605,     0,
     477,   478,   479,     0,     0,     0,     0,   587,     0,   610,
       0,   588,     0,   488,   489,   610,   491,   610,     0,     0,
     610,   494,     0,     0,     0,     0,     0,     0,   610,   957,
       0,     0,   610,     0,     0,   477,   478,   479,     0,     0,
     500,   501,   587,     0,     0,     0,   588,     0,   502,   212,
       0,   491,     0,     0,     0,     0,   589,     0,   504,   590,
     506,   507,   508,   509,     0,   510,     0,     0,   511,     0,
     610,   512,   513,   514,   515,     0,     0,     0,     0,     0,
       0,     0,     0,   502,   606,   610,     0,     0,     0,   957,
     610,   589,     0,   504,   590,   506,     0,     0,   509,     0,
     510,     0,     0,   511,   610,     0,     0,     0,   212,     0,
       0,     0,   610,     1,     2,     3,     4,     0,     0,     5,
       6,     7,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    17,     0,     0,    18,    19,    20,     0,    21,    22,
       0,     0,    23,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    24,    25,     0,     0,     0,     0,     0,
       0,    26,     0,     0,     0,     0,     0,     0,     0,    27,
     143,    28,    29,    30,    31,    32,    33,     0,    34,     0,
       0,    35,     0,     0,    36,    37,    38,    39,     0,     0,
       0,     0,     1,     2,     3,     4,     0,    40,     5,     6,
       7,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      17,     0,     0,    18,    19,    20,     0,    21,    22,     0,
       0,    23,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    24,    25,     0,     0,     0,     0,     0,     0,
      26,     0,     0,     0,     0,     0,     0,     0,    27,   292,
      28,    29,    30,    31,    32,    33,     0,    34,     0,     0,
      35,     0,     0,    36,    37,    38,    39,     0,     0,     0,
       0,   477,   478,   479,   480,     0,    40,   481,   482,   483,
     484,   485,   486,   487,   488,   489,   490,   491,   492,   493,
       0,     0,   494,   495,   496,     0,   497,   498,     0,     0,
     499,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   500,   501,     0,     0,     0,     0,     0,     0,   502,
       0,     0,     0,     0,     0,     0,     0,   503,   642,   504,
     505,   506,   507,   508,   509,     0,   510,     0,     0,   511,
       0,     0,   512,   513,   514,   515,     0,     0,     0,     0,
     477,   478,   479,   480,     0,   516,   481,   482,   483,   484,
     485,   486,   487,   488,   489,   490,   491,   492,   493,     0,
       0,   494,   495,   496,     0,   497,   498,     0,     0,   499,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     500,   501,     0,     0,     0,     0,     0,     0,   502,     0,
       0,     0,     0,     0,     0,     0,   503,   811,   504,   505,
     506,   507,   508,   509,     0,   510,     0,     0,   511,     0,
       0,   512,   513,   514,   515,     0,     0,     0,     0,     1,
       2,     3,     4,     0,   516,     5,     6,     7,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,     0,     0,
      18,    19,    20,     0,    21,    22,     0,     0,    23,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    24,
      25,     0,     0,     0,     0,     0,     0,    26,     0,     0,
       0,     0,     0,     0,     0,    27,     0,    28,    29,    30,
      31,    32,    33,     0,    34,     0,     0,    35,     0,     0,
      36,    37,    38,    39,     0,     0,     0,     0,   477,   478,
     479,   480,     0,    40,   481,   482,   483,   484,   485,   486,
     487,   488,   489,   490,   491,   492,   493,     0,     0,   494,
     495,   496,     0,   497,   498,     0,     0,   499,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   500,   501,
       0,     0,     0,     0,     0,     0,   502,     0,     0,     0,
       0,     0,     0,     0,   503,     0,   504,   505,   506,   507,
     508,   509,     0,   510,     0,     0,   511,     0,     0,   512,
     513,   514,   515,     1,     2,     3,     0,     0,     0,     0,
      88,   210,   516,     0,    89,     0,    12,    13,     0,    15,
       0,     0,     0,     0,    18,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     1,
       2,     3,     0,    24,    25,     0,    88,     0,     0,     0,
      89,    26,    12,    13,     0,    15,     0,     0,     0,    90,
      18,    28,    91,    30,    31,    32,    33,     0,    34,     0,
       0,    35,     0,     0,    36,    37,    38,    39,     0,    24,
      25,     0,     0,     0,     0,     0,     0,    26,     0,     0,
       0,     0,     0,     0,     0,    90,     0,    28,    91,    30,
      31,    32,    33,     0,    34,     0,     0,    35,   297,     0,
      36,    37,    38,    39,     1,     2,     3,     0,     0,     0,
       0,    88,     0,     0,     0,    89,     0,    12,    13,     0,
      15,     0,     0,     0,     0,    18,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       1,     2,     3,     0,    24,    25,     0,    88,     0,     0,
       0,    89,    26,    12,    13,     0,    15,     0,     0,     0,
      90,    18,    28,    91,    30,    31,    32,    33,     0,    34,
     300,     0,    35,     0,     0,    36,    37,    38,    39,     0,
      24,    25,     0,     0,     0,     0,     0,     0,    26,     0,
       0,     0,     0,     0,     0,     0,    90,     0,    28,    91,
      30,    31,    32,    33,     0,    34,     0,     0,    35,   463,
       0,    36,    37,    38,    39,   477,   478,   479,     0,     0,
       0,     0,   587,   729,     0,     0,   588,     0,   488,   489,
       0,   491,     0,     0,     0,     0,   494,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   477,   478,   479,     0,   500,   501,     0,   587,     0,
       0,     0,   588,   502,   488,   489,     0,   491,     0,     0,
       0,   589,   494,   504,   590,   506,   507,   508,   509,     0,
     510,     0,     0,   511,     0,     0,   512,   513,   514,   515,
       0,   500,   501,     0,     0,     0,     0,     0,     0,   502,
       0,     0,     0,     0,     0,     0,     0,   589,     0,   504,
     590,   506,   507,   508,   509,     0,   510,     0,     0,   511,
     816,     0,   512,   513,   514,   515,   477,   478,   479,     0,
       0,     0,     0,   587,     0,     0,     0,   588,     0,   488,
     489,     0,   491,     0,     0,     0,     0,   494,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   477,   478,   479,     0,   500,   501,     0,   587,
       0,     0,     0,   588,   502,   488,   489,     0,   491,     0,
       0,     0,   589,   494,   504,   590,   506,   507,   508,   509,
       0,   510,   819,     0,   511,     0,     0,   512,   513,   514,
     515,     0,   500,   501,     0,     0,     0,     0,     0,     0,
     502,     0,     0,     0,     0,     0,     0,     0,   589,     0,
     504,   590,   506,   507,   508,   509,     0,   510,     0,     0,
     511,  1001,     0,   512,   513,   514,   515,     1,     2,     3,
       0,     0,     0,     0,    88,     0,     0,     0,    89,     0,
      12,    13,     0,    15,     0,     0,     0,     0,    18,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   477,   478,   479,     0,    24,    25,     0,
     587,     0,     0,     0,   588,    26,   488,   489,     0,   491,
       0,     0,     0,    90,   494,    28,    91,    30,    31,    32,
      33,     0,    34,     0,     0,    35,     0,     0,    36,    37,
      38,    39,     0,   500,   501,     0,     0,     0,     0,     0,
       0,   502,     0,     0,     0,     0,     0,   866,     0,   589,
       0,   504,   590,   506,   507,   508,   509,     0,   510,     0,
       0,   511,     0,     0,   512,   513,   514,   515,   772,   773,
       0,     0,     0,   666,   667,   668,   669,   670,   671,   672,
     673,   674,   675,   676,   164,   165,     0,     0,     0,   166,
     167,   168,   169,   170,   171,   172,   173,   174,   175,   176,
       0,     0,     0,     0,     0,     0,     0,   253,   254,     0,
       0,   677,   166,   167,   168,   169,   170,   171,   172,   173,
     174,   175,   176,   664,   665,     0,     0,   177,   666,   667,
     668,   669,   670,   671,   672,   673,   674,   675,   676,     0,
       0,     0,     0,     0,     0,     0,   772,   773,     0,     0,
     177,   666,   667,   668,   669,   670,   671,   672,   673,   674,
     675,   676,     0,     0,     0,     0,   677,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   677
};

static const yytype_int16 yycheck[] =
{
       0,    16,    22,     0,    27,   498,   598,   767,    99,   441,
     417,   452,   523,   248,   487,   955,    16,    46,   697,    16,
     452,   682,   683,   197,   497,   698,   867,    27,     6,    11,
      27,   336,   482,   482,   475,   503,   699,   510,   492,    21,
     700,   882,   883,   475,   701,     1,   351,   352,     1,   650,
      41,    22,    34,    74,    75,    41,   657,    62,    41,    70,
     492,    22,    67,    70,    41,    94,    68,   474,    83,    41,
       1,   503,    66,  1013,     1,    67,    87,   678,    70,   108,
      87,   110,   593,    83,    78,    79,    83,    62,    62,     1,
      70,     1,     1,    68,    85,  1035,   607,     1,   609,    85,
     274,   702,    85,   704,    67,   559,    62,    70,    85,   414,
      88,   952,   441,    85,   793,    86,   151,   778,   779,   560,
     355,   794,   157,   452,   565,    86,   886,   559,   560,   144,
     145,    87,   795,   565,    87,    62,   796,   587,   587,    70,
     797,   582,     1,   178,   144,   145,   475,   144,   145,   131,
     582,   133,     1,   135,   136,   756,    87,   630,   586,   632,
      87,   634,   635,   492,     1,     1,    70,   202,   473,   204,
     577,  1012,     1,   774,   503,    87,   158,    87,    87,   161,
       1,    12,    13,    87,     1,   658,     1,    18,   661,   643,
     644,    41,     6,    24,    25,   224,     1,   798,     1,   800,
      31,    32,   237,    62,     1,    36,    37,    38,    39,     1,
     445,   643,   644,   693,   694,   695,   696,     1,     1,    47,
     255,    70,   524,   983,   684,   685,   686,     8,    87,   198,
     559,   560,   743,    70,    70,    85,   565,    60,    87,   199,
      62,    62,   200,    62,   279,     8,   281,    70,    67,    62,
      87,    87,   234,   582,    62,    70,    69,    68,    87,   687,
     688,   689,   690,   691,   692,    70,    87,   708,   250,   201,
      87,   291,    87,    70,    88,    68,   708,   368,    70,    60,
     753,    66,    87,   111,    87,    59,    70,   182,   183,   890,
      87,   892,   893,    78,    79,    87,   769,    60,    62,    35,
      36,    37,    38,    87,    87,    69,   275,   899,   610,   789,
     790,   791,   792,    68,   643,   644,   276,   810,    62,   277,
     780,   781,   782,    66,   359,    69,   361,   362,    66,    74,
      75,   193,   194,   195,   196,    78,    79,   938,   990,   940,
      78,    79,   943,    68,    60,    62,   278,    67,   179,   180,
     181,    82,    69,   335,    70,   783,   784,   785,   786,   787,
     788,    62,  1014,    62,   259,   260,  1018,    62,    69,    62,
      69,  1023,   407,    68,   409,    62,    69,   412,    23,   708,
      23,    70,    69,    72,    70,   400,    72,   402,  1040,   404,
      84,  1043,   993,   866,    39,    40,    39,    40,    83,    70,
     400,    72,   402,   400,   404,   402,    42,   404,   270,   271,
     272,   273,  1064,   854,   184,   185,   186,     0,    68,   401,
     455,    71,   854,    73,    86,   256,   257,   258,    70,    68,
      72,   413,    71,    68,    73,    80,    81,    80,    81,   867,
     868,   869,   870,   871,   872,   873,   874,   875,   876,   877,
     878,   879,   880,   881,   882,   883,    82,   472,    68,   932,
      83,    71,    68,    73,    84,    71,    42,    73,    22,    23,
      70,    68,   472,    21,    71,   472,    73,   931,   951,   933,
      62,   935,    45,    46,    47,    39,    40,   469,    43,    44,
      11,   261,   262,   263,    62,    60,    61,    62,    63,   931,
      21,   933,    68,   935,    62,    71,    70,    73,    72,    68,
      82,   979,    71,    34,    73,    68,    30,    31,    71,    70,
      73,    72,   990,    83,   952,   854,    80,    81,    45,    46,
      47,    22,    23,    42,    70,  1008,    72,    84,  1031,    35,
      36,    37,    38,    45,    46,    47,  1014,   979,    39,    40,
    1018,    70,   567,   573,  1027,  1023,    87,  1011,   990,   574,
      45,    46,    47,  1017,    69,    70,    87,   567,    69,    70,
     567,  1025,  1040,    68,   574,  1043,  1069,   574,  1032,  1011,
      67,    70,  1014,    72,  1012,  1017,  1018,   569,    67,    80,
      81,  1023,    70,  1025,    72,    70,  1064,    72,  1066,  1067,
    1032,    62,   931,  1057,   933,  1059,   935,    62,  1040,    62,
     131,  1043,   133,    67,   135,   136,    68,  1071,    35,    36,
      37,    38,  1076,  1077,    68,  1057,    59,  1059,    69,    70,
     151,    68,  1064,    59,  1066,  1067,   157,   158,    67,  1071,
     161,    22,    23,    59,  1076,  1077,    60,    61,    62,    63,
     979,    69,    70,    62,   487,    74,    75,   178,    39,    40,
      62,   990,    22,    23,   497,    35,    36,    37,    38,    60,
      61,    62,    63,    35,    36,    37,    38,   510,    22,    39,
      40,   202,  1011,   204,    70,  1014,    69,    70,  1017,  1018,
      35,    36,    37,    38,  1023,    87,  1025,    69,    70,    80,
      81,    69,    70,  1032,    69,    70,    67,   722,    59,   724,
      59,  1040,    62,   234,  1043,    34,   237,    69,    70,    59,
      80,    81,   722,     7,   724,   722,    69,   724,  1057,   250,
    1059,    69,    70,    86,   255,  1064,    59,  1066,  1067,    69,
      70,    87,  1071,    60,    61,    62,    63,  1076,  1077,    69,
      70,   488,   489,   586,    69,    70,    68,   494,   279,    22,
     281,    69,    70,   500,   501,   342,   343,   344,   345,    62,
     507,   508,    74,    75,    62,   512,   513,   514,   515,    62,
      43,    44,    43,    44,    70,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    68,   630,    68,   632,
      68,   634,   635,   873,   874,   875,   876,    30,    31,    69,
      70,    68,    87,    59,   335,   336,    60,   650,    69,    70,
      69,    70,    67,    86,   657,   658,    69,    70,   661,    82,
     351,   352,   353,   856,   857,    69,    70,    84,   359,   859,
     361,   362,     3,     4,     5,   678,   861,    69,    70,    10,
      69,    70,    83,    14,    69,    70,   856,   857,    19,   856,
     857,   861,    69,    70,   861,    69,    70,    69,    70,   702,
      42,   704,    69,    70,    69,    70,    69,    70,    59,    69,
     401,    60,    60,    86,    68,    83,   407,    42,   409,    21,
      51,   412,   413,   414,    82,    84,   417,    70,    59,    62,
      61,    62,    63,    62,    60,    66,    59,    68,    60,    67,
      71,    31,   187,   188,   189,   190,   191,   192,    69,    60,
     753,    83,    62,   756,    84,    82,    42,    70,    62,    68,
      67,   946,   947,    87,   455,    62,   769,    67,    62,    68,
      68,   774,   679,   680,   681,    67,   946,   947,   469,   946,
     947,    60,   473,   474,    60,     1,    59,     3,     4,     5,
      68,    59,    67,    59,    10,   798,    60,   800,    14,    62,
      16,    17,    22,    19,    62,    67,    87,    59,    24,    70,
      59,    62,    59,    34,     7,    69,    86,    59,    87,   264,
     265,   266,   267,   268,   269,    60,    59,    43,    44,    60,
      69,    60,    60,    59,    31,    51,    60,    67,    60,   364,
     295,    60,    69,    59,   346,    61,    62,    63,    64,    65,
      66,    60,    68,    60,   348,    71,   347,   349,    74,    75,
      76,    77,   350,   866,   867,   244,   470,   718,   775,   776,
     777,    87,   571,   895,   878,   814,   881,   877,   569,   882,
     883,   884,   879,   763,   880,  1009,   577,   890,  1053,   892,
     893,   336,   337,   338,   339,   340,   341,   342,   343,   344,
     345,   346,   347,   348,   349,   350,   351,   352,  1029,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     1,    -1,
       3,     4,     5,    -1,    -1,    -1,    -1,    10,    -1,   932,
      -1,    14,    -1,    16,    17,   938,    19,   940,    -1,    -1,
     943,    24,    -1,    -1,    -1,    -1,    -1,    -1,   951,   952,
      -1,    -1,   955,    -1,    -1,     3,     4,     5,    -1,    -1,
      43,    44,    10,    -1,    -1,    -1,    14,    -1,    51,   414,
      -1,    19,    -1,    -1,    -1,    -1,    59,    -1,    61,    62,
      63,    64,    65,    66,    -1,    68,    -1,    -1,    71,    -1,
     993,    74,    75,    76,    77,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    51,    87,  1008,    -1,    -1,    -1,  1012,
    1013,    59,    -1,    61,    62,    63,    -1,    -1,    66,    -1,
      68,    -1,    -1,    71,  1027,    -1,    -1,    -1,   473,    -1,
      -1,    -1,  1035,     3,     4,     5,     6,    -1,    -1,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    -1,    -1,    24,    25,    26,    -1,    28,    29,
      -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    43,    44,    -1,    -1,    -1,    -1,    -1,
      -1,    51,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    59,
      60,    61,    62,    63,    64,    65,    66,    -1,    68,    -1,
      -1,    71,    -1,    -1,    74,    75,    76,    77,    -1,    -1,
      -1,    -1,     3,     4,     5,     6,    -1,    87,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    -1,    -1,    24,    25,    26,    -1,    28,    29,    -1,
      -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    43,    44,    -1,    -1,    -1,    -1,    -1,    -1,
      51,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    59,    60,
      61,    62,    63,    64,    65,    66,    -1,    68,    -1,    -1,
      71,    -1,    -1,    74,    75,    76,    77,    -1,    -1,    -1,
      -1,     3,     4,     5,     6,    -1,    87,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      -1,    -1,    24,    25,    26,    -1,    28,    29,    -1,    -1,
      32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    43,    44,    -1,    -1,    -1,    -1,    -1,    -1,    51,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    59,    60,    61,
      62,    63,    64,    65,    66,    -1,    68,    -1,    -1,    71,
      -1,    -1,    74,    75,    76,    77,    -1,    -1,    -1,    -1,
       3,     4,     5,     6,    -1,    87,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    -1,
      -1,    24,    25,    26,    -1,    28,    29,    -1,    -1,    32,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      43,    44,    -1,    -1,    -1,    -1,    -1,    -1,    51,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    59,    60,    61,    62,
      63,    64,    65,    66,    -1,    68,    -1,    -1,    71,    -1,
      -1,    74,    75,    76,    77,    -1,    -1,    -1,    -1,     3,
       4,     5,     6,    -1,    87,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    -1,    -1,
      24,    25,    26,    -1,    28,    29,    -1,    -1,    32,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    43,
      44,    -1,    -1,    -1,    -1,    -1,    -1,    51,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    59,    -1,    61,    62,    63,
      64,    65,    66,    -1,    68,    -1,    -1,    71,    -1,    -1,
      74,    75,    76,    77,    -1,    -1,    -1,    -1,     3,     4,
       5,     6,    -1,    87,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    -1,    -1,    24,
      25,    26,    -1,    28,    29,    -1,    -1,    32,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    43,    44,
      -1,    -1,    -1,    -1,    -1,    -1,    51,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    59,    -1,    61,    62,    63,    64,
      65,    66,    -1,    68,    -1,    -1,    71,    -1,    -1,    74,
      75,    76,    77,     3,     4,     5,    -1,    -1,    -1,    -1,
      10,    11,    87,    -1,    14,    -1,    16,    17,    -1,    19,
      -1,    -1,    -1,    -1,    24,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,
       4,     5,    -1,    43,    44,    -1,    10,    -1,    -1,    -1,
      14,    51,    16,    17,    -1,    19,    -1,    -1,    -1,    59,
      24,    61,    62,    63,    64,    65,    66,    -1,    68,    -1,
      -1,    71,    -1,    -1,    74,    75,    76,    77,    -1,    43,
      44,    -1,    -1,    -1,    -1,    -1,    -1,    51,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    59,    -1,    61,    62,    63,
      64,    65,    66,    -1,    68,    -1,    -1,    71,    72,    -1,
      74,    75,    76,    77,     3,     4,     5,    -1,    -1,    -1,
      -1,    10,    -1,    -1,    -1,    14,    -1,    16,    17,    -1,
      19,    -1,    -1,    -1,    -1,    24,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
       3,     4,     5,    -1,    43,    44,    -1,    10,    -1,    -1,
      -1,    14,    51,    16,    17,    -1,    19,    -1,    -1,    -1,
      59,    24,    61,    62,    63,    64,    65,    66,    -1,    68,
      69,    -1,    71,    -1,    -1,    74,    75,    76,    77,    -1,
      43,    44,    -1,    -1,    -1,    -1,    -1,    -1,    51,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    59,    -1,    61,    62,
      63,    64,    65,    66,    -1,    68,    -1,    -1,    71,    72,
      -1,    74,    75,    76,    77,     3,     4,     5,    -1,    -1,
      -1,    -1,    10,    11,    -1,    -1,    14,    -1,    16,    17,
      -1,    19,    -1,    -1,    -1,    -1,    24,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,     3,     4,     5,    -1,    43,    44,    -1,    10,    -1,
      -1,    -1,    14,    51,    16,    17,    -1,    19,    -1,    -1,
      -1,    59,    24,    61,    62,    63,    64,    65,    66,    -1,
      68,    -1,    -1,    71,    -1,    -1,    74,    75,    76,    77,
      -1,    43,    44,    -1,    -1,    -1,    -1,    -1,    -1,    51,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    59,    -1,    61,
      62,    63,    64,    65,    66,    -1,    68,    -1,    -1,    71,
      72,    -1,    74,    75,    76,    77,     3,     4,     5,    -1,
      -1,    -1,    -1,    10,    -1,    -1,    -1,    14,    -1,    16,
      17,    -1,    19,    -1,    -1,    -1,    -1,    24,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     3,     4,     5,    -1,    43,    44,    -1,    10,
      -1,    -1,    -1,    14,    51,    16,    17,    -1,    19,    -1,
      -1,    -1,    59,    24,    61,    62,    63,    64,    65,    66,
      -1,    68,    69,    -1,    71,    -1,    -1,    74,    75,    76,
      77,    -1,    43,    44,    -1,    -1,    -1,    -1,    -1,    -1,
      51,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    59,    -1,
      61,    62,    63,    64,    65,    66,    -1,    68,    -1,    -1,
      71,    72,    -1,    74,    75,    76,    77,     3,     4,     5,
      -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,    14,    -1,
      16,    17,    -1,    19,    -1,    -1,    -1,    -1,    24,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,     3,     4,     5,    -1,    43,    44,    -1,
      10,    -1,    -1,    -1,    14,    51,    16,    17,    -1,    19,
      -1,    -1,    -1,    59,    24,    61,    62,    63,    64,    65,
      66,    -1,    68,    -1,    -1,    71,    -1,    -1,    74,    75,
      76,    77,    -1,    43,    44,    -1,    -1,    -1,    -1,    -1,
      -1,    51,    -1,    -1,    -1,    -1,    -1,    22,    -1,    59,
      -1,    61,    62,    63,    64,    65,    66,    -1,    68,    -1,
      -1,    71,    -1,    -1,    74,    75,    76,    77,    43,    44,
      -1,    -1,    -1,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    43,    44,    -1,    -1,    -1,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    58,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    43,    44,    -1,
      -1,    86,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    57,    58,    43,    44,    -1,    -1,    86,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    58,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    43,    44,    -1,    -1,
      86,    48,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    58,    -1,    -1,    -1,    -1,    86,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    86
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_int16 yystos[] =
{
       0,     3,     4,     5,     6,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    24,    25,
      26,    28,    29,    32,    43,    44,    51,    59,    61,    62,
      63,    64,    65,    66,    68,    71,    74,    75,    76,    77,
      87,    89,    93,    94,    99,   101,   103,   107,   109,   110,
     112,   114,   116,   118,   121,   124,   127,   130,   133,   136,
     139,   142,   145,   149,   150,   151,   152,   155,   160,   161,
     162,   163,   166,   167,   168,   169,   170,   176,   177,   178,
     179,   180,   184,   185,     1,    62,    87,    68,    10,    14,
      59,    62,    92,    93,    98,   100,   181,    62,   153,    62,
     156,   157,     1,    62,    87,    62,     1,    87,    98,   100,
     102,   106,   108,   110,   111,   113,   115,   117,   119,   122,
     125,   128,   131,   134,   137,   140,   143,   147,   106,   111,
     111,    68,   150,    68,   111,    68,    68,   147,   151,     1,
      87,   111,   111,    60,   185,    67,   111,   111,   147,    70,
      95,    96,    97,   111,   111,   111,   111,    68,    71,    73,
     104,    71,    73,   104,    43,    44,    48,    49,    50,    51,
      52,    53,    54,    55,    56,    57,    58,    86,   146,    66,
      78,    79,    74,    75,    45,    46,    47,    22,    23,    39,
      40,    80,    81,    35,    36,    37,    38,    82,    83,    84,
      42,    41,    85,     1,    70,    87,     0,   150,     1,    87,
      11,   106,   117,   120,   123,   126,   129,   132,   135,   138,
     141,   144,   148,   165,    98,   100,    62,    68,    60,    61,
      62,    63,    90,    91,    71,    73,   104,    86,   158,     1,
      70,    87,   158,     1,    70,    87,     1,    87,    68,   104,
      71,    73,   104,    43,    44,   146,    66,    78,    79,    74,
      75,    45,    46,    47,    22,    23,    39,    40,    80,    81,
      35,    36,    37,    38,    82,    83,    84,    42,    41,    85,
       1,    70,    87,   147,    21,   147,   147,   147,     1,    87,
      30,    31,    60,   150,    69,    70,    72,    72,   143,    70,
      69,   105,   143,   147,    62,   147,    62,   143,   111,   111,
     111,   113,   113,   115,   115,   115,   117,   117,   117,   117,
     117,   117,   119,   119,   119,   119,   122,   125,   128,   131,
     134,   143,   143,    62,   154,    22,   146,    23,    39,    40,
      80,    81,    35,    36,    37,    38,    82,    83,    84,    42,
      41,    85,    70,    87,   104,    68,    62,    69,   182,    67,
      62,    67,    67,    60,    70,   147,    62,   143,    62,   157,
      69,   182,   147,    62,   143,   111,   111,   111,   113,   113,
     115,   115,   115,   117,   117,   117,   117,   117,   117,   119,
     119,   119,   119,   122,   125,   128,   131,   134,   143,   143,
      69,    68,    69,    69,    69,    68,   151,    96,    69,    70,
      72,    72,    67,    22,    86,   159,    70,    87,   147,   106,
     144,   117,   117,   117,   117,   117,   120,   120,   120,   120,
     123,   126,   129,   132,   135,   144,   144,   147,   164,    69,
     182,    59,    69,    70,   143,    68,   143,   143,    60,    90,
      72,   158,    59,    69,    72,    67,   150,   147,   150,    59,
     171,   150,    62,    72,   143,   143,   143,   147,   144,    22,
      62,   164,    69,    67,    87,    59,    69,     3,     4,     5,
       6,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    24,    25,    26,    28,    29,    32,
      43,    44,    51,    59,    61,    62,    63,    64,    65,    66,
      68,    71,    74,    75,    76,    77,    87,   183,   186,   190,
     191,   196,   198,   200,   204,   206,   207,   209,   211,   213,
     215,   218,   221,   224,   227,   230,   233,   236,   239,   242,
     246,   247,   248,   249,   252,   257,   258,   259,   260,   263,
     264,   265,   266,   267,   273,   274,   275,   276,   277,   281,
      59,    62,    69,   182,   183,    59,   143,    34,    69,     7,
     172,   173,   174,    69,    69,   147,   159,    87,   150,   144,
     164,   183,    59,     1,    62,    87,    68,    10,    14,    59,
      62,   189,   190,   195,   197,   278,    62,   250,    62,   253,
     254,     1,    62,    87,    62,     1,    87,   195,   197,   199,
     203,   205,   207,   208,   210,   212,   214,   216,   219,   222,
     225,   228,   231,   234,   237,   240,   244,   203,   208,   208,
      68,   247,    68,   208,    68,    68,   244,   248,     1,    87,
     208,   208,    60,   281,    67,   208,   208,   244,    70,   192,
     193,   194,   208,   208,   208,   208,    60,    68,    71,    73,
     201,    71,    73,   201,    43,    44,    48,    49,    50,    51,
      52,    53,    54,    55,    56,    57,    58,    86,   243,    66,
      78,    79,    74,    75,    45,    46,    47,    22,    23,    39,
      40,    80,    81,    35,    36,    37,    38,    82,    83,    84,
      42,    41,    85,     1,    70,    87,   247,   183,    59,    69,
      60,   183,   150,     1,    87,   147,     8,    60,   175,   174,
     151,   150,    69,   164,    69,    60,   183,     1,    87,    11,
     203,   214,   217,   220,   223,   226,   229,   232,   235,   238,
     241,   245,   262,   195,   197,    62,    68,    60,    61,    62,
      63,   187,   188,    71,    73,   201,    86,   255,     1,    70,
      87,   255,     1,    70,    87,     1,    87,    68,   201,    71,
      73,   201,    43,    44,   243,    66,    78,    79,    74,    75,
      45,    46,    47,    22,    23,    39,    40,    80,    81,    35,
      36,    37,    38,    82,    83,    84,    42,    41,    85,     1,
      70,    87,   244,    21,   244,   244,   244,     1,    87,    30,
      31,    60,   247,    69,    70,    72,    72,   240,    70,    69,
     202,   240,   244,    62,   244,    62,   240,   208,   208,   208,
     210,   210,   212,   212,   212,   214,   214,   214,   214,   214,
     214,   216,   216,   216,   216,   219,   222,   225,   228,   231,
     240,   240,    60,   183,    59,    60,    67,    67,   172,    31,
     150,    69,   150,    60,    62,   251,    22,   243,    23,    39,
      40,    80,    81,    35,    36,    37,    38,    82,    83,    84,
      42,    41,    85,    70,    87,   201,    68,    62,    69,   279,
      67,    62,    67,    67,    60,    70,   244,    62,   240,    62,
     254,    69,   279,   244,    62,   240,   208,   208,   208,   210,
     210,   212,   212,   212,   214,   214,   214,   214,   214,   214,
     216,   216,   216,   216,   219,   222,   225,   228,   231,   240,
     240,    69,    68,    69,    69,    69,    68,   248,   193,    69,
      70,    72,    72,    67,    60,   183,   185,   185,    60,   151,
     150,    22,    86,   256,    70,    87,   244,   203,   241,   214,
     214,   214,   214,   214,   217,   217,   217,   217,   220,   223,
     226,   229,   232,   241,   241,   244,   261,    69,   279,    59,
      69,    70,   240,    68,   240,   240,    60,   187,    72,   255,
      59,    69,    72,    67,   247,   244,   247,    59,   268,   247,
      62,    72,   240,   240,   240,    60,   244,   241,    22,    62,
     261,    69,    67,    87,    59,    69,   280,   281,    59,    62,
      69,   279,   280,    59,   240,    34,    69,     7,   269,   270,
     271,    69,    69,   244,   256,    87,   247,   241,   261,   280,
      59,    60,   280,    59,    69,    60,   280,   247,     1,    87,
     244,     8,    60,   272,   271,   248,   247,    69,   261,    69,
      60,   280,    60,   280,    59,    60,    67,    67,   269,    31,
     247,    69,   247,    60,    60,   280,   281,   281,    60,   248,
     247,    60
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_int16 yyr1[] =
{
       0,    88,    89,    89,    89,    89,    89,    89,    89,    90,
      90,    90,    90,    90,    91,    91,    92,    92,    92,    92,
      93,    93,    93,    93,    93,    94,    94,    94,    95,    95,
      96,    96,    97,    97,    98,    98,    98,    98,    98,    99,
      99,    99,    99,   100,   100,   101,   101,   102,   102,   102,
     102,   103,   103,   103,   103,   104,   104,   105,   105,   106,
     106,   107,   107,   108,   108,   108,   109,   109,   109,   110,
     110,   110,   110,   110,   110,   110,   110,   110,   110,   110,
     111,   111,   112,   112,   113,   113,   113,   113,   114,   114,
     114,   114,   115,   115,   115,   116,   116,   116,   117,   117,
     117,   117,   118,   118,   118,   118,   119,   119,   119,   119,
     119,   119,   119,   120,   120,   120,   120,   120,   120,   121,
     121,   121,   121,   121,   121,   121,   122,   122,   122,   122,
     122,   123,   123,   123,   123,   123,   124,   124,   124,   124,
     124,   125,   125,   126,   126,   127,   127,   128,   128,   129,
     129,   130,   130,   131,   131,   132,   132,   133,   133,   134,
     134,   135,   135,   136,   136,   137,   137,   138,   138,   139,
     139,   140,   140,   141,   141,   142,   142,   143,   143,   144,
     144,   145,   145,   146,   146,   146,   146,   146,   146,   146,
     146,   146,   146,   146,   146,   147,   147,   148,   148,   149,
     149,   150,   150,   150,   150,   150,   150,   150,   150,   150,
     150,   150,   150,   150,   150,   150,   150,   150,   151,   151,
     152,   152,   153,   153,   153,   153,   154,   154,   154,   154,
     155,   155,   156,   156,   157,   157,   158,   159,   160,   161,
     161,   162,   162,   163,   163,   163,   163,   163,   163,   163,
     163,   164,   164,   165,   165,   166,   166,   166,   166,   167,
     167,   167,   167,   168,   168,   168,   168,   169,   170,   171,
     171,   172,   172,   173,   173,   174,   174,   175,   175,   176,
     177,   177,   178,   178,   178,   179,   179,   180,   180,   181,
     181,   181,   181,   182,   182,   183,   183,   184,   184,   185,
     185,   186,   186,   186,   186,   186,   186,   186,   187,   187,
     187,   187,   187,   188,   188,   189,   189,   189,   189,   190,
     190,   190,   190,   190,   191,   191,   191,   192,   192,   193,
     193,   194,   194,   195,   195,   195,   195,   195,   196,   196,
     196,   196,   197,   197,   198,   198,   199,   199,   199,   199,
     200,   200,   200,   200,   201,   201,   202,   202,   203,   203,
     204,   204,   205,   205,   205,   206,   206,   206,   207,   207,
     207,   207,   207,   207,   207,   207,   207,   207,   207,   208,
     208,   209,   209,   210,   210,   210,   210,   211,   211,   211,
     211,   212,   212,   212,   213,   213,   213,   214,   214,   214,
     214,   215,   215,   215,   215,   216,   216,   216,   216,   216,
     216,   216,   217,   217,   217,   217,   217,   217,   218,   218,
     218,   218,   218,   218,   218,   219,   219,   219,   219,   219,
     220,   220,   220,   220,   220,   221,   221,   221,   221,   221,
     222,   222,   223,   223,   224,   224,   225,   225,   226,   226,
     227,   227,   228,   228,   229,   229,   230,   230,   231,   231,
     232,   232,   233,   233,   234,   234,   235,   235,   236,   236,
     237,   237,   238,   238,   239,   239,   240,   240,   241,   241,
     242,   242,   243,   243,   243,   243,   243,   243,   243,   243,
     243,   243,   243,   243,   244,   244,   245,   245,   246,   246,
     247,   247,   247,   247,   247,   247,   247,   247,   247,   247,
     247,   247,   247,   247,   247,   247,   247,   248,   248,   249,
     249,   250,   250,   250,   250,   251,   251,   251,   251,   252,
     252,   253,   253,   254,   254,   255,   256,   257,   258,   258,
     259,   259,   260,   260,   260,   260,   260,   260,   260,   260,
     261,   261,   262,   262,   263,   263,   263,   263,   264,   264,
     264,   264,   265,   265,   265,   265,   266,   267,   268,   268,
     269,   269,   270,   270,   271,   271,   272,   272,   273,   274,
     274,   275,   275,   275,   276,   276,   277,   277,   278,   278,
     278,   278,   279,   279,   280,   280,   281,   281
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     1,     1,     1,     1,     1,     1,     3,
       3,     3,     7,     8,     1,     3,     1,     2,     3,     4,
       1,     1,     1,     1,     3,     3,     3,     5,     2,     4,
       0,     1,     1,     2,     1,     1,     4,     3,     3,     1,
       4,     3,     3,     1,     2,     1,     2,     2,     2,     4,
       3,     2,     2,     4,     3,     2,     3,     1,     3,     1,
       1,     1,     1,     1,     2,     2,     1,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       1,     1,     1,     1,     1,     3,     3,     3,     1,     3,
       3,     3,     1,     3,     3,     1,     3,     3,     1,     3,
       3,     3,     1,     3,     3,     3,     1,     3,     3,     3,
       3,     3,     3,     1,     3,     3,     3,     3,     3,     1,
       3,     3,     3,     3,     3,     3,     1,     3,     3,     3,
       3,     1,     3,     3,     3,     3,     1,     3,     3,     3,
       3,     1,     3,     1,     3,     1,     3,     1,     3,     1,
       3,     1,     3,     1,     3,     1,     3,     1,     3,     1,
       3,     1,     3,     1,     3,     1,     3,     1,     3,     1,
       3,     1,     5,     1,     5,     1,     5,     1,     3,     1,
       3,     1,     3,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     3,     1,     3,     1,
       3,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     2,     3,
       3,     3,     1,     2,     3,     4,     1,     2,     3,     4,
       3,     3,     1,     3,     1,     2,     2,     2,     1,     2,
       2,     5,     7,     7,     7,     5,     9,    10,     7,     8,
       9,     0,     1,     0,     1,     2,     2,     3,     3,     2,
       2,     3,     3,     2,     2,     3,     3,     5,     5,     3,
       5,     0,     1,     1,     2,     3,     4,     2,     3,     3,
       3,     3,     4,     7,     9,     2,     2,     7,     8,     6,
       7,     7,     8,     1,     3,     0,     1,     0,     1,     1,
       2,     1,     1,     1,     1,     1,     1,     1,     3,     3,
       3,     7,     8,     1,     3,     1,     2,     3,     4,     1,
       1,     1,     1,     3,     3,     3,     5,     2,     4,     0,
       1,     1,     2,     1,     1,     4,     3,     3,     1,     4,
       3,     3,     1,     2,     1,     2,     2,     2,     4,     3,
       2,     2,     4,     3,     2,     3,     1,     3,     1,     1,
       1,     1,     1,     2,     2,     1,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     1,
       1,     1,     1,     1,     3,     3,     3,     1,     3,     3,
       3,     1,     3,     3,     1,     3,     3,     1,     3,     3,
       3,     1,     3,     3,     3,     1,     3,     3,     3,     3,
       3,     3,     1,     3,     3,     3,     3,     3,     1,     3,
       3,     3,     3,     3,     3,     1,     3,     3,     3,     3,
       1,     3,     3,     3,     3,     1,     3,     3,     3,     3,
       1,     3,     1,     3,     1,     3,     1,     3,     1,     3,
       1,     3,     1,     3,     1,     3,     1,     3,     1,     3,
       1,     3,     1,     3,     1,     3,     1,     3,     1,     3,
       1,     5,     1,     5,     1,     5,     1,     3,     1,     3,
       1,     3,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     3,     1,     3,     1,     3,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     2,     3,     3,
       3,     1,     2,     3,     4,     1,     2,     3,     4,     3,
       3,     1,     3,     1,     2,     2,     2,     1,     2,     2,
       5,     7,     7,     7,     5,     9,    10,     7,     8,     9,
       0,     1,     0,     1,     2,     2,     3,     3,     2,     2,
       3,     3,     2,     2,     3,     3,     5,     5,     3,     5,
       0,     1,     1,     2,     3,     4,     2,     3,     3,     3,
       3,     4,     7,     9,     2,     2,     7,     8,     6,     7,
       7,     8,     1,     3,     0,     1,     1,     2
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


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
        yyerror (&yylloc, globalPtr, YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF

/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)                                \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;        \
          (Current).first_column = YYRHSLOC (Rhs, 1).first_column;      \
          (Current).last_line    = YYRHSLOC (Rhs, N).last_line;         \
          (Current).last_column  = YYRHSLOC (Rhs, N).last_column;       \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).first_line   = (Current).last_line   =              \
            YYRHSLOC (Rhs, 0).last_line;                                \
          (Current).first_column = (Current).last_column =              \
            YYRHSLOC (Rhs, 0).last_column;                              \
        }                                                               \
    while (0)
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K])


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


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

# ifndef YY_LOCATION_PRINT
#  if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL

/* Print *YYLOCP on YYO.  Private, do not rely on its existence. */

YY_ATTRIBUTE_UNUSED
static int
yy_location_print_ (FILE *yyo, YYLTYPE const * const yylocp)
{
  int res = 0;
  int end_col = 0 != yylocp->last_column ? yylocp->last_column - 1 : 0;
  if (0 <= yylocp->first_line)
    {
      res += YYFPRINTF (yyo, "%d", yylocp->first_line);
      if (0 <= yylocp->first_column)
        res += YYFPRINTF (yyo, ".%d", yylocp->first_column);
    }
  if (0 <= yylocp->last_line)
    {
      if (yylocp->first_line < yylocp->last_line)
        {
          res += YYFPRINTF (yyo, "-%d", yylocp->last_line);
          if (0 <= end_col)
            res += YYFPRINTF (yyo, ".%d", end_col);
        }
      else if (0 <= end_col && yylocp->first_column < end_col)
        res += YYFPRINTF (yyo, "-%d", end_col);
    }
  return res;
 }

#   define YY_LOCATION_PRINT(File, Loc)          \
  yy_location_print_ (File, &(Loc))

#  else
#   define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#  endif
# endif /* !defined YY_LOCATION_PRINT */


# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value, Location, globalPtr); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp, void *globalPtr)
{
  FILE *yyoutput = yyo;
  YYUSE (yyoutput);
  YYUSE (yylocationp);
  YYUSE (globalPtr);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yykind < YYNTOKENS)
    YYPRINT (yyo, yytoknum[yykind], *yyvaluep);
# endif
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp, void *globalPtr)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  YY_LOCATION_PRINT (yyo, *yylocationp);
  YYFPRINTF (yyo, ": ");
  yy_symbol_value_print (yyo, yykind, yyvaluep, yylocationp, globalPtr);
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
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp, YYLTYPE *yylsp,
                 int yyrule, void *globalPtr)
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
                       &yyvsp[(yyi + 1) - (yynrhs)],
                       &(yylsp[(yyi + 1) - (yynrhs)]), globalPtr);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, yylsp, Rule, globalPtr); \
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
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep, YYLTYPE *yylocationp, void *globalPtr)
{
  YYUSE (yyvaluep);
  YYUSE (yylocationp);
  YYUSE (globalPtr);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}






/*----------.
| yyparse.  |
`----------*/

int
yyparse (void *globalPtr)
{
/* Lookahead token kind.  */
int yychar;


/* The semantic value of the lookahead symbol.  */
/* Default value used for initialization, for pacifying older GCCs
   or non-GCC compilers.  */
YY_INITIAL_VALUE (static YYSTYPE yyval_default;)
YYSTYPE yylval YY_INITIAL_VALUE (= yyval_default);

/* Location data for the lookahead symbol.  */
static YYLTYPE yyloc_default
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
  = { 1, 1, 1, 1 }
# endif
;
YYLTYPE yylloc = yyloc_default;

    /* Number of syntax errors so far.  */
    int yynerrs = 0;

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

    /* The location stack: array, bottom, top.  */
    YYLTYPE yylsa[YYINITDEPTH];
    YYLTYPE *yyls = yylsa;
    YYLTYPE *yylsp = yyls;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
  YYLTYPE yyloc;

  /* The locations where the error started and ended.  */
  YYLTYPE yyerror_range[3];



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N), yylsp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */
  yylsp[0] = yylloc;
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
    goto yyexhaustedlab;
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
        YYLTYPE *yyls1 = yyls;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yyls1, yysize * YYSIZEOF (*yylsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
        yyls = yyls1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
        YYSTACK_RELOCATE (yyls_alloc, yyls);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;
      yylsp = yyls + yysize - 1;

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
      yychar = yylex (&yylval, &yylloc, globalPtr);
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
      yyerror_range[1] = yylloc;
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
  *++yylsp = yylloc;

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

  /* Default location. */
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);
  yyerror_range[1] = yyloc;
  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2: /* Literal: NULLTOKEN  */
                                        { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(new (GLOBAL_DATA) NullNode(GLOBAL_DATA), 0, 1); }
    break;

  case 3: /* Literal: TRUETOKEN  */
                                        { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(new (GLOBAL_DATA) BooleanNode(GLOBAL_DATA, true), 0, 1); }
    break;

  case 4: /* Literal: FALSETOKEN  */
                                        { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(new (GLOBAL_DATA) BooleanNode(GLOBAL_DATA, false), 0, 1); }
    break;

  case 5: /* Literal: NUMBER  */
                                        { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(makeNumberNode(GLOBAL_DATA, (yyvsp[0].doubleValue)), 0, 1); }
    break;

  case 6: /* Literal: STRING  */
                                        { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(new (GLOBAL_DATA) StringNode(GLOBAL_DATA, *(yyvsp[0].ident)), 0, 1); }
    break;

  case 7: /* Literal: '/'  */
                                        {
                                            Lexer& l = *GLOBAL_DATA->lexer;
                                            const Identifier* pattern;
                                            const Identifier* flags;
                                            if (!l.scanRegExp(pattern, flags))
                                                YYABORT;
                                            RegExpNode* node = new (GLOBAL_DATA) RegExpNode(GLOBAL_DATA, *pattern, *flags);
                                            int size = pattern->size() + 2; // + 2 for the two /'s
                                            setExceptionLocation(node, (yylsp[0]).first_column, (yylsp[0]).first_column + size, (yylsp[0]).first_column + size);
                                            (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(node, 0, 0);
                                        }
    break;

  case 8: /* Literal: DIVEQUAL  */
                                        {
                                            Lexer& l = *GLOBAL_DATA->lexer;
                                            const Identifier* pattern;
                                            const Identifier* flags;
                                            if (!l.scanRegExp(pattern, flags, '='))
                                                YYABORT;
                                            RegExpNode* node = new (GLOBAL_DATA) RegExpNode(GLOBAL_DATA, *pattern, *flags);
                                            int size = pattern->size() + 2; // + 2 for the two /'s
                                            setExceptionLocation(node, (yylsp[0]).first_column, (yylsp[0]).first_column + size, (yylsp[0]).first_column + size);
                                            (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(node, 0, 0);
                                        }
    break;

  case 9: /* Property: IDENT ':' AssignmentExpr  */
                                        { (yyval.propertyNode) = createNodeInfo<PropertyNode*>(new (GLOBAL_DATA) PropertyNode(GLOBAL_DATA, *(yyvsp[-2].ident), (yyvsp[0].expressionNode).m_node, PropertyNode::Constant), (yyvsp[0].expressionNode).m_features, (yyvsp[0].expressionNode).m_numConstants); }
    break;

  case 10: /* Property: STRING ':' AssignmentExpr  */
                                        { (yyval.propertyNode) = createNodeInfo<PropertyNode*>(new (GLOBAL_DATA) PropertyNode(GLOBAL_DATA, *(yyvsp[-2].ident), (yyvsp[0].expressionNode).m_node, PropertyNode::Constant), (yyvsp[0].expressionNode).m_features, (yyvsp[0].expressionNode).m_numConstants); }
    break;

  case 11: /* Property: NUMBER ':' AssignmentExpr  */
                                        { (yyval.propertyNode) = createNodeInfo<PropertyNode*>(new (GLOBAL_DATA) PropertyNode(GLOBAL_DATA, (yyvsp[-2].doubleValue), (yyvsp[0].expressionNode).m_node, PropertyNode::Constant), (yyvsp[0].expressionNode).m_features, (yyvsp[0].expressionNode).m_numConstants); }
    break;

  case 12: /* Property: IDENT IDENT '(' ')' OPENBRACE FunctionBody CLOSEBRACE  */
                                                             { (yyval.propertyNode) = createNodeInfo<PropertyNode*>(makeGetterOrSetterPropertyNode(GLOBAL_DATA, *(yyvsp[-6].ident), *(yyvsp[-5].ident), 0, (yyvsp[-1].functionBodyNode), GLOBAL_DATA->lexer->sourceCode((yyvsp[-2].intValue), (yyvsp[0].intValue), (yylsp[-2]).first_line)), ClosureFeature, 0); setStatementLocation((yyvsp[-1].functionBodyNode), (yylsp[-2]), (yylsp[0])); if (!(yyval.propertyNode).m_node) YYABORT; }
    break;

  case 13: /* Property: IDENT IDENT '(' FormalParameterList ')' OPENBRACE FunctionBody CLOSEBRACE  */
                                                             {
                                                                 (yyval.propertyNode) = createNodeInfo<PropertyNode*>(makeGetterOrSetterPropertyNode(GLOBAL_DATA, *(yyvsp[-7].ident), *(yyvsp[-6].ident), (yyvsp[-4].parameterList).m_node.head, (yyvsp[-1].functionBodyNode), GLOBAL_DATA->lexer->sourceCode((yyvsp[-2].intValue), (yyvsp[0].intValue), (yylsp[-2]).first_line)), (yyvsp[-4].parameterList).m_features | ClosureFeature, 0); 
                                                                 if ((yyvsp[-4].parameterList).m_features & ArgumentsFeature)
                                                                     (yyvsp[-1].functionBodyNode)->setUsesArguments(); 
                                                                 setStatementLocation((yyvsp[-1].functionBodyNode), (yylsp[-2]), (yylsp[0])); 
                                                                 if (!(yyval.propertyNode).m_node) 
                                                                     YYABORT; 
                                                             }
    break;

  case 14: /* PropertyList: Property  */
                                        { (yyval.propertyList).m_node.head = new (GLOBAL_DATA) PropertyListNode(GLOBAL_DATA, (yyvsp[0].propertyNode).m_node); 
                                          (yyval.propertyList).m_node.tail = (yyval.propertyList).m_node.head;
                                          (yyval.propertyList).m_features = (yyvsp[0].propertyNode).m_features;
                                          (yyval.propertyList).m_numConstants = (yyvsp[0].propertyNode).m_numConstants; }
    break;

  case 15: /* PropertyList: PropertyList ',' Property  */
                                        { (yyval.propertyList).m_node.head = (yyvsp[-2].propertyList).m_node.head;
                                          (yyval.propertyList).m_node.tail = new (GLOBAL_DATA) PropertyListNode(GLOBAL_DATA, (yyvsp[0].propertyNode).m_node, (yyvsp[-2].propertyList).m_node.tail);
                                          (yyval.propertyList).m_features = (yyvsp[-2].propertyList).m_features | (yyvsp[0].propertyNode).m_features;
                                          (yyval.propertyList).m_numConstants = (yyvsp[-2].propertyList).m_numConstants + (yyvsp[0].propertyNode).m_numConstants; }
    break;

  case 17: /* PrimaryExpr: OPENBRACE CLOSEBRACE  */
                                                     { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(new (GLOBAL_DATA) ObjectLiteralNode(GLOBAL_DATA), 0, 0); }
    break;

  case 18: /* PrimaryExpr: OPENBRACE PropertyList CLOSEBRACE  */
                                                     { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(new (GLOBAL_DATA) ObjectLiteralNode(GLOBAL_DATA, (yyvsp[-1].propertyList).m_node.head), (yyvsp[-1].propertyList).m_features, (yyvsp[-1].propertyList).m_numConstants); }
    break;

  case 19: /* PrimaryExpr: OPENBRACE PropertyList ',' CLOSEBRACE  */
                                                     { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(new (GLOBAL_DATA) ObjectLiteralNode(GLOBAL_DATA, (yyvsp[-2].propertyList).m_node.head), (yyvsp[-2].propertyList).m_features, (yyvsp[-2].propertyList).m_numConstants); }
    break;

  case 20: /* PrimaryExprNoBrace: THISTOKEN  */
                                        { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(new (GLOBAL_DATA) ThisNode(GLOBAL_DATA), ThisFeature, 0); }
    break;

  case 23: /* PrimaryExprNoBrace: IDENT  */
                                        { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(new (GLOBAL_DATA) ResolveNode(GLOBAL_DATA, *(yyvsp[0].ident), (yylsp[0]).first_column), (*(yyvsp[0].ident) == GLOBAL_DATA->propertyNames->arguments) ? ArgumentsFeature : 0, 0); }
    break;

  case 24: /* PrimaryExprNoBrace: '(' Expr ')'  */
                                        { (yyval.expressionNode) = (yyvsp[-1].expressionNode); }
    break;

  case 25: /* ArrayLiteral: '[' ElisionOpt ']'  */
                                        { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(new (GLOBAL_DATA) ArrayNode(GLOBAL_DATA, (yyvsp[-1].intValue)), 0, (yyvsp[-1].intValue) ? 1 : 0); }
    break;

  case 26: /* ArrayLiteral: '[' ElementList ']'  */
                                        { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(new (GLOBAL_DATA) ArrayNode(GLOBAL_DATA, (yyvsp[-1].elementList).m_node.head), (yyvsp[-1].elementList).m_features, (yyvsp[-1].elementList).m_numConstants); }
    break;

  case 27: /* ArrayLiteral: '[' ElementList ',' ElisionOpt ']'  */
                                        { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(new (GLOBAL_DATA) ArrayNode(GLOBAL_DATA, (yyvsp[-1].intValue), (yyvsp[-3].elementList).m_node.head), (yyvsp[-3].elementList).m_features, (yyvsp[-1].intValue) ? (yyvsp[-3].elementList).m_numConstants + 1 : (yyvsp[-3].elementList).m_numConstants); }
    break;

  case 28: /* ElementList: ElisionOpt AssignmentExpr  */
                                        { (yyval.elementList).m_node.head = new (GLOBAL_DATA) ElementNode(GLOBAL_DATA, (yyvsp[-1].intValue), (yyvsp[0].expressionNode).m_node);
                                          (yyval.elementList).m_node.tail = (yyval.elementList).m_node.head;
                                          (yyval.elementList).m_features = (yyvsp[0].expressionNode).m_features;
                                          (yyval.elementList).m_numConstants = (yyvsp[0].expressionNode).m_numConstants; }
    break;

  case 29: /* ElementList: ElementList ',' ElisionOpt AssignmentExpr  */
                                        { (yyval.elementList).m_node.head = (yyvsp[-3].elementList).m_node.head;
                                          (yyval.elementList).m_node.tail = new (GLOBAL_DATA) ElementNode(GLOBAL_DATA, (yyvsp[-3].elementList).m_node.tail, (yyvsp[-1].intValue), (yyvsp[0].expressionNode).m_node);
                                          (yyval.elementList).m_features = (yyvsp[-3].elementList).m_features | (yyvsp[0].expressionNode).m_features;
                                          (yyval.elementList).m_numConstants = (yyvsp[-3].elementList).m_numConstants + (yyvsp[0].expressionNode).m_numConstants; }
    break;

  case 30: /* ElisionOpt: %empty  */
                                        { (yyval.intValue) = 0; }
    break;

  case 32: /* Elision: ','  */
                                        { (yyval.intValue) = 1; }
    break;

  case 33: /* Elision: Elision ','  */
                                        { (yyval.intValue) = (yyvsp[-1].intValue) + 1; }
    break;

  case 35: /* MemberExpr: FunctionExpr  */
                                        { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>((yyvsp[0].funcExprNode).m_node, (yyvsp[0].funcExprNode).m_features, (yyvsp[0].funcExprNode).m_numConstants); }
    break;

  case 36: /* MemberExpr: MemberExpr '[' Expr ']'  */
                                        { BracketAccessorNode* node = new (GLOBAL_DATA) BracketAccessorNode(GLOBAL_DATA, (yyvsp[-3].expressionNode).m_node, (yyvsp[-1].expressionNode).m_node, (yyvsp[-1].expressionNode).m_features & AssignFeature);
                                          setExceptionLocation(node, (yylsp[-3]).first_column, (yylsp[-3]).last_column, (yylsp[0]).last_column);
                                          (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(node, (yyvsp[-3].expressionNode).m_features | (yyvsp[-1].expressionNode).m_features, (yyvsp[-3].expressionNode).m_numConstants + (yyvsp[-1].expressionNode).m_numConstants); 
                                        }
    break;

  case 37: /* MemberExpr: MemberExpr '.' IDENT  */
                                        { DotAccessorNode* node = new (GLOBAL_DATA) DotAccessorNode(GLOBAL_DATA, (yyvsp[-2].expressionNode).m_node, *(yyvsp[0].ident));
                                          setExceptionLocation(node, (yylsp[-2]).first_column, (yylsp[-2]).last_column, (yylsp[0]).last_column);
                                          (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(node, (yyvsp[-2].expressionNode).m_features, (yyvsp[-2].expressionNode).m_numConstants);
                                        }
    break;

  case 38: /* MemberExpr: NEW MemberExpr Arguments  */
                                        { NewExprNode* node = new (GLOBAL_DATA) NewExprNode(GLOBAL_DATA, (yyvsp[-1].expressionNode).m_node, (yyvsp[0].argumentsNode).m_node);
                                          setExceptionLocation(node, (yylsp[-2]).first_column, (yylsp[-1]).last_column, (yylsp[0]).last_column);
                                          (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(node, (yyvsp[-1].expressionNode).m_features | (yyvsp[0].argumentsNode).m_features, (yyvsp[-1].expressionNode).m_numConstants + (yyvsp[0].argumentsNode).m_numConstants);
                                        }
    break;

  case 40: /* MemberExprNoBF: MemberExprNoBF '[' Expr ']'  */
                                        { BracketAccessorNode* node = new (GLOBAL_DATA) BracketAccessorNode(GLOBAL_DATA, (yyvsp[-3].expressionNode).m_node, (yyvsp[-1].expressionNode).m_node, (yyvsp[-1].expressionNode).m_features & AssignFeature);
                                          setExceptionLocation(node, (yylsp[-3]).first_column, (yylsp[-3]).last_column, (yylsp[0]).last_column);
                                          (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(node, (yyvsp[-3].expressionNode).m_features | (yyvsp[-1].expressionNode).m_features, (yyvsp[-3].expressionNode).m_numConstants + (yyvsp[-1].expressionNode).m_numConstants); 
                                        }
    break;

  case 41: /* MemberExprNoBF: MemberExprNoBF '.' IDENT  */
                                        { DotAccessorNode* node = new (GLOBAL_DATA) DotAccessorNode(GLOBAL_DATA, (yyvsp[-2].expressionNode).m_node, *(yyvsp[0].ident));
                                          setExceptionLocation(node, (yylsp[-2]).first_column, (yylsp[-2]).last_column, (yylsp[0]).last_column);
                                          (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(node, (yyvsp[-2].expressionNode).m_features, (yyvsp[-2].expressionNode).m_numConstants);
                                        }
    break;

  case 42: /* MemberExprNoBF: NEW MemberExpr Arguments  */
                                        { NewExprNode* node = new (GLOBAL_DATA) NewExprNode(GLOBAL_DATA, (yyvsp[-1].expressionNode).m_node, (yyvsp[0].argumentsNode).m_node);
                                          setExceptionLocation(node, (yylsp[-2]).first_column, (yylsp[-1]).last_column, (yylsp[0]).last_column);
                                          (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(node, (yyvsp[-1].expressionNode).m_features | (yyvsp[0].argumentsNode).m_features, (yyvsp[-1].expressionNode).m_numConstants + (yyvsp[0].argumentsNode).m_numConstants);
                                        }
    break;

  case 44: /* NewExpr: NEW NewExpr  */
                                        { NewExprNode* node = new (GLOBAL_DATA) NewExprNode(GLOBAL_DATA, (yyvsp[0].expressionNode).m_node);
                                          setExceptionLocation(node, (yylsp[-1]).first_column, (yylsp[0]).last_column, (yylsp[0]).last_column);
                                          (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(node, (yyvsp[0].expressionNode).m_features, (yyvsp[0].expressionNode).m_numConstants); 
                                        }
    break;

  case 46: /* NewExprNoBF: NEW NewExpr  */
                                        { NewExprNode* node = new (GLOBAL_DATA) NewExprNode(GLOBAL_DATA, (yyvsp[0].expressionNode).m_node);
                                          setExceptionLocation(node, (yylsp[-1]).first_column, (yylsp[0]).last_column, (yylsp[0]).last_column);
                                          (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(node, (yyvsp[0].expressionNode).m_features, (yyvsp[0].expressionNode).m_numConstants);
                                        }
    break;

  case 47: /* CallExpr: MemberExpr Arguments  */
                                        { (yyval.expressionNode) = makeFunctionCallNode(GLOBAL_DATA, (yyvsp[-1].expressionNode), (yyvsp[0].argumentsNode), (yylsp[-1]).first_column, (yylsp[-1]).last_column, (yylsp[0]).last_column); }
    break;

  case 48: /* CallExpr: CallExpr Arguments  */
                                        { (yyval.expressionNode) = makeFunctionCallNode(GLOBAL_DATA, (yyvsp[-1].expressionNode), (yyvsp[0].argumentsNode), (yylsp[-1]).first_column, (yylsp[-1]).last_column, (yylsp[0]).last_column); }
    break;

  case 49: /* CallExpr: CallExpr '[' Expr ']'  */
                                        { BracketAccessorNode* node = new (GLOBAL_DATA) BracketAccessorNode(GLOBAL_DATA, (yyvsp[-3].expressionNode).m_node, (yyvsp[-1].expressionNode).m_node, (yyvsp[-1].expressionNode).m_features & AssignFeature);
                                          setExceptionLocation(node, (yylsp[-3]).first_column, (yylsp[-3]).last_column, (yylsp[0]).last_column);
                                          (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(node, (yyvsp[-3].expressionNode).m_features | (yyvsp[-1].expressionNode).m_features, (yyvsp[-3].expressionNode).m_numConstants + (yyvsp[-1].expressionNode).m_numConstants); 
                                        }
    break;

  case 50: /* CallExpr: CallExpr '.' IDENT  */
                                        { DotAccessorNode* node = new (GLOBAL_DATA) DotAccessorNode(GLOBAL_DATA, (yyvsp[-2].expressionNode).m_node, *(yyvsp[0].ident));
                                          setExceptionLocation(node, (yylsp[-2]).first_column, (yylsp[-2]).last_column, (yylsp[0]).last_column);
                                          (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(node, (yyvsp[-2].expressionNode).m_features, (yyvsp[-2].expressionNode).m_numConstants); }
    break;

  case 51: /* CallExprNoBF: MemberExprNoBF Arguments  */
                                        { (yyval.expressionNode) = makeFunctionCallNode(GLOBAL_DATA, (yyvsp[-1].expressionNode), (yyvsp[0].argumentsNode), (yylsp[-1]).first_column, (yylsp[-1]).last_column, (yylsp[0]).last_column); }
    break;

  case 52: /* CallExprNoBF: CallExprNoBF Arguments  */
                                        { (yyval.expressionNode) = makeFunctionCallNode(GLOBAL_DATA, (yyvsp[-1].expressionNode), (yyvsp[0].argumentsNode), (yylsp[-1]).first_column, (yylsp[-1]).last_column, (yylsp[0]).last_column); }
    break;

  case 53: /* CallExprNoBF: CallExprNoBF '[' Expr ']'  */
                                        { BracketAccessorNode* node = new (GLOBAL_DATA) BracketAccessorNode(GLOBAL_DATA, (yyvsp[-3].expressionNode).m_node, (yyvsp[-1].expressionNode).m_node, (yyvsp[-1].expressionNode).m_features & AssignFeature);
                                          setExceptionLocation(node, (yylsp[-3]).first_column, (yylsp[-3]).last_column, (yylsp[0]).last_column);
                                          (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(node, (yyvsp[-3].expressionNode).m_features | (yyvsp[-1].expressionNode).m_features, (yyvsp[-3].expressionNode).m_numConstants + (yyvsp[-1].expressionNode).m_numConstants); 
                                        }
    break;

  case 54: /* CallExprNoBF: CallExprNoBF '.' IDENT  */
                                        { DotAccessorNode* node = new (GLOBAL_DATA) DotAccessorNode(GLOBAL_DATA, (yyvsp[-2].expressionNode).m_node, *(yyvsp[0].ident));
                                          setExceptionLocation(node, (yylsp[-2]).first_column, (yylsp[-2]).last_column, (yylsp[0]).last_column);
                                          (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(node, (yyvsp[-2].expressionNode).m_features, (yyvsp[-2].expressionNode).m_numConstants); 
                                        }
    break;

  case 55: /* Arguments: '(' ')'  */
                                        { (yyval.argumentsNode) = createNodeInfo<ArgumentsNode*>(new (GLOBAL_DATA) ArgumentsNode(GLOBAL_DATA), 0, 0); }
    break;

  case 56: /* Arguments: '(' ArgumentList ')'  */
                                        { (yyval.argumentsNode) = createNodeInfo<ArgumentsNode*>(new (GLOBAL_DATA) ArgumentsNode(GLOBAL_DATA, (yyvsp[-1].argumentList).m_node.head), (yyvsp[-1].argumentList).m_features, (yyvsp[-1].argumentList).m_numConstants); }
    break;

  case 57: /* ArgumentList: AssignmentExpr  */
                                        { (yyval.argumentList).m_node.head = new (GLOBAL_DATA) ArgumentListNode(GLOBAL_DATA, (yyvsp[0].expressionNode).m_node);
                                          (yyval.argumentList).m_node.tail = (yyval.argumentList).m_node.head;
                                          (yyval.argumentList).m_features = (yyvsp[0].expressionNode).m_features;
                                          (yyval.argumentList).m_numConstants = (yyvsp[0].expressionNode).m_numConstants; }
    break;

  case 58: /* ArgumentList: ArgumentList ',' AssignmentExpr  */
                                        { (yyval.argumentList).m_node.head = (yyvsp[-2].argumentList).m_node.head;
                                          (yyval.argumentList).m_node.tail = new (GLOBAL_DATA) ArgumentListNode(GLOBAL_DATA, (yyvsp[-2].argumentList).m_node.tail, (yyvsp[0].expressionNode).m_node);
                                          (yyval.argumentList).m_features = (yyvsp[-2].argumentList).m_features | (yyvsp[0].expressionNode).m_features;
                                          (yyval.argumentList).m_numConstants = (yyvsp[-2].argumentList).m_numConstants + (yyvsp[0].expressionNode).m_numConstants; }
    break;

  case 64: /* PostfixExpr: LeftHandSideExpr PLUSPLUS  */
                                        { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(makePostfixNode(GLOBAL_DATA, (yyvsp[-1].expressionNode).m_node, OpPlusPlus, (yylsp[-1]).first_column, (yylsp[-1]).last_column, (yylsp[0]).last_column), (yyvsp[-1].expressionNode).m_features | AssignFeature, (yyvsp[-1].expressionNode).m_numConstants); }
    break;

  case 65: /* PostfixExpr: LeftHandSideExpr MINUSMINUS  */
                                        { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(makePostfixNode(GLOBAL_DATA, (yyvsp[-1].expressionNode).m_node, OpMinusMinus, (yylsp[-1]).first_column, (yylsp[-1]).last_column, (yylsp[0]).last_column), (yyvsp[-1].expressionNode).m_features | AssignFeature, (yyvsp[-1].expressionNode).m_numConstants); }
    break;

  case 67: /* PostfixExprNoBF: LeftHandSideExprNoBF PLUSPLUS  */
                                        { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(makePostfixNode(GLOBAL_DATA, (yyvsp[-1].expressionNode).m_node, OpPlusPlus, (yylsp[-1]).first_column, (yylsp[-1]).last_column, (yylsp[0]).last_column), (yyvsp[-1].expressionNode).m_features | AssignFeature, (yyvsp[-1].expressionNode).m_numConstants); }
    break;

  case 68: /* PostfixExprNoBF: LeftHandSideExprNoBF MINUSMINUS  */
                                        { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(makePostfixNode(GLOBAL_DATA, (yyvsp[-1].expressionNode).m_node, OpMinusMinus, (yylsp[-1]).first_column, (yylsp[-1]).last_column, (yylsp[0]).last_column), (yyvsp[-1].expressionNode).m_features | AssignFeature, (yyvsp[-1].expressionNode).m_numConstants); }
    break;

  case 69: /* UnaryExprCommon: DELETETOKEN UnaryExpr  */
                                        { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(makeDeleteNode(GLOBAL_DATA, (yyvsp[0].expressionNode).m_node, (yylsp[-1]).first_column, (yylsp[0]).last_column, (yylsp[0]).last_column), (yyvsp[0].expressionNode).m_features, (yyvsp[0].expressionNode).m_numConstants); }
    break;

  case 70: /* UnaryExprCommon: VOIDTOKEN UnaryExpr  */
                                        { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(new (GLOBAL_DATA) VoidNode(GLOBAL_DATA, (yyvsp[0].expressionNode).m_node), (yyvsp[0].expressionNode).m_features, (yyvsp[0].expressionNode).m_numConstants + 1); }
    break;

  case 71: /* UnaryExprCommon: TYPEOF UnaryExpr  */
                                        { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(makeTypeOfNode(GLOBAL_DATA, (yyvsp[0].expressionNode).m_node), (yyvsp[0].expressionNode).m_features, (yyvsp[0].expressionNode).m_numConstants); }
    break;

  case 72: /* UnaryExprCommon: PLUSPLUS UnaryExpr  */
                                        { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(makePrefixNode(GLOBAL_DATA, (yyvsp[0].expressionNode).m_node, OpPlusPlus, (yylsp[-1]).first_column, (yylsp[0]).first_column + 1, (yylsp[0]).last_column), (yyvsp[0].expressionNode).m_features | AssignFeature, (yyvsp[0].expressionNode).m_numConstants); }
    break;

  case 73: /* UnaryExprCommon: AUTOPLUSPLUS UnaryExpr  */
                                        { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(makePrefixNode(GLOBAL_DATA, (yyvsp[0].expressionNode).m_node, OpPlusPlus, (yylsp[-1]).first_column, (yylsp[0]).first_column + 1, (yylsp[0]).last_column), (yyvsp[0].expressionNode).m_features | AssignFeature, (yyvsp[0].expressionNode).m_numConstants); }
    break;

  case 74: /* UnaryExprCommon: MINUSMINUS UnaryExpr  */
                                        { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(makePrefixNode(GLOBAL_DATA, (yyvsp[0].expressionNode).m_node, OpMinusMinus, (yylsp[-1]).first_column, (yylsp[0]).first_column + 1, (yylsp[0]).last_column), (yyvsp[0].expressionNode).m_features | AssignFeature, (yyvsp[0].expressionNode).m_numConstants); }
    break;

  case 75: /* UnaryExprCommon: AUTOMINUSMINUS UnaryExpr  */
                                        { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(makePrefixNode(GLOBAL_DATA, (yyvsp[0].expressionNode).m_node, OpMinusMinus, (yylsp[-1]).first_column, (yylsp[0]).first_column + 1, (yylsp[0]).last_column), (yyvsp[0].expressionNode).m_features | AssignFeature, (yyvsp[0].expressionNode).m_numConstants); }
    break;

  case 76: /* UnaryExprCommon: '+' UnaryExpr  */
                                        { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(new (GLOBAL_DATA) UnaryPlusNode(GLOBAL_DATA, (yyvsp[0].expressionNode).m_node), (yyvsp[0].expressionNode).m_features, (yyvsp[0].expressionNode).m_numConstants); }
    break;

  case 77: /* UnaryExprCommon: '-' UnaryExpr  */
                                        { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(makeNegateNode(GLOBAL_DATA, (yyvsp[0].expressionNode).m_node), (yyvsp[0].expressionNode).m_features, (yyvsp[0].expressionNode).m_numConstants); }
    break;

  case 78: /* UnaryExprCommon: '~' UnaryExpr  */
                                        { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(makeBitwiseNotNode(GLOBAL_DATA, (yyvsp[0].expressionNode).m_node), (yyvsp[0].expressionNode).m_features, (yyvsp[0].expressionNode).m_numConstants); }
    break;

  case 79: /* UnaryExprCommon: '!' UnaryExpr  */
                                        { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(new (GLOBAL_DATA) LogicalNotNode(GLOBAL_DATA, (yyvsp[0].expressionNode).m_node), (yyvsp[0].expressionNode).m_features, (yyvsp[0].expressionNode).m_numConstants); }
    break;

  case 85: /* MultiplicativeExpr: MultiplicativeExpr '*' UnaryExpr  */
                                        { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(makeMultNode(GLOBAL_DATA, (yyvsp[-2].expressionNode).m_node, (yyvsp[0].expressionNode).m_node, (yyvsp[0].expressionNode).m_features & AssignFeature), (yyvsp[-2].expressionNode).m_features | (yyvsp[0].expressionNode).m_features, (yyvsp[-2].expressionNode).m_numConstants + (yyvsp[0].expressionNode).m_numConstants); }
    break;

  case 86: /* MultiplicativeExpr: MultiplicativeExpr '/' UnaryExpr  */
                                        { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(makeDivNode(GLOBAL_DATA, (yyvsp[-2].expressionNode).m_node, (yyvsp[0].expressionNode).m_node, (yyvsp[0].expressionNode).m_features & AssignFeature), (yyvsp[-2].expressionNode).m_features | (yyvsp[0].expressionNode).m_features, (yyvsp[-2].expressionNode).m_numConstants + (yyvsp[0].expressionNode).m_numConstants); }
    break;

  case 87: /* MultiplicativeExpr: MultiplicativeExpr '%' UnaryExpr  */
                                        { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(new (GLOBAL_DATA) ModNode(GLOBAL_DATA, (yyvsp[-2].expressionNode).m_node, (yyvsp[0].expressionNode).m_node, (yyvsp[0].expressionNode).m_features & AssignFeature), (yyvsp[-2].expressionNode).m_features | (yyvsp[0].expressionNode).m_features, (yyvsp[-2].expressionNode).m_numConstants + (yyvsp[0].expressionNode).m_numConstants); }
    break;

  case 89: /* MultiplicativeExprNoBF: MultiplicativeExprNoBF '*' UnaryExpr  */
                                        { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(makeMultNode(GLOBAL_DATA, (yyvsp[-2].expressionNode).m_node, (yyvsp[0].expressionNode).m_node, (yyvsp[0].expressionNode).m_features & AssignFeature), (yyvsp[-2].expressionNode).m_features | (yyvsp[0].expressionNode).m_features, (yyvsp[-2].expressionNode).m_numConstants + (yyvsp[0].expressionNode).m_numConstants); }
    break;

  case 90: /* MultiplicativeExprNoBF: MultiplicativeExprNoBF '/' UnaryExpr  */
                                        { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(makeDivNode(GLOBAL_DATA, (yyvsp[-2].expressionNode).m_node, (yyvsp[0].expressionNode).m_node, (yyvsp[0].expressionNode).m_features & AssignFeature), (yyvsp[-2].expressionNode).m_features | (yyvsp[0].expressionNode).m_features, (yyvsp[-2].expressionNode).m_numConstants + (yyvsp[0].expressionNode).m_numConstants); }
    break;

  case 91: /* MultiplicativeExprNoBF: MultiplicativeExprNoBF '%' UnaryExpr  */
                                        { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(new (GLOBAL_DATA) ModNode(GLOBAL_DATA, (yyvsp[-2].expressionNode).m_node, (yyvsp[0].expressionNode).m_node, (yyvsp[0].expressionNode).m_features & AssignFeature), (yyvsp[-2].expressionNode).m_features | (yyvsp[0].expressionNode).m_features, (yyvsp[-2].expressionNode).m_numConstants + (yyvsp[0].expressionNode).m_numConstants); }
    break;

  case 93: /* AdditiveExpr: AdditiveExpr '+' MultiplicativeExpr  */
                                        { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(makeAddNode(GLOBAL_DATA, (yyvsp[-2].expressionNode).m_node, (yyvsp[0].expressionNode).m_node, (yyvsp[0].expressionNode).m_features & AssignFeature), (yyvsp[-2].expressionNode).m_features | (yyvsp[0].expressionNode).m_features, (yyvsp[-2].expressionNode).m_numConstants + (yyvsp[0].expressionNode).m_numConstants); }
    break;

  case 94: /* AdditiveExpr: AdditiveExpr '-' MultiplicativeExpr  */
                                        { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(makeSubNode(GLOBAL_DATA, (yyvsp[-2].expressionNode).m_node, (yyvsp[0].expressionNode).m_node, (yyvsp[0].expressionNode).m_features & AssignFeature), (yyvsp[-2].expressionNode).m_features | (yyvsp[0].expressionNode).m_features, (yyvsp[-2].expressionNode).m_numConstants + (yyvsp[0].expressionNode).m_numConstants); }
    break;

  case 96: /* AdditiveExprNoBF: AdditiveExprNoBF '+' MultiplicativeExpr  */
                                        { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(makeAddNode(GLOBAL_DATA, (yyvsp[-2].expressionNode).m_node, (yyvsp[0].expressionNode).m_node, (yyvsp[0].expressionNode).m_features & AssignFeature), (yyvsp[-2].expressionNode).m_features | (yyvsp[0].expressionNode).m_features, (yyvsp[-2].expressionNode).m_numConstants + (yyvsp[0].expressionNode).m_numConstants); }
    break;

  case 97: /* AdditiveExprNoBF: AdditiveExprNoBF '-' MultiplicativeExpr  */
                                        { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(makeSubNode(GLOBAL_DATA, (yyvsp[-2].expressionNode).m_node, (yyvsp[0].expressionNode).m_node, (yyvsp[0].expressionNode).m_features & AssignFeature), (yyvsp[-2].expressionNode).m_features | (yyvsp[0].expressionNode).m_features, (yyvsp[-2].expressionNode).m_numConstants + (yyvsp[0].expressionNode).m_numConstants); }
    break;

  case 99: /* ShiftExpr: ShiftExpr LSHIFT AdditiveExpr  */
                                        { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(makeLeftShiftNode(GLOBAL_DATA, (yyvsp[-2].expressionNode).m_node, (yyvsp[0].expressionNode).m_node, (yyvsp[0].expressionNode).m_features & AssignFeature), (yyvsp[-2].expressionNode).m_features | (yyvsp[0].expressionNode).m_features, (yyvsp[-2].expressionNode).m_numConstants + (yyvsp[0].expressionNode).m_numConstants); }
    break;

  case 100: /* ShiftExpr: ShiftExpr RSHIFT AdditiveExpr  */
                                        { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(makeRightShiftNode(GLOBAL_DATA, (yyvsp[-2].expressionNode).m_node, (yyvsp[0].expressionNode).m_node, (yyvsp[0].expressionNode).m_features & AssignFeature), (yyvsp[-2].expressionNode).m_features | (yyvsp[0].expressionNode).m_features, (yyvsp[-2].expressionNode).m_numConstants + (yyvsp[0].expressionNode).m_numConstants); }
    break;

  case 101: /* ShiftExpr: ShiftExpr URSHIFT AdditiveExpr  */
                                        { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(new (GLOBAL_DATA) UnsignedRightShiftNode(GLOBAL_DATA, (yyvsp[-2].expressionNode).m_node, (yyvsp[0].expressionNode).m_node, (yyvsp[0].expressionNode).m_features & AssignFeature), (yyvsp[-2].expressionNode).m_features | (yyvsp[0].expressionNode).m_features, (yyvsp[-2].expressionNode).m_numConstants + (yyvsp[0].expressionNode).m_numConstants); }
    break;

  case 103: /* ShiftExprNoBF: ShiftExprNoBF LSHIFT AdditiveExpr  */
                                        { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(makeLeftShiftNode(GLOBAL_DATA, (yyvsp[-2].expressionNode).m_node, (yyvsp[0].expressionNode).m_node, (yyvsp[0].expressionNode).m_features & AssignFeature), (yyvsp[-2].expressionNode).m_features | (yyvsp[0].expressionNode).m_features, (yyvsp[-2].expressionNode).m_numConstants + (yyvsp[0].expressionNode).m_numConstants); }
    break;

  case 104: /* ShiftExprNoBF: ShiftExprNoBF RSHIFT AdditiveExpr  */
                                        { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(makeRightShiftNode(GLOBAL_DATA, (yyvsp[-2].expressionNode).m_node, (yyvsp[0].expressionNode).m_node, (yyvsp[0].expressionNode).m_features & AssignFeature), (yyvsp[-2].expressionNode).m_features | (yyvsp[0].expressionNode).m_features, (yyvsp[-2].expressionNode).m_numConstants + (yyvsp[0].expressionNode).m_numConstants); }
    break;

  case 105: /* ShiftExprNoBF: ShiftExprNoBF URSHIFT AdditiveExpr  */
                                        { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(new (GLOBAL_DATA) UnsignedRightShiftNode(GLOBAL_DATA, (yyvsp[-2].expressionNode).m_node, (yyvsp[0].expressionNode).m_node, (yyvsp[0].expressionNode).m_features & AssignFeature), (yyvsp[-2].expressionNode).m_features | (yyvsp[0].expressionNode).m_features, (yyvsp[-2].expressionNode).m_numConstants + (yyvsp[0].expressionNode).m_numConstants); }
    break;

  case 107: /* RelationalExpr: RelationalExpr '<' ShiftExpr  */
                                        { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(new (GLOBAL_DATA) LessNode(GLOBAL_DATA, (yyvsp[-2].expressionNode).m_node, (yyvsp[0].expressionNode).m_node, (yyvsp[0].expressionNode).m_features & AssignFeature), (yyvsp[-2].expressionNode).m_features | (yyvsp[0].expressionNode).m_features, (yyvsp[-2].expressionNode).m_numConstants + (yyvsp[0].expressionNode).m_numConstants); }
    break;

  case 108: /* RelationalExpr: RelationalExpr '>' ShiftExpr  */
                                        { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(new (GLOBAL_DATA) GreaterNode(GLOBAL_DATA, (yyvsp[-2].expressionNode).m_node, (yyvsp[0].expressionNode).m_node, (yyvsp[0].expressionNode).m_features & AssignFeature), (yyvsp[-2].expressionNode).m_features | (yyvsp[0].expressionNode).m_features, (yyvsp[-2].expressionNode).m_numConstants + (yyvsp[0].expressionNode).m_numConstants); }
    break;

  case 109: /* RelationalExpr: RelationalExpr LE ShiftExpr  */
                                        { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(new (GLOBAL_DATA) LessEqNode(GLOBAL_DATA, (yyvsp[-2].expressionNode).m_node, (yyvsp[0].expressionNode).m_node, (yyvsp[0].expressionNode).m_features & AssignFeature), (yyvsp[-2].expressionNode).m_features | (yyvsp[0].expressionNode).m_features, (yyvsp[-2].expressionNode).m_numConstants + (yyvsp[0].expressionNode).m_numConstants); }
    break;

  case 110: /* RelationalExpr: RelationalExpr GE ShiftExpr  */
                                        { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(new (GLOBAL_DATA) GreaterEqNode(GLOBAL_DATA, (yyvsp[-2].expressionNode).m_node, (yyvsp[0].expressionNode).m_node, (yyvsp[0].expressionNode).m_features & AssignFeature), (yyvsp[-2].expressionNode).m_features | (yyvsp[0].expressionNode).m_features, (yyvsp[-2].expressionNode).m_numConstants + (yyvsp[0].expressionNode).m_numConstants); }
    break;

  case 111: /* RelationalExpr: RelationalExpr INSTANCEOF ShiftExpr  */
                                        { InstanceOfNode* node = new (GLOBAL_DATA) InstanceOfNode(GLOBAL_DATA, (yyvsp[-2].expressionNode).m_node, (yyvsp[0].expressionNode).m_node, (yyvsp[0].expressionNode).m_features & AssignFeature);
                                          setExceptionLocation(node, (yylsp[-2]).first_column, (yylsp[0]).first_column, (yylsp[0]).last_column);  
                                          (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(node, (yyvsp[-2].expressionNode).m_features | (yyvsp[0].expressionNode).m_features, (yyvsp[-2].expressionNode).m_numConstants + (yyvsp[0].expressionNode).m_numConstants); }
    break;

  case 112: /* RelationalExpr: RelationalExpr INTOKEN ShiftExpr  */
                                        { InNode* node = new (GLOBAL_DATA) InNode(GLOBAL_DATA, (yyvsp[-2].expressionNode).m_node, (yyvsp[0].expressionNode).m_node, (yyvsp[0].expressionNode).m_features & AssignFeature);
                                          setExceptionLocation(node, (yylsp[-2]).first_column, (yylsp[0]).first_column, (yylsp[0]).last_column);  
                                          (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(node, (yyvsp[-2].expressionNode).m_features | (yyvsp[0].expressionNode).m_features, (yyvsp[-2].expressionNode).m_numConstants + (yyvsp[0].expressionNode).m_numConstants); }
    break;

  case 114: /* RelationalExprNoIn: RelationalExprNoIn '<' ShiftExpr  */
                                        { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(new (GLOBAL_DATA) LessNode(GLOBAL_DATA, (yyvsp[-2].expressionNode).m_node, (yyvsp[0].expressionNode).m_node, (yyvsp[0].expressionNode).m_features & AssignFeature), (yyvsp[-2].expressionNode).m_features | (yyvsp[0].expressionNode).m_features, (yyvsp[-2].expressionNode).m_numConstants + (yyvsp[0].expressionNode).m_numConstants); }
    break;

  case 115: /* RelationalExprNoIn: RelationalExprNoIn '>' ShiftExpr  */
                                        { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(new (GLOBAL_DATA) GreaterNode(GLOBAL_DATA, (yyvsp[-2].expressionNode).m_node, (yyvsp[0].expressionNode).m_node, (yyvsp[0].expressionNode).m_features & AssignFeature), (yyvsp[-2].expressionNode).m_features | (yyvsp[0].expressionNode).m_features, (yyvsp[-2].expressionNode).m_numConstants + (yyvsp[0].expressionNode).m_numConstants); }
    break;

  case 116: /* RelationalExprNoIn: RelationalExprNoIn LE ShiftExpr  */
                                        { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(new (GLOBAL_DATA) LessEqNode(GLOBAL_DATA, (yyvsp[-2].expressionNode).m_node, (yyvsp[0].expressionNode).m_node, (yyvsp[0].expressionNode).m_features & AssignFeature), (yyvsp[-2].expressionNode).m_features | (yyvsp[0].expressionNode).m_features, (yyvsp[-2].expressionNode).m_numConstants + (yyvsp[0].expressionNode).m_numConstants); }
    break;

  case 117: /* RelationalExprNoIn: RelationalExprNoIn GE ShiftExpr  */
                                        { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(new (GLOBAL_DATA) GreaterEqNode(GLOBAL_DATA, (yyvsp[-2].expressionNode).m_node, (yyvsp[0].expressionNode).m_node, (yyvsp[0].expressionNode).m_features & AssignFeature), (yyvsp[-2].expressionNode).m_features | (yyvsp[0].expressionNode).m_features, (yyvsp[-2].expressionNode).m_numConstants + (yyvsp[0].expressionNode).m_numConstants); }
    break;

  case 118: /* RelationalExprNoIn: RelationalExprNoIn INSTANCEOF ShiftExpr  */
                                        { InstanceOfNode* node = new (GLOBAL_DATA) InstanceOfNode(GLOBAL_DATA, (yyvsp[-2].expressionNode).m_node, (yyvsp[0].expressionNode).m_node, (yyvsp[0].expressionNode).m_features & AssignFeature);
                                          setExceptionLocation(node, (yylsp[-2]).first_column, (yylsp[0]).first_column, (yylsp[0]).last_column);  
                                          (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(node, (yyvsp[-2].expressionNode).m_features | (yyvsp[0].expressionNode).m_features, (yyvsp[-2].expressionNode).m_numConstants + (yyvsp[0].expressionNode).m_numConstants); }
    break;

  case 120: /* RelationalExprNoBF: RelationalExprNoBF '<' ShiftExpr  */
                                        { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(new (GLOBAL_DATA) LessNode(GLOBAL_DATA, (yyvsp[-2].expressionNode).m_node, (yyvsp[0].expressionNode).m_node, (yyvsp[0].expressionNode).m_features & AssignFeature), (yyvsp[-2].expressionNode).m_features | (yyvsp[0].expressionNode).m_features, (yyvsp[-2].expressionNode).m_numConstants + (yyvsp[0].expressionNode).m_numConstants); }
    break;

  case 121: /* RelationalExprNoBF: RelationalExprNoBF '>' ShiftExpr  */
                                        { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(new (GLOBAL_DATA) GreaterNode(GLOBAL_DATA, (yyvsp[-2].expressionNode).m_node, (yyvsp[0].expressionNode).m_node, (yyvsp[0].expressionNode).m_features & AssignFeature), (yyvsp[-2].expressionNode).m_features | (yyvsp[0].expressionNode).m_features, (yyvsp[-2].expressionNode).m_numConstants + (yyvsp[0].expressionNode).m_numConstants); }
    break;

  case 122: /* RelationalExprNoBF: RelationalExprNoBF LE ShiftExpr  */
                                        { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(new (GLOBAL_DATA) LessEqNode(GLOBAL_DATA, (yyvsp[-2].expressionNode).m_node, (yyvsp[0].expressionNode).m_node, (yyvsp[0].expressionNode).m_features & AssignFeature), (yyvsp[-2].expressionNode).m_features | (yyvsp[0].expressionNode).m_features, (yyvsp[-2].expressionNode).m_numConstants + (yyvsp[0].expressionNode).m_numConstants); }
    break;

  case 123: /* RelationalExprNoBF: RelationalExprNoBF GE ShiftExpr  */
                                        { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(new (GLOBAL_DATA) GreaterEqNode(GLOBAL_DATA, (yyvsp[-2].expressionNode).m_node, (yyvsp[0].expressionNode).m_node, (yyvsp[0].expressionNode).m_features & AssignFeature), (yyvsp[-2].expressionNode).m_features | (yyvsp[0].expressionNode).m_features, (yyvsp[-2].expressionNode).m_numConstants + (yyvsp[0].expressionNode).m_numConstants); }
    break;

  case 124: /* RelationalExprNoBF: RelationalExprNoBF INSTANCEOF ShiftExpr  */
                                        { InstanceOfNode* node = new (GLOBAL_DATA) InstanceOfNode(GLOBAL_DATA, (yyvsp[-2].expressionNode).m_node, (yyvsp[0].expressionNode).m_node, (yyvsp[0].expressionNode).m_features & AssignFeature);
                                          setExceptionLocation(node, (yylsp[-2]).first_column, (yylsp[0]).first_column, (yylsp[0]).last_column);  
                                          (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(node, (yyvsp[-2].expressionNode).m_features | (yyvsp[0].expressionNode).m_features, (yyvsp[-2].expressionNode).m_numConstants + (yyvsp[0].expressionNode).m_numConstants); }
    break;

  case 125: /* RelationalExprNoBF: RelationalExprNoBF INTOKEN ShiftExpr  */
                                        { InNode* node = new (GLOBAL_DATA) InNode(GLOBAL_DATA, (yyvsp[-2].expressionNode).m_node, (yyvsp[0].expressionNode).m_node, (yyvsp[0].expressionNode).m_features & AssignFeature);
                                          setExceptionLocation(node, (yylsp[-2]).first_column, (yylsp[0]).first_column, (yylsp[0]).last_column);  
                                          (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(node, (yyvsp[-2].expressionNode).m_features | (yyvsp[0].expressionNode).m_features, (yyvsp[-2].expressionNode).m_numConstants + (yyvsp[0].expressionNode).m_numConstants); }
    break;

  case 127: /* EqualityExpr: EqualityExpr EQEQ RelationalExpr  */
                                        { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(new (GLOBAL_DATA) EqualNode(GLOBAL_DATA, (yyvsp[-2].expressionNode).m_node, (yyvsp[0].expressionNode).m_node, (yyvsp[0].expressionNode).m_features & AssignFeature), (yyvsp[-2].expressionNode).m_features | (yyvsp[0].expressionNode).m_features, (yyvsp[-2].expressionNode).m_numConstants + (yyvsp[0].expressionNode).m_numConstants); }
    break;

  case 128: /* EqualityExpr: EqualityExpr NE RelationalExpr  */
                                        { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(new (GLOBAL_DATA) NotEqualNode(GLOBAL_DATA, (yyvsp[-2].expressionNode).m_node, (yyvsp[0].expressionNode).m_node, (yyvsp[0].expressionNode).m_features & AssignFeature), (yyvsp[-2].expressionNode).m_features | (yyvsp[0].expressionNode).m_features, (yyvsp[-2].expressionNode).m_numConstants + (yyvsp[0].expressionNode).m_numConstants); }
    break;

  case 129: /* EqualityExpr: EqualityExpr STREQ RelationalExpr  */
                                        { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(new (GLOBAL_DATA) StrictEqualNode(GLOBAL_DATA, (yyvsp[-2].expressionNode).m_node, (yyvsp[0].expressionNode).m_node, (yyvsp[0].expressionNode).m_features & AssignFeature), (yyvsp[-2].expressionNode).m_features | (yyvsp[0].expressionNode).m_features, (yyvsp[-2].expressionNode).m_numConstants + (yyvsp[0].expressionNode).m_numConstants); }
    break;

  case 130: /* EqualityExpr: EqualityExpr STRNEQ RelationalExpr  */
                                        { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(new (GLOBAL_DATA) NotStrictEqualNode(GLOBAL_DATA, (yyvsp[-2].expressionNode).m_node, (yyvsp[0].expressionNode).m_node, (yyvsp[0].expressionNode).m_features & AssignFeature), (yyvsp[-2].expressionNode).m_features | (yyvsp[0].expressionNode).m_features, (yyvsp[-2].expressionNode).m_numConstants + (yyvsp[0].expressionNode).m_numConstants); }
    break;

  case 132: /* EqualityExprNoIn: EqualityExprNoIn EQEQ RelationalExprNoIn  */
                                        { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(new (GLOBAL_DATA) EqualNode(GLOBAL_DATA, (yyvsp[-2].expressionNode).m_node, (yyvsp[0].expressionNode).m_node, (yyvsp[0].expressionNode).m_features & AssignFeature), (yyvsp[-2].expressionNode).m_features | (yyvsp[0].expressionNode).m_features, (yyvsp[-2].expressionNode).m_numConstants + (yyvsp[0].expressionNode).m_numConstants); }
    break;

  case 133: /* EqualityExprNoIn: EqualityExprNoIn NE RelationalExprNoIn  */
                                        { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(new (GLOBAL_DATA) NotEqualNode(GLOBAL_DATA, (yyvsp[-2].expressionNode).m_node, (yyvsp[0].expressionNode).m_node, (yyvsp[0].expressionNode).m_features & AssignFeature), (yyvsp[-2].expressionNode).m_features | (yyvsp[0].expressionNode).m_features, (yyvsp[-2].expressionNode).m_numConstants + (yyvsp[0].expressionNode).m_numConstants); }
    break;

  case 134: /* EqualityExprNoIn: EqualityExprNoIn STREQ RelationalExprNoIn  */
                                        { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(new (GLOBAL_DATA) StrictEqualNode(GLOBAL_DATA, (yyvsp[-2].expressionNode).m_node, (yyvsp[0].expressionNode).m_node, (yyvsp[0].expressionNode).m_features & AssignFeature), (yyvsp[-2].expressionNode).m_features | (yyvsp[0].expressionNode).m_features, (yyvsp[-2].expressionNode).m_numConstants + (yyvsp[0].expressionNode).m_numConstants); }
    break;

  case 135: /* EqualityExprNoIn: EqualityExprNoIn STRNEQ RelationalExprNoIn  */
                                        { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(new (GLOBAL_DATA) NotStrictEqualNode(GLOBAL_DATA, (yyvsp[-2].expressionNode).m_node, (yyvsp[0].expressionNode).m_node, (yyvsp[0].expressionNode).m_features & AssignFeature), (yyvsp[-2].expressionNode).m_features | (yyvsp[0].expressionNode).m_features, (yyvsp[-2].expressionNode).m_numConstants + (yyvsp[0].expressionNode).m_numConstants); }
    break;

  case 137: /* EqualityExprNoBF: EqualityExprNoBF EQEQ RelationalExpr  */
                                        { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(new (GLOBAL_DATA) EqualNode(GLOBAL_DATA, (yyvsp[-2].expressionNode).m_node, (yyvsp[0].expressionNode).m_node, (yyvsp[0].expressionNode).m_features & AssignFeature), (yyvsp[-2].expressionNode).m_features | (yyvsp[0].expressionNode).m_features, (yyvsp[-2].expressionNode).m_numConstants + (yyvsp[0].expressionNode).m_numConstants); }
    break;

  case 138: /* EqualityExprNoBF: EqualityExprNoBF NE RelationalExpr  */
                                        { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(new (GLOBAL_DATA) NotEqualNode(GLOBAL_DATA, (yyvsp[-2].expressionNode).m_node, (yyvsp[0].expressionNode).m_node, (yyvsp[0].expressionNode).m_features & AssignFeature), (yyvsp[-2].expressionNode).m_features | (yyvsp[0].expressionNode).m_features, (yyvsp[-2].expressionNode).m_numConstants + (yyvsp[0].expressionNode).m_numConstants); }
    break;

  case 139: /* EqualityExprNoBF: EqualityExprNoBF STREQ RelationalExpr  */
                                        { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(new (GLOBAL_DATA) StrictEqualNode(GLOBAL_DATA, (yyvsp[-2].expressionNode).m_node, (yyvsp[0].expressionNode).m_node, (yyvsp[0].expressionNode).m_features & AssignFeature), (yyvsp[-2].expressionNode).m_features | (yyvsp[0].expressionNode).m_features, (yyvsp[-2].expressionNode).m_numConstants + (yyvsp[0].expressionNode).m_numConstants); }
    break;

  case 140: /* EqualityExprNoBF: EqualityExprNoBF STRNEQ RelationalExpr  */
                                        { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(new (GLOBAL_DATA) NotStrictEqualNode(GLOBAL_DATA, (yyvsp[-2].expressionNode).m_node, (yyvsp[0].expressionNode).m_node, (yyvsp[0].expressionNode).m_features & AssignFeature), (yyvsp[-2].expressionNode).m_features | (yyvsp[0].expressionNode).m_features, (yyvsp[-2].expressionNode).m_numConstants + (yyvsp[0].expressionNode).m_numConstants); }
    break;

  case 142: /* BitwiseANDExpr: BitwiseANDExpr '&' EqualityExpr  */
                                        { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(new (GLOBAL_DATA) BitAndNode(GLOBAL_DATA, (yyvsp[-2].expressionNode).m_node, (yyvsp[0].expressionNode).m_node, (yyvsp[0].expressionNode).m_features & AssignFeature), (yyvsp[-2].expressionNode).m_features | (yyvsp[0].expressionNode).m_features, (yyvsp[-2].expressionNode).m_numConstants + (yyvsp[0].expressionNode).m_numConstants); }
    break;

  case 144: /* BitwiseANDExprNoIn: BitwiseANDExprNoIn '&' EqualityExprNoIn  */
                                        { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(new (GLOBAL_DATA) BitAndNode(GLOBAL_DATA, (yyvsp[-2].expressionNode).m_node, (yyvsp[0].expressionNode).m_node, (yyvsp[0].expressionNode).m_features & AssignFeature), (yyvsp[-2].expressionNode).m_features | (yyvsp[0].expressionNode).m_features, (yyvsp[-2].expressionNode).m_numConstants + (yyvsp[0].expressionNode).m_numConstants); }
    break;

  case 146: /* BitwiseANDExprNoBF: BitwiseANDExprNoBF '&' EqualityExpr  */
                                        { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(new (GLOBAL_DATA) BitAndNode(GLOBAL_DATA, (yyvsp[-2].expressionNode).m_node, (yyvsp[0].expressionNode).m_node, (yyvsp[0].expressionNode).m_features & AssignFeature), (yyvsp[-2].expressionNode).m_features | (yyvsp[0].expressionNode).m_features, (yyvsp[-2].expressionNode).m_numConstants + (yyvsp[0].expressionNode).m_numConstants); }
    break;

  case 148: /* BitwiseXORExpr: BitwiseXORExpr '^' BitwiseANDExpr  */
                                        { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(new (GLOBAL_DATA) BitXOrNode(GLOBAL_DATA, (yyvsp[-2].expressionNode).m_node, (yyvsp[0].expressionNode).m_node, (yyvsp[0].expressionNode).m_features & AssignFeature), (yyvsp[-2].expressionNode).m_features | (yyvsp[0].expressionNode).m_features, (yyvsp[-2].expressionNode).m_numConstants + (yyvsp[0].expressionNode).m_numConstants); }
    break;

  case 150: /* BitwiseXORExprNoIn: BitwiseXORExprNoIn '^' BitwiseANDExprNoIn  */
                                        { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(new (GLOBAL_DATA) BitXOrNode(GLOBAL_DATA, (yyvsp[-2].expressionNode).m_node, (yyvsp[0].expressionNode).m_node, (yyvsp[0].expressionNode).m_features & AssignFeature), (yyvsp[-2].expressionNode).m_features | (yyvsp[0].expressionNode).m_features, (yyvsp[-2].expressionNode).m_numConstants + (yyvsp[0].expressionNode).m_numConstants); }
    break;

  case 152: /* BitwiseXORExprNoBF: BitwiseXORExprNoBF '^' BitwiseANDExpr  */
                                        { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(new (GLOBAL_DATA) BitXOrNode(GLOBAL_DATA, (yyvsp[-2].expressionNode).m_node, (yyvsp[0].expressionNode).m_node, (yyvsp[0].expressionNode).m_features & AssignFeature), (yyvsp[-2].expressionNode).m_features | (yyvsp[0].expressionNode).m_features, (yyvsp[-2].expressionNode).m_numConstants + (yyvsp[0].expressionNode).m_numConstants); }
    break;

  case 154: /* BitwiseORExpr: BitwiseORExpr '|' BitwiseXORExpr  */
                                        { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(new (GLOBAL_DATA) BitOrNode(GLOBAL_DATA, (yyvsp[-2].expressionNode).m_node, (yyvsp[0].expressionNode).m_node, (yyvsp[0].expressionNode).m_features & AssignFeature), (yyvsp[-2].expressionNode).m_features | (yyvsp[0].expressionNode).m_features, (yyvsp[-2].expressionNode).m_numConstants + (yyvsp[0].expressionNode).m_numConstants); }
    break;

  case 156: /* BitwiseORExprNoIn: BitwiseORExprNoIn '|' BitwiseXORExprNoIn  */
                                        { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(new (GLOBAL_DATA) BitOrNode(GLOBAL_DATA, (yyvsp[-2].expressionNode).m_node, (yyvsp[0].expressionNode).m_node, (yyvsp[0].expressionNode).m_features & AssignFeature), (yyvsp[-2].expressionNode).m_features | (yyvsp[0].expressionNode).m_features, (yyvsp[-2].expressionNode).m_numConstants + (yyvsp[0].expressionNode).m_numConstants); }
    break;

  case 158: /* BitwiseORExprNoBF: BitwiseORExprNoBF '|' BitwiseXORExpr  */
                                        { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(new (GLOBAL_DATA) BitOrNode(GLOBAL_DATA, (yyvsp[-2].expressionNode).m_node, (yyvsp[0].expressionNode).m_node, (yyvsp[0].expressionNode).m_features & AssignFeature), (yyvsp[-2].expressionNode).m_features | (yyvsp[0].expressionNode).m_features, (yyvsp[-2].expressionNode).m_numConstants + (yyvsp[0].expressionNode).m_numConstants); }
    break;

  case 160: /* LogicalANDExpr: LogicalANDExpr AND BitwiseORExpr  */
                                        { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(new (GLOBAL_DATA) LogicalOpNode(GLOBAL_DATA, (yyvsp[-2].expressionNode).m_node, (yyvsp[0].expressionNode).m_node, OpLogicalAnd), (yyvsp[-2].expressionNode).m_features | (yyvsp[0].expressionNode).m_features, (yyvsp[-2].expressionNode).m_numConstants + (yyvsp[0].expressionNode).m_numConstants); }
    break;

  case 162: /* LogicalANDExprNoIn: LogicalANDExprNoIn AND BitwiseORExprNoIn  */
                                        { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(new (GLOBAL_DATA) LogicalOpNode(GLOBAL_DATA, (yyvsp[-2].expressionNode).m_node, (yyvsp[0].expressionNode).m_node, OpLogicalAnd), (yyvsp[-2].expressionNode).m_features | (yyvsp[0].expressionNode).m_features, (yyvsp[-2].expressionNode).m_numConstants + (yyvsp[0].expressionNode).m_numConstants); }
    break;

  case 164: /* LogicalANDExprNoBF: LogicalANDExprNoBF AND BitwiseORExpr  */
                                        { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(new (GLOBAL_DATA) LogicalOpNode(GLOBAL_DATA, (yyvsp[-2].expressionNode).m_node, (yyvsp[0].expressionNode).m_node, OpLogicalAnd), (yyvsp[-2].expressionNode).m_features | (yyvsp[0].expressionNode).m_features, (yyvsp[-2].expressionNode).m_numConstants + (yyvsp[0].expressionNode).m_numConstants); }
    break;

  case 166: /* LogicalORExpr: LogicalORExpr OR LogicalANDExpr  */
                                        { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(new (GLOBAL_DATA) LogicalOpNode(GLOBAL_DATA, (yyvsp[-2].expressionNode).m_node, (yyvsp[0].expressionNode).m_node, OpLogicalOr), (yyvsp[-2].expressionNode).m_features | (yyvsp[0].expressionNode).m_features, (yyvsp[-2].expressionNode).m_numConstants + (yyvsp[0].expressionNode).m_numConstants); }
    break;

  case 168: /* LogicalORExprNoIn: LogicalORExprNoIn OR LogicalANDExprNoIn  */
                                        { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(new (GLOBAL_DATA) LogicalOpNode(GLOBAL_DATA, (yyvsp[-2].expressionNode).m_node, (yyvsp[0].expressionNode).m_node, OpLogicalOr), (yyvsp[-2].expressionNode).m_features | (yyvsp[0].expressionNode).m_features, (yyvsp[-2].expressionNode).m_numConstants + (yyvsp[0].expressionNode).m_numConstants); }
    break;

  case 170: /* LogicalORExprNoBF: LogicalORExprNoBF OR LogicalANDExpr  */
                                        { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(new (GLOBAL_DATA) LogicalOpNode(GLOBAL_DATA, (yyvsp[-2].expressionNode).m_node, (yyvsp[0].expressionNode).m_node, OpLogicalOr), (yyvsp[-2].expressionNode).m_features | (yyvsp[0].expressionNode).m_features, (yyvsp[-2].expressionNode).m_numConstants + (yyvsp[0].expressionNode).m_numConstants); }
    break;

  case 172: /* ConditionalExpr: LogicalORExpr '?' AssignmentExpr ':' AssignmentExpr  */
                                        { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(new (GLOBAL_DATA) ConditionalNode(GLOBAL_DATA, (yyvsp[-4].expressionNode).m_node, (yyvsp[-2].expressionNode).m_node, (yyvsp[0].expressionNode).m_node), (yyvsp[-4].expressionNode).m_features | (yyvsp[-2].expressionNode).m_features | (yyvsp[0].expressionNode).m_features, (yyvsp[-4].expressionNode).m_numConstants + (yyvsp[-2].expressionNode).m_numConstants + (yyvsp[0].expressionNode).m_numConstants); }
    break;

  case 174: /* ConditionalExprNoIn: LogicalORExprNoIn '?' AssignmentExprNoIn ':' AssignmentExprNoIn  */
                                        { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(new (GLOBAL_DATA) ConditionalNode(GLOBAL_DATA, (yyvsp[-4].expressionNode).m_node, (yyvsp[-2].expressionNode).m_node, (yyvsp[0].expressionNode).m_node), (yyvsp[-4].expressionNode).m_features | (yyvsp[-2].expressionNode).m_features | (yyvsp[0].expressionNode).m_features, (yyvsp[-4].expressionNode).m_numConstants + (yyvsp[-2].expressionNode).m_numConstants + (yyvsp[0].expressionNode).m_numConstants); }
    break;

  case 176: /* ConditionalExprNoBF: LogicalORExprNoBF '?' AssignmentExpr ':' AssignmentExpr  */
                                        { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(new (GLOBAL_DATA) ConditionalNode(GLOBAL_DATA, (yyvsp[-4].expressionNode).m_node, (yyvsp[-2].expressionNode).m_node, (yyvsp[0].expressionNode).m_node), (yyvsp[-4].expressionNode).m_features | (yyvsp[-2].expressionNode).m_features | (yyvsp[0].expressionNode).m_features, (yyvsp[-4].expressionNode).m_numConstants + (yyvsp[-2].expressionNode).m_numConstants + (yyvsp[0].expressionNode).m_numConstants); }
    break;

  case 178: /* AssignmentExpr: LeftHandSideExpr AssignmentOperator AssignmentExpr  */
                                        { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(makeAssignNode(GLOBAL_DATA, (yyvsp[-2].expressionNode).m_node, (yyvsp[-1].op), (yyvsp[0].expressionNode).m_node, (yyvsp[-2].expressionNode).m_features & AssignFeature, (yyvsp[0].expressionNode).m_features & AssignFeature, 
                                                                                                     (yylsp[-2]).first_column, (yylsp[-1]).first_column + 1, (yylsp[0]).last_column), (yyvsp[-2].expressionNode).m_features | (yyvsp[0].expressionNode).m_features | AssignFeature, (yyvsp[-2].expressionNode).m_numConstants + (yyvsp[0].expressionNode).m_numConstants); 
                                        }
    break;

  case 180: /* AssignmentExprNoIn: LeftHandSideExpr AssignmentOperator AssignmentExprNoIn  */
                                        { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(makeAssignNode(GLOBAL_DATA, (yyvsp[-2].expressionNode).m_node, (yyvsp[-1].op), (yyvsp[0].expressionNode).m_node, (yyvsp[-2].expressionNode).m_features & AssignFeature, (yyvsp[0].expressionNode).m_features & AssignFeature, 
                                                                                                     (yylsp[-2]).first_column, (yylsp[-1]).first_column + 1, (yylsp[0]).last_column), (yyvsp[-2].expressionNode).m_features | (yyvsp[0].expressionNode).m_features | AssignFeature, (yyvsp[-2].expressionNode).m_numConstants + (yyvsp[0].expressionNode).m_numConstants);
                                        }
    break;

  case 182: /* AssignmentExprNoBF: LeftHandSideExprNoBF AssignmentOperator AssignmentExpr  */
                                        { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(makeAssignNode(GLOBAL_DATA, (yyvsp[-2].expressionNode).m_node, (yyvsp[-1].op), (yyvsp[0].expressionNode).m_node, (yyvsp[-2].expressionNode).m_features & AssignFeature, (yyvsp[0].expressionNode).m_features & AssignFeature,
                                                                                                     (yylsp[-2]).first_column, (yylsp[-1]).first_column + 1, (yylsp[0]).last_column), (yyvsp[-2].expressionNode).m_features | (yyvsp[0].expressionNode).m_features | AssignFeature, (yyvsp[-2].expressionNode).m_numConstants + (yyvsp[0].expressionNode).m_numConstants); 
                                        }
    break;

  case 183: /* AssignmentOperator: '='  */
                                        { (yyval.op) = OpEqual; }
    break;

  case 184: /* AssignmentOperator: PLUSEQUAL  */
                                        { (yyval.op) = OpPlusEq; }
    break;

  case 185: /* AssignmentOperator: MINUSEQUAL  */
                                        { (yyval.op) = OpMinusEq; }
    break;

  case 186: /* AssignmentOperator: MULTEQUAL  */
                                        { (yyval.op) = OpMultEq; }
    break;

  case 187: /* AssignmentOperator: DIVEQUAL  */
                                        { (yyval.op) = OpDivEq; }
    break;

  case 188: /* AssignmentOperator: LSHIFTEQUAL  */
                                        { (yyval.op) = OpLShift; }
    break;

  case 189: /* AssignmentOperator: RSHIFTEQUAL  */
                                        { (yyval.op) = OpRShift; }
    break;

  case 190: /* AssignmentOperator: URSHIFTEQUAL  */
                                        { (yyval.op) = OpURShift; }
    break;

  case 191: /* AssignmentOperator: ANDEQUAL  */
                                        { (yyval.op) = OpAndEq; }
    break;

  case 192: /* AssignmentOperator: XOREQUAL  */
                                        { (yyval.op) = OpXOrEq; }
    break;

  case 193: /* AssignmentOperator: OREQUAL  */
                                        { (yyval.op) = OpOrEq; }
    break;

  case 194: /* AssignmentOperator: MODEQUAL  */
                                        { (yyval.op) = OpModEq; }
    break;

  case 196: /* Expr: Expr ',' AssignmentExpr  */
                                        { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(combineCommaNodes(GLOBAL_DATA, (yyvsp[-2].expressionNode).m_node, (yyvsp[0].expressionNode).m_node), (yyvsp[-2].expressionNode).m_features | (yyvsp[0].expressionNode).m_features, (yyvsp[-2].expressionNode).m_numConstants + (yyvsp[0].expressionNode).m_numConstants); }
    break;

  case 198: /* ExprNoIn: ExprNoIn ',' AssignmentExprNoIn  */
                                        { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(combineCommaNodes(GLOBAL_DATA, (yyvsp[-2].expressionNode).m_node, (yyvsp[0].expressionNode).m_node), (yyvsp[-2].expressionNode).m_features | (yyvsp[0].expressionNode).m_features, (yyvsp[-2].expressionNode).m_numConstants + (yyvsp[0].expressionNode).m_numConstants); }
    break;

  case 200: /* ExprNoBF: ExprNoBF ',' AssignmentExpr  */
                                        { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(combineCommaNodes(GLOBAL_DATA, (yyvsp[-2].expressionNode).m_node, (yyvsp[0].expressionNode).m_node), (yyvsp[-2].expressionNode).m_features | (yyvsp[0].expressionNode).m_features, (yyvsp[-2].expressionNode).m_numConstants + (yyvsp[0].expressionNode).m_numConstants); }
    break;

  case 218: /* Block: OPENBRACE CLOSEBRACE  */
                                        { (yyval.statementNode) = createNodeDeclarationInfo<StatementNode*>(new (GLOBAL_DATA) BlockNode(GLOBAL_DATA, 0), 0, 0, 0, 0);
                                          setStatementLocation((yyval.statementNode).m_node, (yylsp[-1]), (yylsp[0])); }
    break;

  case 219: /* Block: OPENBRACE SourceElements CLOSEBRACE  */
                                        { (yyval.statementNode) = createNodeDeclarationInfo<StatementNode*>(new (GLOBAL_DATA) BlockNode(GLOBAL_DATA, (yyvsp[-1].sourceElements).m_node), (yyvsp[-1].sourceElements).m_varDeclarations, (yyvsp[-1].sourceElements).m_funcDeclarations, (yyvsp[-1].sourceElements).m_features, (yyvsp[-1].sourceElements).m_numConstants);
                                          setStatementLocation((yyval.statementNode).m_node, (yylsp[-2]), (yylsp[0])); }
    break;

  case 220: /* VariableStatement: VAR VariableDeclarationList ';'  */
                                        { (yyval.statementNode) = createNodeDeclarationInfo<StatementNode*>(makeVarStatementNode(GLOBAL_DATA, (yyvsp[-1].varDeclList).m_node), (yyvsp[-1].varDeclList).m_varDeclarations, (yyvsp[-1].varDeclList).m_funcDeclarations, (yyvsp[-1].varDeclList).m_features, (yyvsp[-1].varDeclList).m_numConstants);
                                          setStatementLocation((yyval.statementNode).m_node, (yylsp[-2]), (yylsp[0])); }
    break;

  case 221: /* VariableStatement: VAR VariableDeclarationList error  */
                                        { (yyval.statementNode) = createNodeDeclarationInfo<StatementNode*>(makeVarStatementNode(GLOBAL_DATA, (yyvsp[-1].varDeclList).m_node), (yyvsp[-1].varDeclList).m_varDeclarations, (yyvsp[-1].varDeclList).m_funcDeclarations, (yyvsp[-1].varDeclList).m_features, (yyvsp[-1].varDeclList).m_numConstants);
                                          setStatementLocation((yyval.statementNode).m_node, (yylsp[-2]), (yylsp[-1]));
                                          AUTO_SEMICOLON; }
    break;

  case 222: /* VariableDeclarationList: IDENT  */
                                        { (yyval.varDeclList).m_node = 0;
                                          (yyval.varDeclList).m_varDeclarations = new (GLOBAL_DATA) ParserArenaData<DeclarationStacks::VarStack>;
                                          appendToVarDeclarationList(GLOBAL_DATA, (yyval.varDeclList).m_varDeclarations, *(yyvsp[0].ident), 0);
                                          (yyval.varDeclList).m_funcDeclarations = 0;
                                          (yyval.varDeclList).m_features = (*(yyvsp[0].ident) == GLOBAL_DATA->propertyNames->arguments) ? ArgumentsFeature : 0;
                                          (yyval.varDeclList).m_numConstants = 0;
                                        }
    break;

  case 223: /* VariableDeclarationList: IDENT Initializer  */
                                        { AssignResolveNode* node = new (GLOBAL_DATA) AssignResolveNode(GLOBAL_DATA, *(yyvsp[-1].ident), (yyvsp[0].expressionNode).m_node, (yyvsp[0].expressionNode).m_features & AssignFeature);
                                          setExceptionLocation(node, (yylsp[-1]).first_column, (yylsp[0]).first_column + 1, (yylsp[0]).last_column);
                                          (yyval.varDeclList).m_node = node;
                                          (yyval.varDeclList).m_varDeclarations = new (GLOBAL_DATA) ParserArenaData<DeclarationStacks::VarStack>;
                                          appendToVarDeclarationList(GLOBAL_DATA, (yyval.varDeclList).m_varDeclarations, *(yyvsp[-1].ident), DeclarationStacks::HasInitializer);
                                          (yyval.varDeclList).m_funcDeclarations = 0;
                                          (yyval.varDeclList).m_features = ((*(yyvsp[-1].ident) == GLOBAL_DATA->propertyNames->arguments) ? ArgumentsFeature : 0) | (yyvsp[0].expressionNode).m_features;
                                          (yyval.varDeclList).m_numConstants = (yyvsp[0].expressionNode).m_numConstants;
                                        }
    break;

  case 224: /* VariableDeclarationList: VariableDeclarationList ',' IDENT  */
                                        { (yyval.varDeclList).m_node = (yyvsp[-2].varDeclList).m_node;
                                          (yyval.varDeclList).m_varDeclarations = (yyvsp[-2].varDeclList).m_varDeclarations;
                                          appendToVarDeclarationList(GLOBAL_DATA, (yyval.varDeclList).m_varDeclarations, *(yyvsp[0].ident), 0);
                                          (yyval.varDeclList).m_funcDeclarations = 0;
                                          (yyval.varDeclList).m_features = (yyvsp[-2].varDeclList).m_features | ((*(yyvsp[0].ident) == GLOBAL_DATA->propertyNames->arguments) ? ArgumentsFeature : 0);
                                          (yyval.varDeclList).m_numConstants = (yyvsp[-2].varDeclList).m_numConstants;
                                        }
    break;

  case 225: /* VariableDeclarationList: VariableDeclarationList ',' IDENT Initializer  */
                                        { AssignResolveNode* node = new (GLOBAL_DATA) AssignResolveNode(GLOBAL_DATA, *(yyvsp[-1].ident), (yyvsp[0].expressionNode).m_node, (yyvsp[0].expressionNode).m_features & AssignFeature);
                                          setExceptionLocation(node, (yylsp[-1]).first_column, (yylsp[0]).first_column + 1, (yylsp[0]).last_column);
                                          (yyval.varDeclList).m_node = combineCommaNodes(GLOBAL_DATA, (yyvsp[-3].varDeclList).m_node, node);
                                          (yyval.varDeclList).m_varDeclarations = (yyvsp[-3].varDeclList).m_varDeclarations;
                                          appendToVarDeclarationList(GLOBAL_DATA, (yyval.varDeclList).m_varDeclarations, *(yyvsp[-1].ident), DeclarationStacks::HasInitializer);
                                          (yyval.varDeclList).m_funcDeclarations = 0;
                                          (yyval.varDeclList).m_features = (yyvsp[-3].varDeclList).m_features | ((*(yyvsp[-1].ident) == GLOBAL_DATA->propertyNames->arguments) ? ArgumentsFeature : 0) | (yyvsp[0].expressionNode).m_features;
                                          (yyval.varDeclList).m_numConstants = (yyvsp[-3].varDeclList).m_numConstants + (yyvsp[0].expressionNode).m_numConstants;
                                        }
    break;

  case 226: /* VariableDeclarationListNoIn: IDENT  */
                                        { (yyval.varDeclList).m_node = 0;
                                          (yyval.varDeclList).m_varDeclarations = new (GLOBAL_DATA) ParserArenaData<DeclarationStacks::VarStack>;
                                          appendToVarDeclarationList(GLOBAL_DATA, (yyval.varDeclList).m_varDeclarations, *(yyvsp[0].ident), 0);
                                          (yyval.varDeclList).m_funcDeclarations = 0;
                                          (yyval.varDeclList).m_features = (*(yyvsp[0].ident) == GLOBAL_DATA->propertyNames->arguments) ? ArgumentsFeature : 0;
                                          (yyval.varDeclList).m_numConstants = 0;
                                        }
    break;

  case 227: /* VariableDeclarationListNoIn: IDENT InitializerNoIn  */
                                        { AssignResolveNode* node = new (GLOBAL_DATA) AssignResolveNode(GLOBAL_DATA, *(yyvsp[-1].ident), (yyvsp[0].expressionNode).m_node, (yyvsp[0].expressionNode).m_features & AssignFeature);
                                          setExceptionLocation(node, (yylsp[-1]).first_column, (yylsp[0]).first_column + 1, (yylsp[0]).last_column);
                                          (yyval.varDeclList).m_node = node;
                                          (yyval.varDeclList).m_varDeclarations = new (GLOBAL_DATA) ParserArenaData<DeclarationStacks::VarStack>;
                                          appendToVarDeclarationList(GLOBAL_DATA, (yyval.varDeclList).m_varDeclarations, *(yyvsp[-1].ident), DeclarationStacks::HasInitializer);
                                          (yyval.varDeclList).m_funcDeclarations = 0;
                                          (yyval.varDeclList).m_features = ((*(yyvsp[-1].ident) == GLOBAL_DATA->propertyNames->arguments) ? ArgumentsFeature : 0) | (yyvsp[0].expressionNode).m_features;
                                          (yyval.varDeclList).m_numConstants = (yyvsp[0].expressionNode).m_numConstants;
                                        }
    break;

  case 228: /* VariableDeclarationListNoIn: VariableDeclarationListNoIn ',' IDENT  */
                                        { (yyval.varDeclList).m_node = (yyvsp[-2].varDeclList).m_node;
                                          (yyval.varDeclList).m_varDeclarations = (yyvsp[-2].varDeclList).m_varDeclarations;
                                          appendToVarDeclarationList(GLOBAL_DATA, (yyval.varDeclList).m_varDeclarations, *(yyvsp[0].ident), 0);
                                          (yyval.varDeclList).m_funcDeclarations = 0;
                                          (yyval.varDeclList).m_features = (yyvsp[-2].varDeclList).m_features | ((*(yyvsp[0].ident) == GLOBAL_DATA->propertyNames->arguments) ? ArgumentsFeature : 0);
                                          (yyval.varDeclList).m_numConstants = (yyvsp[-2].varDeclList).m_numConstants;
                                        }
    break;

  case 229: /* VariableDeclarationListNoIn: VariableDeclarationListNoIn ',' IDENT InitializerNoIn  */
                                        { AssignResolveNode* node = new (GLOBAL_DATA) AssignResolveNode(GLOBAL_DATA, *(yyvsp[-1].ident), (yyvsp[0].expressionNode).m_node, (yyvsp[0].expressionNode).m_features & AssignFeature);
                                          setExceptionLocation(node, (yylsp[-1]).first_column, (yylsp[0]).first_column + 1, (yylsp[0]).last_column);
                                          (yyval.varDeclList).m_node = combineCommaNodes(GLOBAL_DATA, (yyvsp[-3].varDeclList).m_node, node);
                                          (yyval.varDeclList).m_varDeclarations = (yyvsp[-3].varDeclList).m_varDeclarations;
                                          appendToVarDeclarationList(GLOBAL_DATA, (yyval.varDeclList).m_varDeclarations, *(yyvsp[-1].ident), DeclarationStacks::HasInitializer);
                                          (yyval.varDeclList).m_funcDeclarations = 0;
                                          (yyval.varDeclList).m_features = (yyvsp[-3].varDeclList).m_features | ((*(yyvsp[-1].ident) == GLOBAL_DATA->propertyNames->arguments) ? ArgumentsFeature : 0) | (yyvsp[0].expressionNode).m_features;
                                          (yyval.varDeclList).m_numConstants = (yyvsp[-3].varDeclList).m_numConstants + (yyvsp[0].expressionNode).m_numConstants;
                                        }
    break;

  case 230: /* ConstStatement: CONSTTOKEN ConstDeclarationList ';'  */
                                        { (yyval.statementNode) = createNodeDeclarationInfo<StatementNode*>(new (GLOBAL_DATA) ConstStatementNode(GLOBAL_DATA, (yyvsp[-1].constDeclList).m_node.head), (yyvsp[-1].constDeclList).m_varDeclarations, (yyvsp[-1].constDeclList).m_funcDeclarations, (yyvsp[-1].constDeclList).m_features, (yyvsp[-1].constDeclList).m_numConstants);
                                          setStatementLocation((yyval.statementNode).m_node, (yylsp[-2]), (yylsp[0])); }
    break;

  case 231: /* ConstStatement: CONSTTOKEN ConstDeclarationList error  */
                                        { (yyval.statementNode) = createNodeDeclarationInfo<StatementNode*>(new (GLOBAL_DATA) ConstStatementNode(GLOBAL_DATA, (yyvsp[-1].constDeclList).m_node.head), (yyvsp[-1].constDeclList).m_varDeclarations, (yyvsp[-1].constDeclList).m_funcDeclarations, (yyvsp[-1].constDeclList).m_features, (yyvsp[-1].constDeclList).m_numConstants);
                                          setStatementLocation((yyval.statementNode).m_node, (yylsp[-2]), (yylsp[-1])); AUTO_SEMICOLON; }
    break;

  case 232: /* ConstDeclarationList: ConstDeclaration  */
                                        { (yyval.constDeclList).m_node.head = (yyvsp[0].constDeclNode).m_node;
                                          (yyval.constDeclList).m_node.tail = (yyval.constDeclList).m_node.head;
                                          (yyval.constDeclList).m_varDeclarations = new (GLOBAL_DATA) ParserArenaData<DeclarationStacks::VarStack>;
                                          appendToVarDeclarationList(GLOBAL_DATA, (yyval.constDeclList).m_varDeclarations, (yyvsp[0].constDeclNode).m_node);
                                          (yyval.constDeclList).m_funcDeclarations = 0; 
                                          (yyval.constDeclList).m_features = (yyvsp[0].constDeclNode).m_features;
                                          (yyval.constDeclList).m_numConstants = (yyvsp[0].constDeclNode).m_numConstants;
    }
    break;

  case 233: /* ConstDeclarationList: ConstDeclarationList ',' ConstDeclaration  */
                                        { (yyval.constDeclList).m_node.head = (yyvsp[-2].constDeclList).m_node.head;
                                          (yyvsp[-2].constDeclList).m_node.tail->m_next = (yyvsp[0].constDeclNode).m_node;
                                          (yyval.constDeclList).m_node.tail = (yyvsp[0].constDeclNode).m_node;
                                          (yyval.constDeclList).m_varDeclarations = (yyvsp[-2].constDeclList).m_varDeclarations;
                                          appendToVarDeclarationList(GLOBAL_DATA, (yyval.constDeclList).m_varDeclarations, (yyvsp[0].constDeclNode).m_node);
                                          (yyval.constDeclList).m_funcDeclarations = 0;
                                          (yyval.constDeclList).m_features = (yyvsp[-2].constDeclList).m_features | (yyvsp[0].constDeclNode).m_features;
                                          (yyval.constDeclList).m_numConstants = (yyvsp[-2].constDeclList).m_numConstants + (yyvsp[0].constDeclNode).m_numConstants; }
    break;

  case 234: /* ConstDeclaration: IDENT  */
                                        { (yyval.constDeclNode) = createNodeInfo<ConstDeclNode*>(new (GLOBAL_DATA) ConstDeclNode(GLOBAL_DATA, *(yyvsp[0].ident), 0), (*(yyvsp[0].ident) == GLOBAL_DATA->propertyNames->arguments) ? ArgumentsFeature : 0, 0); }
    break;

  case 235: /* ConstDeclaration: IDENT Initializer  */
                                        { (yyval.constDeclNode) = createNodeInfo<ConstDeclNode*>(new (GLOBAL_DATA) ConstDeclNode(GLOBAL_DATA, *(yyvsp[-1].ident), (yyvsp[0].expressionNode).m_node), ((*(yyvsp[-1].ident) == GLOBAL_DATA->propertyNames->arguments) ? ArgumentsFeature : 0) | (yyvsp[0].expressionNode).m_features, (yyvsp[0].expressionNode).m_numConstants); }
    break;

  case 236: /* Initializer: '=' AssignmentExpr  */
                                        { (yyval.expressionNode) = (yyvsp[0].expressionNode); }
    break;

  case 237: /* InitializerNoIn: '=' AssignmentExprNoIn  */
                                        { (yyval.expressionNode) = (yyvsp[0].expressionNode); }
    break;

  case 238: /* EmptyStatement: ';'  */
                                        { (yyval.statementNode) = createNodeDeclarationInfo<StatementNode*>(new (GLOBAL_DATA) EmptyStatementNode(GLOBAL_DATA), 0, 0, 0, 0); }
    break;

  case 239: /* ExprStatement: ExprNoBF ';'  */
                                        { (yyval.statementNode) = createNodeDeclarationInfo<StatementNode*>(new (GLOBAL_DATA) ExprStatementNode(GLOBAL_DATA, (yyvsp[-1].expressionNode).m_node), 0, 0, (yyvsp[-1].expressionNode).m_features, (yyvsp[-1].expressionNode).m_numConstants);
                                          setStatementLocation((yyval.statementNode).m_node, (yylsp[-1]), (yylsp[0])); }
    break;

  case 240: /* ExprStatement: ExprNoBF error  */
                                        { (yyval.statementNode) = createNodeDeclarationInfo<StatementNode*>(new (GLOBAL_DATA) ExprStatementNode(GLOBAL_DATA, (yyvsp[-1].expressionNode).m_node), 0, 0, (yyvsp[-1].expressionNode).m_features, (yyvsp[-1].expressionNode).m_numConstants);
                                          setStatementLocation((yyval.statementNode).m_node, (yylsp[-1]), (yylsp[-1])); AUTO_SEMICOLON; }
    break;

  case 241: /* IfStatement: IF '(' Expr ')' Statement  */
                                        { (yyval.statementNode) = createNodeDeclarationInfo<StatementNode*>(new (GLOBAL_DATA) IfNode(GLOBAL_DATA, (yyvsp[-2].expressionNode).m_node, (yyvsp[0].statementNode).m_node), (yyvsp[0].statementNode).m_varDeclarations, (yyvsp[0].statementNode).m_funcDeclarations, (yyvsp[-2].expressionNode).m_features | (yyvsp[0].statementNode).m_features, (yyvsp[-2].expressionNode).m_numConstants + (yyvsp[0].statementNode).m_numConstants);
                                          setStatementLocation((yyval.statementNode).m_node, (yylsp[-4]), (yylsp[-1])); }
    break;

  case 242: /* IfStatement: IF '(' Expr ')' Statement ELSE Statement  */
                                        { (yyval.statementNode) = createNodeDeclarationInfo<StatementNode*>(new (GLOBAL_DATA) IfElseNode(GLOBAL_DATA, (yyvsp[-4].expressionNode).m_node, (yyvsp[-2].statementNode).m_node, (yyvsp[0].statementNode).m_node), 
                                                                                         mergeDeclarationLists((yyvsp[-2].statementNode).m_varDeclarations, (yyvsp[0].statementNode).m_varDeclarations),
                                                                                         mergeDeclarationLists((yyvsp[-2].statementNode).m_funcDeclarations, (yyvsp[0].statementNode).m_funcDeclarations),
                                                                                         (yyvsp[-4].expressionNode).m_features | (yyvsp[-2].statementNode).m_features | (yyvsp[0].statementNode).m_features,
                                                                                         (yyvsp[-4].expressionNode).m_numConstants + (yyvsp[-2].statementNode).m_numConstants + (yyvsp[0].statementNode).m_numConstants); 
                                          setStatementLocation((yyval.statementNode).m_node, (yylsp[-6]), (yylsp[-3])); }
    break;

  case 243: /* IterationStatement: DO Statement WHILE '(' Expr ')' ';'  */
                                           { (yyval.statementNode) = createNodeDeclarationInfo<StatementNode*>(new (GLOBAL_DATA) DoWhileNode(GLOBAL_DATA, (yyvsp[-5].statementNode).m_node, (yyvsp[-2].expressionNode).m_node), (yyvsp[-5].statementNode).m_varDeclarations, (yyvsp[-5].statementNode).m_funcDeclarations, (yyvsp[-5].statementNode).m_features | (yyvsp[-2].expressionNode).m_features, (yyvsp[-5].statementNode).m_numConstants + (yyvsp[-2].expressionNode).m_numConstants);
                                             setStatementLocation((yyval.statementNode).m_node, (yylsp[-6]), (yylsp[-4])); }
    break;

  case 244: /* IterationStatement: DO Statement WHILE '(' Expr ')' error  */
                                           { (yyval.statementNode) = createNodeDeclarationInfo<StatementNode*>(new (GLOBAL_DATA) DoWhileNode(GLOBAL_DATA, (yyvsp[-5].statementNode).m_node, (yyvsp[-2].expressionNode).m_node), (yyvsp[-5].statementNode).m_varDeclarations, (yyvsp[-5].statementNode).m_funcDeclarations, (yyvsp[-5].statementNode).m_features | (yyvsp[-2].expressionNode).m_features, (yyvsp[-5].statementNode).m_numConstants + (yyvsp[-2].expressionNode).m_numConstants);
                                             setStatementLocation((yyval.statementNode).m_node, (yylsp[-6]), (yylsp[-4])); }
    break;

  case 245: /* IterationStatement: WHILE '(' Expr ')' Statement  */
                                        { (yyval.statementNode) = createNodeDeclarationInfo<StatementNode*>(new (GLOBAL_DATA) WhileNode(GLOBAL_DATA, (yyvsp[-2].expressionNode).m_node, (yyvsp[0].statementNode).m_node), (yyvsp[0].statementNode).m_varDeclarations, (yyvsp[0].statementNode).m_funcDeclarations, (yyvsp[-2].expressionNode).m_features | (yyvsp[0].statementNode).m_features, (yyvsp[-2].expressionNode).m_numConstants + (yyvsp[0].statementNode).m_numConstants);
                                          setStatementLocation((yyval.statementNode).m_node, (yylsp[-4]), (yylsp[-1])); }
    break;

  case 246: /* IterationStatement: FOR '(' ExprNoInOpt ';' ExprOpt ';' ExprOpt ')' Statement  */
                                        { (yyval.statementNode) = createNodeDeclarationInfo<StatementNode*>(new (GLOBAL_DATA) ForNode(GLOBAL_DATA, (yyvsp[-6].expressionNode).m_node, (yyvsp[-4].expressionNode).m_node, (yyvsp[-2].expressionNode).m_node, (yyvsp[0].statementNode).m_node, false), (yyvsp[0].statementNode).m_varDeclarations, (yyvsp[0].statementNode).m_funcDeclarations, 
                                                                                         (yyvsp[-6].expressionNode).m_features | (yyvsp[-4].expressionNode).m_features | (yyvsp[-2].expressionNode).m_features | (yyvsp[0].statementNode).m_features,
                                                                                         (yyvsp[-6].expressionNode).m_numConstants + (yyvsp[-4].expressionNode).m_numConstants + (yyvsp[-2].expressionNode).m_numConstants + (yyvsp[0].statementNode).m_numConstants);
                                          setStatementLocation((yyval.statementNode).m_node, (yylsp[-8]), (yylsp[-1])); 
                                        }
    break;

  case 247: /* IterationStatement: FOR '(' VAR VariableDeclarationListNoIn ';' ExprOpt ';' ExprOpt ')' Statement  */
                                        { (yyval.statementNode) = createNodeDeclarationInfo<StatementNode*>(new (GLOBAL_DATA) ForNode(GLOBAL_DATA, (yyvsp[-6].varDeclList).m_node, (yyvsp[-4].expressionNode).m_node, (yyvsp[-2].expressionNode).m_node, (yyvsp[0].statementNode).m_node, true),
                                                                                         mergeDeclarationLists((yyvsp[-6].varDeclList).m_varDeclarations, (yyvsp[0].statementNode).m_varDeclarations),
                                                                                         mergeDeclarationLists((yyvsp[-6].varDeclList).m_funcDeclarations, (yyvsp[0].statementNode).m_funcDeclarations),
                                                                                         (yyvsp[-6].varDeclList).m_features | (yyvsp[-4].expressionNode).m_features | (yyvsp[-2].expressionNode).m_features | (yyvsp[0].statementNode).m_features,
                                                                                         (yyvsp[-6].varDeclList).m_numConstants + (yyvsp[-4].expressionNode).m_numConstants + (yyvsp[-2].expressionNode).m_numConstants + (yyvsp[0].statementNode).m_numConstants);
                                          setStatementLocation((yyval.statementNode).m_node, (yylsp[-9]), (yylsp[-1])); }
    break;

  case 248: /* IterationStatement: FOR '(' LeftHandSideExpr INTOKEN Expr ')' Statement  */
                                        {
                                            ForInNode* node = new (GLOBAL_DATA) ForInNode(GLOBAL_DATA, (yyvsp[-4].expressionNode).m_node, (yyvsp[-2].expressionNode).m_node, (yyvsp[0].statementNode).m_node);
                                            setExceptionLocation(node, (yylsp[-4]).first_column, (yylsp[-4]).last_column, (yylsp[-2]).last_column);
                                            (yyval.statementNode) = createNodeDeclarationInfo<StatementNode*>(node, (yyvsp[0].statementNode).m_varDeclarations, (yyvsp[0].statementNode).m_funcDeclarations,
                                                                                           (yyvsp[-4].expressionNode).m_features | (yyvsp[-2].expressionNode).m_features | (yyvsp[0].statementNode).m_features,
                                                                                           (yyvsp[-4].expressionNode).m_numConstants + (yyvsp[-2].expressionNode).m_numConstants + (yyvsp[0].statementNode).m_numConstants);
                                            setStatementLocation((yyval.statementNode).m_node, (yylsp[-6]), (yylsp[-1]));
                                        }
    break;

  case 249: /* IterationStatement: FOR '(' VAR IDENT INTOKEN Expr ')' Statement  */
                                        { ForInNode *forIn = new (GLOBAL_DATA) ForInNode(GLOBAL_DATA, *(yyvsp[-4].ident), 0, (yyvsp[-2].expressionNode).m_node, (yyvsp[0].statementNode).m_node, (yylsp[-3]).first_column, (yylsp[-3]).first_column - (yylsp[-4]).first_column, (yylsp[-2]).last_column - (yylsp[-3]).first_column);
                                          setExceptionLocation(forIn, (yylsp[-4]).first_column, (yylsp[-3]).first_column + 1, (yylsp[-2]).last_column);
                                          appendToVarDeclarationList(GLOBAL_DATA, (yyvsp[0].statementNode).m_varDeclarations, *(yyvsp[-4].ident), DeclarationStacks::HasInitializer);
                                          (yyval.statementNode) = createNodeDeclarationInfo<StatementNode*>(forIn, (yyvsp[0].statementNode).m_varDeclarations, (yyvsp[0].statementNode).m_funcDeclarations, ((*(yyvsp[-4].ident) == GLOBAL_DATA->propertyNames->arguments) ? ArgumentsFeature : 0) | (yyvsp[-2].expressionNode).m_features | (yyvsp[0].statementNode).m_features, (yyvsp[-2].expressionNode).m_numConstants + (yyvsp[0].statementNode).m_numConstants);
                                          setStatementLocation((yyval.statementNode).m_node, (yylsp[-7]), (yylsp[-1])); }
    break;

  case 250: /* IterationStatement: FOR '(' VAR IDENT InitializerNoIn INTOKEN Expr ')' Statement  */
                                        { ForInNode *forIn = new (GLOBAL_DATA) ForInNode(GLOBAL_DATA, *(yyvsp[-5].ident), (yyvsp[-4].expressionNode).m_node, (yyvsp[-2].expressionNode).m_node, (yyvsp[0].statementNode).m_node, (yylsp[-4]).first_column, (yylsp[-4]).first_column - (yylsp[-5]).first_column, (yylsp[-4]).last_column - (yylsp[-4]).first_column);
                                          setExceptionLocation(forIn, (yylsp[-5]).first_column, (yylsp[-3]).first_column + 1, (yylsp[-2]).last_column);
                                          appendToVarDeclarationList(GLOBAL_DATA, (yyvsp[0].statementNode).m_varDeclarations, *(yyvsp[-5].ident), DeclarationStacks::HasInitializer);
                                          (yyval.statementNode) = createNodeDeclarationInfo<StatementNode*>(forIn, (yyvsp[0].statementNode).m_varDeclarations, (yyvsp[0].statementNode).m_funcDeclarations,
                                                                                         ((*(yyvsp[-5].ident) == GLOBAL_DATA->propertyNames->arguments) ? ArgumentsFeature : 0) | (yyvsp[-4].expressionNode).m_features | (yyvsp[-2].expressionNode).m_features | (yyvsp[0].statementNode).m_features,
                                                                                         (yyvsp[-4].expressionNode).m_numConstants + (yyvsp[-2].expressionNode).m_numConstants + (yyvsp[0].statementNode).m_numConstants);
                                          setStatementLocation((yyval.statementNode).m_node, (yylsp[-8]), (yylsp[-1])); }
    break;

  case 251: /* ExprOpt: %empty  */
                                        { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(0, 0, 0); }
    break;

  case 253: /* ExprNoInOpt: %empty  */
                                        { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(0, 0, 0); }
    break;

  case 255: /* ContinueStatement: CONTINUE ';'  */
                                        { ContinueNode* node = new (GLOBAL_DATA) ContinueNode(GLOBAL_DATA);
                                          setExceptionLocation(node, (yylsp[-1]).first_column, (yylsp[-1]).last_column, (yylsp[-1]).last_column); 
                                          (yyval.statementNode) = createNodeDeclarationInfo<StatementNode*>(node, 0, 0, 0, 0);
                                          setStatementLocation((yyval.statementNode).m_node, (yylsp[-1]), (yylsp[0])); }
    break;

  case 256: /* ContinueStatement: CONTINUE error  */
                                        { ContinueNode* node = new (GLOBAL_DATA) ContinueNode(GLOBAL_DATA);
                                          setExceptionLocation(node, (yylsp[-1]).first_column, (yylsp[-1]).last_column, (yylsp[-1]).last_column); 
                                          (yyval.statementNode) = createNodeDeclarationInfo<StatementNode*>(node, 0, 0, 0, 0);
                                          setStatementLocation((yyval.statementNode).m_node, (yylsp[-1]), (yylsp[-1])); AUTO_SEMICOLON; }
    break;

  case 257: /* ContinueStatement: CONTINUE IDENT ';'  */
                                        { ContinueNode* node = new (GLOBAL_DATA) ContinueNode(GLOBAL_DATA, *(yyvsp[-1].ident));
                                          setExceptionLocation(node, (yylsp[-2]).first_column, (yylsp[-1]).last_column, (yylsp[-1]).last_column); 
                                          (yyval.statementNode) = createNodeDeclarationInfo<StatementNode*>(node, 0, 0, 0, 0);
                                          setStatementLocation((yyval.statementNode).m_node, (yylsp[-2]), (yylsp[0])); }
    break;

  case 258: /* ContinueStatement: CONTINUE IDENT error  */
                                        { ContinueNode* node = new (GLOBAL_DATA) ContinueNode(GLOBAL_DATA, *(yyvsp[-1].ident));
                                          setExceptionLocation(node, (yylsp[-2]).first_column, (yylsp[-1]).last_column, (yylsp[-1]).last_column); 
                                          (yyval.statementNode) = createNodeDeclarationInfo<StatementNode*>(node, 0, 0, 0, 0);
                                          setStatementLocation((yyval.statementNode).m_node, (yylsp[-2]), (yylsp[-1])); AUTO_SEMICOLON; }
    break;

  case 259: /* BreakStatement: BREAK ';'  */
                                        { BreakNode* node = new (GLOBAL_DATA) BreakNode(GLOBAL_DATA);
                                          setExceptionLocation(node, (yylsp[-1]).first_column, (yylsp[-1]).last_column, (yylsp[-1]).last_column);
                                          (yyval.statementNode) = createNodeDeclarationInfo<StatementNode*>(node, 0, 0, 0, 0); setStatementLocation((yyval.statementNode).m_node, (yylsp[-1]), (yylsp[0])); }
    break;

  case 260: /* BreakStatement: BREAK error  */
                                        { BreakNode* node = new (GLOBAL_DATA) BreakNode(GLOBAL_DATA);
                                          setExceptionLocation(node, (yylsp[-1]).first_column, (yylsp[-1]).last_column, (yylsp[-1]).last_column);
                                          (yyval.statementNode) = createNodeDeclarationInfo<StatementNode*>(new (GLOBAL_DATA) BreakNode(GLOBAL_DATA), 0, 0, 0, 0); setStatementLocation((yyval.statementNode).m_node, (yylsp[-1]), (yylsp[-1])); AUTO_SEMICOLON; }
    break;

  case 261: /* BreakStatement: BREAK IDENT ';'  */
                                        { BreakNode* node = new (GLOBAL_DATA) BreakNode(GLOBAL_DATA, *(yyvsp[-1].ident));
                                          setExceptionLocation(node, (yylsp[-2]).first_column, (yylsp[-1]).last_column, (yylsp[-1]).last_column);
                                          (yyval.statementNode) = createNodeDeclarationInfo<StatementNode*>(node, 0, 0, 0, 0); setStatementLocation((yyval.statementNode).m_node, (yylsp[-2]), (yylsp[0])); }
    break;

  case 262: /* BreakStatement: BREAK IDENT error  */
                                        { BreakNode* node = new (GLOBAL_DATA) BreakNode(GLOBAL_DATA, *(yyvsp[-1].ident));
                                          setExceptionLocation(node, (yylsp[-2]).first_column, (yylsp[-1]).last_column, (yylsp[-1]).last_column);
                                          (yyval.statementNode) = createNodeDeclarationInfo<StatementNode*>(new (GLOBAL_DATA) BreakNode(GLOBAL_DATA, *(yyvsp[-1].ident)), 0, 0, 0, 0); setStatementLocation((yyval.statementNode).m_node, (yylsp[-2]), (yylsp[-1])); AUTO_SEMICOLON; }
    break;

  case 263: /* ReturnStatement: RETURN ';'  */
                                        { ReturnNode* node = new (GLOBAL_DATA) ReturnNode(GLOBAL_DATA, 0); 
                                          setExceptionLocation(node, (yylsp[-1]).first_column, (yylsp[-1]).last_column, (yylsp[-1]).last_column); 
                                          (yyval.statementNode) = createNodeDeclarationInfo<StatementNode*>(node, 0, 0, 0, 0); setStatementLocation((yyval.statementNode).m_node, (yylsp[-1]), (yylsp[0])); }
    break;

  case 264: /* ReturnStatement: RETURN error  */
                                        { ReturnNode* node = new (GLOBAL_DATA) ReturnNode(GLOBAL_DATA, 0); 
                                          setExceptionLocation(node, (yylsp[-1]).first_column, (yylsp[-1]).last_column, (yylsp[-1]).last_column); 
                                          (yyval.statementNode) = createNodeDeclarationInfo<StatementNode*>(node, 0, 0, 0, 0); setStatementLocation((yyval.statementNode).m_node, (yylsp[-1]), (yylsp[-1])); AUTO_SEMICOLON; }
    break;

  case 265: /* ReturnStatement: RETURN Expr ';'  */
                                        { ReturnNode* node = new (GLOBAL_DATA) ReturnNode(GLOBAL_DATA, (yyvsp[-1].expressionNode).m_node); 
                                          setExceptionLocation(node, (yylsp[-2]).first_column, (yylsp[-1]).last_column, (yylsp[-1]).last_column);
                                          (yyval.statementNode) = createNodeDeclarationInfo<StatementNode*>(node, 0, 0, (yyvsp[-1].expressionNode).m_features, (yyvsp[-1].expressionNode).m_numConstants); setStatementLocation((yyval.statementNode).m_node, (yylsp[-2]), (yylsp[0])); }
    break;

  case 266: /* ReturnStatement: RETURN Expr error  */
                                        { ReturnNode* node = new (GLOBAL_DATA) ReturnNode(GLOBAL_DATA, (yyvsp[-1].expressionNode).m_node); 
                                          setExceptionLocation(node, (yylsp[-2]).first_column, (yylsp[-1]).last_column, (yylsp[-1]).last_column); 
                                          (yyval.statementNode) = createNodeDeclarationInfo<StatementNode*>(node, 0, 0, (yyvsp[-1].expressionNode).m_features, (yyvsp[-1].expressionNode).m_numConstants); setStatementLocation((yyval.statementNode).m_node, (yylsp[-2]), (yylsp[-1])); AUTO_SEMICOLON; }
    break;

  case 267: /* WithStatement: WITH '(' Expr ')' Statement  */
                                        { (yyval.statementNode) = createNodeDeclarationInfo<StatementNode*>(new (GLOBAL_DATA) WithNode(GLOBAL_DATA, (yyvsp[-2].expressionNode).m_node, (yyvsp[0].statementNode).m_node, (yylsp[-2]).last_column, (yylsp[-2]).last_column - (yylsp[-2]).first_column),
                                                                                         (yyvsp[0].statementNode).m_varDeclarations, (yyvsp[0].statementNode).m_funcDeclarations, (yyvsp[-2].expressionNode).m_features | (yyvsp[0].statementNode).m_features | WithFeature, (yyvsp[-2].expressionNode).m_numConstants + (yyvsp[0].statementNode).m_numConstants);
                                          setStatementLocation((yyval.statementNode).m_node, (yylsp[-4]), (yylsp[-1])); }
    break;

  case 268: /* SwitchStatement: SWITCH '(' Expr ')' CaseBlock  */
                                        { (yyval.statementNode) = createNodeDeclarationInfo<StatementNode*>(new (GLOBAL_DATA) SwitchNode(GLOBAL_DATA, (yyvsp[-2].expressionNode).m_node, (yyvsp[0].caseBlockNode).m_node), (yyvsp[0].caseBlockNode).m_varDeclarations, (yyvsp[0].caseBlockNode).m_funcDeclarations,
                                                                                         (yyvsp[-2].expressionNode).m_features | (yyvsp[0].caseBlockNode).m_features, (yyvsp[-2].expressionNode).m_numConstants + (yyvsp[0].caseBlockNode).m_numConstants);
                                          setStatementLocation((yyval.statementNode).m_node, (yylsp[-4]), (yylsp[-1])); }
    break;

  case 269: /* CaseBlock: OPENBRACE CaseClausesOpt CLOSEBRACE  */
                                                     { (yyval.caseBlockNode) = createNodeDeclarationInfo<CaseBlockNode*>(new (GLOBAL_DATA) CaseBlockNode(GLOBAL_DATA, (yyvsp[-1].clauseList).m_node.head, 0, 0), (yyvsp[-1].clauseList).m_varDeclarations, (yyvsp[-1].clauseList).m_funcDeclarations, (yyvsp[-1].clauseList).m_features, (yyvsp[-1].clauseList).m_numConstants); }
    break;

  case 270: /* CaseBlock: OPENBRACE CaseClausesOpt DefaultClause CaseClausesOpt CLOSEBRACE  */
                                        { (yyval.caseBlockNode) = createNodeDeclarationInfo<CaseBlockNode*>(new (GLOBAL_DATA) CaseBlockNode(GLOBAL_DATA, (yyvsp[-3].clauseList).m_node.head, (yyvsp[-2].caseClauseNode).m_node, (yyvsp[-1].clauseList).m_node.head),
                                                                                         mergeDeclarationLists(mergeDeclarationLists((yyvsp[-3].clauseList).m_varDeclarations, (yyvsp[-2].caseClauseNode).m_varDeclarations), (yyvsp[-1].clauseList).m_varDeclarations),
                                                                                         mergeDeclarationLists(mergeDeclarationLists((yyvsp[-3].clauseList).m_funcDeclarations, (yyvsp[-2].caseClauseNode).m_funcDeclarations), (yyvsp[-1].clauseList).m_funcDeclarations),
                                                                                         (yyvsp[-3].clauseList).m_features | (yyvsp[-2].caseClauseNode).m_features | (yyvsp[-1].clauseList).m_features,
                                                                                         (yyvsp[-3].clauseList).m_numConstants + (yyvsp[-2].caseClauseNode).m_numConstants + (yyvsp[-1].clauseList).m_numConstants); }
    break;

  case 271: /* CaseClausesOpt: %empty  */
                                        { (yyval.clauseList).m_node.head = 0; (yyval.clauseList).m_node.tail = 0; (yyval.clauseList).m_varDeclarations = 0; (yyval.clauseList).m_funcDeclarations = 0; (yyval.clauseList).m_features = 0; (yyval.clauseList).m_numConstants = 0; }
    break;

  case 273: /* CaseClauses: CaseClause  */
                                        { (yyval.clauseList).m_node.head = new (GLOBAL_DATA) ClauseListNode(GLOBAL_DATA, (yyvsp[0].caseClauseNode).m_node);
                                          (yyval.clauseList).m_node.tail = (yyval.clauseList).m_node.head;
                                          (yyval.clauseList).m_varDeclarations = (yyvsp[0].caseClauseNode).m_varDeclarations;
                                          (yyval.clauseList).m_funcDeclarations = (yyvsp[0].caseClauseNode).m_funcDeclarations; 
                                          (yyval.clauseList).m_features = (yyvsp[0].caseClauseNode).m_features;
                                          (yyval.clauseList).m_numConstants = (yyvsp[0].caseClauseNode).m_numConstants; }
    break;

  case 274: /* CaseClauses: CaseClauses CaseClause  */
                                        { (yyval.clauseList).m_node.head = (yyvsp[-1].clauseList).m_node.head;
                                          (yyval.clauseList).m_node.tail = new (GLOBAL_DATA) ClauseListNode(GLOBAL_DATA, (yyvsp[-1].clauseList).m_node.tail, (yyvsp[0].caseClauseNode).m_node);
                                          (yyval.clauseList).m_varDeclarations = mergeDeclarationLists((yyvsp[-1].clauseList).m_varDeclarations, (yyvsp[0].caseClauseNode).m_varDeclarations);
                                          (yyval.clauseList).m_funcDeclarations = mergeDeclarationLists((yyvsp[-1].clauseList).m_funcDeclarations, (yyvsp[0].caseClauseNode).m_funcDeclarations);
                                          (yyval.clauseList).m_features = (yyvsp[-1].clauseList).m_features | (yyvsp[0].caseClauseNode).m_features;
                                          (yyval.clauseList).m_numConstants = (yyvsp[-1].clauseList).m_numConstants + (yyvsp[0].caseClauseNode).m_numConstants;
                                        }
    break;

  case 275: /* CaseClause: CASE Expr ':'  */
                                        { (yyval.caseClauseNode) = createNodeDeclarationInfo<CaseClauseNode*>(new (GLOBAL_DATA) CaseClauseNode(GLOBAL_DATA, (yyvsp[-1].expressionNode).m_node), 0, 0, (yyvsp[-1].expressionNode).m_features, (yyvsp[-1].expressionNode).m_numConstants); }
    break;

  case 276: /* CaseClause: CASE Expr ':' SourceElements  */
                                        { (yyval.caseClauseNode) = createNodeDeclarationInfo<CaseClauseNode*>(new (GLOBAL_DATA) CaseClauseNode(GLOBAL_DATA, (yyvsp[-2].expressionNode).m_node, (yyvsp[0].sourceElements).m_node), (yyvsp[0].sourceElements).m_varDeclarations, (yyvsp[0].sourceElements).m_funcDeclarations, (yyvsp[-2].expressionNode).m_features | (yyvsp[0].sourceElements).m_features, (yyvsp[-2].expressionNode).m_numConstants + (yyvsp[0].sourceElements).m_numConstants); }
    break;

  case 277: /* DefaultClause: DEFAULT ':'  */
                                        { (yyval.caseClauseNode) = createNodeDeclarationInfo<CaseClauseNode*>(new (GLOBAL_DATA) CaseClauseNode(GLOBAL_DATA, 0), 0, 0, 0, 0); }
    break;

  case 278: /* DefaultClause: DEFAULT ':' SourceElements  */
                                        { (yyval.caseClauseNode) = createNodeDeclarationInfo<CaseClauseNode*>(new (GLOBAL_DATA) CaseClauseNode(GLOBAL_DATA, 0, (yyvsp[0].sourceElements).m_node), (yyvsp[0].sourceElements).m_varDeclarations, (yyvsp[0].sourceElements).m_funcDeclarations, (yyvsp[0].sourceElements).m_features, (yyvsp[0].sourceElements).m_numConstants); }
    break;

  case 279: /* LabelledStatement: IDENT ':' Statement  */
                                        { LabelNode* node = new (GLOBAL_DATA) LabelNode(GLOBAL_DATA, *(yyvsp[-2].ident), (yyvsp[0].statementNode).m_node);
                                          setExceptionLocation(node, (yylsp[-2]).first_column, (yylsp[-1]).last_column, (yylsp[-1]).last_column);
                                          (yyval.statementNode) = createNodeDeclarationInfo<StatementNode*>(node, (yyvsp[0].statementNode).m_varDeclarations, (yyvsp[0].statementNode).m_funcDeclarations, (yyvsp[0].statementNode).m_features, (yyvsp[0].statementNode).m_numConstants); }
    break;

  case 280: /* ThrowStatement: THROW Expr ';'  */
                                        { ThrowNode* node = new (GLOBAL_DATA) ThrowNode(GLOBAL_DATA, (yyvsp[-1].expressionNode).m_node);
                                          setExceptionLocation(node, (yylsp[-2]).first_column, (yylsp[-1]).last_column, (yylsp[-1]).last_column);
                                          (yyval.statementNode) = createNodeDeclarationInfo<StatementNode*>(node, 0, 0, (yyvsp[-1].expressionNode).m_features, (yyvsp[-1].expressionNode).m_numConstants); setStatementLocation((yyval.statementNode).m_node, (yylsp[-2]), (yylsp[-1]));
                                        }
    break;

  case 281: /* ThrowStatement: THROW Expr error  */
                                        { ThrowNode* node = new (GLOBAL_DATA) ThrowNode(GLOBAL_DATA, (yyvsp[-1].expressionNode).m_node);
                                          setExceptionLocation(node, (yylsp[-2]).first_column, (yylsp[-1]).last_column, (yylsp[-1]).last_column);
                                          (yyval.statementNode) = createNodeDeclarationInfo<StatementNode*>(node, 0, 0, (yyvsp[-1].expressionNode).m_features, (yyvsp[-1].expressionNode).m_numConstants); setStatementLocation((yyval.statementNode).m_node, (yylsp[-2]), (yylsp[-1])); AUTO_SEMICOLON; 
                                        }
    break;

  case 282: /* TryStatement: TRY Block FINALLY Block  */
                                        { (yyval.statementNode) = createNodeDeclarationInfo<StatementNode*>(new (GLOBAL_DATA) TryNode(GLOBAL_DATA, (yyvsp[-2].statementNode).m_node, GLOBAL_DATA->propertyNames->nullIdentifier, false, 0, (yyvsp[0].statementNode).m_node),
                                                                                         mergeDeclarationLists((yyvsp[-2].statementNode).m_varDeclarations, (yyvsp[0].statementNode).m_varDeclarations),
                                                                                         mergeDeclarationLists((yyvsp[-2].statementNode).m_funcDeclarations, (yyvsp[0].statementNode).m_funcDeclarations),
                                                                                         (yyvsp[-2].statementNode).m_features | (yyvsp[0].statementNode).m_features,
                                                                                         (yyvsp[-2].statementNode).m_numConstants + (yyvsp[0].statementNode).m_numConstants);
                                          setStatementLocation((yyval.statementNode).m_node, (yylsp[-3]), (yylsp[-2])); }
    break;

  case 283: /* TryStatement: TRY Block CATCH '(' IDENT ')' Block  */
                                        { (yyval.statementNode) = createNodeDeclarationInfo<StatementNode*>(new (GLOBAL_DATA) TryNode(GLOBAL_DATA, (yyvsp[-5].statementNode).m_node, *(yyvsp[-2].ident), ((yyvsp[0].statementNode).m_features & EvalFeature) != 0, (yyvsp[0].statementNode).m_node, 0),
                                                                                         mergeDeclarationLists((yyvsp[-5].statementNode).m_varDeclarations, (yyvsp[0].statementNode).m_varDeclarations),
                                                                                         mergeDeclarationLists((yyvsp[-5].statementNode).m_funcDeclarations, (yyvsp[0].statementNode).m_funcDeclarations),
                                                                                         (yyvsp[-5].statementNode).m_features | (yyvsp[0].statementNode).m_features | CatchFeature,
                                                                                         (yyvsp[-5].statementNode).m_numConstants + (yyvsp[0].statementNode).m_numConstants);
                                          setStatementLocation((yyval.statementNode).m_node, (yylsp[-6]), (yylsp[-5])); }
    break;

  case 284: /* TryStatement: TRY Block CATCH '(' IDENT ')' Block FINALLY Block  */
                                        { (yyval.statementNode) = createNodeDeclarationInfo<StatementNode*>(new (GLOBAL_DATA) TryNode(GLOBAL_DATA, (yyvsp[-7].statementNode).m_node, *(yyvsp[-4].ident), ((yyvsp[-2].statementNode).m_features & EvalFeature) != 0, (yyvsp[-2].statementNode).m_node, (yyvsp[0].statementNode).m_node),
                                                                                         mergeDeclarationLists(mergeDeclarationLists((yyvsp[-7].statementNode).m_varDeclarations, (yyvsp[-2].statementNode).m_varDeclarations), (yyvsp[0].statementNode).m_varDeclarations),
                                                                                         mergeDeclarationLists(mergeDeclarationLists((yyvsp[-7].statementNode).m_funcDeclarations, (yyvsp[-2].statementNode).m_funcDeclarations), (yyvsp[0].statementNode).m_funcDeclarations),
                                                                                         (yyvsp[-7].statementNode).m_features | (yyvsp[-2].statementNode).m_features | (yyvsp[0].statementNode).m_features | CatchFeature,
                                                                                         (yyvsp[-7].statementNode).m_numConstants + (yyvsp[-2].statementNode).m_numConstants + (yyvsp[0].statementNode).m_numConstants);
                                          setStatementLocation((yyval.statementNode).m_node, (yylsp[-8]), (yylsp[-7])); }
    break;

  case 285: /* DebuggerStatement: DEBUGGER ';'  */
                                        { (yyval.statementNode) = createNodeDeclarationInfo<StatementNode*>(new (GLOBAL_DATA) DebuggerStatementNode(GLOBAL_DATA), 0, 0, 0, 0);
                                          setStatementLocation((yyval.statementNode).m_node, (yylsp[-1]), (yylsp[0])); }
    break;

  case 286: /* DebuggerStatement: DEBUGGER error  */
                                        { (yyval.statementNode) = createNodeDeclarationInfo<StatementNode*>(new (GLOBAL_DATA) DebuggerStatementNode(GLOBAL_DATA), 0, 0, 0, 0);
                                          setStatementLocation((yyval.statementNode).m_node, (yylsp[-1]), (yylsp[-1])); AUTO_SEMICOLON; }
    break;

  case 287: /* FunctionDeclaration: FUNCTION IDENT '(' ')' OPENBRACE FunctionBody CLOSEBRACE  */
                                                             { (yyval.statementNode) = createNodeDeclarationInfo<StatementNode*>(new (GLOBAL_DATA) FuncDeclNode(GLOBAL_DATA, *(yyvsp[-5].ident), (yyvsp[-1].functionBodyNode), GLOBAL_DATA->lexer->sourceCode((yyvsp[-2].intValue), (yyvsp[0].intValue), (yylsp[-2]).first_line)), 0, new (GLOBAL_DATA) ParserArenaData<DeclarationStacks::FunctionStack>, ((*(yyvsp[-5].ident) == GLOBAL_DATA->propertyNames->arguments) ? ArgumentsFeature : 0) | ClosureFeature, 0); setStatementLocation((yyvsp[-1].functionBodyNode), (yylsp[-2]), (yylsp[0])); (yyval.statementNode).m_funcDeclarations->data.append(static_cast<FuncDeclNode*>((yyval.statementNode).m_node)->body()); }
    break;

  case 288: /* FunctionDeclaration: FUNCTION IDENT '(' FormalParameterList ')' OPENBRACE FunctionBody CLOSEBRACE  */
      {
          (yyval.statementNode) = createNodeDeclarationInfo<StatementNode*>(new (GLOBAL_DATA) FuncDeclNode(GLOBAL_DATA, *(yyvsp[-6].ident), (yyvsp[-1].functionBodyNode), GLOBAL_DATA->lexer->sourceCode((yyvsp[-2].intValue), (yyvsp[0].intValue), (yylsp[-2]).first_line), (yyvsp[-4].parameterList).m_node.head), 0, new (GLOBAL_DATA) ParserArenaData<DeclarationStacks::FunctionStack>, ((*(yyvsp[-6].ident) == GLOBAL_DATA->propertyNames->arguments) ? ArgumentsFeature : 0) | (yyvsp[-4].parameterList).m_features | ClosureFeature, 0);
          if ((yyvsp[-4].parameterList).m_features & ArgumentsFeature)
              (yyvsp[-1].functionBodyNode)->setUsesArguments();
          setStatementLocation((yyvsp[-1].functionBodyNode), (yylsp[-2]), (yylsp[0]));
          (yyval.statementNode).m_funcDeclarations->data.append(static_cast<FuncDeclNode*>((yyval.statementNode).m_node)->body());
      }
    break;

  case 289: /* FunctionExpr: FUNCTION '(' ')' OPENBRACE FunctionBody CLOSEBRACE  */
                                                       { (yyval.funcExprNode) = createNodeInfo(new (GLOBAL_DATA) FuncExprNode(GLOBAL_DATA, GLOBAL_DATA->propertyNames->nullIdentifier, (yyvsp[-1].functionBodyNode), GLOBAL_DATA->lexer->sourceCode((yyvsp[-2].intValue), (yyvsp[0].intValue), (yylsp[-2]).first_line)), ClosureFeature, 0); setStatementLocation((yyvsp[-1].functionBodyNode), (yylsp[-2]), (yylsp[0])); }
    break;

  case 290: /* FunctionExpr: FUNCTION '(' FormalParameterList ')' OPENBRACE FunctionBody CLOSEBRACE  */
      {
          (yyval.funcExprNode) = createNodeInfo(new (GLOBAL_DATA) FuncExprNode(GLOBAL_DATA, GLOBAL_DATA->propertyNames->nullIdentifier, (yyvsp[-1].functionBodyNode), GLOBAL_DATA->lexer->sourceCode((yyvsp[-2].intValue), (yyvsp[0].intValue), (yylsp[-2]).first_line), (yyvsp[-4].parameterList).m_node.head), (yyvsp[-4].parameterList).m_features | ClosureFeature, 0);
          if ((yyvsp[-4].parameterList).m_features & ArgumentsFeature)
              (yyvsp[-1].functionBodyNode)->setUsesArguments();
          setStatementLocation((yyvsp[-1].functionBodyNode), (yylsp[-2]), (yylsp[0]));
      }
    break;

  case 291: /* FunctionExpr: FUNCTION IDENT '(' ')' OPENBRACE FunctionBody CLOSEBRACE  */
                                                             { (yyval.funcExprNode) = createNodeInfo(new (GLOBAL_DATA) FuncExprNode(GLOBAL_DATA, *(yyvsp[-5].ident), (yyvsp[-1].functionBodyNode), GLOBAL_DATA->lexer->sourceCode((yyvsp[-2].intValue), (yyvsp[0].intValue), (yylsp[-2]).first_line)), ClosureFeature, 0); setStatementLocation((yyvsp[-1].functionBodyNode), (yylsp[-2]), (yylsp[0])); }
    break;

  case 292: /* FunctionExpr: FUNCTION IDENT '(' FormalParameterList ')' OPENBRACE FunctionBody CLOSEBRACE  */
      {
          (yyval.funcExprNode) = createNodeInfo(new (GLOBAL_DATA) FuncExprNode(GLOBAL_DATA, *(yyvsp[-6].ident), (yyvsp[-1].functionBodyNode), GLOBAL_DATA->lexer->sourceCode((yyvsp[-2].intValue), (yyvsp[0].intValue), (yylsp[-2]).first_line), (yyvsp[-4].parameterList).m_node.head), (yyvsp[-4].parameterList).m_features | ClosureFeature, 0); 
          if ((yyvsp[-4].parameterList).m_features & ArgumentsFeature)
              (yyvsp[-1].functionBodyNode)->setUsesArguments();
          setStatementLocation((yyvsp[-1].functionBodyNode), (yylsp[-2]), (yylsp[0]));
      }
    break;

  case 293: /* FormalParameterList: IDENT  */
                                        { (yyval.parameterList).m_node.head = new (GLOBAL_DATA) ParameterNode(GLOBAL_DATA, *(yyvsp[0].ident));
                                          (yyval.parameterList).m_features = (*(yyvsp[0].ident) == GLOBAL_DATA->propertyNames->arguments) ? ArgumentsFeature : 0;
                                          (yyval.parameterList).m_node.tail = (yyval.parameterList).m_node.head; }
    break;

  case 294: /* FormalParameterList: FormalParameterList ',' IDENT  */
                                        { (yyval.parameterList).m_node.head = (yyvsp[-2].parameterList).m_node.head;
                                          (yyval.parameterList).m_features = (yyvsp[-2].parameterList).m_features | ((*(yyvsp[0].ident) == GLOBAL_DATA->propertyNames->arguments) ? ArgumentsFeature : 0);
                                          (yyval.parameterList).m_node.tail = new (GLOBAL_DATA) ParameterNode(GLOBAL_DATA, (yyvsp[-2].parameterList).m_node.tail, *(yyvsp[0].ident));  }
    break;

  case 295: /* FunctionBody: %empty  */
                                        { (yyval.functionBodyNode) = FunctionBodyNode::create(GLOBAL_DATA); }
    break;

  case 296: /* FunctionBody: SourceElements_NoNode  */
                                        { (yyval.functionBodyNode) = FunctionBodyNode::create(GLOBAL_DATA); }
    break;

  case 297: /* Program: %empty  */
                                        { GLOBAL_DATA->parser->didFinishParsing(new (GLOBAL_DATA) SourceElements(GLOBAL_DATA), 0, 0, NoFeatures, (yylsp[0]).last_line, 0); }
    break;

  case 298: /* Program: SourceElements  */
                                        { GLOBAL_DATA->parser->didFinishParsing((yyvsp[0].sourceElements).m_node, (yyvsp[0].sourceElements).m_varDeclarations, (yyvsp[0].sourceElements).m_funcDeclarations, (yyvsp[0].sourceElements).m_features, 
                                                                                (yylsp[0]).last_line, (yyvsp[0].sourceElements).m_numConstants); }
    break;

  case 299: /* SourceElements: Statement  */
                                        { (yyval.sourceElements).m_node = new (GLOBAL_DATA) SourceElements(GLOBAL_DATA);
                                          (yyval.sourceElements).m_node->append((yyvsp[0].statementNode).m_node);
                                          (yyval.sourceElements).m_varDeclarations = (yyvsp[0].statementNode).m_varDeclarations;
                                          (yyval.sourceElements).m_funcDeclarations = (yyvsp[0].statementNode).m_funcDeclarations;
                                          (yyval.sourceElements).m_features = (yyvsp[0].statementNode).m_features;
                                          (yyval.sourceElements).m_numConstants = (yyvsp[0].statementNode).m_numConstants;
                                        }
    break;

  case 300: /* SourceElements: SourceElements Statement  */
                                        { (yyval.sourceElements).m_node->append((yyvsp[0].statementNode).m_node);
                                          (yyval.sourceElements).m_varDeclarations = mergeDeclarationLists((yyvsp[-1].sourceElements).m_varDeclarations, (yyvsp[0].statementNode).m_varDeclarations);
                                          (yyval.sourceElements).m_funcDeclarations = mergeDeclarationLists((yyvsp[-1].sourceElements).m_funcDeclarations, (yyvsp[0].statementNode).m_funcDeclarations);
                                          (yyval.sourceElements).m_features = (yyvsp[-1].sourceElements).m_features | (yyvsp[0].statementNode).m_features;
                                          (yyval.sourceElements).m_numConstants = (yyvsp[-1].sourceElements).m_numConstants + (yyvsp[0].statementNode).m_numConstants;
                                        }
    break;

  case 304: /* Literal_NoNode: NUMBER  */
           { }
    break;

  case 305: /* Literal_NoNode: STRING  */
           { }
    break;

  case 306: /* Literal_NoNode: '/'  */
                     { if (!GLOBAL_DATA->lexer->skipRegExp()) YYABORT; }
    break;

  case 307: /* Literal_NoNode: DIVEQUAL  */
                                  { if (!GLOBAL_DATA->lexer->skipRegExp()) YYABORT; }
    break;

  case 308: /* Property_NoNode: IDENT ':' AssignmentExpr_NoNode  */
                                    { }
    break;

  case 309: /* Property_NoNode: STRING ':' AssignmentExpr_NoNode  */
                                     { }
    break;

  case 310: /* Property_NoNode: NUMBER ':' AssignmentExpr_NoNode  */
                                     { }
    break;

  case 311: /* Property_NoNode: IDENT IDENT '(' ')' OPENBRACE FunctionBody_NoNode CLOSEBRACE  */
                                                                 { if (*(yyvsp[-6].ident) != "get" && *(yyvsp[-6].ident) != "set") YYABORT; }
    break;

  case 312: /* Property_NoNode: IDENT IDENT '(' FormalParameterList_NoNode ')' OPENBRACE FunctionBody_NoNode CLOSEBRACE  */
                                                                                            { if (*(yyvsp[-7].ident) != "get" && *(yyvsp[-7].ident) != "set") YYABORT; }
    break;

  case 316: /* PrimaryExpr_NoNode: OPENBRACE CLOSEBRACE  */
                         { }
    break;

  case 317: /* PrimaryExpr_NoNode: OPENBRACE PropertyList_NoNode CLOSEBRACE  */
                                             { }
    break;

  case 318: /* PrimaryExpr_NoNode: OPENBRACE PropertyList_NoNode ',' CLOSEBRACE  */
                                                 { }
    break;

  case 322: /* PrimaryExprNoBrace_NoNode: IDENT  */
          { }
    break;

  case 517: /* Block_NoNode: OPENBRACE CLOSEBRACE  */
                         { }
    break;

  case 518: /* Block_NoNode: OPENBRACE SourceElements_NoNode CLOSEBRACE  */
                                               { }
    break;

  case 520: /* VariableStatement_NoNode: VAR VariableDeclarationList_NoNode error  */
                                             { AUTO_SEMICOLON; }
    break;

  case 521: /* VariableDeclarationList_NoNode: IDENT  */
          { }
    break;

  case 522: /* VariableDeclarationList_NoNode: IDENT Initializer_NoNode  */
                             { }
    break;

  case 525: /* VariableDeclarationListNoIn_NoNode: IDENT  */
          { }
    break;

  case 526: /* VariableDeclarationListNoIn_NoNode: IDENT InitializerNoIn_NoNode  */
                                 { }
    break;

  case 530: /* ConstStatement_NoNode: CONSTTOKEN ConstDeclarationList_NoNode error  */
                                                 { AUTO_SEMICOLON; }
    break;

  case 533: /* ConstDeclaration_NoNode: IDENT  */
          { }
    break;

  case 534: /* ConstDeclaration_NoNode: IDENT Initializer_NoNode  */
                             { }
    break;

  case 539: /* ExprStatement_NoNode: ExprNoBF_NoNode error  */
                          { AUTO_SEMICOLON; }
    break;

  case 555: /* ContinueStatement_NoNode: CONTINUE error  */
                   { AUTO_SEMICOLON; }
    break;

  case 557: /* ContinueStatement_NoNode: CONTINUE IDENT error  */
                         { AUTO_SEMICOLON; }
    break;

  case 559: /* BreakStatement_NoNode: BREAK error  */
                { AUTO_SEMICOLON; }
    break;

  case 561: /* BreakStatement_NoNode: BREAK IDENT error  */
                      { AUTO_SEMICOLON; }
    break;

  case 563: /* ReturnStatement_NoNode: RETURN error  */
                 { AUTO_SEMICOLON; }
    break;

  case 565: /* ReturnStatement_NoNode: RETURN Expr_NoNode error  */
                             { AUTO_SEMICOLON; }
    break;

  case 568: /* CaseBlock_NoNode: OPENBRACE CaseClausesOpt_NoNode CLOSEBRACE  */
                                               { }
    break;

  case 569: /* CaseBlock_NoNode: OPENBRACE CaseClausesOpt_NoNode DefaultClause_NoNode CaseClausesOpt_NoNode CLOSEBRACE  */
                                                                                          { }
    break;

  case 578: /* LabelledStatement_NoNode: IDENT ':' Statement_NoNode  */
                               { }
    break;

  case 580: /* ThrowStatement_NoNode: THROW Expr_NoNode error  */
                            { AUTO_SEMICOLON; }
    break;

  case 585: /* DebuggerStatement_NoNode: DEBUGGER error  */
                   { AUTO_SEMICOLON; }
    break;

  case 592: /* FormalParameterList_NoNode: IDENT  */
          { }
    break;



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
  *++yylsp = yyloc;

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
      yyerror (&yylloc, globalPtr, YY_("syntax error"));
    }

  yyerror_range[1] = yylloc;
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
                      yytoken, &yylval, &yylloc, globalPtr);
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

      yyerror_range[1] = *yylsp;
      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp, yylsp, globalPtr);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  yyerror_range[2] = yylloc;
  ++yylsp;
  YYLLOC_DEFAULT (*yylsp, yyerror_range, 2);

  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;


#if !defined yyoverflow
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (&yylloc, globalPtr, YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturn;
#endif


/*-------------------------------------------------------.
| yyreturn -- parsing is finished, clean up and return.  |
`-------------------------------------------------------*/
yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval, &yylloc, globalPtr);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp, yylsp, globalPtr);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}



#undef GLOBAL_DATA

static ExpressionNode* makeAssignNode(JSGlobalData* globalData, ExpressionNode* loc, Operator op, ExpressionNode* expr, bool locHasAssignments, bool exprHasAssignments, int start, int divot, int end)
{
    if (!loc->isLocation())
        return new (globalData) AssignErrorNode(globalData, loc, op, expr, divot, divot - start, end - divot);

    if (loc->isResolveNode()) {
        ResolveNode* resolve = static_cast<ResolveNode*>(loc);
        if (op == OpEqual) {
            AssignResolveNode* node = new (globalData) AssignResolveNode(globalData, resolve->identifier(), expr, exprHasAssignments);
            setExceptionLocation(node, start, divot, end);
            return node;
        } else
            return new (globalData) ReadModifyResolveNode(globalData, resolve->identifier(), op, expr, exprHasAssignments, divot, divot - start, end - divot);
    }
    if (loc->isBracketAccessorNode()) {
        BracketAccessorNode* bracket = static_cast<BracketAccessorNode*>(loc);
        if (op == OpEqual)
            return new (globalData) AssignBracketNode(globalData, bracket->base(), bracket->subscript(), expr, locHasAssignments, exprHasAssignments, bracket->divot(), bracket->divot() - start, end - bracket->divot());
        else {
            ReadModifyBracketNode* node = new (globalData) ReadModifyBracketNode(globalData, bracket->base(), bracket->subscript(), op, expr, locHasAssignments, exprHasAssignments, divot, divot - start, end - divot);
            node->setSubexpressionInfo(bracket->divot(), bracket->endOffset());
            return node;
        }
    }
    Q_ASSERT(loc->isDotAccessorNode());
    DotAccessorNode* dot = static_cast<DotAccessorNode*>(loc);
    if (op == OpEqual)
        return new (globalData) AssignDotNode(globalData, dot->base(), dot->identifier(), expr, exprHasAssignments, dot->divot(), dot->divot() - start, end - dot->divot());

    ReadModifyDotNode* node = new (globalData) ReadModifyDotNode(globalData, dot->base(), dot->identifier(), op, expr, exprHasAssignments, divot, divot - start, end - divot);
    node->setSubexpressionInfo(dot->divot(), dot->endOffset());
    return node;
}

static ExpressionNode* makePrefixNode(JSGlobalData* globalData, ExpressionNode* expr, Operator op, int start, int divot, int end)
{
    if (!expr->isLocation())
        return new (globalData) PrefixErrorNode(globalData, expr, op, divot, divot - start, end - divot);
    
    if (expr->isResolveNode()) {
        ResolveNode* resolve = static_cast<ResolveNode*>(expr);
        return new (globalData) PrefixResolveNode(globalData, resolve->identifier(), op, divot, divot - start, end - divot);
    }
    if (expr->isBracketAccessorNode()) {
        BracketAccessorNode* bracket = static_cast<BracketAccessorNode*>(expr);
        PrefixBracketNode* node = new (globalData) PrefixBracketNode(globalData, bracket->base(), bracket->subscript(), op, divot, divot - start, end - divot);
        node->setSubexpressionInfo(bracket->divot(), bracket->startOffset());
        return node;
    }
    Q_ASSERT(expr->isDotAccessorNode());
    DotAccessorNode* dot = static_cast<DotAccessorNode*>(expr);
    PrefixDotNode* node = new (globalData) PrefixDotNode(globalData, dot->base(), dot->identifier(), op, divot, divot - start, end - divot);
    node->setSubexpressionInfo(dot->divot(), dot->startOffset());
    return node;
}

static ExpressionNode* makePostfixNode(JSGlobalData* globalData, ExpressionNode* expr, Operator op, int start, int divot, int end)
{ 
    if (!expr->isLocation())
        return new (globalData) PostfixErrorNode(globalData, expr, op, divot, divot - start, end - divot);
    
    if (expr->isResolveNode()) {
        ResolveNode* resolve = static_cast<ResolveNode*>(expr);
        return new (globalData) PostfixResolveNode(globalData, resolve->identifier(), op, divot, divot - start, end - divot);
    }
    if (expr->isBracketAccessorNode()) {
        BracketAccessorNode* bracket = static_cast<BracketAccessorNode*>(expr);
        PostfixBracketNode* node = new (globalData) PostfixBracketNode(globalData, bracket->base(), bracket->subscript(), op, divot, divot - start, end - divot);
        node->setSubexpressionInfo(bracket->divot(), bracket->endOffset());
        return node;
        
    }
    Q_ASSERT(expr->isDotAccessorNode());
    DotAccessorNode* dot = static_cast<DotAccessorNode*>(expr);
    PostfixDotNode* node = new (globalData) PostfixDotNode(globalData, dot->base(), dot->identifier(), op, divot, divot - start, end - divot);
    node->setSubexpressionInfo(dot->divot(), dot->endOffset());
    return node;
}

static ExpressionNodeInfo makeFunctionCallNode(JSGlobalData* globalData, ExpressionNodeInfo func, ArgumentsNodeInfo args, int start, int divot, int end)
{
    CodeFeatures features = func.m_features | args.m_features;
    int numConstants = func.m_numConstants + args.m_numConstants;
    if (!func.m_node->isLocation())
        return createNodeInfo<ExpressionNode*>(new (globalData) FunctionCallValueNode(globalData, func.m_node, args.m_node, divot, divot - start, end - divot), features, numConstants);
    if (func.m_node->isResolveNode()) {
        ResolveNode* resolve = static_cast<ResolveNode*>(func.m_node);
        const Identifier& identifier = resolve->identifier();
        if (identifier == globalData->propertyNames->eval)
            return createNodeInfo<ExpressionNode*>(new (globalData) EvalFunctionCallNode(globalData, args.m_node, divot, divot - start, end - divot), EvalFeature | features, numConstants);
        return createNodeInfo<ExpressionNode*>(new (globalData) FunctionCallResolveNode(globalData, identifier, args.m_node, divot, divot - start, end - divot), features, numConstants);
    }
    if (func.m_node->isBracketAccessorNode()) {
        BracketAccessorNode* bracket = static_cast<BracketAccessorNode*>(func.m_node);
        FunctionCallBracketNode* node = new (globalData) FunctionCallBracketNode(globalData, bracket->base(), bracket->subscript(), args.m_node, divot, divot - start, end - divot);
        node->setSubexpressionInfo(bracket->divot(), bracket->endOffset());
        return createNodeInfo<ExpressionNode*>(node, features, numConstants);
    }
    Q_ASSERT(func.m_node->isDotAccessorNode());
    DotAccessorNode* dot = static_cast<DotAccessorNode*>(func.m_node);
    FunctionCallDotNode* node;
    if (dot->identifier() == globalData->propertyNames->call)
        node = new (globalData) CallFunctionCallDotNode(globalData, dot->base(), dot->identifier(), args.m_node, divot, divot - start, end - divot);
    else if (dot->identifier() == globalData->propertyNames->apply)
        node = new (globalData) ApplyFunctionCallDotNode(globalData, dot->base(), dot->identifier(), args.m_node, divot, divot - start, end - divot);
    else
        node = new (globalData) FunctionCallDotNode(globalData, dot->base(), dot->identifier(), args.m_node, divot, divot - start, end - divot);
    node->setSubexpressionInfo(dot->divot(), dot->endOffset());
    return createNodeInfo<ExpressionNode*>(node, features, numConstants);
}

static ExpressionNode* makeTypeOfNode(JSGlobalData* globalData, ExpressionNode* expr)
{
    if (expr->isResolveNode()) {
        ResolveNode* resolve = static_cast<ResolveNode*>(expr);
        return new (globalData) TypeOfResolveNode(globalData, resolve->identifier());
    }
    return new (globalData) TypeOfValueNode(globalData, expr);
}

static ExpressionNode* makeDeleteNode(JSGlobalData* globalData, ExpressionNode* expr, int start, int divot, int end)
{
    if (!expr->isLocation())
        return new (globalData) DeleteValueNode(globalData, expr);
    if (expr->isResolveNode()) {
        ResolveNode* resolve = static_cast<ResolveNode*>(expr);
        return new (globalData) DeleteResolveNode(globalData, resolve->identifier(), divot, divot - start, end - divot);
    }
    if (expr->isBracketAccessorNode()) {
        BracketAccessorNode* bracket = static_cast<BracketAccessorNode*>(expr);
        return new (globalData) DeleteBracketNode(globalData, bracket->base(), bracket->subscript(), divot, divot - start, end - divot);
    }
    Q_ASSERT(expr->isDotAccessorNode());
    DotAccessorNode* dot = static_cast<DotAccessorNode*>(expr);
    return new (globalData) DeleteDotNode(globalData, dot->base(), dot->identifier(), divot, divot - start, end - divot);
}

static PropertyNode* makeGetterOrSetterPropertyNode(JSGlobalData* globalData, const Identifier& getOrSet, const Identifier& name, ParameterNode* params, FunctionBodyNode* body, const SourceCode& source)
{
    PropertyNode::Type type;
    if (getOrSet == "get")
        type = PropertyNode::Getter;
    else if (getOrSet == "set")
        type = PropertyNode::Setter;
    else
        return 0;
    return new (globalData) PropertyNode(globalData, name, new (globalData) FuncExprNode(globalData, globalData->propertyNames->nullIdentifier, body, source, params), type);
}

static ExpressionNode* makeNegateNode(JSGlobalData* globalData, ExpressionNode* n)
{
    if (n->isNumber()) {
        NumberNode* numberNode = static_cast<NumberNode*>(n);
        numberNode->setValue(-numberNode->value());
        return numberNode;
    }

    return new (globalData) NegateNode(globalData, n);
}

static NumberNode* makeNumberNode(JSGlobalData* globalData, double d)
{
    return new (globalData) NumberNode(globalData, d);
}

static ExpressionNode* makeBitwiseNotNode(JSGlobalData* globalData, ExpressionNode* expr)
{
    if (expr->isNumber())
        return makeNumberNode(globalData, ~toInt32(static_cast<NumberNode*>(expr)->value()));
    return new (globalData) BitwiseNotNode(globalData, expr);
}

static ExpressionNode* makeMultNode(JSGlobalData* globalData, ExpressionNode* expr1, ExpressionNode* expr2, bool rightHasAssignments)
{
    expr1 = expr1->stripUnaryPlus();
    expr2 = expr2->stripUnaryPlus();

    if (expr1->isNumber() && expr2->isNumber())
        return makeNumberNode(globalData, static_cast<NumberNode*>(expr1)->value() * static_cast<NumberNode*>(expr2)->value());

    if (expr1->isNumber() && static_cast<NumberNode*>(expr1)->value() == 1)
        return new (globalData) UnaryPlusNode(globalData, expr2);

    if (expr2->isNumber() && static_cast<NumberNode*>(expr2)->value() == 1)
        return new (globalData) UnaryPlusNode(globalData, expr1);

    return new (globalData) MultNode(globalData, expr1, expr2, rightHasAssignments);
}

static ExpressionNode* makeDivNode(JSGlobalData* globalData, ExpressionNode* expr1, ExpressionNode* expr2, bool rightHasAssignments)
{
    expr1 = expr1->stripUnaryPlus();
    expr2 = expr2->stripUnaryPlus();

    if (expr1->isNumber() && expr2->isNumber())
        return makeNumberNode(globalData, static_cast<NumberNode*>(expr1)->value() / static_cast<NumberNode*>(expr2)->value());
    return new (globalData) DivNode(globalData, expr1, expr2, rightHasAssignments);
}

static ExpressionNode* makeAddNode(JSGlobalData* globalData, ExpressionNode* expr1, ExpressionNode* expr2, bool rightHasAssignments)
{
    if (expr1->isNumber() && expr2->isNumber())
        return makeNumberNode(globalData, static_cast<NumberNode*>(expr1)->value() + static_cast<NumberNode*>(expr2)->value());
    return new (globalData) AddNode(globalData, expr1, expr2, rightHasAssignments);
}

static ExpressionNode* makeSubNode(JSGlobalData* globalData, ExpressionNode* expr1, ExpressionNode* expr2, bool rightHasAssignments)
{
    expr1 = expr1->stripUnaryPlus();
    expr2 = expr2->stripUnaryPlus();

    if (expr1->isNumber() && expr2->isNumber())
        return makeNumberNode(globalData, static_cast<NumberNode*>(expr1)->value() - static_cast<NumberNode*>(expr2)->value());
    return new (globalData) SubNode(globalData, expr1, expr2, rightHasAssignments);
}

static ExpressionNode* makeLeftShiftNode(JSGlobalData* globalData, ExpressionNode* expr1, ExpressionNode* expr2, bool rightHasAssignments)
{
    if (expr1->isNumber() && expr2->isNumber())
        return makeNumberNode(globalData, toInt32(static_cast<NumberNode*>(expr1)->value()) << (toUInt32(static_cast<NumberNode*>(expr2)->value()) & 0x1f));
    return new (globalData) LeftShiftNode(globalData, expr1, expr2, rightHasAssignments);
}

static ExpressionNode* makeRightShiftNode(JSGlobalData* globalData, ExpressionNode* expr1, ExpressionNode* expr2, bool rightHasAssignments)
{
    if (expr1->isNumber() && expr2->isNumber())
        return makeNumberNode(globalData, toInt32(static_cast<NumberNode*>(expr1)->value()) >> (toUInt32(static_cast<NumberNode*>(expr2)->value()) & 0x1f));
    return new (globalData) RightShiftNode(globalData, expr1, expr2, rightHasAssignments);
}

// May we automatically insert a semicolon?
static bool allowAutomaticSemicolon(Lexer& lexer, int yychar)
{
    return yychar == CLOSEBRACE || yychar == 0 || lexer.prevTerminator();
}

static ExpressionNode* combineCommaNodes(JSGlobalData* globalData, ExpressionNode* list, ExpressionNode* init)
{
    if (!list)
        return init;
    if (list->isCommaNode()) {
        static_cast<CommaNode*>(list)->append(init);
        return list;
    }
    return new (globalData) CommaNode(globalData, list, init);
}

// We turn variable declarations into either assignments or empty
// statements (which later get stripped out), because the actual
// declaration work is hoisted up to the start of the function body
static StatementNode* makeVarStatementNode(JSGlobalData* globalData, ExpressionNode* expr)
{
    if (!expr)
        return new (globalData) EmptyStatementNode(globalData);
    return new (globalData) VarStatementNode(globalData, expr);
}
