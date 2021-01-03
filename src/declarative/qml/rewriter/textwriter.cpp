/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016-2021 Ivailo Monev
**
** This file is part of the QtDeclarative module of the Katie Toolkit.
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

#include "textwriter_p.h"

QT_BEGIN_NAMESPACE

using namespace QDeclarativeJS;

TextWriter::TextWriter()
{
}

static inline bool overlaps(int posA, int lengthA, int posB, int lengthB) {
    return (posA < posB + lengthB && posA + lengthA > posB + lengthB)
            || (posA < posB && posA + lengthA > posB);
}

bool TextWriter::hasOverlap(int pos, int length) const
{
    foreach (const Replace &cmd, replaceList) {
        if (overlaps(pos, length, cmd.pos, cmd.length))
            return true;
    }
    return false;
}

void TextWriter::replace(int pos, int length, const QString &replacement)
{
    Q_ASSERT(!hasOverlap(pos, length));

    Replace cmd;
    cmd.pos = pos;
    cmd.length = length;
    cmd.replacement = replacement;
    replaceList += cmd;
}

void TextWriter::write(QString *s)
{
    {
        Replace cmd;
        while (!replaceList.isEmpty()) {
            cmd = replaceList.first();
            replaceList.removeFirst();
            int diff = cmd.replacement.size() - cmd.length;
            {
                QMutableListIterator<Replace> i(replaceList);
                while (i.hasNext()) {
                    Replace &c = i.next();
                    if (cmd.pos < c.pos) {
                        c.pos += diff;
                    } else if (cmd.pos + cmd.length < c.pos + c.length) {
                        c.length += diff;
                    }
                }
            }
            if (s) {
                s->replace(cmd.pos, cmd.length, cmd.replacement);
            }
        }
    }
}

QT_END_NAMESPACE
