/* A Bison parser, made by GNU Bison 3.5.4.  */

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

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.5.4"

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
#include <wtf/MathExtras.h>

#define YYMALLOC fastMalloc
#define YYFREE fastFree

#define YYMAXDEPTH 10000
#define YYENABLE_NLS 0

// Default values for bison.
#define YYDEBUG 0 // Set to 1 to debug a parse error.
#define jscyydebug 0 // Set to 1 to debug a parse error.
#if !OS(DARWIN)
// Avoid triggering warnings in older bison by not setting this on the Darwin platform.
// FIXME: Is this still needed?
#define YYERROR_VERBOSE
#endif

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

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Use api.header.include to #include this header
   instead of duplicating it here.  */
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

#if ! defined yyoverflow || YYERROR_VERBOSE

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
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


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

#define YYUNDEFTOK  2
#define YYMAXUTOK   320


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

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
       0,   296,   296,   297,   298,   299,   300,   301,   312,   326,
     327,   328,   329,   330,   342,   346,   353,   354,   355,   357,
     361,   362,   363,   364,   365,   369,   370,   371,   375,   379,
     387,   388,   392,   393,   397,   398,   399,   403,   407,   414,
     415,   419,   423,   430,   431,   438,   439,   446,   447,   448,
     452,   458,   459,   460,   464,   471,   472,   476,   480,   487,
     488,   492,   493,   497,   498,   499,   503,   504,   505,   509,
     510,   511,   512,   513,   514,   515,   516,   517,   518,   519,
     522,   523,   527,   528,   532,   533,   534,   535,   539,   540,
     542,   544,   549,   550,   551,   555,   556,   558,   563,   564,
     565,   566,   570,   571,   572,   573,   577,   578,   579,   580,
     581,   582,   585,   591,   592,   593,   594,   595,   596,   603,
     604,   605,   606,   607,   608,   612,   619,   620,   621,   622,
     623,   627,   628,   630,   632,   634,   639,   640,   642,   643,
     645,   650,   651,   655,   656,   661,   662,   666,   667,   671,
     672,   677,   678,   683,   684,   688,   689,   694,   695,   700,
     701,   705,   706,   711,   712,   717,   718,   722,   723,   728,
     729,   733,   734,   739,   740,   745,   746,   751,   752,   759,
     760,   767,   768,   775,   776,   777,   778,   779,   780,   781,
     782,   783,   784,   785,   786,   790,   791,   795,   796,   800,
     801,   805,   806,   807,   808,   809,   810,   811,   812,   813,
     814,   815,   816,   817,   818,   819,   820,   821,   825,   827,
     832,   834,   840,   847,   856,   864,   877,   884,   893,   901,
     914,   916,   922,   930,   942,   943,   947,   951,   955,   959,
     961,   966,   969,   979,   981,   983,   985,   991,   998,  1007,
    1013,  1024,  1025,  1029,  1030,  1034,  1038,  1042,  1046,  1053,
    1056,  1059,  1062,  1068,  1071,  1074,  1077,  1083,  1089,  1095,
    1096,  1105,  1106,  1110,  1116,  1126,  1127,  1131,  1132,  1136,
    1142,  1146,  1153,  1159,  1165,  1175,  1177,  1182,  1183,  1194,
    1195,  1202,  1203,  1213,  1216,  1222,  1223,  1227,  1228,  1233,
    1240,  1251,  1252,  1253,  1254,  1255,  1256,  1257,  1261,  1262,
    1263,  1264,  1265,  1269,  1270,  1274,  1275,  1276,  1278,  1282,
    1283,  1284,  1285,  1286,  1290,  1291,  1292,  1296,  1297,  1300,
    1302,  1306,  1307,  1311,  1312,  1313,  1314,  1315,  1319,  1320,
    1321,  1322,  1326,  1327,  1331,  1332,  1336,  1337,  1338,  1339,
    1343,  1344,  1345,  1346,  1350,  1351,  1355,  1356,  1360,  1361,
    1365,  1366,  1370,  1371,  1372,  1376,  1377,  1378,  1382,  1383,
    1384,  1385,  1386,  1387,  1388,  1389,  1390,  1391,  1392,  1395,
    1396,  1400,  1401,  1405,  1406,  1407,  1408,  1412,  1413,  1414,
    1415,  1419,  1420,  1421,  1425,  1426,  1427,  1431,  1432,  1433,
    1434,  1438,  1439,  1440,  1441,  1445,  1446,  1447,  1448,  1449,
    1450,  1451,  1455,  1456,  1457,  1458,  1459,  1460,  1464,  1465,
    1466,  1467,  1468,  1469,  1470,  1474,  1475,  1476,  1477,  1478,
    1482,  1483,  1484,  1485,  1486,  1490,  1491,  1492,  1493,  1494,
    1498,  1499,  1503,  1504,  1508,  1509,  1513,  1514,  1518,  1519,
    1523,  1524,  1528,  1529,  1533,  1534,  1538,  1539,  1543,  1544,
    1548,  1549,  1553,  1554,  1558,  1559,  1563,  1564,  1568,  1569,
    1573,  1574,  1578,  1579,  1583,  1584,  1588,  1589,  1593,  1594,
    1598,  1599,  1603,  1604,  1605,  1606,  1607,  1608,  1609,  1610,
    1611,  1612,  1613,  1614,  1618,  1619,  1623,  1624,  1628,  1629,
    1633,  1634,  1635,  1636,  1637,  1638,  1639,  1640,  1641,  1642,
    1643,  1644,  1645,  1646,  1647,  1648,  1649,  1653,  1654,  1658,
    1659,  1663,  1664,  1665,  1666,  1670,  1671,  1672,  1673,  1677,
    1678,  1682,  1683,  1687,  1688,  1692,  1696,  1700,  1704,  1705,
    1709,  1710,  1714,  1715,  1716,  1717,  1718,  1719,  1720,  1721,
    1724,  1726,  1729,  1731,  1735,  1736,  1737,  1738,  1742,  1743,
    1744,  1745,  1749,  1750,  1751,  1752,  1756,  1760,  1764,  1765,
    1768,  1770,  1774,  1775,  1779,  1780,  1784,  1785,  1789,  1793,
    1794,  1798,  1799,  1800,  1804,  1805,  1809,  1810,  1814,  1815,
    1816,  1817,  1821,  1822,  1825,  1827,  1831,  1832
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "NULLTOKEN", "TRUETOKEN", "FALSETOKEN",
  "BREAK", "CASE", "DEFAULT", "FOR", "NEW", "VAR", "CONSTTOKEN",
  "CONTINUE", "FUNCTION", "RETURN", "VOIDTOKEN", "DELETETOKEN", "IF",
  "THISTOKEN", "DO", "WHILE", "INTOKEN", "INSTANCEOF", "TYPEOF", "SWITCH",
  "WITH", "RESERVED", "THROW", "TRY", "CATCH", "FINALLY", "DEBUGGER",
  "IF_WITHOUT_ELSE", "ELSE", "EQEQ", "NE", "STREQ", "STRNEQ", "LE", "GE",
  "OR", "AND", "PLUSPLUS", "MINUSMINUS", "LSHIFT", "RSHIFT", "URSHIFT",
  "PLUSEQUAL", "MINUSEQUAL", "MULTEQUAL", "DIVEQUAL", "LSHIFTEQUAL",
  "RSHIFTEQUAL", "URSHIFTEQUAL", "ANDEQUAL", "MODEQUAL", "XOREQUAL",
  "OREQUAL", "OPENBRACE", "CLOSEBRACE", "NUMBER", "IDENT", "STRING",
  "AUTOPLUSPLUS", "AUTOMINUSMINUS", "'/'", "':'", "'('", "')'", "','",
  "'['", "']'", "'.'", "'+'", "'-'", "'~'", "'!'", "'*'", "'%'", "'<'",
  "'>'", "'&'", "'^'", "'|'", "'?'", "'='", "';'", "$accept", "Literal",
  "Property", "PropertyList", "PrimaryExpr", "PrimaryExprNoBrace",
  "ArrayLiteral", "ElementList", "ElisionOpt", "Elision", "MemberExpr",
  "MemberExprNoBF", "NewExpr", "NewExprNoBF", "CallExpr", "CallExprNoBF",
  "Arguments", "ArgumentList", "LeftHandSideExpr", "LeftHandSideExprNoBF",
  "PostfixExpr", "PostfixExprNoBF", "UnaryExprCommon", "UnaryExpr",
  "UnaryExprNoBF", "MultiplicativeExpr", "MultiplicativeExprNoBF",
  "AdditiveExpr", "AdditiveExprNoBF", "ShiftExpr", "ShiftExprNoBF",
  "RelationalExpr", "RelationalExprNoIn", "RelationalExprNoBF",
  "EqualityExpr", "EqualityExprNoIn", "EqualityExprNoBF", "BitwiseANDExpr",
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
#endif

# ifdef YYPRINT
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
# endif

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


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

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

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256


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

#ifndef YY_LOCATION_PRINT
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL

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

#  define YY_LOCATION_PRINT(File, Loc)          \
  yy_location_print_ (File, &(Loc))

# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value, Location, globalPtr); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp, void *globalPtr)
{
  FILE *yyoutput = yyo;
  YYUSE (yyoutput);
  YYUSE (yylocationp);
  YYUSE (globalPtr);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyo, yytoknum[yytype], *yyvaluep);
