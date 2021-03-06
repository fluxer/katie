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

#include "qdeclarativegraphicswidget_p.h"
#include "qdeclarativeanchors_p.h"
#include "qdeclarativeitem_p.h"
#include "qdeclarativeanchors_p_p.h"

QT_BEGIN_NAMESPACE

class QDeclarativeGraphicsWidgetPrivate : public QObjectPrivate {
    Q_DECLARE_PUBLIC(QDeclarativeGraphicsWidget)
public :
    QDeclarativeGraphicsWidgetPrivate() :
        _anchors(0), _anchorLines(0)
    {}
    QDeclarativeItemPrivate::AnchorLines *anchorLines() const;
    QDeclarativeAnchors *_anchors;
    mutable QDeclarativeItemPrivate::AnchorLines *_anchorLines;
};

QDeclarativeGraphicsWidget::QDeclarativeGraphicsWidget(QObject *parent) :
    QObject(*new QDeclarativeGraphicsWidgetPrivate, parent)
{
}
QDeclarativeGraphicsWidget::~QDeclarativeGraphicsWidget()
{
    Q_D(QDeclarativeGraphicsWidget);
    delete d->_anchorLines; d->_anchorLines = 0;
    delete d->_anchors; d->_anchors = 0;
}

QDeclarativeAnchors *QDeclarativeGraphicsWidget::anchors()
{
    Q_D(QDeclarativeGraphicsWidget);
    if (!d->_anchors)
        d->_anchors = new QDeclarativeAnchors(static_cast<QGraphicsObject *>(parent()));
    return d->_anchors;
}

QDeclarativeItemPrivate::AnchorLines *QDeclarativeGraphicsWidgetPrivate::anchorLines() const
{
    Q_Q(const QDeclarativeGraphicsWidget);
    if (!_anchorLines)
        _anchorLines = new QDeclarativeItemPrivate::AnchorLines(static_cast<QGraphicsObject *>(q->parent()));
    return _anchorLines;
}

QDeclarativeAnchorLine QDeclarativeGraphicsWidget::left() const
{
    Q_D(const QDeclarativeGraphicsWidget);
    return d->anchorLines()->left;
}

QDeclarativeAnchorLine QDeclarativeGraphicsWidget::right() const
{
    Q_D(const QDeclarativeGraphicsWidget);
    return d->anchorLines()->right;
}

QDeclarativeAnchorLine QDeclarativeGraphicsWidget::horizontalCenter() const
{
    Q_D(const QDeclarativeGraphicsWidget);
    return d->anchorLines()->hCenter;
}

QDeclarativeAnchorLine QDeclarativeGraphicsWidget::top() const
{
    Q_D(const QDeclarativeGraphicsWidget);
    return d->anchorLines()->top;
}

QDeclarativeAnchorLine QDeclarativeGraphicsWidget::bottom() const
{
    Q_D(const QDeclarativeGraphicsWidget);
    return d->anchorLines()->bottom;
}

QDeclarativeAnchorLine QDeclarativeGraphicsWidget::verticalCenter() const
{
    Q_D(const QDeclarativeGraphicsWidget);
    return d->anchorLines()->vCenter;
}

QT_END_NAMESPACE

#include "moc_qdeclarativegraphicswidget_p.h"
