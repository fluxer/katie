/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
**
** This file is part of the QtCore module of the Katie Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
**
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Katie API.  It exists for the convenience
// of other Qt classes.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

// This file was generated by qlalr - DO NOT EDIT!
#ifndef QSCRIPTGRAMMAR_P_H
#define QSCRIPTGRAMMAR_P_H

#include <QtCore/qglobal.h>

QT_BEGIN_NAMESPACE

static const char *const QScriptGrammar_spell [] = {
  "end of file", "&", "&&", "&=", "break", "case", "catch", ":", ";", "continue", 
  "default", "delete", "/", "/=", "do", ".", "else", "=", "==", "===", 
  "finally", "for", "function", ">=", ">", ">>", ">>=", ">>>", ">>>=", "identifier", 
  "if", "in", "instanceof", "{", "[", "<=", "(", "<", "<<", "<<=", 
  "-", "-=", "--", "new", "!", "!=", "!==", "numeric literal", "|", "|=", 
  "||", "+", "+=", "++", "?", "}", "]", "%", "%=", "return", 
  ")", ";", 0, "*", "*=", "string literal", "switch", "this", "throw", "~", 
  "try", "typeof", "var", "void", "while", "with", "^", "^=", "null", "true", 
  "false", "const", "debugger", "reserved word", 
#ifndef QLALR_NO_QSCRIPTGRAMMAR_DEBUG_INFO
"Program", "PrimaryExpression", "ElisionOpt", "ElementList", "PropertyNameAndValueListOpt", "PropertyNameAndValueList", 
  "Expression", "AssignmentExpression", "Elision", "PropertyName", "ReservedIdentifier", "PropertyIdentifier", "MemberExpression", "FunctionExpression", "Arguments", "NewExpression", 
  "CallExpression", "ArgumentList", "LeftHandSideExpression", "PostfixExpression", "UnaryExpression", "MultiplicativeExpression", "AdditiveExpression", "ShiftExpression", "RelationalExpression", "RelationalExpressionNotIn", 
  "EqualityExpression", "EqualityExpressionNotIn", "BitwiseANDExpression", "BitwiseANDExpressionNotIn", "BitwiseXORExpression", "BitwiseXORExpressionNotIn", "BitwiseORExpression", "BitwiseORExpressionNotIn", "LogicalANDExpression", "LogicalANDExpressionNotIn", 
  "LogicalORExpression", "LogicalORExpressionNotIn", "ConditionalExpression", "ConditionalExpressionNotIn", "AssignmentExpressionNotIn", "AssignmentOperator", "ExpressionOpt", "ExpressionNotIn", "ExpressionNotInOpt", "Statement", 
  "Block", "VariableStatement", "EmptyStatement", "ExpressionStatement", "IfStatement", "IterationStatement", "ContinueStatement", "BreakStatement", "ReturnStatement", "WithStatement", 
  "LabelledStatement", "SwitchStatement", "ThrowStatement", "TryStatement", "DebuggerStatement", "StatementListOpt", "StatementList", "VariableDeclarationKind", "VariableDeclarationList", "VariableDeclaration", 
  "VariableDeclarationListNotIn", "VariableDeclarationNotIn", "InitialiserOpt", "InitialiserNotInOpt", "Initialiser", "InitialiserNotIn", "CaseBlock", "CaseClausesOpt", "DefaultClause", "CaseClauses", 
  "CaseClause", "Catch", "Finally", "FunctionDeclaration", "FormalParameterListOpt", "FunctionBodyOpt", "IdentifierOpt", "FormalParameterList", "FunctionBody", "SourceElements", 
  "SourceElement", "$accept"
#endif // QLALR_NO_QSCRIPTGRAMMAR_DEBUG_INFO
};

static const short QScriptGrammar_lhs [] = {
  85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 
  85, 85, 85, 85, 85, 87, 87, 92, 92, 86, 
  86, 89, 89, 93, 93, 93, 93, 94, 94, 94, 
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94, 
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94, 
  94, 94, 94, 94, 94, 94, 94, 94, 95, 95, 
  96, 96, 96, 96, 96, 99, 99, 100, 100, 100, 
  100, 98, 98, 101, 101, 102, 102, 103, 103, 103, 
  104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 
  105, 105, 105, 105, 106, 106, 106, 107, 107, 107, 
  107, 108, 108, 108, 108, 108, 108, 108, 109, 109, 
  109, 109, 109, 109, 110, 110, 110, 110, 110, 111, 
  111, 111, 111, 111, 112, 112, 113, 113, 114, 114, 
  115, 115, 116, 116, 117, 117, 118, 118, 119, 119, 
  120, 120, 121, 121, 122, 122, 123, 123, 91, 91, 
  124, 124, 125, 125, 125, 125, 125, 125, 125, 125, 
  125, 125, 125, 125, 90, 90, 126, 126, 127, 127, 
  128, 128, 129, 129, 129, 129, 129, 129, 129, 129, 
  129, 129, 129, 129, 129, 129, 129, 130, 146, 146, 
  145, 145, 131, 131, 147, 147, 148, 148, 150, 150, 
  149, 151, 154, 152, 152, 155, 153, 153, 132, 133, 
  133, 134, 134, 135, 135, 135, 135, 135, 135, 135, 
  136, 136, 136, 136, 137, 137, 137, 137, 138, 138, 
  139, 141, 156, 156, 159, 159, 157, 157, 160, 158, 
  140, 142, 142, 143, 143, 143, 161, 162, 144, 144, 
  163, 97, 167, 167, 164, 164, 165, 165, 168, 84, 
  169, 169, 170, 170, 166, 166, 88, 88, 171};

static const short QScriptGrammar_rhs [] = {
  1, 1, 1, 1, 1, 1, 1, 1, 1, 3, 
  3, 5, 3, 4, 3, 2, 4, 1, 2, 0, 
  1, 3, 5, 1, 1, 1, 1, 1, 1, 1, 
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
  1, 1, 4, 3, 3, 1, 2, 2, 2, 4, 
  3, 2, 3, 1, 3, 1, 1, 1, 2, 2, 
  1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 
  1, 3, 3, 3, 1, 3, 3, 1, 3, 3, 
  3, 1, 3, 3, 3, 3, 3, 3, 1, 3, 
  3, 3, 3, 3, 1, 3, 3, 3, 3, 1, 
  3, 3, 3, 3, 1, 3, 1, 3, 1, 3, 
  1, 3, 1, 3, 1, 3, 1, 3, 1, 3, 
  1, 3, 1, 3, 1, 5, 1, 5, 1, 3, 
  1, 3, 1, 1, 1, 1, 1, 1, 1, 1, 
  1, 1, 1, 1, 1, 3, 0, 1, 1, 3, 
  0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
  1, 1, 1, 1, 1, 1, 1, 3, 1, 2, 
  0, 1, 3, 3, 1, 1, 1, 3, 1, 3, 
  2, 2, 2, 0, 1, 2, 0, 1, 1, 2, 
  2, 7, 5, 7, 7, 5, 9, 10, 7, 8, 
  2, 2, 3, 3, 2, 2, 3, 3, 3, 3, 
  5, 5, 3, 5, 1, 2, 0, 1, 4, 3, 
  3, 3, 3, 3, 3, 4, 5, 2, 2, 2, 
  8, 8, 1, 3, 0, 1, 0, 1, 1, 1, 
  1, 2, 1, 1, 0, 1, 0, 1, 2};


