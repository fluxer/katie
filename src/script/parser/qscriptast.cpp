/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
**
** This file is part of the QtScript module of the Katie Toolkit.
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

#include "qscriptast_p.h"

QT_BEGIN_NAMESPACE

namespace QScript { namespace AST {

ExpressionNode *Node::expressionCast()
{
    return 0;
}

BinaryExpression *Node::binaryExpressionCast()
{
    return 0;
}

Statement *Node::statementCast()
{
    return 0;
}

ExpressionNode *ExpressionNode::expressionCast()
{
    return this;
}

BinaryExpression *BinaryExpression::binaryExpressionCast()
{
    return this;
}

Statement *Statement::statementCast()
{
    return this;
}

void ThisExpression::accept0()
{
}

void IdentifierExpression::accept0()
{
}

void NullExpression::accept0()
{
}

void TrueLiteral::accept0()
{
}

void FalseLiteral::accept0()
{
}

void StringLiteral::accept0()
{
}

void NumericLiteral::accept0()
{
}

void RegExpLiteral::accept0()
{
}

void ArrayLiteral::accept0()
{
    acceptChild(elements);
    acceptChild(elision);
}

void ObjectLiteral::accept0()
{
    acceptChild(properties);
}

void ElementList::accept0()
{
    ElementList *it = this;
    do {
        acceptChild(it->elision);
        acceptChild(it->expression);
        it = it->next;
    } while (it);
}

void Elision::accept0()
{
    // ###
}

void PropertyNameAndValueList::accept0()
{
    PropertyNameAndValueList *it = this;
    do {
        acceptChild(it->name);
        acceptChild(it->value);
        it = it->next;
    } while (it);
}

void IdentifierPropertyName::accept0()
{
}

void StringLiteralPropertyName::accept0()
{
}

void NumericLiteralPropertyName::accept0()
{
}

void ArrayMemberExpression::accept0()
{
    acceptChild(base);
    acceptChild(expression);
}

void FieldMemberExpression::accept0()
{
    acceptChild(base);
}

void NewMemberExpression::accept0()
{
    acceptChild(base);
    acceptChild(arguments);
}

void NewExpression::accept0()
{
    acceptChild(expression);
}

void CallExpression::accept0()
{
    acceptChild(base);
    acceptChild(arguments);
}

void ArgumentList::accept0()
{
    ArgumentList *it = this;
    do {
        acceptChild(it->expression);
        it = it->next;
    } while (it);
}

void PostIncrementExpression::accept0()
{
    acceptChild(base);
}

void PostDecrementExpression::accept0()
{
    acceptChild(base);
}

void DeleteExpression::accept0()
{
    acceptChild(expression);
}

void VoidExpression::accept0()
{
    acceptChild(expression);
}

void TypeOfExpression::accept0()
{
    acceptChild(expression);
}

void PreIncrementExpression::accept0()
{
    acceptChild(expression);
}

void PreDecrementExpression::accept0()
{
    acceptChild(expression);
}

void UnaryPlusExpression::accept0()
{
    acceptChild(expression);
}

void UnaryMinusExpression::accept0()
{
    acceptChild(expression);
}

void TildeExpression::accept0()
{
    acceptChild(expression);
}

void NotExpression::accept0()
{
    acceptChild(expression);
}

void BinaryExpression::accept0()
{
    acceptChild(left);
    acceptChild(right);
}

void ConditionalExpression::accept0()
{
    acceptChild(expression);
    acceptChild(ok);
    acceptChild(ko);
}

void Expression::accept0()
{
    acceptChild(left);
    acceptChild(right);
}

void Block::accept0()
{
    acceptChild(statements);
}

void StatementList::accept0()
{
    StatementList *it = this;
    do {
        acceptChild(it->statement);
        it = it->next;
    } while (it);
}

void VariableStatement::accept0()
{
    acceptChild(declarations);
}

void VariableDeclarationList::accept0()
{
    VariableDeclarationList *it = this;
    do {
        acceptChild(it->declaration);
        it = it->next;
    } while (it);
}

void VariableDeclaration::accept0()
{
    acceptChild(expression);
}

void EmptyStatement::accept0()
{
}

void ExpressionStatement::accept0()
{
    acceptChild(expression);
}

void IfStatement::accept0()
{
    acceptChild(expression);
    acceptChild(ok);
    acceptChild(ko);
}

void DoWhileStatement::accept0()
{
    acceptChild(statement);
    acceptChild(expression);
}

void WhileStatement::accept0()
{
    acceptChild(expression);
    acceptChild(statement);
}

void ForStatement::accept0()
{
    acceptChild(initialiser);
    acceptChild(condition);
    acceptChild(expression);
    acceptChild(statement);
}

void LocalForStatement::accept0()
{
    acceptChild(declarations);
    acceptChild(condition);
    acceptChild(expression);
    acceptChild(statement);
}

void ForEachStatement::accept0()
{
    acceptChild(initialiser);
    acceptChild(expression);
    acceptChild(statement);
}

void LocalForEachStatement::accept0()
{
    acceptChild(declaration);
    acceptChild(expression);
    acceptChild(statement);
}

void ContinueStatement::accept0()
{
}

void BreakStatement::accept0()
{
}

void ReturnStatement::accept0()
{
    acceptChild(expression);
}

void WithStatement::accept0()
{
    acceptChild(expression);
    acceptChild(statement);
}

void SwitchStatement::accept0()
{
    acceptChild(expression);
    acceptChild(block);
}

void CaseBlock::accept0()
{
    acceptChild(clauses);
    acceptChild(defaultClause);
    acceptChild(moreClauses);
}

void CaseClauses::accept0()
{
    CaseClauses *it = this;
    do {
        acceptChild(it->clause);
        it = it->next;
    } while (it);
}

void CaseClause::accept0()
{
    acceptChild(expression);
    acceptChild(statements);
}

void DefaultClause::accept0()
{
    acceptChild(statements);
}

void LabelledStatement::accept0()
{
    acceptChild(statement);
}

void ThrowStatement::accept0()
{
    acceptChild(expression);
}

void TryStatement::accept0()
{
    acceptChild(statement);
    acceptChild(catchExpression);
    acceptChild(finallyExpression);
}

void Catch::accept0()
{
    acceptChild(statement);
}

void Finally::accept0()
{
    acceptChild(statement);
}

void FunctionDeclaration::accept0()
{
    acceptChild(formals);
    acceptChild(body);
}

void FunctionExpression::accept0()
{
    acceptChild(formals);
    acceptChild(body);
}

void FormalParameterList::accept0()
{
    // ###
}

void FunctionBody::accept0()
{
    acceptChild(elements);
}

void Program::accept0()
{
    acceptChild(elements);
}

void SourceElements::accept0()
{
    SourceElements *it = this;
    do {
        acceptChild(it->element);
        it = it->next;
    } while (it);
}

void FunctionSourceElement::accept0()
{
    acceptChild(declaration);
}

void StatementSourceElement::accept0()
{
    acceptChild(statement);
}

void DebuggerStatement::accept0()
{
}

} } // namespace QScript::AST

QT_END_NAMESPACE
