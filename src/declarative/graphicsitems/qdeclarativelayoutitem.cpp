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

#include "qdeclarativelayoutitem_p.h"

#include <QDebug>

#include <limits.h>

QT_BEGIN_NAMESPACE

/*!
    \qmlclass LayoutItem QDeclarativeLayoutItem
    \ingroup qml-utility-elements
    \since 4.7
    \brief The LayoutItem element allows declarative UI elements to be placed inside Qt's Graphics View layouts.

    LayoutItem is a variant of \l Item with additional size hint properties. These properties provide the size hints
    necessary for items to work in conjunction with Qt \l{Graphics View Framework}{Graphics View} layout classes
    such as QGraphicsLinearLayout and QGraphicsGridLayout. The Qt layout mechanisms will resize the LayoutItem as appropriate,
    taking its size hints into account, and you can propagate this to the other elements in your UI via anchors and bindings.

    This is a QGraphicsLayoutItem subclass, and its properties merely expose the QGraphicsLayoutItem functionality to QML.

    The \l{declarative/cppextensions/qgraphicslayouts/layoutitem}{LayoutItem example}
    demonstrates how a LayoutItem can be used within a \l{Graphics View Framework}{Graphics View}
    scene.
*/

/*!
    \qmlproperty QSizeF LayoutItem::maximumSize

    The maximumSize property can be set to specify the maximum desired size of this LayoutItem
*/

/*!
    \qmlproperty QSizeF LayoutItem::minimumSize

    The minimumSize property can be set to specify the minimum desired size of this LayoutItem
*/

/*!
    \qmlproperty QSizeF LayoutItem::preferredSize

    The preferredSize property can be set to specify the preferred size of this LayoutItem
*/

QDeclarativeLayoutItem::QDeclarativeLayoutItem(QDeclarativeItem* parent)
    : QDeclarativeItem(parent), m_maximumSize(INT_MAX,INT_MAX), m_minimumSize(0,0), m_preferredSize(0,0)
{
    setGraphicsItem(this);
}

void QDeclarativeLayoutItem::setGeometry(const QRectF & rect)
{
    setX(rect.x());
    setY(rect.y());
    setWidth(rect.width());
    setHeight(rect.height());
}

QSizeF QDeclarativeLayoutItem::sizeHint(Qt::SizeHint w, const QSizeF &constraint) const
{
    Q_UNUSED(constraint);
    if(w == Qt::MinimumSize){
        return m_minimumSize;
    }else if(w == Qt::MaximumSize){
        return m_maximumSize;
    }else{
        return m_preferredSize;
    }
}

QT_END_NAMESPACE
#include "moc_qdeclarativelayoutitem_p.h"
