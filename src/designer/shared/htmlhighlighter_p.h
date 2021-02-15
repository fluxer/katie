/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
**
** This file is part of the Katie Designer of the Katie Toolkit.
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
// of Katie Designer.  This header
// file may change from version to version without notice, or even be removed.
//
// We mean it.
//

#ifndef HTMLHIGHLIGHTER_H
#define HTMLHIGHLIGHTER_H

#include <QtGui/QSyntaxHighlighter>

QT_BEGIN_NAMESPACE

namespace qdesigner_internal {

/* HTML syntax highlighter based on Qt Quarterly example */
class HtmlHighlighter : public QSyntaxHighlighter
{
    Q_OBJECT

public:
    enum Construct {
        Entity,
        Tag,
        Comment,
        Attribute,
        Value,
        LastConstruct = Value
    };

    HtmlHighlighter(QTextEdit *textEdit);

    void setFormatFor(Construct construct, const QTextCharFormat &format);

    QTextCharFormat formatFor(Construct construct) const
    { return m_formats[construct]; }

protected:
    enum State {
        NormalState = -1,
        InComment,
        InTag
    };

    void highlightBlock(const QString &text);

private:
    QTextCharFormat m_formats[LastConstruct + 1];
};

} // namespace qdesigner_internal

QT_END_NAMESPACE

#endif // HTMLHIGHLIGHTER_H
