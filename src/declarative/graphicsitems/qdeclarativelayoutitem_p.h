/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
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
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QDECLARATIVEGRAPHICSLAYOUTITEM_H
#define QDECLARATIVEGRAPHICSLAYOUTITEM_H
#include "qdeclarativeitem.h"

#include <QtGui/qgraphicslayoutitem.h>
#include <QtCore/qsize.h>


QT_BEGIN_NAMESPACE


class QDeclarativeLayoutItem : public QDeclarativeItem, public QGraphicsLayoutItem
{
    Q_OBJECT
    Q_INTERFACES(QGraphicsLayoutItem)
    Q_PROPERTY(QSizeF maximumSize READ maximumSize WRITE setMaximumSize NOTIFY maximumSizeChanged)
    Q_PROPERTY(QSizeF minimumSize READ minimumSize WRITE setMinimumSize NOTIFY minimumSizeChanged)
    Q_PROPERTY(QSizeF preferredSize READ preferredSize WRITE setPreferredSize NOTIFY preferredSizeChanged)
public:
    QDeclarativeLayoutItem(QDeclarativeItem* parent=0);

    QSizeF maximumSize() const { return m_maximumSize; }
    void setMaximumSize(const QSizeF &s) { if(s==m_maximumSize) return; m_maximumSize = s; emit maximumSizeChanged(); }

    QSizeF minimumSize() const { return m_minimumSize; }
    void setMinimumSize(const QSizeF &s) { if(s==m_minimumSize) return; m_minimumSize = s; emit minimumSizeChanged(); }

    QSizeF preferredSize() const { return m_preferredSize; }
    void setPreferredSize(const QSizeF &s) { if(s==m_preferredSize) return; m_preferredSize = s; emit preferredSizeChanged(); }

    virtual void setGeometry(const QRectF & rect);
protected:
    virtual QSizeF sizeHint(Qt::SizeHint which, const QSizeF &constraint = QSizeF()) const;

Q_SIGNALS:
    void maximumSizeChanged();
    void minimumSizeChanged();
    void preferredSizeChanged();

private:
    QSizeF m_maximumSize;
    QSizeF m_minimumSize;
    QSizeF m_preferredSize;
};

QT_END_NAMESPACE

QML_DECLARE_TYPE(QDeclarativeLayoutItem)

#endif