#ifndef QLALR_NO_QSCRIPTGRAMMAR_DEBUG_INFO
static const int QScriptGrammar_rule_info [] = {
    85, 67
  , 85, 29
  , 85, 78
  , 85, 79
  , 85, 80
  , 85, 47
  , 85, 65
  , 85, 12
  , 85, 13
  , 85, 34, 86, 56
  , 85, 34, 87, 56
  , 85, 34, 87, 8, 86, 56
  , 85, 33, 88, 55
  , 85, 33, 89, 8, 55
  , 85, 36, 90, 60
  , 87, 86, 91
  , 87, 87, 8, 86, 91
  , 92, 8
  , 92, 92, 8
  , 86
  , 86, 92
  , 89, 93, 7, 91
  , 89, 89, 8, 93, 7, 91
  , 93, 29
  , 93, 65
  , 93, 47
  , 93, 94
  , 94, 4
  , 94, 5
  , 94, 6
  , 94, 9
  , 94, 10
  , 94, 11
  , 94, 14
  , 94, 16
  , 94, 80
  , 94, 20
  , 94, 21
  , 94, 22
  , 94, 30
  , 94, 31
  , 94, 32
  , 94, 43
  , 94, 78
  , 94, 59
  , 94, 66
  , 94, 67
  , 94, 68
  , 94, 79
  , 94, 70
  , 94, 71
  , 94, 72
  , 94, 73
  , 94, 74
  , 94, 81
  , 94, 82
  , 94, 83
  , 94, 75
  , 95, 29
  , 95, 94
  , 96, 85
  , 96, 97
  , 96, 96, 34, 90, 56
  , 96, 96, 15, 95
  , 96, 43, 96, 98
  , 99, 96
  , 99, 43, 99
  , 100, 96, 98
  , 100, 100, 98
  , 100, 100, 34, 90, 56
  , 100, 100, 15, 95
  , 98, 36, 60
  , 98, 36, 101, 60
  , 101, 91
  , 101, 101, 8, 91
  , 102, 99
  , 102, 100
  , 103, 102
  , 103, 102, 53
  , 103, 102, 42
  , 104, 103
  , 104, 11, 104
  , 104, 73, 104
  , 104, 71, 104
  , 104, 53, 104
  , 104, 42, 104
  , 104, 51, 104
  , 104, 40, 104
  , 104, 69, 104
  , 104, 44, 104
  , 105, 104
  , 105, 105, 63, 104
  , 105, 105, 12, 104
  , 105, 105, 57, 104
  , 106, 105
  , 106, 106, 51, 105
  , 106, 106, 40, 105
  , 107, 106
  , 107, 107, 38, 106
  , 107, 107, 25, 106
  , 107, 107, 27, 106
  , 108, 107
  , 108, 108, 37, 107
  , 108, 108, 24, 107
  , 108, 108, 35, 107
  , 108, 108, 23, 107
  , 108, 108, 32, 107
  , 108, 108, 31, 107
  , 109, 107
  , 109, 109, 37, 107
  , 109, 109, 24, 107
  , 109, 109, 35, 107
  , 109, 109, 23, 107
  , 109, 109, 32, 107
  , 110, 108
  , 110, 110, 18, 108
  , 110, 110, 45, 108
  , 110, 110, 19, 108
  , 110, 110, 46, 108
  , 111, 109
  , 111, 111, 18, 109
  , 111, 111, 45, 109
  , 111, 111, 19, 109
  , 111, 111, 46, 109
  , 112, 110
  , 112, 112, 1, 110
  , 113, 111
  , 113, 113, 1, 111
  , 114, 112
  , 114, 114, 76, 112
  , 115, 113
  , 115, 115, 76, 113
  , 116, 114
  , 116, 116, 48, 114
  , 117, 115
  , 117, 117, 48, 115
  , 118, 116
  , 118, 118, 2, 116
  , 119, 117
  , 119, 119, 2, 117
  , 120, 118
  , 120, 120, 50, 118
  , 121, 119
  , 121, 121, 50, 119
  , 122, 120
  , 122, 120, 54, 91, 7, 91
  , 123, 121
  , 123, 121, 54, 124, 7, 124
  , 91, 122
  , 91, 102, 125, 91
  , 124, 123
  , 124, 102, 125, 124
  , 125, 17
  , 125, 64
  , 125, 13
  , 125, 58
  , 125, 52
  , 125, 41
  , 125, 39
  , 125, 26
  , 125, 28
  , 125, 3
  , 125, 77
  , 125, 49
  , 90, 91
  , 90, 90, 8, 91
  , 126
  , 126, 90
  , 127, 124
  , 127, 127, 8, 124
  , 128
  , 128, 127
  , 129, 130
  , 129, 131
  , 129, 132
  , 129, 133
  , 129, 134
  , 129, 135
  , 129, 136
  , 129, 137
  , 129, 138
  , 129, 139
  , 129, 140
  , 129, 141
  , 129, 142
  , 129, 143
  , 129, 144
  , 130, 33, 145, 55
  , 146, 129
  , 146, 146, 129
  , 145
  , 145, 146
  , 131, 147, 148, 62
  , 131, 147, 148, 61
  , 147, 81
  , 147, 72
  , 148, 149
  , 148, 148, 8, 149
  , 150, 151
  , 150, 150, 8, 151
  , 149, 29, 152
  , 151, 29, 153
  , 154, 17, 91
  , 152
  , 152, 154
  , 155, 17, 124
  , 153
  , 153, 155
  , 132, 61
  , 133, 90, 62
  , 133, 90, 61
  , 134, 30, 36, 90, 60, 129, 16, 129
  , 134, 30, 36, 90, 60, 129
  , 135, 14, 129, 74, 36, 90, 60, 62
  , 135, 14, 129, 74, 36, 90, 60, 61
  , 135, 74, 36, 90, 60, 129
  , 135, 21, 36, 128, 61, 126, 61, 126, 60, 129
  , 135, 21, 36, 72, 150, 61, 126, 61, 126, 60, 129
  , 135, 21, 36, 102, 31, 90, 60, 129
  , 135, 21, 36, 72, 151, 31, 90, 60, 129
  , 136, 9, 62
  , 136, 9, 61
  , 136, 9, 29, 62
  , 136, 9, 29, 61
  , 137, 4, 62
  , 137, 4, 61
  , 137, 4, 29, 62
  , 137, 4, 29, 61
  , 138, 59, 126, 62
  , 138, 59, 126, 61
  , 139, 75, 36, 90, 60, 129
  , 141, 66, 36, 90, 60, 156
  , 156, 33, 157, 55
  , 156, 33, 157, 158, 157, 55
  , 159, 160
  , 159, 159, 160
  , 157
  , 157, 159
  , 160, 5, 90, 7, 145
  , 158, 10, 7, 145
  , 140, 29, 7, 129
  , 142, 68, 90, 62
  , 142, 68, 90, 61
  , 143, 70, 130, 161
  , 143, 70, 130, 162
  , 143, 70, 130, 161, 162
  , 161, 6, 36, 29, 60, 130
  , 162, 20, 130
  , 144, 82, 62
  , 144, 82, 61
  , 163, 22, 29, 36, 164, 60, 33, 165, 55
  , 97, 22, 166, 36, 164, 60, 33, 165, 55
  , 167, 29
  , 167, 167, 8, 29
  , 164
  , 164, 167
  , 165
  , 165, 168
  , 168, 169
  , 84, 169
  , 169, 170
  , 169, 169, 170
  , 170, 129
  , 170, 163
  , 166
  , 166, 29
  , 88
  , 88, 89
  , 171, 84, 0};