# endif
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp, void *globalPtr)
{
  YYFPRINTF (yyo, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  YY_LOCATION_PRINT (yyo, *yylocationp);
  YYFPRINTF (yyo, ": ");
  yy_symbol_value_print (yyo, yytype, yyvaluep, yylocationp, globalPtr);
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
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp, YYLTYPE *yylsp, int yyrule, void *globalPtr)
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
                       yystos[+yyssp[yyi + 1 - yynrhs]],
                       &yyvsp[(yyi + 1) - (yynrhs)]
                       , &(yylsp[(yyi + 1) - (yynrhs)])                       , globalPtr);
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
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
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


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen(S) (YY_CAST (YYPTRDIFF_T, strlen (S)))
#  else
/* Return the length of YYSTR.  */
static YYPTRDIFF_T
yystrlen (const char *yystr)
{
  YYPTRDIFF_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYPTRDIFF_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYPTRDIFF_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            else
              goto append;

          append:
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (yyres)
    return yystpcpy (yyres, yystr) - yyres;
  else
    return yystrlen (yystr);
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYPTRDIFF_T *yymsg_alloc, char **yymsg,
                yy_state_t *yyssp, int yytoken)
{
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat: reported tokens (one for the "unexpected",
     one per "expected"). */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Actual size of YYARG. */
  int yycount = 0;
  /* Cumulated lengths of YYARG.  */
  YYPTRDIFF_T yysize = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[+*yyssp];
      YYPTRDIFF_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
      yysize = yysize0;
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYPTRDIFF_T yysize1
                    = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
                    yysize = yysize1;
                  else
                    return 2;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
    default: /* Avoid compiler warnings. */
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    /* Don't count the "%s"s in the final size, but reserve room for
       the terminator.  */
    YYPTRDIFF_T yysize1 = yysize + (yystrlen (yyformat) - 2 * yycount) + 1;
    if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
      yysize = yysize1;
    else
      return 2;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          ++yyp;
          ++yyformat;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep, YYLTYPE *yylocationp, void *globalPtr)
{
  YYUSE (yyvaluep);
  YYUSE (yylocationp);
  YYUSE (globalPtr);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void *globalPtr)
{
/* The lookahead symbol.  */
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
    int yynerrs;

    yy_state_fast_t yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.
       'yyls': related to locations.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss;
    yy_state_t *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    /* The location stack.  */
    YYLTYPE yylsa[YYINITDEPTH];
    YYLTYPE *yyls;
    YYLTYPE *yylsp;

    /* The locations where the error started and ended.  */
    YYLTYPE yyerror_range[3];

    YYPTRDIFF_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
  YYLTYPE yyloc;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYPTRDIFF_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N), yylsp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yylsp = yyls = yylsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
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
# undef YYSTACK_RELOCATE
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

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex (&yylval, &yylloc, globalPtr);
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
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
  case 2:
                                        { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(new (GLOBAL_DATA) NullNode(GLOBAL_DATA), 0, 1); }
    break;

  case 3:
                                        { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(new (GLOBAL_DATA) BooleanNode(GLOBAL_DATA, true), 0, 1); }
    break;

  case 4:
                                        { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(new (GLOBAL_DATA) BooleanNode(GLOBAL_DATA, false), 0, 1); }
    break;

  case 5:
                                        { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(makeNumberNode(GLOBAL_DATA, (yyvsp[0].doubleValue)), 0, 1); }
    break;

  case 6:
                                        { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(new (GLOBAL_DATA) StringNode(GLOBAL_DATA, *(yyvsp[0].ident)), 0, 1); }
    break;

  case 7:
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

  case 8:
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

  case 9:
                                        { (yyval.propertyNode) = createNodeInfo<PropertyNode*>(new (GLOBAL_DATA) PropertyNode(GLOBAL_DATA, *(yyvsp[-2].ident), (yyvsp[0].expressionNode).m_node, PropertyNode::Constant), (yyvsp[0].expressionNode).m_features, (yyvsp[0].expressionNode).m_numConstants); }
    break;

  case 10:
                                        { (yyval.propertyNode) = createNodeInfo<PropertyNode*>(new (GLOBAL_DATA) PropertyNode(GLOBAL_DATA, *(yyvsp[-2].ident), (yyvsp[0].expressionNode).m_node, PropertyNode::Constant), (yyvsp[0].expressionNode).m_features, (yyvsp[0].expressionNode).m_numConstants); }
    break;

  case 11:
                                        { (yyval.propertyNode) = createNodeInfo<PropertyNode*>(new (GLOBAL_DATA) PropertyNode(GLOBAL_DATA, (yyvsp[-2].doubleValue), (yyvsp[0].expressionNode).m_node, PropertyNode::Constant), (yyvsp[0].expressionNode).m_features, (yyvsp[0].expressionNode).m_numConstants); }
    break;

  case 12:
                                                             { (yyval.propertyNode) = createNodeInfo<PropertyNode*>(makeGetterOrSetterPropertyNode(GLOBAL_DATA, *(yyvsp[-6].ident), *(yyvsp[-5].ident), 0, (yyvsp[-1].functionBodyNode), GLOBAL_DATA->lexer->sourceCode((yyvsp[-2].intValue), (yyvsp[0].intValue), (yylsp[-2]).first_line)), ClosureFeature, 0); setStatementLocation((yyvsp[-1].functionBodyNode), (yylsp[-2]), (yylsp[0])); if (!(yyval.propertyNode).m_node) YYABORT; }
    break;

  case 13:
                                                             {
                                                                 (yyval.propertyNode) = createNodeInfo<PropertyNode*>(makeGetterOrSetterPropertyNode(GLOBAL_DATA, *(yyvsp[-7].ident), *(yyvsp[-6].ident), (yyvsp[-4].parameterList).m_node.head, (yyvsp[-1].functionBodyNode), GLOBAL_DATA->lexer->sourceCode((yyvsp[-2].intValue), (yyvsp[0].intValue), (yylsp[-2]).first_line)), (yyvsp[-4].parameterList).m_features | ClosureFeature, 0); 
                                                                 if ((yyvsp[-4].parameterList).m_features & ArgumentsFeature)
                                                                     (yyvsp[-1].functionBodyNode)->setUsesArguments(); 
                                                                 setStatementLocation((yyvsp[-1].functionBodyNode), (yylsp[-2]), (yylsp[0])); 
                                                                 if (!(yyval.propertyNode).m_node) 
                                                                     YYABORT; 
                                                             }
    break;

  case 14:
                                        { (yyval.propertyList).m_node.head = new (GLOBAL_DATA) PropertyListNode(GLOBAL_DATA, (yyvsp[0].propertyNode).m_node); 
                                          (yyval.propertyList).m_node.tail = (yyval.propertyList).m_node.head;
                                          (yyval.propertyList).m_features = (yyvsp[0].propertyNode).m_features;
                                          (yyval.propertyList).m_numConstants = (yyvsp[0].propertyNode).m_numConstants; }
    break;

  case 15:
                                        { (yyval.propertyList).m_node.head = (yyvsp[-2].propertyList).m_node.head;
                                          (yyval.propertyList).m_node.tail = new (GLOBAL_DATA) PropertyListNode(GLOBAL_DATA, (yyvsp[0].propertyNode).m_node, (yyvsp[-2].propertyList).m_node.tail);
                                          (yyval.propertyList).m_features = (yyvsp[-2].propertyList).m_features | (yyvsp[0].propertyNode).m_features;
                                          (yyval.propertyList).m_numConstants = (yyvsp[-2].propertyList).m_numConstants + (yyvsp[0].propertyNode).m_numConstants; }
    break;

  case 17:
                                                     { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(new (GLOBAL_DATA) ObjectLiteralNode(GLOBAL_DATA), 0, 0); }
    break;

  case 18:
                                                     { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(new (GLOBAL_DATA) ObjectLiteralNode(GLOBAL_DATA, (yyvsp[-1].propertyList).m_node.head), (yyvsp[-1].propertyList).m_features, (yyvsp[-1].propertyList).m_numConstants); }
    break;

  case 19:
                                                     { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(new (GLOBAL_DATA) ObjectLiteralNode(GLOBAL_DATA, (yyvsp[-2].propertyList).m_node.head), (yyvsp[-2].propertyList).m_features, (yyvsp[-2].propertyList).m_numConstants); }
    break;

  case 20:
                                        { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(new (GLOBAL_DATA) ThisNode(GLOBAL_DATA), ThisFeature, 0); }
    break;

  case 23:
                                        { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(new (GLOBAL_DATA) ResolveNode(GLOBAL_DATA, *(yyvsp[0].ident), (yylsp[0]).first_column), (*(yyvsp[0].ident) == GLOBAL_DATA->propertyNames->arguments) ? ArgumentsFeature : 0, 0); }
    break;

  case 24:
                                        { (yyval.expressionNode) = (yyvsp[-1].expressionNode); }
    break;

  case 25:
                                        { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(new (GLOBAL_DATA) ArrayNode(GLOBAL_DATA, (yyvsp[-1].intValue)), 0, (yyvsp[-1].intValue) ? 1 : 0); }
    break;

  case 26:
                                        { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(new (GLOBAL_DATA) ArrayNode(GLOBAL_DATA, (yyvsp[-1].elementList).m_node.head), (yyvsp[-1].elementList).m_features, (yyvsp[-1].elementList).m_numConstants); }
    break;

  case 27:
                                        { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(new (GLOBAL_DATA) ArrayNode(GLOBAL_DATA, (yyvsp[-1].intValue), (yyvsp[-3].elementList).m_node.head), (yyvsp[-3].elementList).m_features, (yyvsp[-1].intValue) ? (yyvsp[-3].elementList).m_numConstants + 1 : (yyvsp[-3].elementList).m_numConstants); }
    break;

  case 28:
                                        { (yyval.elementList).m_node.head = new (GLOBAL_DATA) ElementNode(GLOBAL_DATA, (yyvsp[-1].intValue), (yyvsp[0].expressionNode).m_node);
                                          (yyval.elementList).m_node.tail = (yyval.elementList).m_node.head;
                                          (yyval.elementList).m_features = (yyvsp[0].expressionNode).m_features;
                                          (yyval.elementList).m_numConstants = (yyvsp[0].expressionNode).m_numConstants; }
    break;

  case 29:
                                        { (yyval.elementList).m_node.head = (yyvsp[-3].elementList).m_node.head;
                                          (yyval.elementList).m_node.tail = new (GLOBAL_DATA) ElementNode(GLOBAL_DATA, (yyvsp[-3].elementList).m_node.tail, (yyvsp[-1].intValue), (yyvsp[0].expressionNode).m_node);
                                          (yyval.elementList).m_features = (yyvsp[-3].elementList).m_features | (yyvsp[0].expressionNode).m_features;
                                          (yyval.elementList).m_numConstants = (yyvsp[-3].elementList).m_numConstants + (yyvsp[0].expressionNode).m_numConstants; }
    break;

  case 30:
                                        { (yyval.intValue) = 0; }
    break;

  case 32:
                                        { (yyval.intValue) = 1; }
    break;

  case 33:
                                        { (yyval.intValue) = (yyvsp[-1].intValue) + 1; }
    break;

  case 35:
                                        { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>((yyvsp[0].funcExprNode).m_node, (yyvsp[0].funcExprNode).m_features, (yyvsp[0].funcExprNode).m_numConstants); }
    break;

  case 36:
                                        { BracketAccessorNode* node = new (GLOBAL_DATA) BracketAccessorNode(GLOBAL_DATA, (yyvsp[-3].expressionNode).m_node, (yyvsp[-1].expressionNode).m_node, (yyvsp[-1].expressionNode).m_features & AssignFeature);
                                          setExceptionLocation(node, (yylsp[-3]).first_column, (yylsp[-3]).last_column, (yylsp[0]).last_column);
                                          (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(node, (yyvsp[-3].expressionNode).m_features | (yyvsp[-1].expressionNode).m_features, (yyvsp[-3].expressionNode).m_numConstants + (yyvsp[-1].expressionNode).m_numConstants); 
                                        }
    break;

  case 37:
                                        { DotAccessorNode* node = new (GLOBAL_DATA) DotAccessorNode(GLOBAL_DATA, (yyvsp[-2].expressionNode).m_node, *(yyvsp[0].ident));
                                          setExceptionLocation(node, (yylsp[-2]).first_column, (yylsp[-2]).last_column, (yylsp[0]).last_column);
                                          (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(node, (yyvsp[-2].expressionNode).m_features, (yyvsp[-2].expressionNode).m_numConstants);
                                        }
    break;

  case 38:
                                        { NewExprNode* node = new (GLOBAL_DATA) NewExprNode(GLOBAL_DATA, (yyvsp[-1].expressionNode).m_node, (yyvsp[0].argumentsNode).m_node);
                                          setExceptionLocation(node, (yylsp[-2]).first_column, (yylsp[-1]).last_column, (yylsp[0]).last_column);
                                          (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(node, (yyvsp[-1].expressionNode).m_features | (yyvsp[0].argumentsNode).m_features, (yyvsp[-1].expressionNode).m_numConstants + (yyvsp[0].argumentsNode).m_numConstants);
                                        }
    break;

  case 40:
                                        { BracketAccessorNode* node = new (GLOBAL_DATA) BracketAccessorNode(GLOBAL_DATA, (yyvsp[-3].expressionNode).m_node, (yyvsp[-1].expressionNode).m_node, (yyvsp[-1].expressionNode).m_features & AssignFeature);
                                          setExceptionLocation(node, (yylsp[-3]).first_column, (yylsp[-3]).last_column, (yylsp[0]).last_column);
                                          (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(node, (yyvsp[-3].expressionNode).m_features | (yyvsp[-1].expressionNode).m_features, (yyvsp[-3].expressionNode).m_numConstants + (yyvsp[-1].expressionNode).m_numConstants); 
                                        }
    break;

  case 41:
                                        { DotAccessorNode* node = new (GLOBAL_DATA) DotAccessorNode(GLOBAL_DATA, (yyvsp[-2].expressionNode).m_node, *(yyvsp[0].ident));
                                          setExceptionLocation(node, (yylsp[-2]).first_column, (yylsp[-2]).last_column, (yylsp[0]).last_column);
                                          (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(node, (yyvsp[-2].expressionNode).m_features, (yyvsp[-2].expressionNode).m_numConstants);
                                        }
    break;

  case 42:
                                        { NewExprNode* node = new (GLOBAL_DATA) NewExprNode(GLOBAL_DATA, (yyvsp[-1].expressionNode).m_node, (yyvsp[0].argumentsNode).m_node);
                                          setExceptionLocation(node, (yylsp[-2]).first_column, (yylsp[-1]).last_column, (yylsp[0]).last_column);
                                          (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(node, (yyvsp[-1].expressionNode).m_features | (yyvsp[0].argumentsNode).m_features, (yyvsp[-1].expressionNode).m_numConstants + (yyvsp[0].argumentsNode).m_numConstants);
                                        }
    break;

  case 44:
                                        { NewExprNode* node = new (GLOBAL_DATA) NewExprNode(GLOBAL_DATA, (yyvsp[0].expressionNode).m_node);
                                          setExceptionLocation(node, (yylsp[-1]).first_column, (yylsp[0]).last_column, (yylsp[0]).last_column);
                                          (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(node, (yyvsp[0].expressionNode).m_features, (yyvsp[0].expressionNode).m_numConstants); 
                                        }
    break;

  case 46:
                                        { NewExprNode* node = new (GLOBAL_DATA) NewExprNode(GLOBAL_DATA, (yyvsp[0].expressionNode).m_node);
                                          setExceptionLocation(node, (yylsp[-1]).first_column, (yylsp[0]).last_column, (yylsp[0]).last_column);
                                          (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(node, (yyvsp[0].expressionNode).m_features, (yyvsp[0].expressionNode).m_numConstants);
                                        }
    break;

  case 47:
                                        { (yyval.expressionNode) = makeFunctionCallNode(GLOBAL_DATA, (yyvsp[-1].expressionNode), (yyvsp[0].argumentsNode), (yylsp[-1]).first_column, (yylsp[-1]).last_column, (yylsp[0]).last_column); }
    break;

  case 48:
                                        { (yyval.expressionNode) = makeFunctionCallNode(GLOBAL_DATA, (yyvsp[-1].expressionNode), (yyvsp[0].argumentsNode), (yylsp[-1]).first_column, (yylsp[-1]).last_column, (yylsp[0]).last_column); }
    break;

  case 49:
                                        { BracketAccessorNode* node = new (GLOBAL_DATA) BracketAccessorNode(GLOBAL_DATA, (yyvsp[-3].expressionNode).m_node, (yyvsp[-1].expressionNode).m_node, (yyvsp[-1].expressionNode).m_features & AssignFeature);
                                          setExceptionLocation(node, (yylsp[-3]).first_column, (yylsp[-3]).last_column, (yylsp[0]).last_column);
                                          (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(node, (yyvsp[-3].expressionNode).m_features | (yyvsp[-1].expressionNode).m_features, (yyvsp[-3].expressionNode).m_numConstants + (yyvsp[-1].expressionNode).m_numConstants); 
                                        }
    break;

  case 50:
                                        { DotAccessorNode* node = new (GLOBAL_DATA) DotAccessorNode(GLOBAL_DATA, (yyvsp[-2].expressionNode).m_node, *(yyvsp[0].ident));
                                          setExceptionLocation(node, (yylsp[-2]).first_column, (yylsp[-2]).last_column, (yylsp[0]).last_column);
                                          (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(node, (yyvsp[-2].expressionNode).m_features, (yyvsp[-2].expressionNode).m_numConstants); }
    break;

  case 51:
                                        { (yyval.expressionNode) = makeFunctionCallNode(GLOBAL_DATA, (yyvsp[-1].expressionNode), (yyvsp[0].argumentsNode), (yylsp[-1]).first_column, (yylsp[-1]).last_column, (yylsp[0]).last_column); }
    break;

  case 52:
                                        { (yyval.expressionNode) = makeFunctionCallNode(GLOBAL_DATA, (yyvsp[-1].expressionNode), (yyvsp[0].argumentsNode), (yylsp[-1]).first_column, (yylsp[-1]).last_column, (yylsp[0]).last_column); }
    break;

  case 53:
                                        { BracketAccessorNode* node = new (GLOBAL_DATA) BracketAccessorNode(GLOBAL_DATA, (yyvsp[-3].expressionNode).m_node, (yyvsp[-1].expressionNode).m_node, (yyvsp[-1].expressionNode).m_features & AssignFeature);
                                          setExceptionLocation(node, (yylsp[-3]).first_column, (yylsp[-3]).last_column, (yylsp[0]).last_column);
                                          (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(node, (yyvsp[-3].expressionNode).m_features | (yyvsp[-1].expressionNode).m_features, (yyvsp[-3].expressionNode).m_numConstants + (yyvsp[-1].expressionNode).m_numConstants); 
                                        }
    break;

  case 54:
                                        { DotAccessorNode* node = new (GLOBAL_DATA) DotAccessorNode(GLOBAL_DATA, (yyvsp[-2].expressionNode).m_node, *(yyvsp[0].ident));
                                          setExceptionLocation(node, (yylsp[-2]).first_column, (yylsp[-2]).last_column, (yylsp[0]).last_column);
                                          (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(node, (yyvsp[-2].expressionNode).m_features, (yyvsp[-2].expressionNode).m_numConstants); 
                                        }
    break;

  case 55:
                                        { (yyval.argumentsNode) = createNodeInfo<ArgumentsNode*>(new (GLOBAL_DATA) ArgumentsNode(GLOBAL_DATA), 0, 0); }
    break;

  case 56:
                                        { (yyval.argumentsNode) = createNodeInfo<ArgumentsNode*>(new (GLOBAL_DATA) ArgumentsNode(GLOBAL_DATA, (yyvsp[-1].argumentList).m_node.head), (yyvsp[-1].argumentList).m_features, (yyvsp[-1].argumentList).m_numConstants); }
    break;

  case 57:
                                        { (yyval.argumentList).m_node.head = new (GLOBAL_DATA) ArgumentListNode(GLOBAL_DATA, (yyvsp[0].expressionNode).m_node);
                                          (yyval.argumentList).m_node.tail = (yyval.argumentList).m_node.head;
                                          (yyval.argumentList).m_features = (yyvsp[0].expressionNode).m_features;
                                          (yyval.argumentList).m_numConstants = (yyvsp[0].expressionNode).m_numConstants; }
    break;

  case 58:
                                        { (yyval.argumentList).m_node.head = (yyvsp[-2].argumentList).m_node.head;
                                          (yyval.argumentList).m_node.tail = new (GLOBAL_DATA) ArgumentListNode(GLOBAL_DATA, (yyvsp[-2].argumentList).m_node.tail, (yyvsp[0].expressionNode).m_node);
                                          (yyval.argumentList).m_features = (yyvsp[-2].argumentList).m_features | (yyvsp[0].expressionNode).m_features;
                                          (yyval.argumentList).m_numConstants = (yyvsp[-2].argumentList).m_numConstants + (yyvsp[0].expressionNode).m_numConstants; }
    break;

  case 64:
                                        { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(makePostfixNode(GLOBAL_DATA, (yyvsp[-1].expressionNode).m_node, OpPlusPlus, (yylsp[-1]).first_column, (yylsp[-1]).last_column, (yylsp[0]).last_column), (yyvsp[-1].expressionNode).m_features | AssignFeature, (yyvsp[-1].expressionNode).m_numConstants); }
    break;

  case 65:
                                        { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(makePostfixNode(GLOBAL_DATA, (yyvsp[-1].expressionNode).m_node, OpMinusMinus, (yylsp[-1]).first_column, (yylsp[-1]).last_column, (yylsp[0]).last_column), (yyvsp[-1].expressionNode).m_features | AssignFeature, (yyvsp[-1].expressionNode).m_numConstants); }
    break;

  case 67:
                                        { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(makePostfixNode(GLOBAL_DATA, (yyvsp[-1].expressionNode).m_node, OpPlusPlus, (yylsp[-1]).first_column, (yylsp[-1]).last_column, (yylsp[0]).last_column), (yyvsp[-1].expressionNode).m_features | AssignFeature, (yyvsp[-1].expressionNode).m_numConstants); }
    break;

  case 68:
                                        { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(makePostfixNode(GLOBAL_DATA, (yyvsp[-1].expressionNode).m_node, OpMinusMinus, (yylsp[-1]).first_column, (yylsp[-1]).last_column, (yylsp[0]).last_column), (yyvsp[-1].expressionNode).m_features | AssignFeature, (yyvsp[-1].expressionNode).m_numConstants); }
    break;

  case 69:
                                        { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(makeDeleteNode(GLOBAL_DATA, (yyvsp[0].expressionNode).m_node, (yylsp[-1]).first_column, (yylsp[0]).last_column, (yylsp[0]).last_column), (yyvsp[0].expressionNode).m_features, (yyvsp[0].expressionNode).m_numConstants); }
    break;

  case 70:
                                        { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(new (GLOBAL_DATA) VoidNode(GLOBAL_DATA, (yyvsp[0].expressionNode).m_node), (yyvsp[0].expressionNode).m_features, (yyvsp[0].expressionNode).m_numConstants + 1); }
    break;

  case 71:
                                        { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(makeTypeOfNode(GLOBAL_DATA, (yyvsp[0].expressionNode).m_node), (yyvsp[0].expressionNode).m_features, (yyvsp[0].expressionNode).m_numConstants); }
    break;

  case 72:
                                        { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(makePrefixNode(GLOBAL_DATA, (yyvsp[0].expressionNode).m_node, OpPlusPlus, (yylsp[-1]).first_column, (yylsp[0]).first_column + 1, (yylsp[0]).last_column), (yyvsp[0].expressionNode).m_features | AssignFeature, (yyvsp[0].expressionNode).m_numConstants); }
    break;

  case 73:
                                        { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(makePrefixNode(GLOBAL_DATA, (yyvsp[0].expressionNode).m_node, OpPlusPlus, (yylsp[-1]).first_column, (yylsp[0]).first_column + 1, (yylsp[0]).last_column), (yyvsp[0].expressionNode).m_features | AssignFeature, (yyvsp[0].expressionNode).m_numConstants); }
    break;

  case 74:
                                        { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(makePrefixNode(GLOBAL_DATA, (yyvsp[0].expressionNode).m_node, OpMinusMinus, (yylsp[-1]).first_column, (yylsp[0]).first_column + 1, (yylsp[0]).last_column), (yyvsp[0].expressionNode).m_features | AssignFeature, (yyvsp[0].expressionNode).m_numConstants); }
    break;

  case 75:
                                        { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(makePrefixNode(GLOBAL_DATA, (yyvsp[0].expressionNode).m_node, OpMinusMinus, (yylsp[-1]).first_column, (yylsp[0]).first_column + 1, (yylsp[0]).last_column), (yyvsp[0].expressionNode).m_features | AssignFeature, (yyvsp[0].expressionNode).m_numConstants); }
    break;

  case 76:
                                        { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(new (GLOBAL_DATA) UnaryPlusNode(GLOBAL_DATA, (yyvsp[0].expressionNode).m_node), (yyvsp[0].expressionNode).m_features, (yyvsp[0].expressionNode).m_numConstants); }
    break;

  case 77:
                                        { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(makeNegateNode(GLOBAL_DATA, (yyvsp[0].expressionNode).m_node), (yyvsp[0].expressionNode).m_features, (yyvsp[0].expressionNode).m_numConstants); }
    break;

  case 78:
                                        { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(makeBitwiseNotNode(GLOBAL_DATA, (yyvsp[0].expressionNode).m_node), (yyvsp[0].expressionNode).m_features, (yyvsp[0].expressionNode).m_numConstants); }
    break;

  case 79:
                                        { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(new (GLOBAL_DATA) LogicalNotNode(GLOBAL_DATA, (yyvsp[0].expressionNode).m_node), (yyvsp[0].expressionNode).m_features, (yyvsp[0].expressionNode).m_numConstants); }
    break;

  case 85:
                                        { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(makeMultNode(GLOBAL_DATA, (yyvsp[-2].expressionNode).m_node, (yyvsp[0].expressionNode).m_node, (yyvsp[0].expressionNode).m_features & AssignFeature), (yyvsp[-2].expressionNode).m_features | (yyvsp[0].expressionNode).m_features, (yyvsp[-2].expressionNode).m_numConstants + (yyvsp[0].expressionNode).m_numConstants); }
    break;

  case 86:
                                        { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(makeDivNode(GLOBAL_DATA, (yyvsp[-2].expressionNode).m_node, (yyvsp[0].expressionNode).m_node, (yyvsp[0].expressionNode).m_features & AssignFeature), (yyvsp[-2].expressionNode).m_features | (yyvsp[0].expressionNode).m_features, (yyvsp[-2].expressionNode).m_numConstants + (yyvsp[0].expressionNode).m_numConstants); }
    break;

  case 87:
                                        { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(new (GLOBAL_DATA) ModNode(GLOBAL_DATA, (yyvsp[-2].expressionNode).m_node, (yyvsp[0].expressionNode).m_node, (yyvsp[0].expressionNode).m_features & AssignFeature), (yyvsp[-2].expressionNode).m_features | (yyvsp[0].expressionNode).m_features, (yyvsp[-2].expressionNode).m_numConstants + (yyvsp[0].expressionNode).m_numConstants); }
    break;

  case 89:
                                        { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(makeMultNode(GLOBAL_DATA, (yyvsp[-2].expressionNode).m_node, (yyvsp[0].expressionNode).m_node, (yyvsp[0].expressionNode).m_features & AssignFeature), (yyvsp[-2].expressionNode).m_features | (yyvsp[0].expressionNode).m_features, (yyvsp[-2].expressionNode).m_numConstants + (yyvsp[0].expressionNode).m_numConstants); }
    break;

  case 90:
                                        { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(makeDivNode(GLOBAL_DATA, (yyvsp[-2].expressionNode).m_node, (yyvsp[0].expressionNode).m_node, (yyvsp[0].expressionNode).m_features & AssignFeature), (yyvsp[-2].expressionNode).m_features | (yyvsp[0].expressionNode).m_features, (yyvsp[-2].expressionNode).m_numConstants + (yyvsp[0].expressionNode).m_numConstants); }
    break;

  case 91:
                                        { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(new (GLOBAL_DATA) ModNode(GLOBAL_DATA, (yyvsp[-2].expressionNode).m_node, (yyvsp[0].expressionNode).m_node, (yyvsp[0].expressionNode).m_features & AssignFeature), (yyvsp[-2].expressionNode).m_features | (yyvsp[0].expressionNode).m_features, (yyvsp[-2].expressionNode).m_numConstants + (yyvsp[0].expressionNode).m_numConstants); }
    break;

  case 93:
                                        { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(makeAddNode(GLOBAL_DATA, (yyvsp[-2].expressionNode).m_node, (yyvsp[0].expressionNode).m_node, (yyvsp[0].expressionNode).m_features & AssignFeature), (yyvsp[-2].expressionNode).m_features | (yyvsp[0].expressionNode).m_features, (yyvsp[-2].expressionNode).m_numConstants + (yyvsp[0].expressionNode).m_numConstants); }
    break;

  case 94:
                                        { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(makeSubNode(GLOBAL_DATA, (yyvsp[-2].expressionNode).m_node, (yyvsp[0].expressionNode).m_node, (yyvsp[0].expressionNode).m_features & AssignFeature), (yyvsp[-2].expressionNode).m_features | (yyvsp[0].expressionNode).m_features, (yyvsp[-2].expressionNode).m_numConstants + (yyvsp[0].expressionNode).m_numConstants); }
    break;

  case 96:
                                        { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(makeAddNode(GLOBAL_DATA, (yyvsp[-2].expressionNode).m_node, (yyvsp[0].expressionNode).m_node, (yyvsp[0].expressionNode).m_features & AssignFeature), (yyvsp[-2].expressionNode).m_features | (yyvsp[0].expressionNode).m_features, (yyvsp[-2].expressionNode).m_numConstants + (yyvsp[0].expressionNode).m_numConstants); }
    break;

  case 97:
                                        { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(makeSubNode(GLOBAL_DATA, (yyvsp[-2].expressionNode).m_node, (yyvsp[0].expressionNode).m_node, (yyvsp[0].expressionNode).m_features & AssignFeature), (yyvsp[-2].expressionNode).m_features | (yyvsp[0].expressionNode).m_features, (yyvsp[-2].expressionNode).m_numConstants + (yyvsp[0].expressionNode).m_numConstants); }
    break;

  case 99:
                                        { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(makeLeftShiftNode(GLOBAL_DATA, (yyvsp[-2].expressionNode).m_node, (yyvsp[0].expressionNode).m_node, (yyvsp[0].expressionNode).m_features & AssignFeature), (yyvsp[-2].expressionNode).m_features | (yyvsp[0].expressionNode).m_features, (yyvsp[-2].expressionNode).m_numConstants + (yyvsp[0].expressionNode).m_numConstants); }
    break;

  case 100:
                                        { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(makeRightShiftNode(GLOBAL_DATA, (yyvsp[-2].expressionNode).m_node, (yyvsp[0].expressionNode).m_node, (yyvsp[0].expressionNode).m_features & AssignFeature), (yyvsp[-2].expressionNode).m_features | (yyvsp[0].expressionNode).m_features, (yyvsp[-2].expressionNode).m_numConstants + (yyvsp[0].expressionNode).m_numConstants); }
    break;

  case 101:
                                        { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(new (GLOBAL_DATA) UnsignedRightShiftNode(GLOBAL_DATA, (yyvsp[-2].expressionNode).m_node, (yyvsp[0].expressionNode).m_node, (yyvsp[0].expressionNode).m_features & AssignFeature), (yyvsp[-2].expressionNode).m_features | (yyvsp[0].expressionNode).m_features, (yyvsp[-2].expressionNode).m_numConstants + (yyvsp[0].expressionNode).m_numConstants); }
    break;

  case 103:
                                        { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(makeLeftShiftNode(GLOBAL_DATA, (yyvsp[-2].expressionNode).m_node, (yyvsp[0].expressionNode).m_node, (yyvsp[0].expressionNode).m_features & AssignFeature), (yyvsp[-2].expressionNode).m_features | (yyvsp[0].expressionNode).m_features, (yyvsp[-2].expressionNode).m_numConstants + (yyvsp[0].expressionNode).m_numConstants); }
    break;

  case 104:
                                        { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(makeRightShiftNode(GLOBAL_DATA, (yyvsp[-2].expressionNode).m_node, (yyvsp[0].expressionNode).m_node, (yyvsp[0].expressionNode).m_features & AssignFeature), (yyvsp[-2].expressionNode).m_features | (yyvsp[0].expressionNode).m_features, (yyvsp[-2].expressionNode).m_numConstants + (yyvsp[0].expressionNode).m_numConstants); }
    break;

  case 105:
                                        { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(new (GLOBAL_DATA) UnsignedRightShiftNode(GLOBAL_DATA, (yyvsp[-2].expressionNode).m_node, (yyvsp[0].expressionNode).m_node, (yyvsp[0].expressionNode).m_features & AssignFeature), (yyvsp[-2].expressionNode).m_features | (yyvsp[0].expressionNode).m_features, (yyvsp[-2].expressionNode).m_numConstants + (yyvsp[0].expressionNode).m_numConstants); }
    break;

  case 107:
                                        { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(new (GLOBAL_DATA) LessNode(GLOBAL_DATA, (yyvsp[-2].expressionNode).m_node, (yyvsp[0].expressionNode).m_node, (yyvsp[0].expressionNode).m_features & AssignFeature), (yyvsp[-2].expressionNode).m_features | (yyvsp[0].expressionNode).m_features, (yyvsp[-2].expressionNode).m_numConstants + (yyvsp[0].expressionNode).m_numConstants); }
    break;

  case 108:
                                        { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(new (GLOBAL_DATA) GreaterNode(GLOBAL_DATA, (yyvsp[-2].expressionNode).m_node, (yyvsp[0].expressionNode).m_node, (yyvsp[0].expressionNode).m_features & AssignFeature), (yyvsp[-2].expressionNode).m_features | (yyvsp[0].expressionNode).m_features, (yyvsp[-2].expressionNode).m_numConstants + (yyvsp[0].expressionNode).m_numConstants); }
    break;

  case 109:
                                        { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(new (GLOBAL_DATA) LessEqNode(GLOBAL_DATA, (yyvsp[-2].expressionNode).m_node, (yyvsp[0].expressionNode).m_node, (yyvsp[0].expressionNode).m_features & AssignFeature), (yyvsp[-2].expressionNode).m_features | (yyvsp[0].expressionNode).m_features, (yyvsp[-2].expressionNode).m_numConstants + (yyvsp[0].expressionNode).m_numConstants); }
    break;

  case 110:
                                        { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(new (GLOBAL_DATA) GreaterEqNode(GLOBAL_DATA, (yyvsp[-2].expressionNode).m_node, (yyvsp[0].expressionNode).m_node, (yyvsp[0].expressionNode).m_features & AssignFeature), (yyvsp[-2].expressionNode).m_features | (yyvsp[0].expressionNode).m_features, (yyvsp[-2].expressionNode).m_numConstants + (yyvsp[0].expressionNode).m_numConstants); }
    break;

  case 111:
                                        { InstanceOfNode* node = new (GLOBAL_DATA) InstanceOfNode(GLOBAL_DATA, (yyvsp[-2].expressionNode).m_node, (yyvsp[0].expressionNode).m_node, (yyvsp[0].expressionNode).m_features & AssignFeature);
                                          setExceptionLocation(node, (yylsp[-2]).first_column, (yylsp[0]).first_column, (yylsp[0]).last_column);  
                                          (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(node, (yyvsp[-2].expressionNode).m_features | (yyvsp[0].expressionNode).m_features, (yyvsp[-2].expressionNode).m_numConstants + (yyvsp[0].expressionNode).m_numConstants); }
    break;

  case 112:
                                        { InNode* node = new (GLOBAL_DATA) InNode(GLOBAL_DATA, (yyvsp[-2].expressionNode).m_node, (yyvsp[0].expressionNode).m_node, (yyvsp[0].expressionNode).m_features & AssignFeature);
                                          setExceptionLocation(node, (yylsp[-2]).first_column, (yylsp[0]).first_column, (yylsp[0]).last_column);  
                                          (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(node, (yyvsp[-2].expressionNode).m_features | (yyvsp[0].expressionNode).m_features, (yyvsp[-2].expressionNode).m_numConstants + (yyvsp[0].expressionNode).m_numConstants); }
    break;

  case 114:
                                        { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(new (GLOBAL_DATA) LessNode(GLOBAL_DATA, (yyvsp[-2].expressionNode).m_node, (yyvsp[0].expressionNode).m_node, (yyvsp[0].expressionNode).m_features & AssignFeature), (yyvsp[-2].expressionNode).m_features | (yyvsp[0].expressionNode).m_features, (yyvsp[-2].expressionNode).m_numConstants + (yyvsp[0].expressionNode).m_numConstants); }
    break;

  case 115:
                                        { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(new (GLOBAL_DATA) GreaterNode(GLOBAL_DATA, (yyvsp[-2].expressionNode).m_node, (yyvsp[0].expressionNode).m_node, (yyvsp[0].expressionNode).m_features & AssignFeature), (yyvsp[-2].expressionNode).m_features | (yyvsp[0].expressionNode).m_features, (yyvsp[-2].expressionNode).m_numConstants + (yyvsp[0].expressionNode).m_numConstants); }
    break;

  case 116:
                                        { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(new (GLOBAL_DATA) LessEqNode(GLOBAL_DATA, (yyvsp[-2].expressionNode).m_node, (yyvsp[0].expressionNode).m_node, (yyvsp[0].expressionNode).m_features & AssignFeature), (yyvsp[-2].expressionNode).m_features | (yyvsp[0].expressionNode).m_features, (yyvsp[-2].expressionNode).m_numConstants + (yyvsp[0].expressionNode).m_numConstants); }
    break;

  case 117:
                                        { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(new (GLOBAL_DATA) GreaterEqNode(GLOBAL_DATA, (yyvsp[-2].expressionNode).m_node, (yyvsp[0].expressionNode).m_node, (yyvsp[0].expressionNode).m_features & AssignFeature), (yyvsp[-2].expressionNode).m_features | (yyvsp[0].expressionNode).m_features, (yyvsp[-2].expressionNode).m_numConstants + (yyvsp[0].expressionNode).m_numConstants); }
    break;

  case 118:
                                        { InstanceOfNode* node = new (GLOBAL_DATA) InstanceOfNode(GLOBAL_DATA, (yyvsp[-2].expressionNode).m_node, (yyvsp[0].expressionNode).m_node, (yyvsp[0].expressionNode).m_features & AssignFeature);
                                          setExceptionLocation(node, (yylsp[-2]).first_column, (yylsp[0]).first_column, (yylsp[0]).last_column);  
                                          (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(node, (yyvsp[-2].expressionNode).m_features | (yyvsp[0].expressionNode).m_features, (yyvsp[-2].expressionNode).m_numConstants + (yyvsp[0].expressionNode).m_numConstants); }
    break;

  case 120:
                                        { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(new (GLOBAL_DATA) LessNode(GLOBAL_DATA, (yyvsp[-2].expressionNode).m_node, (yyvsp[0].expressionNode).m_node, (yyvsp[0].expressionNode).m_features & AssignFeature), (yyvsp[-2].expressionNode).m_features | (yyvsp[0].expressionNode).m_features, (yyvsp[-2].expressionNode).m_numConstants + (yyvsp[0].expressionNode).m_numConstants); }
    break;

  case 121:
                                        { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(new (GLOBAL_DATA) GreaterNode(GLOBAL_DATA, (yyvsp[-2].expressionNode).m_node, (yyvsp[0].expressionNode).m_node, (yyvsp[0].expressionNode).m_features & AssignFeature), (yyvsp[-2].expressionNode).m_features | (yyvsp[0].expressionNode).m_features, (yyvsp[-2].expressionNode).m_numConstants + (yyvsp[0].expressionNode).m_numConstants); }
    break;

  case 122:
                                        { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(new (GLOBAL_DATA) LessEqNode(GLOBAL_DATA, (yyvsp[-2].expressionNode).m_node, (yyvsp[0].expressionNode).m_node, (yyvsp[0].expressionNode).m_features & AssignFeature), (yyvsp[-2].expressionNode).m_features | (yyvsp[0].expressionNode).m_features, (yyvsp[-2].expressionNode).m_numConstants + (yyvsp[0].expressionNode).m_numConstants); }
    break;

  case 123:
                                        { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(new (GLOBAL_DATA) GreaterEqNode(GLOBAL_DATA, (yyvsp[-2].expressionNode).m_node, (yyvsp[0].expressionNode).m_node, (yyvsp[0].expressionNode).m_features & AssignFeature), (yyvsp[-2].expressionNode).m_features | (yyvsp[0].expressionNode).m_features, (yyvsp[-2].expressionNode).m_numConstants + (yyvsp[0].expressionNode).m_numConstants); }
    break;

  case 124:
                                        { InstanceOfNode* node = new (GLOBAL_DATA) InstanceOfNode(GLOBAL_DATA, (yyvsp[-2].expressionNode).m_node, (yyvsp[0].expressionNode).m_node, (yyvsp[0].expressionNode).m_features & AssignFeature);
                                          setExceptionLocation(node, (yylsp[-2]).first_column, (yylsp[0]).first_column, (yylsp[0]).last_column);  
                                          (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(node, (yyvsp[-2].expressionNode).m_features | (yyvsp[0].expressionNode).m_features, (yyvsp[-2].expressionNode).m_numConstants + (yyvsp[0].expressionNode).m_numConstants); }
    break;

  case 125:
                                        { InNode* node = new (GLOBAL_DATA) InNode(GLOBAL_DATA, (yyvsp[-2].expressionNode).m_node, (yyvsp[0].expressionNode).m_node, (yyvsp[0].expressionNode).m_features & AssignFeature);
                                          setExceptionLocation(node, (yylsp[-2]).first_column, (yylsp[0]).first_column, (yylsp[0]).last_column);  
                                          (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(node, (yyvsp[-2].expressionNode).m_features | (yyvsp[0].expressionNode).m_features, (yyvsp[-2].expressionNode).m_numConstants + (yyvsp[0].expressionNode).m_numConstants); }
    break;

  case 127:
                                        { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(new (GLOBAL_DATA) EqualNode(GLOBAL_DATA, (yyvsp[-2].expressionNode).m_node, (yyvsp[0].expressionNode).m_node, (yyvsp[0].expressionNode).m_features & AssignFeature), (yyvsp[-2].expressionNode).m_features | (yyvsp[0].expressionNode).m_features, (yyvsp[-2].expressionNode).m_numConstants + (yyvsp[0].expressionNode).m_numConstants); }
    break;

  case 128:
                                        { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(new (GLOBAL_DATA) NotEqualNode(GLOBAL_DATA, (yyvsp[-2].expressionNode).m_node, (yyvsp[0].expressionNode).m_node, (yyvsp[0].expressionNode).m_features & AssignFeature), (yyvsp[-2].expressionNode).m_features | (yyvsp[0].expressionNode).m_features, (yyvsp[-2].expressionNode).m_numConstants + (yyvsp[0].expressionNode).m_numConstants); }
    break;

  case 129:
                                        { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(new (GLOBAL_DATA) StrictEqualNode(GLOBAL_DATA, (yyvsp[-2].expressionNode).m_node, (yyvsp[0].expressionNode).m_node, (yyvsp[0].expressionNode).m_features & AssignFeature), (yyvsp[-2].expressionNode).m_features | (yyvsp[0].expressionNode).m_features, (yyvsp[-2].expressionNode).m_numConstants + (yyvsp[0].expressionNode).m_numConstants); }
    break;

  case 130:
                                        { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(new (GLOBAL_DATA) NotStrictEqualNode(GLOBAL_DATA, (yyvsp[-2].expressionNode).m_node, (yyvsp[0].expressionNode).m_node, (yyvsp[0].expressionNode).m_features & AssignFeature), (yyvsp[-2].expressionNode).m_features | (yyvsp[0].expressionNode).m_features, (yyvsp[-2].expressionNode).m_numConstants + (yyvsp[0].expressionNode).m_numConstants); }
    break;

  case 132:
                                        { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(new (GLOBAL_DATA) EqualNode(GLOBAL_DATA, (yyvsp[-2].expressionNode).m_node, (yyvsp[0].expressionNode).m_node, (yyvsp[0].expressionNode).m_features & AssignFeature), (yyvsp[-2].expressionNode).m_features | (yyvsp[0].expressionNode).m_features, (yyvsp[-2].expressionNode).m_numConstants + (yyvsp[0].expressionNode).m_numConstants); }
    break;

  case 133:
                                        { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(new (GLOBAL_DATA) NotEqualNode(GLOBAL_DATA, (yyvsp[-2].expressionNode).m_node, (yyvsp[0].expressionNode).m_node, (yyvsp[0].expressionNode).m_features & AssignFeature), (yyvsp[-2].expressionNode).m_features | (yyvsp[0].expressionNode).m_features, (yyvsp[-2].expressionNode).m_numConstants + (yyvsp[0].expressionNode).m_numConstants); }
    break;

  case 134:
                                        { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(new (GLOBAL_DATA) StrictEqualNode(GLOBAL_DATA, (yyvsp[-2].expressionNode).m_node, (yyvsp[0].expressionNode).m_node, (yyvsp[0].expressionNode).m_features & AssignFeature), (yyvsp[-2].expressionNode).m_features | (yyvsp[0].expressionNode).m_features, (yyvsp[-2].expressionNode).m_numConstants + (yyvsp[0].expressionNode).m_numConstants); }
    break;

  case 135:
                                        { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(new (GLOBAL_DATA) NotStrictEqualNode(GLOBAL_DATA, (yyvsp[-2].expressionNode).m_node, (yyvsp[0].expressionNode).m_node, (yyvsp[0].expressionNode).m_features & AssignFeature), (yyvsp[-2].expressionNode).m_features | (yyvsp[0].expressionNode).m_features, (yyvsp[-2].expressionNode).m_numConstants + (yyvsp[0].expressionNode).m_numConstants); }
    break;

  case 137:
                                        { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(new (GLOBAL_DATA) EqualNode(GLOBAL_DATA, (yyvsp[-2].expressionNode).m_node, (yyvsp[0].expressionNode).m_node, (yyvsp[0].expressionNode).m_features & AssignFeature), (yyvsp[-2].expressionNode).m_features | (yyvsp[0].expressionNode).m_features, (yyvsp[-2].expressionNode).m_numConstants + (yyvsp[0].expressionNode).m_numConstants); }
    break;

  case 138:
                                        { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(new (GLOBAL_DATA) NotEqualNode(GLOBAL_DATA, (yyvsp[-2].expressionNode).m_node, (yyvsp[0].expressionNode).m_node, (yyvsp[0].expressionNode).m_features & AssignFeature), (yyvsp[-2].expressionNode).m_features | (yyvsp[0].expressionNode).m_features, (yyvsp[-2].expressionNode).m_numConstants + (yyvsp[0].expressionNode).m_numConstants); }
    break;

  case 139:
                                        { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(new (GLOBAL_DATA) StrictEqualNode(GLOBAL_DATA, (yyvsp[-2].expressionNode).m_node, (yyvsp[0].expressionNode).m_node, (yyvsp[0].expressionNode).m_features & AssignFeature), (yyvsp[-2].expressionNode).m_features | (yyvsp[0].expressionNode).m_features, (yyvsp[-2].expressionNode).m_numConstants + (yyvsp[0].expressionNode).m_numConstants); }
    break;

  case 140:
                                        { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(new (GLOBAL_DATA) NotStrictEqualNode(GLOBAL_DATA, (yyvsp[-2].expressionNode).m_node, (yyvsp[0].expressionNode).m_node, (yyvsp[0].expressionNode).m_features & AssignFeature), (yyvsp[-2].expressionNode).m_features | (yyvsp[0].expressionNode).m_features, (yyvsp[-2].expressionNode).m_numConstants + (yyvsp[0].expressionNode).m_numConstants); }
    break;

  case 142:
                                        { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(new (GLOBAL_DATA) BitAndNode(GLOBAL_DATA, (yyvsp[-2].expressionNode).m_node, (yyvsp[0].expressionNode).m_node, (yyvsp[0].expressionNode).m_features & AssignFeature), (yyvsp[-2].expressionNode).m_features | (yyvsp[0].expressionNode).m_features, (yyvsp[-2].expressionNode).m_numConstants + (yyvsp[0].expressionNode).m_numConstants); }
    break;

  case 144:
                                        { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(new (GLOBAL_DATA) BitAndNode(GLOBAL_DATA, (yyvsp[-2].expressionNode).m_node, (yyvsp[0].expressionNode).m_node, (yyvsp[0].expressionNode).m_features & AssignFeature), (yyvsp[-2].expressionNode).m_features | (yyvsp[0].expressionNode).m_features, (yyvsp[-2].expressionNode).m_numConstants + (yyvsp[0].expressionNode).m_numConstants); }
    break;

  case 146:
                                        { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(new (GLOBAL_DATA) BitAndNode(GLOBAL_DATA, (yyvsp[-2].expressionNode).m_node, (yyvsp[0].expressionNode).m_node, (yyvsp[0].expressionNode).m_features & AssignFeature), (yyvsp[-2].expressionNode).m_features | (yyvsp[0].expressionNode).m_features, (yyvsp[-2].expressionNode).m_numConstants + (yyvsp[0].expressionNode).m_numConstants); }
    break;

  case 148:
                                        { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(new (GLOBAL_DATA) BitXOrNode(GLOBAL_DATA, (yyvsp[-2].expressionNode).m_node, (yyvsp[0].expressionNode).m_node, (yyvsp[0].expressionNode).m_features & AssignFeature), (yyvsp[-2].expressionNode).m_features | (yyvsp[0].expressionNode).m_features, (yyvsp[-2].expressionNode).m_numConstants + (yyvsp[0].expressionNode).m_numConstants); }
    break;

  case 150:
                                        { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(new (GLOBAL_DATA) BitXOrNode(GLOBAL_DATA, (yyvsp[-2].expressionNode).m_node, (yyvsp[0].expressionNode).m_node, (yyvsp[0].expressionNode).m_features & AssignFeature), (yyvsp[-2].expressionNode).m_features | (yyvsp[0].expressionNode).m_features, (yyvsp[-2].expressionNode).m_numConstants + (yyvsp[0].expressionNode).m_numConstants); }
    break;

  case 152:
                                        { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(new (GLOBAL_DATA) BitXOrNode(GLOBAL_DATA, (yyvsp[-2].expressionNode).m_node, (yyvsp[0].expressionNode).m_node, (yyvsp[0].expressionNode).m_features & AssignFeature), (yyvsp[-2].expressionNode).m_features | (yyvsp[0].expressionNode).m_features, (yyvsp[-2].expressionNode).m_numConstants + (yyvsp[0].expressionNode).m_numConstants); }
    break;

  case 154:
                                        { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(new (GLOBAL_DATA) BitOrNode(GLOBAL_DATA, (yyvsp[-2].expressionNode).m_node, (yyvsp[0].expressionNode).m_node, (yyvsp[0].expressionNode).m_features & AssignFeature), (yyvsp[-2].expressionNode).m_features | (yyvsp[0].expressionNode).m_features, (yyvsp[-2].expressionNode).m_numConstants + (yyvsp[0].expressionNode).m_numConstants); }
    break;

  case 156:
                                        { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(new (GLOBAL_DATA) BitOrNode(GLOBAL_DATA, (yyvsp[-2].expressionNode).m_node, (yyvsp[0].expressionNode).m_node, (yyvsp[0].expressionNode).m_features & AssignFeature), (yyvsp[-2].expressionNode).m_features | (yyvsp[0].expressionNode).m_features, (yyvsp[-2].expressionNode).m_numConstants + (yyvsp[0].expressionNode).m_numConstants); }
    break;

  case 158:
                                        { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(new (GLOBAL_DATA) BitOrNode(GLOBAL_DATA, (yyvsp[-2].expressionNode).m_node, (yyvsp[0].expressionNode).m_node, (yyvsp[0].expressionNode).m_features & AssignFeature), (yyvsp[-2].expressionNode).m_features | (yyvsp[0].expressionNode).m_features, (yyvsp[-2].expressionNode).m_numConstants + (yyvsp[0].expressionNode).m_numConstants); }
    break;

  case 160:
                                        { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(new (GLOBAL_DATA) LogicalOpNode(GLOBAL_DATA, (yyvsp[-2].expressionNode).m_node, (yyvsp[0].expressionNode).m_node, OpLogicalAnd), (yyvsp[-2].expressionNode).m_features | (yyvsp[0].expressionNode).m_features, (yyvsp[-2].expressionNode).m_numConstants + (yyvsp[0].expressionNode).m_numConstants); }
    break;

  case 162:
                                        { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(new (GLOBAL_DATA) LogicalOpNode(GLOBAL_DATA, (yyvsp[-2].expressionNode).m_node, (yyvsp[0].expressionNode).m_node, OpLogicalAnd), (yyvsp[-2].expressionNode).m_features | (yyvsp[0].expressionNode).m_features, (yyvsp[-2].expressionNode).m_numConstants + (yyvsp[0].expressionNode).m_numConstants); }
    break;

  case 164:
                                        { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(new (GLOBAL_DATA) LogicalOpNode(GLOBAL_DATA, (yyvsp[-2].expressionNode).m_node, (yyvsp[0].expressionNode).m_node, OpLogicalAnd), (yyvsp[-2].expressionNode).m_features | (yyvsp[0].expressionNode).m_features, (yyvsp[-2].expressionNode).m_numConstants + (yyvsp[0].expressionNode).m_numConstants); }
    break;

  case 166:
                                        { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(new (GLOBAL_DATA) LogicalOpNode(GLOBAL_DATA, (yyvsp[-2].expressionNode).m_node, (yyvsp[0].expressionNode).m_node, OpLogicalOr), (yyvsp[-2].expressionNode).m_features | (yyvsp[0].expressionNode).m_features, (yyvsp[-2].expressionNode).m_numConstants + (yyvsp[0].expressionNode).m_numConstants); }
    break;

  case 168:
                                        { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(new (GLOBAL_DATA) LogicalOpNode(GLOBAL_DATA, (yyvsp[-2].expressionNode).m_node, (yyvsp[0].expressionNode).m_node, OpLogicalOr), (yyvsp[-2].expressionNode).m_features | (yyvsp[0].expressionNode).m_features, (yyvsp[-2].expressionNode).m_numConstants + (yyvsp[0].expressionNode).m_numConstants); }
    break;

  case 170:
                                        { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(new (GLOBAL_DATA) LogicalOpNode(GLOBAL_DATA, (yyvsp[-2].expressionNode).m_node, (yyvsp[0].expressionNode).m_node, OpLogicalOr), (yyvsp[-2].expressionNode).m_features | (yyvsp[0].expressionNode).m_features, (yyvsp[-2].expressionNode).m_numConstants + (yyvsp[0].expressionNode).m_numConstants); }
    break;

  case 172:
                                        { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(new (GLOBAL_DATA) ConditionalNode(GLOBAL_DATA, (yyvsp[-4].expressionNode).m_node, (yyvsp[-2].expressionNode).m_node, (yyvsp[0].expressionNode).m_node), (yyvsp[-4].expressionNode).m_features | (yyvsp[-2].expressionNode).m_features | (yyvsp[0].expressionNode).m_features, (yyvsp[-4].expressionNode).m_numConstants + (yyvsp[-2].expressionNode).m_numConstants + (yyvsp[0].expressionNode).m_numConstants); }
    break;

  case 174:
                                        { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(new (GLOBAL_DATA) ConditionalNode(GLOBAL_DATA, (yyvsp[-4].expressionNode).m_node, (yyvsp[-2].expressionNode).m_node, (yyvsp[0].expressionNode).m_node), (yyvsp[-4].expressionNode).m_features | (yyvsp[-2].expressionNode).m_features | (yyvsp[0].expressionNode).m_features, (yyvsp[-4].expressionNode).m_numConstants + (yyvsp[-2].expressionNode).m_numConstants + (yyvsp[0].expressionNode).m_numConstants); }
    break;

  case 176:
                                        { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(new (GLOBAL_DATA) ConditionalNode(GLOBAL_DATA, (yyvsp[-4].expressionNode).m_node, (yyvsp[-2].expressionNode).m_node, (yyvsp[0].expressionNode).m_node), (yyvsp[-4].expressionNode).m_features | (yyvsp[-2].expressionNode).m_features | (yyvsp[0].expressionNode).m_features, (yyvsp[-4].expressionNode).m_numConstants + (yyvsp[-2].expressionNode).m_numConstants + (yyvsp[0].expressionNode).m_numConstants); }
    break;

  case 178:
                                        { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(makeAssignNode(GLOBAL_DATA, (yyvsp[-2].expressionNode).m_node, (yyvsp[-1].op), (yyvsp[0].expressionNode).m_node, (yyvsp[-2].expressionNode).m_features & AssignFeature, (yyvsp[0].expressionNode).m_features & AssignFeature, 
                                                                                                     (yylsp[-2]).first_column, (yylsp[-1]).first_column + 1, (yylsp[0]).last_column), (yyvsp[-2].expressionNode).m_features | (yyvsp[0].expressionNode).m_features | AssignFeature, (yyvsp[-2].expressionNode).m_numConstants + (yyvsp[0].expressionNode).m_numConstants); 
                                        }
    break;

  case 180:
                                        { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(makeAssignNode(GLOBAL_DATA, (yyvsp[-2].expressionNode).m_node, (yyvsp[-1].op), (yyvsp[0].expressionNode).m_node, (yyvsp[-2].expressionNode).m_features & AssignFeature, (yyvsp[0].expressionNode).m_features & AssignFeature, 
                                                                                                     (yylsp[-2]).first_column, (yylsp[-1]).first_column + 1, (yylsp[0]).last_column), (yyvsp[-2].expressionNode).m_features | (yyvsp[0].expressionNode).m_features | AssignFeature, (yyvsp[-2].expressionNode).m_numConstants + (yyvsp[0].expressionNode).m_numConstants);
                                        }
    break;

  case 182:
                                        { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(makeAssignNode(GLOBAL_DATA, (yyvsp[-2].expressionNode).m_node, (yyvsp[-1].op), (yyvsp[0].expressionNode).m_node, (yyvsp[-2].expressionNode).m_features & AssignFeature, (yyvsp[0].expressionNode).m_features & AssignFeature,
                                                                                                     (yylsp[-2]).first_column, (yylsp[-1]).first_column + 1, (yylsp[0]).last_column), (yyvsp[-2].expressionNode).m_features | (yyvsp[0].expressionNode).m_features | AssignFeature, (yyvsp[-2].expressionNode).m_numConstants + (yyvsp[0].expressionNode).m_numConstants); 
                                        }
    break;

  case 183:
                                        { (yyval.op) = OpEqual; }
    break;

  case 184:
                                        { (yyval.op) = OpPlusEq; }
    break;

  case 185:
                                        { (yyval.op) = OpMinusEq; }
    break;

  case 186:
                                        { (yyval.op) = OpMultEq; }
    break;

  case 187:
                                        { (yyval.op) = OpDivEq; }
    break;

  case 188:
                                        { (yyval.op) = OpLShift; }
    break;

  case 189:
                                        { (yyval.op) = OpRShift; }
    break;

  case 190:
                                        { (yyval.op) = OpURShift; }
    break;

  case 191:
                                        { (yyval.op) = OpAndEq; }
    break;

  case 192:
                                        { (yyval.op) = OpXOrEq; }
    break;

  case 193:
                                        { (yyval.op) = OpOrEq; }
    break;

  case 194:
                                        { (yyval.op) = OpModEq; }
    break;

  case 196:
                                        { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(combineCommaNodes(GLOBAL_DATA, (yyvsp[-2].expressionNode).m_node, (yyvsp[0].expressionNode).m_node), (yyvsp[-2].expressionNode).m_features | (yyvsp[0].expressionNode).m_features, (yyvsp[-2].expressionNode).m_numConstants + (yyvsp[0].expressionNode).m_numConstants); }
    break;

  case 198:
                                        { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(combineCommaNodes(GLOBAL_DATA, (yyvsp[-2].expressionNode).m_node, (yyvsp[0].expressionNode).m_node), (yyvsp[-2].expressionNode).m_features | (yyvsp[0].expressionNode).m_features, (yyvsp[-2].expressionNode).m_numConstants + (yyvsp[0].expressionNode).m_numConstants); }
    break;

  case 200:
                                        { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(combineCommaNodes(GLOBAL_DATA, (yyvsp[-2].expressionNode).m_node, (yyvsp[0].expressionNode).m_node), (yyvsp[-2].expressionNode).m_features | (yyvsp[0].expressionNode).m_features, (yyvsp[-2].expressionNode).m_numConstants + (yyvsp[0].expressionNode).m_numConstants); }
    break;

  case 218:
                                        { (yyval.statementNode) = createNodeDeclarationInfo<StatementNode*>(new (GLOBAL_DATA) BlockNode(GLOBAL_DATA, 0), 0, 0, 0, 0);
                                          setStatementLocation((yyval.statementNode).m_node, (yylsp[-1]), (yylsp[0])); }
    break;

  case 219:
                                        { (yyval.statementNode) = createNodeDeclarationInfo<StatementNode*>(new (GLOBAL_DATA) BlockNode(GLOBAL_DATA, (yyvsp[-1].sourceElements).m_node), (yyvsp[-1].sourceElements).m_varDeclarations, (yyvsp[-1].sourceElements).m_funcDeclarations, (yyvsp[-1].sourceElements).m_features, (yyvsp[-1].sourceElements).m_numConstants);
                                          setStatementLocation((yyval.statementNode).m_node, (yylsp[-2]), (yylsp[0])); }
    break;

  case 220:
                                        { (yyval.statementNode) = createNodeDeclarationInfo<StatementNode*>(makeVarStatementNode(GLOBAL_DATA, (yyvsp[-1].varDeclList).m_node), (yyvsp[-1].varDeclList).m_varDeclarations, (yyvsp[-1].varDeclList).m_funcDeclarations, (yyvsp[-1].varDeclList).m_features, (yyvsp[-1].varDeclList).m_numConstants);
                                          setStatementLocation((yyval.statementNode).m_node, (yylsp[-2]), (yylsp[0])); }
    break;

  case 221:
                                        { (yyval.statementNode) = createNodeDeclarationInfo<StatementNode*>(makeVarStatementNode(GLOBAL_DATA, (yyvsp[-1].varDeclList).m_node), (yyvsp[-1].varDeclList).m_varDeclarations, (yyvsp[-1].varDeclList).m_funcDeclarations, (yyvsp[-1].varDeclList).m_features, (yyvsp[-1].varDeclList).m_numConstants);
                                          setStatementLocation((yyval.statementNode).m_node, (yylsp[-2]), (yylsp[-1]));
                                          AUTO_SEMICOLON; }
    break;

  case 222:
                                        { (yyval.varDeclList).m_node = 0;
                                          (yyval.varDeclList).m_varDeclarations = new (GLOBAL_DATA) ParserArenaData<DeclarationStacks::VarStack>;
                                          appendToVarDeclarationList(GLOBAL_DATA, (yyval.varDeclList).m_varDeclarations, *(yyvsp[0].ident), 0);
                                          (yyval.varDeclList).m_funcDeclarations = 0;
                                          (yyval.varDeclList).m_features = (*(yyvsp[0].ident) == GLOBAL_DATA->propertyNames->arguments) ? ArgumentsFeature : 0;
                                          (yyval.varDeclList).m_numConstants = 0;
                                        }
    break;

  case 223:
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

  case 224:
                                        { (yyval.varDeclList).m_node = (yyvsp[-2].varDeclList).m_node;
                                          (yyval.varDeclList).m_varDeclarations = (yyvsp[-2].varDeclList).m_varDeclarations;
                                          appendToVarDeclarationList(GLOBAL_DATA, (yyval.varDeclList).m_varDeclarations, *(yyvsp[0].ident), 0);
                                          (yyval.varDeclList).m_funcDeclarations = 0;
                                          (yyval.varDeclList).m_features = (yyvsp[-2].varDeclList).m_features | ((*(yyvsp[0].ident) == GLOBAL_DATA->propertyNames->arguments) ? ArgumentsFeature : 0);
                                          (yyval.varDeclList).m_numConstants = (yyvsp[-2].varDeclList).m_numConstants;
                                        }
    break;

  case 225:
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

  case 226:
                                        { (yyval.varDeclList).m_node = 0;
                                          (yyval.varDeclList).m_varDeclarations = new (GLOBAL_DATA) ParserArenaData<DeclarationStacks::VarStack>;
                                          appendToVarDeclarationList(GLOBAL_DATA, (yyval.varDeclList).m_varDeclarations, *(yyvsp[0].ident), 0);
                                          (yyval.varDeclList).m_funcDeclarations = 0;
                                          (yyval.varDeclList).m_features = (*(yyvsp[0].ident) == GLOBAL_DATA->propertyNames->arguments) ? ArgumentsFeature : 0;
                                          (yyval.varDeclList).m_numConstants = 0;
                                        }
    break;

  case 227:
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

  case 228:
                                        { (yyval.varDeclList).m_node = (yyvsp[-2].varDeclList).m_node;
                                          (yyval.varDeclList).m_varDeclarations = (yyvsp[-2].varDeclList).m_varDeclarations;
                                          appendToVarDeclarationList(GLOBAL_DATA, (yyval.varDeclList).m_varDeclarations, *(yyvsp[0].ident), 0);
                                          (yyval.varDeclList).m_funcDeclarations = 0;
                                          (yyval.varDeclList).m_features = (yyvsp[-2].varDeclList).m_features | ((*(yyvsp[0].ident) == GLOBAL_DATA->propertyNames->arguments) ? ArgumentsFeature : 0);
                                          (yyval.varDeclList).m_numConstants = (yyvsp[-2].varDeclList).m_numConstants;
                                        }
    break;

  case 229:
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

  case 230:
                                        { (yyval.statementNode) = createNodeDeclarationInfo<StatementNode*>(new (GLOBAL_DATA) ConstStatementNode(GLOBAL_DATA, (yyvsp[-1].constDeclList).m_node.head), (yyvsp[-1].constDeclList).m_varDeclarations, (yyvsp[-1].constDeclList).m_funcDeclarations, (yyvsp[-1].constDeclList).m_features, (yyvsp[-1].constDeclList).m_numConstants);
                                          setStatementLocation((yyval.statementNode).m_node, (yylsp[-2]), (yylsp[0])); }
    break;

  case 231:
                                        { (yyval.statementNode) = createNodeDeclarationInfo<StatementNode*>(new (GLOBAL_DATA) ConstStatementNode(GLOBAL_DATA, (yyvsp[-1].constDeclList).m_node.head), (yyvsp[-1].constDeclList).m_varDeclarations, (yyvsp[-1].constDeclList).m_funcDeclarations, (yyvsp[-1].constDeclList).m_features, (yyvsp[-1].constDeclList).m_numConstants);
                                          setStatementLocation((yyval.statementNode).m_node, (yylsp[-2]), (yylsp[-1])); AUTO_SEMICOLON; }
    break;

  case 232:
                                        { (yyval.constDeclList).m_node.head = (yyvsp[0].constDeclNode).m_node;
                                          (yyval.constDeclList).m_node.tail = (yyval.constDeclList).m_node.head;
                                          (yyval.constDeclList).m_varDeclarations = new (GLOBAL_DATA) ParserArenaData<DeclarationStacks::VarStack>;
                                          appendToVarDeclarationList(GLOBAL_DATA, (yyval.constDeclList).m_varDeclarations, (yyvsp[0].constDeclNode).m_node);
                                          (yyval.constDeclList).m_funcDeclarations = 0; 
                                          (yyval.constDeclList).m_features = (yyvsp[0].constDeclNode).m_features;
                                          (yyval.constDeclList).m_numConstants = (yyvsp[0].constDeclNode).m_numConstants;
    }
    break;

  case 233:
                                        { (yyval.constDeclList).m_node.head = (yyvsp[-2].constDeclList).m_node.head;
                                          (yyvsp[-2].constDeclList).m_node.tail->m_next = (yyvsp[0].constDeclNode).m_node;
                                          (yyval.constDeclList).m_node.tail = (yyvsp[0].constDeclNode).m_node;
                                          (yyval.constDeclList).m_varDeclarations = (yyvsp[-2].constDeclList).m_varDeclarations;
                                          appendToVarDeclarationList(GLOBAL_DATA, (yyval.constDeclList).m_varDeclarations, (yyvsp[0].constDeclNode).m_node);
                                          (yyval.constDeclList).m_funcDeclarations = 0;
                                          (yyval.constDeclList).m_features = (yyvsp[-2].constDeclList).m_features | (yyvsp[0].constDeclNode).m_features;
                                          (yyval.constDeclList).m_numConstants = (yyvsp[-2].constDeclList).m_numConstants + (yyvsp[0].constDeclNode).m_numConstants; }
    break;

  case 234:
                                        { (yyval.constDeclNode) = createNodeInfo<ConstDeclNode*>(new (GLOBAL_DATA) ConstDeclNode(GLOBAL_DATA, *(yyvsp[0].ident), 0), (*(yyvsp[0].ident) == GLOBAL_DATA->propertyNames->arguments) ? ArgumentsFeature : 0, 0); }
    break;

  case 235:
                                        { (yyval.constDeclNode) = createNodeInfo<ConstDeclNode*>(new (GLOBAL_DATA) ConstDeclNode(GLOBAL_DATA, *(yyvsp[-1].ident), (yyvsp[0].expressionNode).m_node), ((*(yyvsp[-1].ident) == GLOBAL_DATA->propertyNames->arguments) ? ArgumentsFeature : 0) | (yyvsp[0].expressionNode).m_features, (yyvsp[0].expressionNode).m_numConstants); }
    break;

  case 236:
                                        { (yyval.expressionNode) = (yyvsp[0].expressionNode); }
    break;

  case 237:
                                        { (yyval.expressionNode) = (yyvsp[0].expressionNode); }
    break;

  case 238:
                                        { (yyval.statementNode) = createNodeDeclarationInfo<StatementNode*>(new (GLOBAL_DATA) EmptyStatementNode(GLOBAL_DATA), 0, 0, 0, 0); }
    break;

  case 239:
                                        { (yyval.statementNode) = createNodeDeclarationInfo<StatementNode*>(new (GLOBAL_DATA) ExprStatementNode(GLOBAL_DATA, (yyvsp[-1].expressionNode).m_node), 0, 0, (yyvsp[-1].expressionNode).m_features, (yyvsp[-1].expressionNode).m_numConstants);
                                          setStatementLocation((yyval.statementNode).m_node, (yylsp[-1]), (yylsp[0])); }
    break;

  case 240:
                                        { (yyval.statementNode) = createNodeDeclarationInfo<StatementNode*>(new (GLOBAL_DATA) ExprStatementNode(GLOBAL_DATA, (yyvsp[-1].expressionNode).m_node), 0, 0, (yyvsp[-1].expressionNode).m_features, (yyvsp[-1].expressionNode).m_numConstants);
                                          setStatementLocation((yyval.statementNode).m_node, (yylsp[-1]), (yylsp[-1])); AUTO_SEMICOLON; }
    break;

  case 241:
                                        { (yyval.statementNode) = createNodeDeclarationInfo<StatementNode*>(new (GLOBAL_DATA) IfNode(GLOBAL_DATA, (yyvsp[-2].expressionNode).m_node, (yyvsp[0].statementNode).m_node), (yyvsp[0].statementNode).m_varDeclarations, (yyvsp[0].statementNode).m_funcDeclarations, (yyvsp[-2].expressionNode).m_features | (yyvsp[0].statementNode).m_features, (yyvsp[-2].expressionNode).m_numConstants + (yyvsp[0].statementNode).m_numConstants);
                                          setStatementLocation((yyval.statementNode).m_node, (yylsp[-4]), (yylsp[-1])); }
    break;

  case 242:
                                        { (yyval.statementNode) = createNodeDeclarationInfo<StatementNode*>(new (GLOBAL_DATA) IfElseNode(GLOBAL_DATA, (yyvsp[-4].expressionNode).m_node, (yyvsp[-2].statementNode).m_node, (yyvsp[0].statementNode).m_node), 
                                                                                         mergeDeclarationLists((yyvsp[-2].statementNode).m_varDeclarations, (yyvsp[0].statementNode).m_varDeclarations),
                                                                                         mergeDeclarationLists((yyvsp[-2].statementNode).m_funcDeclarations, (yyvsp[0].statementNode).m_funcDeclarations),
                                                                                         (yyvsp[-4].expressionNode).m_features | (yyvsp[-2].statementNode).m_features | (yyvsp[0].statementNode).m_features,
                                                                                         (yyvsp[-4].expressionNode).m_numConstants + (yyvsp[-2].statementNode).m_numConstants + (yyvsp[0].statementNode).m_numConstants); 
                                          setStatementLocation((yyval.statementNode).m_node, (yylsp[-6]), (yylsp[-3])); }
    break;

  case 243:
                                           { (yyval.statementNode) = createNodeDeclarationInfo<StatementNode*>(new (GLOBAL_DATA) DoWhileNode(GLOBAL_DATA, (yyvsp[-5].statementNode).m_node, (yyvsp[-2].expressionNode).m_node), (yyvsp[-5].statementNode).m_varDeclarations, (yyvsp[-5].statementNode).m_funcDeclarations, (yyvsp[-5].statementNode).m_features | (yyvsp[-2].expressionNode).m_features, (yyvsp[-5].statementNode).m_numConstants + (yyvsp[-2].expressionNode).m_numConstants);
                                             setStatementLocation((yyval.statementNode).m_node, (yylsp[-6]), (yylsp[-4])); }
    break;

  case 244:
                                           { (yyval.statementNode) = createNodeDeclarationInfo<StatementNode*>(new (GLOBAL_DATA) DoWhileNode(GLOBAL_DATA, (yyvsp[-5].statementNode).m_node, (yyvsp[-2].expressionNode).m_node), (yyvsp[-5].statementNode).m_varDeclarations, (yyvsp[-5].statementNode).m_funcDeclarations, (yyvsp[-5].statementNode).m_features | (yyvsp[-2].expressionNode).m_features, (yyvsp[-5].statementNode).m_numConstants + (yyvsp[-2].expressionNode).m_numConstants);
                                             setStatementLocation((yyval.statementNode).m_node, (yylsp[-6]), (yylsp[-4])); }
    break;

  case 245:
                                        { (yyval.statementNode) = createNodeDeclarationInfo<StatementNode*>(new (GLOBAL_DATA) WhileNode(GLOBAL_DATA, (yyvsp[-2].expressionNode).m_node, (yyvsp[0].statementNode).m_node), (yyvsp[0].statementNode).m_varDeclarations, (yyvsp[0].statementNode).m_funcDeclarations, (yyvsp[-2].expressionNode).m_features | (yyvsp[0].statementNode).m_features, (yyvsp[-2].expressionNode).m_numConstants + (yyvsp[0].statementNode).m_numConstants);
                                          setStatementLocation((yyval.statementNode).m_node, (yylsp[-4]), (yylsp[-1])); }
    break;

  case 246:
                                        { (yyval.statementNode) = createNodeDeclarationInfo<StatementNode*>(new (GLOBAL_DATA) ForNode(GLOBAL_DATA, (yyvsp[-6].expressionNode).m_node, (yyvsp[-4].expressionNode).m_node, (yyvsp[-2].expressionNode).m_node, (yyvsp[0].statementNode).m_node, false), (yyvsp[0].statementNode).m_varDeclarations, (yyvsp[0].statementNode).m_funcDeclarations, 
                                                                                         (yyvsp[-6].expressionNode).m_features | (yyvsp[-4].expressionNode).m_features | (yyvsp[-2].expressionNode).m_features | (yyvsp[0].statementNode).m_features,
                                                                                         (yyvsp[-6].expressionNode).m_numConstants + (yyvsp[-4].expressionNode).m_numConstants + (yyvsp[-2].expressionNode).m_numConstants + (yyvsp[0].statementNode).m_numConstants);
                                          setStatementLocation((yyval.statementNode).m_node, (yylsp[-8]), (yylsp[-1])); 
                                        }
    break;

  case 247:
                                        { (yyval.statementNode) = createNodeDeclarationInfo<StatementNode*>(new (GLOBAL_DATA) ForNode(GLOBAL_DATA, (yyvsp[-6].varDeclList).m_node, (yyvsp[-4].expressionNode).m_node, (yyvsp[-2].expressionNode).m_node, (yyvsp[0].statementNode).m_node, true),
                                                                                         mergeDeclarationLists((yyvsp[-6].varDeclList).m_varDeclarations, (yyvsp[0].statementNode).m_varDeclarations),
                                                                                         mergeDeclarationLists((yyvsp[-6].varDeclList).m_funcDeclarations, (yyvsp[0].statementNode).m_funcDeclarations),
                                                                                         (yyvsp[-6].varDeclList).m_features | (yyvsp[-4].expressionNode).m_features | (yyvsp[-2].expressionNode).m_features | (yyvsp[0].statementNode).m_features,
                                                                                         (yyvsp[-6].varDeclList).m_numConstants + (yyvsp[-4].expressionNode).m_numConstants + (yyvsp[-2].expressionNode).m_numConstants + (yyvsp[0].statementNode).m_numConstants);
                                          setStatementLocation((yyval.statementNode).m_node, (yylsp[-9]), (yylsp[-1])); }
    break;

  case 248:
                                        {
                                            ForInNode* node = new (GLOBAL_DATA) ForInNode(GLOBAL_DATA, (yyvsp[-4].expressionNode).m_node, (yyvsp[-2].expressionNode).m_node, (yyvsp[0].statementNode).m_node);
                                            setExceptionLocation(node, (yylsp[-4]).first_column, (yylsp[-4]).last_column, (yylsp[-2]).last_column);
                                            (yyval.statementNode) = createNodeDeclarationInfo<StatementNode*>(node, (yyvsp[0].statementNode).m_varDeclarations, (yyvsp[0].statementNode).m_funcDeclarations,
                                                                                           (yyvsp[-4].expressionNode).m_features | (yyvsp[-2].expressionNode).m_features | (yyvsp[0].statementNode).m_features,
                                                                                           (yyvsp[-4].expressionNode).m_numConstants + (yyvsp[-2].expressionNode).m_numConstants + (yyvsp[0].statementNode).m_numConstants);
                                            setStatementLocation((yyval.statementNode).m_node, (yylsp[-6]), (yylsp[-1]));
                                        }
    break;

  case 249:
                                        { ForInNode *forIn = new (GLOBAL_DATA) ForInNode(GLOBAL_DATA, *(yyvsp[-4].ident), 0, (yyvsp[-2].expressionNode).m_node, (yyvsp[0].statementNode).m_node, (yylsp[-3]).first_column, (yylsp[-3]).first_column - (yylsp[-4]).first_column, (yylsp[-2]).last_column - (yylsp[-3]).first_column);
                                          setExceptionLocation(forIn, (yylsp[-4]).first_column, (yylsp[-3]).first_column + 1, (yylsp[-2]).last_column);
                                          appendToVarDeclarationList(GLOBAL_DATA, (yyvsp[0].statementNode).m_varDeclarations, *(yyvsp[-4].ident), DeclarationStacks::HasInitializer);
                                          (yyval.statementNode) = createNodeDeclarationInfo<StatementNode*>(forIn, (yyvsp[0].statementNode).m_varDeclarations, (yyvsp[0].statementNode).m_funcDeclarations, ((*(yyvsp[-4].ident) == GLOBAL_DATA->propertyNames->arguments) ? ArgumentsFeature : 0) | (yyvsp[-2].expressionNode).m_features | (yyvsp[0].statementNode).m_features, (yyvsp[-2].expressionNode).m_numConstants + (yyvsp[0].statementNode).m_numConstants);
                                          setStatementLocation((yyval.statementNode).m_node, (yylsp[-7]), (yylsp[-1])); }
    break;

  case 250:
                                        { ForInNode *forIn = new (GLOBAL_DATA) ForInNode(GLOBAL_DATA, *(yyvsp[-5].ident), (yyvsp[-4].expressionNode).m_node, (yyvsp[-2].expressionNode).m_node, (yyvsp[0].statementNode).m_node, (yylsp[-4]).first_column, (yylsp[-4]).first_column - (yylsp[-5]).first_column, (yylsp[-4]).last_column - (yylsp[-4]).first_column);
                                          setExceptionLocation(forIn, (yylsp[-5]).first_column, (yylsp[-3]).first_column + 1, (yylsp[-2]).last_column);
                                          appendToVarDeclarationList(GLOBAL_DATA, (yyvsp[0].statementNode).m_varDeclarations, *(yyvsp[-5].ident), DeclarationStacks::HasInitializer);
                                          (yyval.statementNode) = createNodeDeclarationInfo<StatementNode*>(forIn, (yyvsp[0].statementNode).m_varDeclarations, (yyvsp[0].statementNode).m_funcDeclarations,
                                                                                         ((*(yyvsp[-5].ident) == GLOBAL_DATA->propertyNames->arguments) ? ArgumentsFeature : 0) | (yyvsp[-4].expressionNode).m_features | (yyvsp[-2].expressionNode).m_features | (yyvsp[0].statementNode).m_features,
                                                                                         (yyvsp[-4].expressionNode).m_numConstants + (yyvsp[-2].expressionNode).m_numConstants + (yyvsp[0].statementNode).m_numConstants);
                                          setStatementLocation((yyval.statementNode).m_node, (yylsp[-8]), (yylsp[-1])); }
    break;

  case 251:
                                        { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(0, 0, 0); }
    break;

  case 253:
                                        { (yyval.expressionNode) = createNodeInfo<ExpressionNode*>(0, 0, 0); }
    break;

  case 255:
                                        { ContinueNode* node = new (GLOBAL_DATA) ContinueNode(GLOBAL_DATA);
                                          setExceptionLocation(node, (yylsp[-1]).first_column, (yylsp[-1]).last_column, (yylsp[-1]).last_column); 
                                          (yyval.statementNode) = createNodeDeclarationInfo<StatementNode*>(node, 0, 0, 0, 0);
                                          setStatementLocation((yyval.statementNode).m_node, (yylsp[-1]), (yylsp[0])); }
    break;

  case 256:
                                        { ContinueNode* node = new (GLOBAL_DATA) ContinueNode(GLOBAL_DATA);
                                          setExceptionLocation(node, (yylsp[-1]).first_column, (yylsp[-1]).last_column, (yylsp[-1]).last_column); 
                                          (yyval.statementNode) = createNodeDeclarationInfo<StatementNode*>(node, 0, 0, 0, 0);
                                          setStatementLocation((yyval.statementNode).m_node, (yylsp[-1]), (yylsp[-1])); AUTO_SEMICOLON; }
    break;

  case 257:
                                        { ContinueNode* node = new (GLOBAL_DATA) ContinueNode(GLOBAL_DATA, *(yyvsp[-1].ident));
                                          setExceptionLocation(node, (yylsp[-2]).first_column, (yylsp[-1]).last_column, (yylsp[-1]).last_column); 
                                          (yyval.statementNode) = createNodeDeclarationInfo<StatementNode*>(node, 0, 0, 0, 0);
                                          setStatementLocation((yyval.statementNode).m_node, (yylsp[-2]), (yylsp[0])); }
    break;

  case 258:
                                        { ContinueNode* node = new (GLOBAL_DATA) ContinueNode(GLOBAL_DATA, *(yyvsp[-1].ident));
                                          setExceptionLocation(node, (yylsp[-2]).first_column, (yylsp[-1]).last_column, (yylsp[-1]).last_column); 
                                          (yyval.statementNode) = createNodeDeclarationInfo<StatementNode*>(node, 0, 0, 0, 0);
                                          setStatementLocation((yyval.statementNode).m_node, (yylsp[-2]), (yylsp[-1])); AUTO_SEMICOLON; }
    break;

  case 259:
                                        { BreakNode* node = new (GLOBAL_DATA) BreakNode(GLOBAL_DATA);
                                          setExceptionLocation(node, (yylsp[-1]).first_column, (yylsp[-1]).last_column, (yylsp[-1]).last_column);
                                          (yyval.statementNode) = createNodeDeclarationInfo<StatementNode*>(node, 0, 0, 0, 0); setStatementLocation((yyval.statementNode).m_node, (yylsp[-1]), (yylsp[0])); }
    break;

  case 260:
                                        { BreakNode* node = new (GLOBAL_DATA) BreakNode(GLOBAL_DATA);
                                          setExceptionLocation(node, (yylsp[-1]).first_column, (yylsp[-1]).last_column, (yylsp[-1]).last_column);
                                          (yyval.statementNode) = createNodeDeclarationInfo<StatementNode*>(new (GLOBAL_DATA) BreakNode(GLOBAL_DATA), 0, 0, 0, 0); setStatementLocation((yyval.statementNode).m_node, (yylsp[-1]), (yylsp[-1])); AUTO_SEMICOLON; }
    break;

  case 261:
                                        { BreakNode* node = new (GLOBAL_DATA) BreakNode(GLOBAL_DATA, *(yyvsp[-1].ident));
                                          setExceptionLocation(node, (yylsp[-2]).first_column, (yylsp[-1]).last_column, (yylsp[-1]).last_column);
                                          (yyval.statementNode) = createNodeDeclarationInfo<StatementNode*>(node, 0, 0, 0, 0); setStatementLocation((yyval.statementNode).m_node, (yylsp[-2]), (yylsp[0])); }
    break;

  case 262:
                                        { BreakNode* node = new (GLOBAL_DATA) BreakNode(GLOBAL_DATA, *(yyvsp[-1].ident));
                                          setExceptionLocation(node, (yylsp[-2]).first_column, (yylsp[-1]).last_column, (yylsp[-1]).last_column);
                                          (yyval.statementNode) = createNodeDeclarationInfo<StatementNode*>(new (GLOBAL_DATA) BreakNode(GLOBAL_DATA, *(yyvsp[-1].ident)), 0, 0, 0, 0); setStatementLocation((yyval.statementNode).m_node, (yylsp[-2]), (yylsp[-1])); AUTO_SEMICOLON; }
    break;

  case 263:
                                        { ReturnNode* node = new (GLOBAL_DATA) ReturnNode(GLOBAL_DATA, 0); 
                                          setExceptionLocation(node, (yylsp[-1]).first_column, (yylsp[-1]).last_column, (yylsp[-1]).last_column); 
                                          (yyval.statementNode) = createNodeDeclarationInfo<StatementNode*>(node, 0, 0, 0, 0); setStatementLocation((yyval.statementNode).m_node, (yylsp[-1]), (yylsp[0])); }
    break;

  case 264:
                                        { ReturnNode* node = new (GLOBAL_DATA) ReturnNode(GLOBAL_DATA, 0); 
                                          setExceptionLocation(node, (yylsp[-1]).first_column, (yylsp[-1]).last_column, (yylsp[-1]).last_column); 
                                          (yyval.statementNode) = createNodeDeclarationInfo<StatementNode*>(node, 0, 0, 0, 0); setStatementLocation((yyval.statementNode).m_node, (yylsp[-1]), (yylsp[-1])); AUTO_SEMICOLON; }
    break;

  case 265:
                                        { ReturnNode* node = new (GLOBAL_DATA) ReturnNode(GLOBAL_DATA, (yyvsp[-1].expressionNode).m_node); 
                                          setExceptionLocation(node, (yylsp[-2]).first_column, (yylsp[-1]).last_column, (yylsp[-1]).last_column);
                                          (yyval.statementNode) = createNodeDeclarationInfo<StatementNode*>(node, 0, 0, (yyvsp[-1].expressionNode).m_features, (yyvsp[-1].expressionNode).m_numConstants); setStatementLocation((yyval.statementNode).m_node, (yylsp[-2]), (yylsp[0])); }
    break;

  case 266:
                                        { ReturnNode* node = new (GLOBAL_DATA) ReturnNode(GLOBAL_DATA, (yyvsp[-1].expressionNode).m_node); 
                                          setExceptionLocation(node, (yylsp[-2]).first_column, (yylsp[-1]).last_column, (yylsp[-1]).last_column); 
                                          (yyval.statementNode) = createNodeDeclarationInfo<StatementNode*>(node, 0, 0, (yyvsp[-1].expressionNode).m_features, (yyvsp[-1].expressionNode).m_numConstants); setStatementLocation((yyval.statementNode).m_node, (yylsp[-2]), (yylsp[-1])); AUTO_SEMICOLON; }
    break;

  case 267:
                                        { (yyval.statementNode) = createNodeDeclarationInfo<StatementNode*>(new (GLOBAL_DATA) WithNode(GLOBAL_DATA, (yyvsp[-2].expressionNode).m_node, (yyvsp[0].statementNode).m_node, (yylsp[-2]).last_column, (yylsp[-2]).last_column - (yylsp[-2]).first_column),
                                                                                         (yyvsp[0].statementNode).m_varDeclarations, (yyvsp[0].statementNode).m_funcDeclarations, (yyvsp[-2].expressionNode).m_features | (yyvsp[0].statementNode).m_features | WithFeature, (yyvsp[-2].expressionNode).m_numConstants + (yyvsp[0].statementNode).m_numConstants);
                                          setStatementLocation((yyval.statementNode).m_node, (yylsp[-4]), (yylsp[-1])); }
    break;

  case 268:
                                        { (yyval.statementNode) = createNodeDeclarationInfo<StatementNode*>(new (GLOBAL_DATA) SwitchNode(GLOBAL_DATA, (yyvsp[-2].expressionNode).m_node, (yyvsp[0].caseBlockNode).m_node), (yyvsp[0].caseBlockNode).m_varDeclarations, (yyvsp[0].caseBlockNode).m_funcDeclarations,
                                                                                         (yyvsp[-2].expressionNode).m_features | (yyvsp[0].caseBlockNode).m_features, (yyvsp[-2].expressionNode).m_numConstants + (yyvsp[0].caseBlockNode).m_numConstants);
                                          setStatementLocation((yyval.statementNode).m_node, (yylsp[-4]), (yylsp[-1])); }
    break;

  case 269:
                                                     { (yyval.caseBlockNode) = createNodeDeclarationInfo<CaseBlockNode*>(new (GLOBAL_DATA) CaseBlockNode(GLOBAL_DATA, (yyvsp[-1].clauseList).m_node.head, 0, 0), (yyvsp[-1].clauseList).m_varDeclarations, (yyvsp[-1].clauseList).m_funcDeclarations, (yyvsp[-1].clauseList).m_features, (yyvsp[-1].clauseList).m_numConstants); }
    break;

  case 270:
                                        { (yyval.caseBlockNode) = createNodeDeclarationInfo<CaseBlockNode*>(new (GLOBAL_DATA) CaseBlockNode(GLOBAL_DATA, (yyvsp[-3].clauseList).m_node.head, (yyvsp[-2].caseClauseNode).m_node, (yyvsp[-1].clauseList).m_node.head),
                                                                                         mergeDeclarationLists(mergeDeclarationLists((yyvsp[-3].clauseList).m_varDeclarations, (yyvsp[-2].caseClauseNode).m_varDeclarations), (yyvsp[-1].clauseList).m_varDeclarations),
                                                                                         mergeDeclarationLists(mergeDeclarationLists((yyvsp[-3].clauseList).m_funcDeclarations, (yyvsp[-2].caseClauseNode).m_funcDeclarations), (yyvsp[-1].clauseList).m_funcDeclarations),
                                                                                         (yyvsp[-3].clauseList).m_features | (yyvsp[-2].caseClauseNode).m_features | (yyvsp[-1].clauseList).m_features,
                                                                                         (yyvsp[-3].clauseList).m_numConstants + (yyvsp[-2].caseClauseNode).m_numConstants + (yyvsp[-1].clauseList).m_numConstants); }
    break;

  case 271:
                                        { (yyval.clauseList).m_node.head = 0; (yyval.clauseList).m_node.tail = 0; (yyval.clauseList).m_varDeclarations = 0; (yyval.clauseList).m_funcDeclarations = 0; (yyval.clauseList).m_features = 0; (yyval.clauseList).m_numConstants = 0; }
    break;

  case 273:
                                        { (yyval.clauseList).m_node.head = new (GLOBAL_DATA) ClauseListNode(GLOBAL_DATA, (yyvsp[0].caseClauseNode).m_node);
                                          (yyval.clauseList).m_node.tail = (yyval.clauseList).m_node.head;
                                          (yyval.clauseList).m_varDeclarations = (yyvsp[0].caseClauseNode).m_varDeclarations;
                                          (yyval.clauseList).m_funcDeclarations = (yyvsp[0].caseClauseNode).m_funcDeclarations; 
                                          (yyval.clauseList).m_features = (yyvsp[0].caseClauseNode).m_features;
                                          (yyval.clauseList).m_numConstants = (yyvsp[0].caseClauseNode).m_numConstants; }
    break;

  case 274:
                                        { (yyval.clauseList).m_node.head = (yyvsp[-1].clauseList).m_node.head;
                                          (yyval.clauseList).m_node.tail = new (GLOBAL_DATA) ClauseListNode(GLOBAL_DATA, (yyvsp[-1].clauseList).m_node.tail, (yyvsp[0].caseClauseNode).m_node);
                                          (yyval.clauseList).m_varDeclarations = mergeDeclarationLists((yyvsp[-1].clauseList).m_varDeclarations, (yyvsp[0].caseClauseNode).m_varDeclarations);
                                          (yyval.clauseList).m_funcDeclarations = mergeDeclarationLists((yyvsp[-1].clauseList).m_funcDeclarations, (yyvsp[0].caseClauseNode).m_funcDeclarations);
                                          (yyval.clauseList).m_features = (yyvsp[-1].clauseList).m_features | (yyvsp[0].caseClauseNode).m_features;
                                          (yyval.clauseList).m_numConstants = (yyvsp[-1].clauseList).m_numConstants + (yyvsp[0].caseClauseNode).m_numConstants;
                                        }
    break;

  case 275:
                                        { (yyval.caseClauseNode) = createNodeDeclarationInfo<CaseClauseNode*>(new (GLOBAL_DATA) CaseClauseNode(GLOBAL_DATA, (yyvsp[-1].expressionNode).m_node), 0, 0, (yyvsp[-1].expressionNode).m_features, (yyvsp[-1].expressionNode).m_numConstants); }
    break;

  case 276:
                                        { (yyval.caseClauseNode) = createNodeDeclarationInfo<CaseClauseNode*>(new (GLOBAL_DATA) CaseClauseNode(GLOBAL_DATA, (yyvsp[-2].expressionNode).m_node, (yyvsp[0].sourceElements).m_node), (yyvsp[0].sourceElements).m_varDeclarations, (yyvsp[0].sourceElements).m_funcDeclarations, (yyvsp[-2].expressionNode).m_features | (yyvsp[0].sourceElements).m_features, (yyvsp[-2].expressionNode).m_numConstants + (yyvsp[0].sourceElements).m_numConstants); }
    break;

  case 277:
                                        { (yyval.caseClauseNode) = createNodeDeclarationInfo<CaseClauseNode*>(new (GLOBAL_DATA) CaseClauseNode(GLOBAL_DATA, 0), 0, 0, 0, 0); }
    break;

  case 278:
                                        { (yyval.caseClauseNode) = createNodeDeclarationInfo<CaseClauseNode*>(new (GLOBAL_DATA) CaseClauseNode(GLOBAL_DATA, 0, (yyvsp[0].sourceElements).m_node), (yyvsp[0].sourceElements).m_varDeclarations, (yyvsp[0].sourceElements).m_funcDeclarations, (yyvsp[0].sourceElements).m_features, (yyvsp[0].sourceElements).m_numConstants); }
    break;

  case 279:
                                        { LabelNode* node = new (GLOBAL_DATA) LabelNode(GLOBAL_DATA, *(yyvsp[-2].ident), (yyvsp[0].statementNode).m_node);
                                          setExceptionLocation(node, (yylsp[-2]).first_column, (yylsp[-1]).last_column, (yylsp[-1]).last_column);
                                          (yyval.statementNode) = createNodeDeclarationInfo<StatementNode*>(node, (yyvsp[0].statementNode).m_varDeclarations, (yyvsp[0].statementNode).m_funcDeclarations, (yyvsp[0].statementNode).m_features, (yyvsp[0].statementNode).m_numConstants); }
    break;

  case 280:
                                        { ThrowNode* node = new (GLOBAL_DATA) ThrowNode(GLOBAL_DATA, (yyvsp[-1].expressionNode).m_node);
                                          setExceptionLocation(node, (yylsp[-2]).first_column, (yylsp[-1]).last_column, (yylsp[-1]).last_column);
                                          (yyval.statementNode) = createNodeDeclarationInfo<StatementNode*>(node, 0, 0, (yyvsp[-1].expressionNode).m_features, (yyvsp[-1].expressionNode).m_numConstants); setStatementLocation((yyval.statementNode).m_node, (yylsp[-2]), (yylsp[-1]));
                                        }
    break;

  case 281:
                                        { ThrowNode* node = new (GLOBAL_DATA) ThrowNode(GLOBAL_DATA, (yyvsp[-1].expressionNode).m_node);
                                          setExceptionLocation(node, (yylsp[-2]).first_column, (yylsp[-1]).last_column, (yylsp[-1]).last_column);
                                          (yyval.statementNode) = createNodeDeclarationInfo<StatementNode*>(node, 0, 0, (yyvsp[-1].expressionNode).m_features, (yyvsp[-1].expressionNode).m_numConstants); setStatementLocation((yyval.statementNode).m_node, (yylsp[-2]), (yylsp[-1])); AUTO_SEMICOLON; 
                                        }
    break;

  case 282:
                                        { (yyval.statementNode) = createNodeDeclarationInfo<StatementNode*>(new (GLOBAL_DATA) TryNode(GLOBAL_DATA, (yyvsp[-2].statementNode).m_node, GLOBAL_DATA->propertyNames->nullIdentifier, false, 0, (yyvsp[0].statementNode).m_node),
                                                                                         mergeDeclarationLists((yyvsp[-2].statementNode).m_varDeclarations, (yyvsp[0].statementNode).m_varDeclarations),
                                                                                         mergeDeclarationLists((yyvsp[-2].statementNode).m_funcDeclarations, (yyvsp[0].statementNode).m_funcDeclarations),
                                                                                         (yyvsp[-2].statementNode).m_features | (yyvsp[0].statementNode).m_features,
                                                                                         (yyvsp[-2].statementNode).m_numConstants + (yyvsp[0].statementNode).m_numConstants);
                                          setStatementLocation((yyval.statementNode).m_node, (yylsp[-3]), (yylsp[-2])); }
    break;

  case 283:
                                        { (yyval.statementNode) = createNodeDeclarationInfo<StatementNode*>(new (GLOBAL_DATA) TryNode(GLOBAL_DATA, (yyvsp[-5].statementNode).m_node, *(yyvsp[-2].ident), ((yyvsp[0].statementNode).m_features & EvalFeature) != 0, (yyvsp[0].statementNode).m_node, 0),
                                                                                         mergeDeclarationLists((yyvsp[-5].statementNode).m_varDeclarations, (yyvsp[0].statementNode).m_varDeclarations),
                                                                                         mergeDeclarationLists((yyvsp[-5].statementNode).m_funcDeclarations, (yyvsp[0].statementNode).m_funcDeclarations),
                                                                                         (yyvsp[-5].statementNode).m_features | (yyvsp[0].statementNode).m_features | CatchFeature,
                                                                                         (yyvsp[-5].statementNode).m_numConstants + (yyvsp[0].statementNode).m_numConstants);
                                          setStatementLocation((yyval.statementNode).m_node, (yylsp[-6]), (yylsp[-5])); }
    break;

  case 284:
                                        { (yyval.statementNode) = createNodeDeclarationInfo<StatementNode*>(new (GLOBAL_DATA) TryNode(GLOBAL_DATA, (yyvsp[-7].statementNode).m_node, *(yyvsp[-4].ident), ((yyvsp[-2].statementNode).m_features & EvalFeature) != 0, (yyvsp[-2].statementNode).m_node, (yyvsp[0].statementNode).m_node),
                                                                                         mergeDeclarationLists(mergeDeclarationLists((yyvsp[-7].statementNode).m_varDeclarations, (yyvsp[-2].statementNode).m_varDeclarations), (yyvsp[0].statementNode).m_varDeclarations),
                                                                                         mergeDeclarationLists(mergeDeclarationLists((yyvsp[-7].statementNode).m_funcDeclarations, (yyvsp[-2].statementNode).m_funcDeclarations), (yyvsp[0].statementNode).m_funcDeclarations),
                                                                                         (yyvsp[-7].statementNode).m_features | (yyvsp[-2].statementNode).m_features | (yyvsp[0].statementNode).m_features | CatchFeature,
                                                                                         (yyvsp[-7].statementNode).m_numConstants + (yyvsp[-2].statementNode).m_numConstants + (yyvsp[0].statementNode).m_numConstants);
                                          setStatementLocation((yyval.statementNode).m_node, (yylsp[-8]), (yylsp[-7])); }
    break;

  case 285:
                                        { (yyval.statementNode) = createNodeDeclarationInfo<StatementNode*>(new (GLOBAL_DATA) DebuggerStatementNode(GLOBAL_DATA), 0, 0, 0, 0);
                                          setStatementLocation((yyval.statementNode).m_node, (yylsp[-1]), (yylsp[0])); }
    break;

  case 286:
                                        { (yyval.statementNode) = createNodeDeclarationInfo<StatementNode*>(new (GLOBAL_DATA) DebuggerStatementNode(GLOBAL_DATA), 0, 0, 0, 0);
                                          setStatementLocation((yyval.statementNode).m_node, (yylsp[-1]), (yylsp[-1])); AUTO_SEMICOLON; }
    break;

  case 287:
                                                             { (yyval.statementNode) = createNodeDeclarationInfo<StatementNode*>(new (GLOBAL_DATA) FuncDeclNode(GLOBAL_DATA, *(yyvsp[-5].ident), (yyvsp[-1].functionBodyNode), GLOBAL_DATA->lexer->sourceCode((yyvsp[-2].intValue), (yyvsp[0].intValue), (yylsp[-2]).first_line)), 0, new (GLOBAL_DATA) ParserArenaData<DeclarationStacks::FunctionStack>, ((*(yyvsp[-5].ident) == GLOBAL_DATA->propertyNames->arguments) ? ArgumentsFeature : 0) | ClosureFeature, 0); setStatementLocation((yyvsp[-1].functionBodyNode), (yylsp[-2]), (yylsp[0])); (yyval.statementNode).m_funcDeclarations->data.append(static_cast<FuncDeclNode*>((yyval.statementNode).m_node)->body()); }
    break;

  case 288:
      {
          (yyval.statementNode) = createNodeDeclarationInfo<StatementNode*>(new (GLOBAL_DATA) FuncDeclNode(GLOBAL_DATA, *(yyvsp[-6].ident), (yyvsp[-1].functionBodyNode), GLOBAL_DATA->lexer->sourceCode((yyvsp[-2].intValue), (yyvsp[0].intValue), (yylsp[-2]).first_line), (yyvsp[-4].parameterList).m_node.head), 0, new (GLOBAL_DATA) ParserArenaData<DeclarationStacks::FunctionStack>, ((*(yyvsp[-6].ident) == GLOBAL_DATA->propertyNames->arguments) ? ArgumentsFeature : 0) | (yyvsp[-4].parameterList).m_features | ClosureFeature, 0);
          if ((yyvsp[-4].parameterList).m_features & ArgumentsFeature)
              (yyvsp[-1].functionBodyNode)->setUsesArguments();
          setStatementLocation((yyvsp[-1].functionBodyNode), (yylsp[-2]), (yylsp[0]));
          (yyval.statementNode).m_funcDeclarations->data.append(static_cast<FuncDeclNode*>((yyval.statementNode).m_node)->body());
      }
    break;

  case 289:
                                                       { (yyval.funcExprNode) = createNodeInfo(new (GLOBAL_DATA) FuncExprNode(GLOBAL_DATA, GLOBAL_DATA->propertyNames->nullIdentifier, (yyvsp[-1].functionBodyNode), GLOBAL_DATA->lexer->sourceCode((yyvsp[-2].intValue), (yyvsp[0].intValue), (yylsp[-2]).first_line)), ClosureFeature, 0); setStatementLocation((yyvsp[-1].functionBodyNode), (yylsp[-2]), (yylsp[0])); }
    break;

  case 290:
      {
          (yyval.funcExprNode) = createNodeInfo(new (GLOBAL_DATA) FuncExprNode(GLOBAL_DATA, GLOBAL_DATA->propertyNames->nullIdentifier, (yyvsp[-1].functionBodyNode), GLOBAL_DATA->lexer->sourceCode((yyvsp[-2].intValue), (yyvsp[0].intValue), (yylsp[-2]).first_line), (yyvsp[-4].parameterList).m_node.head), (yyvsp[-4].parameterList).m_features | ClosureFeature, 0);
          if ((yyvsp[-4].parameterList).m_features & ArgumentsFeature)
              (yyvsp[-1].functionBodyNode)->setUsesArguments();
          setStatementLocation((yyvsp[-1].functionBodyNode), (yylsp[-2]), (yylsp[0]));
      }
    break;

  case 291:
                                                             { (yyval.funcExprNode) = createNodeInfo(new (GLOBAL_DATA) FuncExprNode(GLOBAL_DATA, *(yyvsp[-5].ident), (yyvsp[-1].functionBodyNode), GLOBAL_DATA->lexer->sourceCode((yyvsp[-2].intValue), (yyvsp[0].intValue), (yylsp[-2]).first_line)), ClosureFeature, 0); setStatementLocation((yyvsp[-1].functionBodyNode), (yylsp[-2]), (yylsp[0])); }
    break;

  case 292:
      {
          (yyval.funcExprNode) = createNodeInfo(new (GLOBAL_DATA) FuncExprNode(GLOBAL_DATA, *(yyvsp[-6].ident), (yyvsp[-1].functionBodyNode), GLOBAL_DATA->lexer->sourceCode((yyvsp[-2].intValue), (yyvsp[0].intValue), (yylsp[-2]).first_line), (yyvsp[-4].parameterList).m_node.head), (yyvsp[-4].parameterList).m_features | ClosureFeature, 0); 
          if ((yyvsp[-4].parameterList).m_features & ArgumentsFeature)
              (yyvsp[-1].functionBodyNode)->setUsesArguments();
          setStatementLocation((yyvsp[-1].functionBodyNode), (yylsp[-2]), (yylsp[0]));
      }
    break;

  case 293:
                                        { (yyval.parameterList).m_node.head = new (GLOBAL_DATA) ParameterNode(GLOBAL_DATA, *(yyvsp[0].ident));
                                          (yyval.parameterList).m_features = (*(yyvsp[0].ident) == GLOBAL_DATA->propertyNames->arguments) ? ArgumentsFeature : 0;
                                          (yyval.parameterList).m_node.tail = (yyval.parameterList).m_node.head; }
    break;

  case 294:
                                        { (yyval.parameterList).m_node.head = (yyvsp[-2].parameterList).m_node.head;
                                          (yyval.parameterList).m_features = (yyvsp[-2].parameterList).m_features | ((*(yyvsp[0].ident) == GLOBAL_DATA->propertyNames->arguments) ? ArgumentsFeature : 0);
                                          (yyval.parameterList).m_node.tail = new (GLOBAL_DATA) ParameterNode(GLOBAL_DATA, (yyvsp[-2].parameterList).m_node.tail, *(yyvsp[0].ident));  }
    break;

  case 295:
                                        { (yyval.functionBodyNode) = FunctionBodyNode::create(GLOBAL_DATA); }
    break;

  case 296:
                                        { (yyval.functionBodyNode) = FunctionBodyNode::create(GLOBAL_DATA); }
    break;

  case 297:
                                        { GLOBAL_DATA->parser->didFinishParsing(new (GLOBAL_DATA) SourceElements(GLOBAL_DATA), 0, 0, NoFeatures, (yylsp[0]).last_line, 0); }
    break;

  case 298:
                                        { GLOBAL_DATA->parser->didFinishParsing((yyvsp[0].sourceElements).m_node, (yyvsp[0].sourceElements).m_varDeclarations, (yyvsp[0].sourceElements).m_funcDeclarations, (yyvsp[0].sourceElements).m_features, 
                                                                                (yylsp[0]).last_line, (yyvsp[0].sourceElements).m_numConstants); }
    break;

  case 299:
                                        { (yyval.sourceElements).m_node = new (GLOBAL_DATA) SourceElements(GLOBAL_DATA);
                                          (yyval.sourceElements).m_node->append((yyvsp[0].statementNode).m_node);
                                          (yyval.sourceElements).m_varDeclarations = (yyvsp[0].statementNode).m_varDeclarations;
                                          (yyval.sourceElements).m_funcDeclarations = (yyvsp[0].statementNode).m_funcDeclarations;
                                          (yyval.sourceElements).m_features = (yyvsp[0].statementNode).m_features;
                                          (yyval.sourceElements).m_numConstants = (yyvsp[0].statementNode).m_numConstants;
                                        }
    break;

  case 300:
                                        { (yyval.sourceElements).m_node->append((yyvsp[0].statementNode).m_node);
                                          (yyval.sourceElements).m_varDeclarations = mergeDeclarationLists((yyvsp[-1].sourceElements).m_varDeclarations, (yyvsp[0].statementNode).m_varDeclarations);
                                          (yyval.sourceElements).m_funcDeclarations = mergeDeclarationLists((yyvsp[-1].sourceElements).m_funcDeclarations, (yyvsp[0].statementNode).m_funcDeclarations);
                                          (yyval.sourceElements).m_features = (yyvsp[-1].sourceElements).m_features | (yyvsp[0].statementNode).m_features;
                                          (yyval.sourceElements).m_numConstants = (yyvsp[-1].sourceElements).m_numConstants + (yyvsp[0].statementNode).m_numConstants;
                                        }
    break;

  case 304:
           { }
    break;

  case 305:
           { }
    break;

  case 306:
                     { if (!GLOBAL_DATA->lexer->skipRegExp()) YYABORT; }
    break;

  case 307:
                                  { if (!GLOBAL_DATA->lexer->skipRegExp()) YYABORT; }
    break;

  case 308:
                                    { }
    break;

  case 309:
                                     { }
    break;

  case 310:
                                     { }
    break;

  case 311:
                                                                 { if (*(yyvsp[-6].ident) != "get" && *(yyvsp[-6].ident) != "set") YYABORT; }
    break;

  case 312:
                                                                                            { if (*(yyvsp[-7].ident) != "get" && *(yyvsp[-7].ident) != "set") YYABORT; }
    break;

  case 316:
                         { }
    break;

  case 317:
                                             { }
    break;

  case 318:
                                                 { }
    break;

  case 322:
          { }
    break;

  case 517:
                         { }
    break;

  case 518:
                                               { }
    break;

  case 520:
                                             { AUTO_SEMICOLON; }
    break;

  case 521:
          { }
    break;

  case 522:
                             { }
    break;

  case 525:
          { }
    break;

  case 526:
                                 { }
    break;

  case 530:
                                                 { AUTO_SEMICOLON; }
    break;

  case 533:
          { }
    break;

  case 534:
                             { }
    break;

  case 539:
                          { AUTO_SEMICOLON; }
    break;

  case 555:
                   { AUTO_SEMICOLON; }
    break;

  case 557:
                         { AUTO_SEMICOLON; }
    break;

  case 559:
                { AUTO_SEMICOLON; }
    break;

  case 561:
                      { AUTO_SEMICOLON; }
    break;

  case 563:
                 { AUTO_SEMICOLON; }
    break;

  case 565:
                             { AUTO_SEMICOLON; }
    break;

  case 568:
                                               { }
    break;

  case 569:
                                                                                          { }
    break;

  case 578:
                               { }
    break;

  case 580:
                            { AUTO_SEMICOLON; }
    break;

  case 585:
                   { AUTO_SEMICOLON; }
    break;

  case 592:
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
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

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
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (&yylloc, globalPtr, YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = YY_CAST (char *, YYSTACK_ALLOC (YY_CAST (YYSIZE_T, yymsg_alloc)));
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (&yylloc, globalPtr, yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
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

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYTERROR;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
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
                  yystos[yystate], yyvsp, yylsp, globalPtr);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  yyerror_range[2] = yylloc;
  /* Using YYLLOC is tempting, but would change the location of
     the lookahead.  YYLOC is available though.  */
  YYLLOC_DEFAULT (yyloc, yyerror_range, 2);
  *++yylsp = yyloc;

  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

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


#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (&yylloc, globalPtr, YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif


/*-----------------------------------------------------.
| yyreturn -- parsing is finished, return the result.  |
`-----------------------------------------------------*/
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
                  yystos[+*yyssp], yyvsp, yylsp, globalPtr);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
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
