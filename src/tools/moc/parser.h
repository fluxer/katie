/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
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
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef PARSER_H
#define PARSER_H

#include <QStack>
#include "symbols.h"

QT_BEGIN_NAMESPACE

class Parser
{
public:
    Parser():index(0), displayWarnings(true), displayNotes(true) {}
    Symbols symbols;
    int index;
    bool displayWarnings;
    bool displayNotes;

    QStack<QByteArray> currentFilenames;

    inline bool hasNext() const { return (index < symbols.size()); }
    inline Token next() { if (index >= symbols.size()) return NOTOKEN; return symbols.at(index++).token; }
    bool test(Token);
    void next(Token);
    void next(Token, const char *msg);
    inline void prev() {--index;}
    inline Token lookup(int k = 1);
    inline const Symbol &symbol_lookup(int k = 1) { return symbols.at(index-1+k);}
    inline Token token() { return symbols.at(index-1).token;}
    inline QByteArray lexem() { return symbols.at(index-1).lexem();}
    inline QByteArray unquotedLexem() { return symbols.at(index-1).unquotedLexem();}
    inline const Symbol &symbol() { return symbols.at(index-1);}

    void error(int rollback);
    void error(const char *msg = nullptr);
    void warning(const char * = nullptr);
    void note(const char * = nullptr);

};

inline bool Parser::test(Token token)
{
    if (index < symbols.size() && symbols.at(index).token == token) {
        ++index;
        return true;
    }
    return false;
}

inline Token Parser::lookup(int k)
{
    const int l = index - 1 + k;
    return l < symbols.size() ? symbols.at(l).token : NOTOKEN;
}

inline void Parser::next(Token token)
{
    if (!test(token))
        error();
}

inline void Parser::next(Token token, const char *msg)
{
    if (!test(token))
        error(msg);
}

QT_END_NAMESPACE

#endif
