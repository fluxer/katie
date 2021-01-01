/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016-2021 Ivailo Monev
**
** This file is part of the tools applications of the Katie Toolkit.
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
** As a special exception, The Qt Company gives you certain additional
** rights. These rights are described in The Qt Company LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef SYMBOLS_H
#define SYMBOLS_H

#include "token.h"
#include <QString>
#include <QHash>
#include <QVector>
#include <QDebug>

QT_BEGIN_NAMESPACE

struct SubArray
{
    inline SubArray():from(0),len(-1){}
    inline SubArray(const QByteArray &a):array(a),from(0), len(a.size()){}
    inline SubArray(const char *s):array(s),from(0) { len = array.size(); }
    inline SubArray(const QByteArray &a, int from, int len):array(a), from(from), len(len){}
    QByteArray array;
    int from, len;
    inline bool operator==(const SubArray &other) const {
        if (len != other.len)
            return false;
        for (int i = 0; i < len; ++i)
            if (array.at(from + i) != other.array.at(other.from + i))
                return false;
        return true;
    }
};

inline uint qHash(const SubArray &key)
{
    return qHash(QString::fromLatin1(key.array.constData() + key.from, key.len));
}


struct Symbol
{
    inline Symbol() : lineNum(-1),token(NOTOKEN), from(0),len(-1) {}
    inline Symbol(int lineNum, Token token):
        lineNum(lineNum), token(token), from(0), len(-1) {}
    inline Symbol(int lineNum, Token token, const QByteArray &lexem):
        lineNum(lineNum), token(token), lex(lexem), from(0) { len = lex.size(); }
    inline Symbol(int lineNum, Token token, const QByteArray &lexem, int from, int len):
        lineNum(lineNum), token(token),lex(lexem),from(from), len(len){}
    int lineNum;
    Token token;
    inline QByteArray lexem() const { return lex.mid(from, len); }
    inline QByteArray unquotedLexem() const { return lex.mid(from+1, len-2); }
    inline operator QByteArray() const { return lex.mid(from, len); }
    inline operator SubArray() const { return SubArray(lex, from, len); }
    QByteArray lex;
    int from, len;
};
Q_DECLARE_TYPEINFO(Symbol, Q_MOVABLE_TYPE);


typedef QVector<Symbol> Symbols;

QT_END_NAMESPACE

#endif // SYMBOLS_H
