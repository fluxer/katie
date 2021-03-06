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

#include "qdeclarativetranslate_p.h"
#include "qgraphicstransform_p.h"
#include <QDebug>
#include <QtCore/qmath.h>

QT_BEGIN_NAMESPACE

class QDeclarativeTranslatePrivate : public QGraphicsTransformPrivate
{
public:
    QDeclarativeTranslatePrivate()
        : x(0), y(0) {}
    qreal x;
    qreal y;
};

/*!
    Constructs an empty QDeclarativeTranslate object with the given \a parent.
*/
QDeclarativeTranslate::QDeclarativeTranslate(QObject *parent)
    : QGraphicsTransform(*new QDeclarativeTranslatePrivate, parent)
{
}

/*!
    Destroys the graphics scale.
*/
QDeclarativeTranslate::~QDeclarativeTranslate()
{
}

/*!
    \property QDeclarativeTranslate::x
    \brief the horizontal translation.

    The translation can be any real number; the default value is 0.0.

    \sa y
*/
qreal QDeclarativeTranslate::x() const
{
    Q_D(const QDeclarativeTranslate);
    return d->x;
}
void QDeclarativeTranslate::setX(qreal x)
{
    Q_D(QDeclarativeTranslate);
    if (d->x == x)
        return;
    d->x = x;
    update();
    emit xChanged();
}

/*!
    \property QDeclarativeTranslate::y
    \brief the vertical translation.

    The translation can be any real number; the default value is 0.0.

    \sa x
*/
qreal QDeclarativeTranslate::y() const
{
    Q_D(const QDeclarativeTranslate);
    return d->y;
}
void QDeclarativeTranslate::setY(qreal y)
{
    Q_D(QDeclarativeTranslate);
    if (d->y == y)
        return;
    d->y = y;
    update();
    emit yChanged();
}

void QDeclarativeTranslate::applyTo(QMatrix4x4 *matrix) const
{
    Q_D(const QDeclarativeTranslate);
    matrix->translate(d->x, d->y, 0);
}

QT_END_NAMESPACE
#include "moc_qdeclarativetranslate_p.h"