static const int QScriptGrammar_rule_index [] = {
  0, 2, 4, 6, 8, 10, 12, 14, 16, 18, 
  22, 26, 32, 36, 41, 45, 48, 53, 55, 58, 
  59, 61, 65, 71, 73, 75, 77, 79, 81, 83, 
  85, 87, 89, 91, 93, 95, 97, 99, 101, 103, 
  105, 107, 109, 111, 113, 115, 117, 119, 121, 123, 
  125, 127, 129, 131, 133, 135, 137, 139, 141, 143, 
  145, 147, 149, 154, 158, 162, 164, 167, 170, 173, 
  178, 182, 185, 189, 191, 195, 197, 199, 201, 204, 
  207, 209, 212, 215, 218, 221, 224, 227, 230, 233, 
  236, 238, 242, 246, 250, 252, 256, 260, 262, 266, 
  270, 274, 276, 280, 284, 288, 292, 296, 300, 302, 
  306, 310, 314, 318, 322, 324, 328, 332, 336, 340, 
  342, 346, 350, 354, 358, 360, 364, 366, 370, 372, 
  376, 378, 382, 384, 388, 390, 394, 396, 400, 402, 
  406, 408, 412, 414, 418, 420, 426, 428, 434, 436, 
  440, 442, 446, 448, 450, 452, 454, 456, 458, 460, 
  462, 464, 466, 468, 470, 472, 476, 477, 479, 481, 
  485, 486, 488, 490, 492, 494, 496, 498, 500, 502, 
  504, 506, 508, 510, 512, 514, 516, 518, 522, 524, 
  527, 528, 530, 534, 538, 540, 542, 544, 548, 550, 
  554, 557, 560, 563, 564, 566, 569, 570, 572, 574, 
  577, 580, 588, 594, 602, 610, 616, 626, 637, 645, 
  654, 657, 660, 664, 668, 671, 674, 678, 682, 686, 
  690, 696, 702, 706, 712, 714, 717, 718, 720, 725, 
  729, 733, 737, 741, 745, 749, 754, 760, 763, 766, 
  769, 778, 787, 789, 793, 794, 796, 797, 799, 801, 
  803, 805, 808, 810, 812, 813, 815, 816, 818};
#endif // QLALR_NO_QSCRIPTGRAMMAR_DEBUG_INFO

static const short QScriptGrammar_action_default [] = {
  0, 98, 165, 129, 137, 133, 173, 180, 77, 149, 
  179, 187, 175, 125, 0, 176, 264, 62, 177, 178, 
  183, 78, 141, 145, 66, 95, 76, 81, 61, 0, 
  115, 181, 102, 261, 260, 263, 184, 0, 195, 0, 
  0, 0, 8, 9, 0, 5, 0, 265, 2, 0, 
  267, 20, 0, 0, 0, 0, 0, 3, 6, 0, 
  0, 167, 209, 7, 0, 1, 0, 0, 4, 0, 
  0, 196, 0, 0, 0, 185, 186, 91, 0, 174, 
  182, 0, 0, 78, 97, 265, 2, 267, 80, 79, 
  0, 0, 0, 93, 94, 92, 0, 266, 255, 256, 
  0, 253, 0, 254, 0, 257, 258, 0, 259, 252, 
  262, 0, 268, 0, 27, 28, 29, 30, 55, 31, 
  56, 32, 33, 34, 35, 36, 37, 38, 39, 24, 
  40, 41, 42, 43, 44, 26, 57, 45, 25, 46, 
  47, 48, 49, 50, 51, 52, 53, 54, 58, 0, 
  22, 0, 0, 14, 0, 23, 13, 96, 0, 126, 
  0, 0, 0, 0, 116, 0, 0, 0, 0, 0, 
  0, 106, 0, 0, 0, 100, 101, 99, 104, 108, 
  107, 105, 103, 118, 117, 119, 0, 134, 0, 130, 
  69, 0, 0, 0, 71, 60, 59, 0, 0, 70, 
  166, 0, 74, 72, 0, 73, 75, 210, 211, 0, 
  162, 155, 153, 160, 161, 159, 158, 164, 157, 156, 
  154, 163, 150, 0, 138, 0, 0, 142, 0, 0, 
  146, 68, 0, 0, 64, 0, 63, 269, 225, 0, 
  226, 227, 228, 221, 0, 222, 223, 224, 249, 250, 
  82, 0, 0, 0, 0, 0, 214, 215, 171, 169, 
  131, 139, 135, 151, 127, 172, 0, 78, 143, 147, 
  120, 109, 0, 0, 128, 0, 0, 0, 0, 121, 
  0, 0, 0, 0, 0, 113, 111, 114, 112, 110, 
  123, 122, 124, 0, 136, 0, 132, 0, 170, 78, 
  0, 152, 167, 168, 0, 167, 0, 0, 217, 0, 
  0, 0, 219, 0, 140, 0, 0, 144, 0, 0, 
  148, 207, 0, 199, 208, 202, 0, 206, 0, 167, 
  200, 0, 167, 0, 0, 218, 0, 0, 0, 220, 
  266, 255, 0, 0, 257, 0, 251, 0, 241, 0, 
  0, 0, 213, 0, 212, 189, 192, 0, 28, 55, 
  31, 56, 33, 34, 5, 38, 39, 2, 40, 43, 
  3, 6, 167, 7, 46, 1, 48, 4, 50, 51, 
  52, 53, 54, 58, 190, 188, 66, 67, 65, 0, 
  229, 230, 0, 0, 0, 232, 237, 235, 238, 0, 
  0, 236, 237, 0, 233, 0, 234, 191, 240, 0, 
  191, 239, 0, 242, 243, 0, 191, 244, 245, 0, 
  0, 246, 0, 0, 0, 247, 248, 84, 83, 0, 
  0, 0, 216, 0, 0, 0, 231, 0, 21, 0, 
  18, 20, 11, 0, 17, 12, 19, 16, 10, 0, 
  15, 88, 86, 90, 87, 85, 89, 204, 197, 0, 
  205, 201, 0, 203, 193, 0, 194, 198};

static const short QScriptGrammar_goto_default [] = {
  29, 28, 439, 437, 113, 112, 14, 2, 438, 111, 
  114, 194, 24, 17, 190, 26, 8, 201, 21, 27, 
  77, 25, 1, 32, 30, 270, 13, 264, 3, 260, 
  5, 262, 4, 261, 22, 268, 23, 269, 9, 263, 
  259, 300, 389, 265, 266, 35, 6, 79, 12, 15, 
  18, 19, 10, 7, 31, 80, 20, 36, 75, 76, 
  11, 357, 356, 78, 459, 458, 322, 323, 461, 325, 
  460, 324, 395, 399, 402, 398, 397, 417, 418, 16, 
  100, 107, 96, 99, 106, 108, 33, 0};

