/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
**
** This file is part of the QtGui module of the Katie Toolkit.
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

#ifndef QTEXTLIST_H
#define QTEXTLIST_H

#include <QtGui/qtextobject.h>


QT_BEGIN_NAMESPACE


class QTextListPrivate;
class QTextCursor;

class Q_GUI_EXPORT QTextList : public QTextBlockGroup
{
    Q_OBJECT
public:
    explicit QTextList(QTextDocument *doc);
    ~QTextList();

    int count() const;

    inline bool isEmpty() const
    { return count() == 0; }

    QTextBlock item(int i) const;

    int itemNumber(const QTextBlock &) const;
    QString itemText(const QTextBlock &) const;

    void removeItem(int i);
    void remove(const QTextBlock &);

    void add(const QTextBlock &block);

    inline void setFormat(const QTextListFormat &format);
    QTextListFormat format() const { return QTextObject::format().toListFormat(); }

private:
    Q_DISABLE_COPY(QTextList)
    Q_DECLARE_PRIVATE(QTextList)
};

inline void QTextList::setFormat(const QTextListFormat &aformat)
{ QTextObject::setFormat(aformat); }

QT_END_NAMESPACE


#endif // QTEXTLIST_H
