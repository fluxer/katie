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

#include "parser.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>

QT_BEGIN_NAMESPACE

static const char *error_msg = nullptr;

void Parser::error(int rollback) {
    index -= rollback;
    error();
}
void Parser::error(const char *msg) {
    if (msg || error_msg)
        qWarning("%s:%d: Error: %s",
                 currentFilenames.top().constData(), symbol().lineNum, msg?msg:error_msg);
    else
        qWarning("%s:%d: Parse error at \"%s\"",
                 currentFilenames.top().constData(), symbol().lineNum, symbol().lexem().data());
    exit(EXIT_FAILURE);
}

void Parser::warning(const char *msg) {
    if (displayWarnings && msg)
        fprintf(stderr, "%s:%d: Warning: %s\n",
                currentFilenames.top().constData(), qMax(0, index > 0 ? symbol().lineNum : 0), msg);
}

void Parser::note(const char *msg) {
    if (displayNotes && msg)
        fprintf(stderr, "%s:%d: Note: %s\n",
                currentFilenames.top().constData(), qMax(0, index > 0 ? symbol().lineNum : 0), msg);
}

QT_END_NAMESPACE