static const short QScriptGrammar_action_index [] = {
  1257, 50, -84, 40, 17, -16, -84, -84, 186, -84, 
  -84, -84, -84, 200, 145, -84, -84, -84, -84, -84, 
  -84, 271, 69, 61, 134, 120, -84, -84, -84, 62, 
  333, -84, 211, -84, 1257, -84, -84, 129, -84, 153, 
  45, 568, -84, -84, 1337, -84, 1, -1, 70, -11, 
  1417, 12, 568, 568, 568, 304, 568, -84, -84, 568, 
  568, 568, -84, -84, -30, -84, 568, 568, -84, -24, 
  568, -84, 568, -22, -20, -84, -84, -84, 43, -84, 
  -84, 568, 568, 78, 123, 52, -84, 1097, -84, -84, 
  568, 568, 568, -84, -84, -84, 15, -84, 21, 44, 
  18, -84, 27, -84, 25, 1257, -84, -2, 1257, -84, 
  -84, 80, 59, 29, -84, -84, -84, -84, -84, -84, 
  -84, -84, -84, -84, -84, -84, -84, -84, -84, -84, 
  -84, -84, -84, -84, -84, -84, -84, -84, -84, -84, 
  -84, -84, -84, -84, -84, -84, -84, -84, -84, 568, 
  -84, 1177, 38, -84, 568, -84, -84, 132, 568, 198, 
  568, 568, 568, 568, 254, 568, 568, 568, 568, 568, 
  568, 211, 568, 568, 568, 82, 74, 89, 211, 147, 
  211, 146, 211, 234, 244, 224, 568, -13, 568, 60, 
  -84, 1017, 568, 937, -84, -84, -84, 113, 568, -84, 
  -84, 115, -84, -84, 568, -84, -84, -84, -84, 568, 
  -84, -84, -84, -84, -84, -84, -84, -84, -84, -84, 
  -84, -84, -84, 568, 34, 568, 568, 152, 66, 568, 
  -84, -84, 1017, 568, -84, 111, -84, -84, -84, 77, 
  -84, -84, -84, -84, 56, -84, -84, -84, -84, -84, 
  -84, -69, -32, 568, 116, 94, -84, -84, 641, -84, 
  7, -4, -37, -84, 194, 30, -26, 495, 31, 76, 
  272, 211, 5, 568, 185, 568, 568, 568, 568, 272, 
  568, 568, 568, 568, 568, 125, 211, 211, 211, 211, 
  272, 272, 272, 568, -21, 568, 65, 568, -84, 414, 
  568, -84, 568, 10, 22, 568, 26, 1337, -84, 568, 
  86, 1337, -84, 568, 37, 568, 568, 73, 67, 568, 
  -84, -15, 84, -12, -84, -84, 568, -84, -6, 568, 
  -84, -40, 568, -38, 1337, -84, 568, 87, 1337, -84, 
  -10, -5, -31, -3, 1257, -28, -84, 1337, -84, 568, 
  88, 1337, 16, 1337, -84, -84, 1337, -19, 137, 11, 
  149, 36, 568, 1337, 24, -35, 64, 70, -33, 365, 
  0, -7, 864, 3, -25, 6, 568, 8, 13, 568, 
  41, 568, 32, 33, -84, -84, 166, -84, -84, 42, 
  -84, -84, 568, 91, 47, -84, 71, -84, 83, 81, 
  568, -84, 49, 35, -84, 2, -84, 1337, -84, 101, 
  1337, -84, 126, -84, -84, 96, 1337, 39, -84, 28, 
  14, -84, 20, -17, 46, -84, -84, -84, -84, 568, 
  97, 1337, -84, 568, 102, 1337, -84, 104, 9, 787, 
  -84, 4, -84, 714, -84, -84, -84, -84, -84, 105, 
  -84, -84, -84, -84, -84, -84, -84, 72, -84, 135, 
  -84, -84, 568, -84, -84, 19, -84, -84, 

  -60, -88, -88, -88, -88, -88, -88, -88, -88, -88, 
  -88, -88, -88, -88, -88, -88, -88, -88, -88, -88, 
  -88, 3, -88, -88, 47, -88, -88, -88, -88, -88, 
  -88, -88, -88, -88, -69, -88, -88, -88, -88, -88, 
  -88, 127, -88, -88, -27, -88, -88, -88, -88, -88, 
  -30, -88, 14, 130, 131, 140, 136, -88, -88, 141, 
  142, 15, -88, -88, -88, -88, -2, 53, -88, -37, 
  114, -88, 146, -88, -88, -88, -88, -88, -88, -88, 
  -88, 84, 86, -88, -88, -88, -88, -88, -88, -88, 
  90, 94, 95, -88, -88, -88, -88, -88, -88, -88, 
  -88, -88, -88, -88, -88, -88, -88, -88, -55, -88, 
  -88, -88, -88, -88, -88, -88, -88, -88, -88, -88, 
  -88, -88, -88, -88, -88, -88, -88, -88, -88, -88, 
  -88, -88, -88, -88, -88, -88, -88, -88, -88, -88, 
  -88, -88, -88, -88, -88, -88, -88, -88, -88, 43, 
  -88, 38, -88, -88, -4, -88, -88, -88, 139, -88, 
  122, 117, 109, 118, -88, 120, 98, 51, 54, 57, 
  60, -88, 69, 74, 72, -88, -88, -88, -88, -88, 
  -88, -88, -88, -88, -88, -88, 58, -88, 61, -88, 
  -88, 44, 42, -1, -88, -88, -88, -88, 21, -88, 
  -88, -88, -88, -88, 30, -88, -88, -88, -88, 50, 
  -88, -88, -88, -88, -88, -88, -88, -88, -88, -88, 
  -88, -88, -88, 63, -88, 52, 16, -88, -88, 6, 
  -88, -88, 48, 76, -88, -88, -88, -88, -88, -88, 
  -88, -88, -88, -88, -88, -88, -88, -88, -88, -88, 
  -88, -88, -88, 45, -88, -88, -88, -88, 88, -88, 
  -88, -88, -88, -88, -88, -88, -88, -88, -88, -88, 
  -88, -88, -88, 192, -88, 188, 184, 178, 168, -88, 
  99, 100, 101, 107, 108, -88, -88, -88, -88, -88, 
  -88, -88, -88, 145, -88, 152, -88, 154, -88, -88, 
  155, -88, 97, -88, -88, 87, -88, 10, -88, 35, 
  -88, -5, -88, 156, -88, 162, 164, -88, -88, 165, 
  -88, -88, -88, -88, -88, -88, 198, -88, -38, 78, 
  -88, -88, 56, -88, 4, -88, 29, -88, -15, -88, 
  -88, -48, -88, -88, -47, -88, -88, -9, -88, 32, 
  -88, -3, -88, 0, -88, -88, 1, -88, -88, -88, 
  -88, -88, 81, 7, -88, -88, -88, -88, -88, 85, 
  -88, -88, 33, -88, -88, -88, 2, -88, -39, 149, 
  -88, 216, -88, -88, -88, -88, -13, -88, -88, -88, 
  -88, -88, 20, -88, -88, -88, -88, -88, -64, -88, 
  13, -88, -57, -88, -88, -88, -88, 83, -88, -88, 
  92, -88, -88, -88, -88, -88, -34, -56, -88, -88, 
  -19, -88, -88, -88, -32, -88, -88, -88, -88, 18, 
  -88, -40, -88, -6, -88, -35, -88, -88, -88, 49, 
  -88, 41, -88, 26, -88, -88, -88, -88, -88, -88, 
  -88, -88, -88, -88, -88, -88, -88, -88, -88, -88, 
  -88, -88, 46, -88, -88, -63, -88, -88};

