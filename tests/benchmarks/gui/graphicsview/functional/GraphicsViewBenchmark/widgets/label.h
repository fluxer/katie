/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
**
** This file is part of the examples of the Katie Toolkit.
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

#ifndef TEXTITEM_H
#define TEXTITEM_H

#include "gvbwidget.h"

class QPainter;
class QStyleOptionGraphicsItem;
class QGraphicsTextItem;

class Label : public GvbWidget
{
    Q_OBJECT

public:

    Label(const QString& text, QGraphicsItem *parent = 0);
    ~Label();

public:

    void setText(const QString& text);
    QString text() const;
    void setFont(const QFont font);

private:
    void resizeEvent(QGraphicsSceneResizeEvent *event);
    QSizeF sizeHint(Qt::SizeHint which, 
    const QSizeF &constraint = QSizeF()) const;

private:
    Q_DISABLE_COPY(Label)
    QGraphicsSimpleTextItem *m_textItem;
};

#endif
