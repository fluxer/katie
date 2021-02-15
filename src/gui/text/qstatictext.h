/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
**
** This file is part of the test suite of the Katie Toolkit.
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

#ifndef QSTATICTEXT_H
#define QSTATICTEXT_H

#include <QtCore/qsize.h>
#include <QtGui/qtransform.h>
#include <QtGui/qfont.h>
#include <QtGui/qtextoption.h>


QT_BEGIN_NAMESPACE


class QStaticTextPrivate;
class Q_GUI_EXPORT QStaticText
{
public:
    QStaticText();
    QStaticText(const QString &text);
    QStaticText(const QStaticText &other);
    ~QStaticText();

    void setText(const QString &text);
    QString text() const;

    void setTextFormat(Qt::TextFormat textFormat);
    Qt::TextFormat textFormat() const;

    void setTextWidth(qreal textWidth);
    qreal textWidth() const;

    void setTextOption(const QTextOption &textOption);
    QTextOption textOption() const;

    QSizeF size() const;

    void prepare(const QTransform &matrix = QTransform(), const QFont &font = QFont());

    QStaticText &operator=(const QStaticText &);
    bool operator==(const QStaticText &) const;
    bool operator!=(const QStaticText &) const;

private:
    void detach();

    QExplicitlySharedDataPointer<QStaticTextPrivate> data;
    friend class QStaticTextPrivate;
};

QT_END_NAMESPACE

Q_DECLARE_METATYPE(QStaticText)


#endif // QSTATICTEXT_H