static const short QScriptGrammar_action_info [] = {
  -26, 258, 326, 349, 253, 252, 392, -44, 273, 416, 
  -25, 392, 440, -47, 429, -49, 433, 446, 198, 336, 
  440, 332, 334, 321, 101, 349, 341, 346, 340, 343, 
  344, -36, 353, 313, 321, 302, 385, 258, 297, 295, 
  -195, 158, 407, 424, 293, 154, 416, 416, 457, 423, 
  101, 98, 102, 109, 400, 295, 103, 406, 105, 420, 
  188, 158, 237, 188, 422, 186, 273, 151, 429, 433, 
  -196, 223, 457, 229, 319, 313, 400, 347, 104, 416, 
  396, 97, 186, 305, 156, 293, 307, 149, 400, 462, 
  81, 403, 328, 97, 198, 198, 198, 249, 248, 198, 
  -265, 82, 419, 391, 390, 198, 249, 248, 410, 198, 
  198, 225, 441, 198, 81, 226, 420, 247, 246, 198, 
  88, 198, 81, 204, 198, 82, 315, 0, 0, 81, 
  316, 89, 90, 82, 198, 90, 404, 0, 242, 241, 
  82, 0, 0, 465, 90, 329, 311, 338, 351, 232, 
  172, 394, 173, 198, 223, 257, 256, 431, 239, 0, 
  442, 0, 435, 174, 0, 450, 239, 236, 233, 199, 
  193, 172, 172, 173, 173, 205, 255, 91, 244, 0, 
  91, 232, 244, 92, 174, 174, 92, 414, 413, 91, 
  240, 238, 0, 0, 0, 92, 466, 464, 240, 238, 
  233, 191, 193, 275, 276, 0, 208, 207, 0, 0, 
  245, 243, 275, 276, 245, 243, 160, 161, 160, 161, 
  192, 0, 193, 0, 0, 0, 0, 0, 0, 0, 
  277, 278, 0, 0, 0, 0, 172, 0, 173, 277, 
  278, 0, 0, 162, 163, 162, 163, 165, 166, 174, 
  0, 0, 0, 0, 0, 167, 168, 165, 166, 169, 
  0, 170, 0, 0, 0, 167, 168, 165, 166, 169, 
  0, 170, 0, 0, 210, 167, 168, 165, 166, 169, 
  0, 170, 0, 0, 211, 167, 168, 0, 212, 169, 
  0, 170, 0, 0, 0, 280, 281, 213, 0, 214, 
  0, 0, 0, 0, 282, 0, 0, 283, 0, 284, 
  215, 0, 216, 88, 0, 0, 42, 43, 0, 0, 
  217, 0, 0, 218, 89, 0, 85, 0, 0, 219, 
  0, 0, 0, 86, 0, 220, 0, 87, 51, 0, 
  52, 0, 0, 0, 0, 0, 0, 55, 221, 0, 
  0, 58, 0, 0, 0, 0, 165, 166, 0, 0, 
  0, 0, 0, 0, 167, 168, 0, 0, 169, 63, 
  170, 65, 0, 0, 0, 0, 0, 42, 43, 0, 
  0, 0, 57, 68, 45, 0, 0, 85, 0, 0, 
  0, 0, 0, 0, 86, 0, 0, 0, 87, 51, 
  0, 52, 0, 0, 0, 0, 0, 0, 55, 0, 
  0, 0, 58, 0, 0, 0, 0, 210, 0, 0, 
  0, 0, 0, 0, 0, 0, 0, 211, 0, 0, 
  63, 212, 65, 0, 0, 0, 0, 0, 0, 0, 
  213, 0, 214, 57, 68, 45, 0, 0, 0, 0, 
  0, 0, 0, 215, 0, 216, 88, 0, 0, 0, 
  0, 0, 0, 217, 0, 0, 218, 89, 0, 0, 
  0, 0, 219, 0, 0, 0, 0, 0, 220, 0, 
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
  0, 221, 0, 0, 0, 0, 0, 0, 210, 0, 
  0, 0, 0, 0, 0, 0, 0, 0, 211, 0, 
  0, 0, 212, 0, 0, 0, 0, 0, 0, 0, 
  0, 213, 0, 214, 0, 0, 309, 0, 0, 0, 
  0, 0, 0, 0, 215, 0, 216, 88, 0, 0, 
  0, 0, 0, 0, 217, 0, 0, 218, 89, 0, 
  0, 0, 0, 219, 0, 0, 0, 0, 0, 220, 
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
  0, 0, 221, 0, 0, 0, 0, 0, 0, 41, 
  42, 43, 0, 0, 0, 0, 0, 0, 0, 0, 
  85, 0, 0, 0, 0, 0, 0, 86, 0, 0, 
  0, 87, 51, 0, 52, 0, 0, 0, 53, 0, 
  54, 55, 56, 0, 0, 58, 0, 0, 0, 59, 
  0, 60, 0, 0, 0, 0, 0, 0, 0, 0, 
  0, 0, 0, 63, 0, 65, 0, 67, 0, 70, 
  0, 72, 0, 0, 0, 0, 57, 68, 45, 0, 
  0, 0, 41, 42, 43, 0, 0, 0, 0, 0, 
  0, 0, 0, 85, 0, 0, 0, 0, 0, 0, 
  86, 0, 0, 0, 87, 51, 0, 52, 0, 0, 
  0, 53, 0, 54, 55, 56, 0, 0, 58, 0, 
  0, 0, 59, 0, 60, 0, 0, 0, 0, 0, 
  0, 0, 0, 0, 0, 0, 63, 0, 65, 0, 
  67, 0, 70, 272, 72, 0, 0, 0, 0, 57, 
  68, 45, 0, 0, 0, 41, 42, 43, 0, 0, 
  0, 0, 0, 0, 0, 0, 85, 0, 0, 0, 
  0, 0, 0, 86, 0, 0, 0, 87, 51, 0, 
  52, 0, 0, 0, 53, 0, 54, 55, 56, 0, 
  0, 58, 0, 0, 0, 59, 0, 60, 0, 0, 
  445, 0, 0, 0, 0, 0, 0, 0, 0, 63, 
  0, 65, 0, 67, 0, 70, 0, 72, 0, 0, 
  0, 0, 57, 68, 45, 0, 0, 0, 41, 42, 
  43, 0, 0, 0, 0, 0, 0, 0, 0, 85, 
  0, 0, 0, 0, 0, 0, 86, 0, 0, 0, 
  87, 51, 0, 52, 0, 0, 0, 53, 0, 54, 
  55, 56, 0, 0, 58, 0, 0, 0, 59, 0, 
  60, 0, 0, 448, 0, 0, 0, 0, 0, 0, 
  0, 0, 63, 0, 65, 0, 67, 0, 70, 0, 
  72, 0, 0, 0, 0, 57, 68, 45, 0, 0, 
  0, -45, 0, 0, 0, 41, 42, 43, 0, 0, 
  0, 0, 0, 0, 0, 0, 85, 0, 0, 0, 
  0, 0, 0, 86, 0, 0, 0, 87, 51, 0, 
  52, 0, 0, 0, 53, 0, 54, 55, 56, 0, 
  0, 58, 0, 0, 0, 59, 0, 60, 0, 0, 
  0, 0, 0, 0, 0, 0, 0, 0, 0, 63, 
  0, 65, 0, 67, 0, 70, 0, 72, 0, 0, 
  0, 0, 57, 68, 45, 0, 0, 0, 41, 42, 
  43, 0, 0, 0, 0, 0, 0, 0, 0, 85, 
  0, 0, 0, 0, 0, 0, 86, 0, 0, 0, 
  87, 51, 0, 52, 0, 0, 0, 53, 0, 54, 
  55, 56, 0, 0, 58, 0, 0, 0, 59, 0, 
  60, 0, 0, 0, 0, 0, 0, 203, 0, 0, 
  0, 0, 63, 0, 65, 0, 67, 0, 70, 0, 
  72, 0, 0, 0, 0, 57, 68, 45, 0, 0, 
  0, 115, 116, 117, 0, 0, 119, 121, 122, 0, 
  0, 123, 0, 124, 0, 0, 0, 126, 127, 128, 
  0, 0, 0, 0, 0, 0, 196, 130, 131, 132, 
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
  133, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
  0, 0, 0, 0, 0, 0, 137, 0, 0, 0, 
  0, 0, 0, 139, 140, 141, 0, 143, 144, 145, 
  146, 147, 148, 0, 0, 134, 142, 125, 118, 120, 
  136, 115, 116, 117, 0, 0, 119, 121, 122, 0, 
  0, 123, 0, 124, 0, 0, 0, 126, 127, 128, 
  0, 0, 0, 0, 0, 0, 129, 130, 131, 132, 
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
  133, 0, 0, 0, 135, 0, 0, 0, 0, 0, 
  0, 0, 0, 0, 0, 0, 137, 0, 0, 0, 
  0, 0, 138, 139, 140, 141, 0, 143, 144, 145, 
  146, 147, 148, 0, 0, 134, 142, 125, 118, 120, 
  136, 115, 116, 117, 0, 0, 119, 121, 122, 0, 
  0, 123, 0, 124, 0, 0, 0, 126, 127, 128, 
  0, 0, 0, 0, 0, 0, 129, 130, 131, 132, 
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
  133, 0, 0, 0, 135, 0, 0, 0, 0, 0, 
  0, 0, 153, 0, 0, 0, 137, 0, 0, 0, 
  0, 0, 138, 139, 140, 141, 0, 143, 144, 145, 
  146, 147, 148, 0, 0, 134, 142, 125, 118, 120, 
  136, 37, 0, 0, 0, 0, 39, 0, 41, 42, 
  43, 44, 0, 0, 0, 0, 0, 0, 46, 47, 
  0, 0, 0, 0, 0, 0, 48, 49, 0, 0, 
  50, 51, 0, 52, 0, 0, 0, 53, 0, 54, 
  55, 56, 0, 0, 58, 0, 0, 0, 59, 0, 
  60, 0, 0, 0, 0, 0, 61, 0, 62, 0, 
  0, 0, 63, 64, 65, 66, 67, 69, 70, 71, 
  72, 73, 74, 0, 0, 57, 68, 45, 38, 40, 
  0, 37, 0, 0, 0, 0, 39, 0, 41, 42, 
  43, 44, 0, 0, 0, 0, 0, 0, 46, 85, 
  0, 0, 0, 0, 0, 0, 48, 49, 0, 0, 
  50, 51, 0, 52, 0, 0, 0, 53, 0, 54, 
  55, 56, 0, 0, 58, 0, 0, 0, 59, 0, 
  60, 0, 0, 0, 0, 0, 61, 0, 62, 0, 
  0, 0, 63, 64, 65, 66, 67, 69, 70, 71, 
  72, 73, 74, 0, 0, 57, 68, 45, 38, 40, 
  0, 358, 116, 117, 0, 0, 360, 121, 362, 42, 
  43, 363, 0, 124, 0, 0, 0, 126, 365, 366, 
  0, 0, 0, 0, 0, 0, 367, 368, 131, 132, 
  50, 51, 0, 52, 0, 0, 0, 53, 0, 54, 
  369, 56, 0, 0, 371, 0, 0, 0, 59, 0, 
  60, 0, -191, 0, 0, 0, 372, 0, 62, 0, 
  0, 0, 373, 374, 375, 376, 67, 378, 379, 380, 
  381, 382, 383, 0, 0, 370, 377, 364, 359, 361, 
  136, 

  434, 388, 467, 155, 412, 432, 202, 415, 412, 415, 
  436, 355, 401, 230, 425, 355, 405, 110, 251, 409, 
  449, 303, 421, 228, 430, 34, 393, 426, 200, 330, 
  339, 110, 342, 444, 345, 337, 348, 206, 350, 303, 
  312, 310, 352, 443, 209, 354, 384, 152, 197, 335, 
  150, 254, 251, 463, 195, 308, 447, 222, 195, 234, 
  0, 231, 303, 0, 0, 0, 0, 0, 0, 83, 
  83, 83, 83, 456, 179, 83, 83, 180, 83, 83, 
  181, 83, 235, 182, 303, 0, 227, 83, 187, 189, 
  83, 175, 83, 303, 177, 224, 176, 386, 333, 83, 
  387, 250, 83, 303, 83, 84, 267, 157, 83, 0, 
  93, 271, 83, 83, 94, 95, 83, 83, 83, 83, 
  331, 178, 285, 286, 287, 83, 83, 83, 355, 306, 
  288, 289, 83, 184, 427, 83, 83, 355, 83, 304, 
  83, 183, 185, 171, 408, 83, 164, 250, 83, 83, 
  451, 452, 386, 411, 83, 387, 453, 83, 0, 83, 
  83, 454, 455, 83, 83, 159, 428, 83, 271, 427, 
  83, 0, 299, 299, 83, 271, 294, 271, 271, 271, 
  83, 296, 299, 299, 0, 271, 83, 271, 271, 314, 
  0, 271, 0, 292, 298, 301, 83, 317, 0, 0, 
  0, 271, 83, 291, 318, 320, 83, 271, 0, 290, 
  83, 271, 0, 279, 0, 271, 299, 0, 0, 274, 
  0, 271, 0, 0, 0, 0, 0, 0, 0, 0, 
  0, 0, 0, 0, 83, 0, 428, 0, 327, 0, 
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
  0, 0, 0, 0};

