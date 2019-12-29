/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016-2020 Ivailo Monev
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

#include "qdeclarativeimplicitsizeitem_p.h"
#include "qdeclarativeimplicitsizeitem_p_p.h"

QT_BEGIN_NAMESPACE

void QDeclarativeImplicitSizeItemPrivate::implicitWidthChanged()
{
    Q_Q(QDeclarativeImplicitSizeItem);
    emit q->implicitWidthChanged();
}

void QDeclarativeImplicitSizeItemPrivate::implicitHeightChanged()
{
    Q_Q(QDeclarativeImplicitSizeItem);
    emit q->implicitHeightChanged();
}

QDeclarativeImplicitSizeItem::QDeclarativeImplicitSizeItem(QDeclarativeItem *parent)
    : QDeclarativeItem(*(new QDeclarativeImplicitSizeItemPrivate), parent)
{
}

QDeclarativeImplicitSizeItem::QDeclarativeImplicitSizeItem(QDeclarativeImplicitSizeItemPrivate &dd, QDeclarativeItem *parent)
    : QDeclarativeItem(dd, parent)
{
}


void QDeclarativeImplicitSizePaintedItemPrivate::implicitWidthChanged()
{
    Q_Q(QDeclarativeImplicitSizePaintedItem);
    emit q->implicitWidthChanged();
}

void QDeclarativeImplicitSizePaintedItemPrivate::implicitHeightChanged()
{
    Q_Q(QDeclarativeImplicitSizePaintedItem);
    emit q->implicitHeightChanged();
}

QDeclarativeImplicitSizePaintedItem::QDeclarativeImplicitSizePaintedItem(QDeclarativeItem *parent)
    : QDeclarativePaintedItem(*(new QDeclarativeImplicitSizePaintedItemPrivate), parent)
{
}

QDeclarativeImplicitSizePaintedItem::QDeclarativeImplicitSizePaintedItem(QDeclarativeImplicitSizePaintedItemPrivate &dd, QDeclarativeItem *parent)
    : QDeclarativePaintedItem(dd, parent)
{
}

QT_END_NAMESPACE
#include <moc_qdeclarativeimplicitsizeitem_p.h>
