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

#include "qdeclarativescalegrid_p_p.h"

#include "qdeclarative.h"

#include <QBuffer>
#include <QDebug>

QT_BEGIN_NAMESPACE
/*!
    \internal
    \class QDeclarativeScaleGrid
    \brief The QDeclarativeScaleGrid class allows you to specify a 3x3 grid to use in scaling an image.
*/

QDeclarativeScaleGrid::QDeclarativeScaleGrid(QObject *parent) : QObject(parent), _left(0), _top(0), _right(0), _bottom(0)
{
}

QDeclarativeScaleGrid::~QDeclarativeScaleGrid()
{
}

bool QDeclarativeScaleGrid::isNull() const
{
    return !_left && !_top && !_right && !_bottom;
}

void QDeclarativeScaleGrid::setLeft(int pos)
{
    if (_left != pos) {
        _left = pos;
        emit borderChanged();
    }
}

void QDeclarativeScaleGrid::setTop(int pos)
{
    if (_top != pos) {
        _top = pos;
        emit borderChanged();
    }
}

void QDeclarativeScaleGrid::setRight(int pos)
{
    if (_right != pos) {
        _right = pos;
        emit borderChanged();
    }
}

void QDeclarativeScaleGrid::setBottom(int pos)
{
    if (_bottom != pos) {
        _bottom = pos;
        emit borderChanged();
    }
}

QT_END_NAMESPACE

#include "moc_qdeclarativescalegrid_p_p.h"