static const short QScriptGrammar_action_check [] = {
  7, 36, 17, 36, 36, 74, 36, 7, 1, 33, 
  7, 36, 8, 7, 36, 7, 36, 8, 8, 31, 
  8, 61, 60, 29, 29, 36, 36, 55, 29, 60, 
  33, 7, 16, 2, 29, 61, 55, 36, 8, 76, 
  29, 1, 7, 60, 48, 7, 33, 33, 29, 29, 
  29, 36, 8, 55, 5, 76, 29, 55, 33, 20, 
  76, 1, 0, 76, 36, 48, 1, 8, 36, 36, 
  29, 2, 29, 7, 7, 2, 5, 7, 60, 33, 
  33, 29, 48, 61, 55, 48, 60, 7, 5, 17, 
  40, 10, 8, 29, 8, 8, 8, 61, 62, 8, 
  36, 51, 6, 61, 62, 8, 61, 62, 7, 8, 
  8, 50, 8, 8, 40, 54, 20, 61, 62, 8, 
  42, 8, 40, 8, 8, 51, 50, -1, -1, 40, 
  54, 53, 12, 51, 8, 12, 55, -1, 61, 62, 
  51, -1, -1, 8, 12, 61, 60, 60, 60, 15, 
  25, 60, 27, 8, 2, 61, 62, 60, 29, -1, 
  56, -1, 60, 38, -1, 60, 29, 56, 34, 56, 
  36, 25, 25, 27, 27, 60, 60, 57, 29, -1, 
  57, 15, 29, 63, 38, 38, 63, 61, 62, 57, 
  61, 62, -1, -1, -1, 63, 61, 62, 61, 62, 
  34, 15, 36, 18, 19, -1, 61, 62, -1, -1, 
  61, 62, 18, 19, 61, 62, 18, 19, 18, 19, 
  34, -1, 36, -1, -1, -1, -1, -1, -1, -1, 
  45, 46, -1, -1, -1, -1, 25, -1, 27, 45, 
  46, -1, -1, 45, 46, 45, 46, 23, 24, 38, 
  -1, -1, -1, -1, -1, 31, 32, 23, 24, 35, 
  -1, 37, -1, -1, -1, 31, 32, 23, 24, 35, 
  -1, 37, -1, -1, 3, 31, 32, 23, 24, 35, 
  -1, 37, -1, -1, 13, 31, 32, -1, 17, 35, 
  -1, 37, -1, -1, -1, 23, 24, 26, -1, 28, 
  -1, -1, -1, -1, 32, -1, -1, 35, -1, 37, 
  39, -1, 41, 42, -1, -1, 12, 13, -1, -1, 
  49, -1, -1, 52, 53, -1, 22, -1, -1, 58, 
  -1, -1, -1, 29, -1, 64, -1, 33, 34, -1, 
  36, -1, -1, -1, -1, -1, -1, 43, 77, -1, 
  -1, 47, -1, -1, -1, -1, 23, 24, -1, -1, 
  -1, -1, -1, -1, 31, 32, -1, -1, 35, 65, 
  37, 67, -1, -1, -1, -1, -1, 12, 13, -1, 
  -1, -1, 78, 79, 80, -1, -1, 22, -1, -1, 
  -1, -1, -1, -1, 29, -1, -1, -1, 33, 34, 
  -1, 36, -1, -1, -1, -1, -1, -1, 43, -1, 
  -1, -1, 47, -1, -1, -1, -1, 3, -1, -1, 
  -1, -1, -1, -1, -1, -1, -1, 13, -1, -1, 
  65, 17, 67, -1, -1, -1, -1, -1, -1, -1, 
  26, -1, 28, 78, 79, 80, -1, -1, -1, -1, 
  -1, -1, -1, 39, -1, 41, 42, -1, -1, -1, 
  -1, -1, -1, 49, -1, -1, 52, 53, -1, -1, 
  -1, -1, 58, -1, -1, -1, -1, -1, 64, -1, 
  -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
  -1, 77, -1, -1, -1, -1, -1, -1, 3, -1, 
  -1, -1, -1, -1, -1, -1, -1, -1, 13, -1, 
  -1, -1, 17, -1, -1, -1, -1, -1, -1, -1, 
  -1, 26, -1, 28, -1, -1, 31, -1, -1, -1, 
  -1, -1, -1, -1, 39, -1, 41, 42, -1, -1, 
  -1, -1, -1, -1, 49, -1, -1, 52, 53, -1, 
  -1, -1, -1, 58, -1, -1, -1, -1, -1, 64, 
  -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
  -1, -1, 77, -1, -1, -1, -1, -1, -1, 11, 
  12, 13, -1, -1, -1, -1, -1, -1, -1, -1, 
  22, -1, -1, -1, -1, -1, -1, 29, -1, -1, 
  -1, 33, 34, -1, 36, -1, -1, -1, 40, -1, 
  42, 43, 44, -1, -1, 47, -1, -1, -1, 51, 
  -1, 53, -1, -1, -1, -1, -1, -1, -1, -1, 
  -1, -1, -1, 65, -1, 67, -1, 69, -1, 71, 
  -1, 73, -1, -1, -1, -1, 78, 79, 80, -1, 
  -1, -1, 11, 12, 13, -1, -1, -1, -1, -1, 
  -1, -1, -1, 22, -1, -1, -1, -1, -1, -1, 
  29, -1, -1, -1, 33, 34, -1, 36, -1, -1, 
  -1, 40, -1, 42, 43, 44, -1, -1, 47, -1, 
  -1, -1, 51, -1, 53, -1, -1, -1, -1, -1, 
  -1, -1, -1, -1, -1, -1, 65, -1, 67, -1, 
  69, -1, 71, 72, 73, -1, -1, -1, -1, 78, 
  79, 80, -1, -1, -1, 11, 12, 13, -1, -1, 
  -1, -1, -1, -1, -1, -1, 22, -1, -1, -1, 
  -1, -1, -1, 29, -1, -1, -1, 33, 34, -1, 
  36, -1, -1, -1, 40, -1, 42, 43, 44, -1, 
  -1, 47, -1, -1, -1, 51, -1, 53, -1, -1, 
  56, -1, -1, -1, -1, -1, -1, -1, -1, 65, 
  -1, 67, -1, 69, -1, 71, -1, 73, -1, -1, 
  -1, -1, 78, 79, 80, -1, -1, -1, 11, 12, 
  13, -1, -1, -1, -1, -1, -1, -1, -1, 22, 
  -1, -1, -1, -1, -1, -1, 29, -1, -1, -1, 
  33, 34, -1, 36, -1, -1, -1, 40, -1, 42, 
  43, 44, -1, -1, 47, -1, -1, -1, 51, -1, 
  53, -1, -1, 56, -1, -1, -1, -1, -1, -1, 
  -1, -1, 65, -1, 67, -1, 69, -1, 71, -1, 
  73, -1, -1, -1, -1, 78, 79, 80, -1, -1, 
  -1, 7, -1, -1, -1, 11, 12, 13, -1, -1, 
  -1, -1, -1, -1, -1, -1, 22, -1, -1, -1, 
  -1, -1, -1, 29, -1, -1, -1, 33, 34, -1, 
  36, -1, -1, -1, 40, -1, 42, 43, 44, -1, 
  -1, 47, -1, -1, -1, 51, -1, 53, -1, -1, 
  -1, -1, -1, -1, -1, -1, -1, -1, -1, 65, 
  -1, 67, -1, 69, -1, 71, -1, 73, -1, -1, 
  -1, -1, 78, 79, 80, -1, -1, -1, 11, 12, 
  13, -1, -1, -1, -1, -1, -1, -1, -1, 22, 
  -1, -1, -1, -1, -1, -1, 29, -1, -1, -1, 
  33, 34, -1, 36, -1, -1, -1, 40, -1, 42, 
  43, 44, -1, -1, 47, -1, -1, -1, 51, -1, 
  53, -1, -1, -1, -1, -1, -1, 60, -1, -1, 
  -1, -1, 65, -1, 67, -1, 69, -1, 71, -1, 
  73, -1, -1, -1, -1, 78, 79, 80, -1, -1, 
  -1, 4, 5, 6, -1, -1, 9, 10, 11, -1, 
  -1, 14, -1, 16, -1, -1, -1, 20, 21, 22, 
  -1, -1, -1, -1, -1, -1, 29, 30, 31, 32, 
  -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
  43, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
  -1, -1, -1, -1, -1, -1, 59, -1, -1, -1, 
  -1, -1, -1, 66, 67, 68, -1, 70, 71, 72, 
  73, 74, 75, -1, -1, 78, 79, 80, 81, 82, 
  83, 4, 5, 6, -1, -1, 9, 10, 11, -1, 
  -1, 14, -1, 16, -1, -1, -1, 20, 21, 22, 
  -1, -1, -1, -1, -1, -1, 29, 30, 31, 32, 
  -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
  43, -1, -1, -1, 47, -1, -1, -1, -1, -1, 
  -1, -1, -1, -1, -1, -1, 59, -1, -1, -1, 
  -1, -1, 65, 66, 67, 68, -1, 70, 71, 72, 
  73, 74, 75, -1, -1, 78, 79, 80, 81, 82, 
  83, 4, 5, 6, -1, -1, 9, 10, 11, -1, 
  -1, 14, -1, 16, -1, -1, -1, 20, 21, 22, 
  -1, -1, -1, -1, -1, -1, 29, 30, 31, 32, 
  -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
  43, -1, -1, -1, 47, -1, -1, -1, -1, -1, 
  -1, -1, 55, -1, -1, -1, 59, -1, -1, -1, 
  -1, -1, 65, 66, 67, 68, -1, 70, 71, 72, 
  73, 74, 75, -1, -1, 78, 79, 80, 81, 82, 
  83, 4, -1, -1, -1, -1, 9, -1, 11, 12, 
  13, 14, -1, -1, -1, -1, -1, -1, 21, 22, 
  -1, -1, -1, -1, -1, -1, 29, 30, -1, -1, 
  33, 34, -1, 36, -1, -1, -1, 40, -1, 42, 
  43, 44, -1, -1, 47, -1, -1, -1, 51, -1, 
  53, -1, -1, -1, -1, -1, 59, -1, 61, -1, 
  -1, -1, 65, 66, 67, 68, 69, 70, 71, 72, 
  73, 74, 75, -1, -1, 78, 79, 80, 81, 82, 
  -1, 4, -1, -1, -1, -1, 9, -1, 11, 12, 
  13, 14, -1, -1, -1, -1, -1, -1, 21, 22, 
  -1, -1, -1, -1, -1, -1, 29, 30, -1, -1, 
  33, 34, -1, 36, -1, -1, -1, 40, -1, 42, 
  43, 44, -1, -1, 47, -1, -1, -1, 51, -1, 
  53, -1, -1, -1, -1, -1, 59, -1, 61, -1, 
  -1, -1, 65, 66, 67, 68, 69, 70, 71, 72, 
  73, 74, 75, -1, -1, 78, 79, 80, 81, 82, 
  -1, 4, 5, 6, -1, -1, 9, 10, 11, 12, 
  13, 14, -1, 16, -1, -1, -1, 20, 21, 22, 
  -1, -1, -1, -1, -1, -1, 29, 30, 31, 32, 
  33, 34, -1, 36, -1, -1, -1, 40, -1, 42, 
  43, 44, -1, -1, 47, -1, -1, -1, 51, -1, 
  53, -1, 55, -1, -1, -1, 59, -1, 61, -1, 
  -1, -1, 65, 66, 67, 68, 69, 70, 71, 72, 
  73, 74, 75, -1, -1, 78, 79, 80, 81, 82, 
  83, 

  6, 14, 65, 7, 6, 45, 7, 46, 6, 46, 
  45, 45, 76, 7, 46, 45, 73, 86, 45, 6, 
  6, 6, 78, 7, 6, 85, 6, 46, 7, 67, 
  45, 86, 80, 7, 81, 6, 45, 7, 6, 6, 
  45, 6, 45, 2, 41, 45, 45, 9, 6, 45, 
  7, 6, 45, 7, 10, 45, 7, 7, 10, 11, 
  -1, 14, 6, -1, -1, -1, -1, -1, -1, 18, 
  18, 18, 18, 20, 23, 18, 18, 23, 18, 18, 
  23, 18, 6, 23, 6, -1, 34, 18, 30, 28, 
  18, 22, 18, 6, 22, 32, 22, 12, 42, 18, 
  15, 20, 18, 6, 18, 21, 18, 21, 18, -1, 
  20, 23, 18, 18, 20, 20, 18, 18, 18, 18, 
  42, 23, 23, 23, 23, 18, 18, 18, 45, 42, 
  23, 23, 18, 24, 20, 18, 18, 45, 18, 42, 
  18, 24, 24, 23, 61, 18, 24, 20, 18, 18, 
  20, 20, 12, 61, 18, 15, 20, 18, -1, 18, 
  18, 20, 20, 18, 18, 26, 20, 18, 23, 20, 
  18, -1, 18, 18, 18, 23, 31, 23, 23, 23, 
  18, 29, 18, 18, -1, 23, 18, 23, 23, 33, 
  -1, 23, -1, 25, 40, 40, 18, 35, -1, -1, 
  -1, 23, 18, 25, 40, 40, 18, 23, -1, 25, 
  18, 23, -1, 25, -1, 23, 18, -1, -1, 27, 
  -1, 23, -1, -1, -1, -1, -1, -1, -1, -1, 
  -1, -1, -1, -1, 18, -1, 20, -1, 40, -1, 
  -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
  -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
  -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
  -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
  -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
  -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
  -1, -1, -1, -1};

class QScriptGrammar
{
public:
  enum VariousConstants {
    EOF_SYMBOL = 0,
    T_AND = 1,
    T_AND_AND = 2,
    T_AND_EQ = 3,
    T_AUTOMATIC_SEMICOLON = 62,
    T_BREAK = 4,
    T_CASE = 5,
    T_CATCH = 6,
    T_COLON = 7,
    T_COMMA = 8,
    T_CONST = 81,
    T_CONTINUE = 9,
    T_DEBUGGER = 82,
    T_DEFAULT = 10,
    T_DELETE = 11,
    T_DIVIDE_ = 12,
    T_DIVIDE_EQ = 13,
    T_DO = 14,
    T_DOT = 15,
    T_ELSE = 16,
    T_EQ = 17,
    T_EQ_EQ = 18,
    T_EQ_EQ_EQ = 19,
    T_FALSE = 80,
    T_FINALLY = 20,
    T_FOR = 21,
    T_FUNCTION = 22,
    T_GE = 23,
    T_GT = 24,
    T_GT_GT = 25,
    T_GT_GT_EQ = 26,
    T_GT_GT_GT = 27,
    T_GT_GT_GT_EQ = 28,
    T_IDENTIFIER = 29,
    T_IF = 30,
    T_IN = 31,
    T_INSTANCEOF = 32,
    T_LBRACE = 33,
    T_LBRACKET = 34,
    T_LE = 35,
    T_LPAREN = 36,
    T_LT = 37,
    T_LT_LT = 38,
    T_LT_LT_EQ = 39,
    T_MINUS = 40,
    T_MINUS_EQ = 41,
    T_MINUS_MINUS = 42,
    T_NEW = 43,
    T_NOT = 44,
    T_NOT_EQ = 45,
    T_NOT_EQ_EQ = 46,
    T_NULL = 78,
    T_NUMERIC_LITERAL = 47,
    T_OR = 48,
    T_OR_EQ = 49,
    T_OR_OR = 50,
    T_PLUS = 51,
    T_PLUS_EQ = 52,
    T_PLUS_PLUS = 53,
    T_QUESTION = 54,
    T_RBRACE = 55,
    T_RBRACKET = 56,
    T_REMAINDER = 57,
    T_REMAINDER_EQ = 58,
    T_RESERVED_WORD = 83,
    T_RETURN = 59,
    T_RPAREN = 60,
    T_SEMICOLON = 61,
    T_STAR = 63,
    T_STAR_EQ = 64,
    T_STRING_LITERAL = 65,
    T_SWITCH = 66,
    T_THIS = 67,
    T_THROW = 68,
    T_TILDE = 69,
    T_TRUE = 79,
    T_TRY = 70,
    T_TYPEOF = 71,
    T_VAR = 72,
    T_VOID = 73,
    T_WHILE = 74,
    T_WITH = 75,
    T_XOR = 76,
    T_XOR_EQ = 77,

    ACCEPT_STATE = 237,
    RULE_COUNT = 269,
    STATE_COUNT = 468,
    TERMINAL_COUNT = 84,
    NON_TERMINAL_COUNT = 88,

    GOTO_INDEX_OFFSET = 468,
    GOTO_INFO_OFFSET = 1501,
    GOTO_CHECK_OFFSET = 1501
  };

  static inline int nt_action (int state, int nt)
  {
    const int yyn = QScriptGrammar_action_index[GOTO_INDEX_OFFSET + state] + nt;
    if (yyn < 0 || QScriptGrammar_action_check[GOTO_CHECK_OFFSET + yyn] != nt)
      return QScriptGrammar_goto_default[nt];

    return QScriptGrammar_action_info[GOTO_INFO_OFFSET + yyn];
  }

  static inline int t_action (int state, int token)
  {
    const int yyn = QScriptGrammar_action_index[state] + token;

    if (yyn < 0 || QScriptGrammar_action_check[yyn] != token)
      return - QScriptGrammar_action_default[state];

    return QScriptGrammar_action_info[yyn];
  }
};

QT_END_NAMESPACE

#endif // QSCRIPTGRAMMAR_P_H

