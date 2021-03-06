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

#ifndef QDECLARATIVEFLIPABLE_H
#define QDECLARATIVEFLIPABLE_H

#include "qdeclarativeitem.h"

#include <QtCore/QObject>
#include <QtGui/QTransform>
#include <QtGui/qvector3d.h>


QT_BEGIN_NAMESPACE


class QDeclarativeFlipablePrivate;
class Q_AUTOTEST_EXPORT QDeclarativeFlipable : public QDeclarativeItem
{
    Q_OBJECT

    Q_ENUMS(Side)
    Q_PROPERTY(QGraphicsObject *front READ front WRITE setFront NOTIFY frontChanged)
    Q_PROPERTY(QGraphicsObject *back READ back WRITE setBack NOTIFY backChanged)
    Q_PROPERTY(Side side READ side NOTIFY sideChanged)
    //### flipAxis
    //### flipRotation
public:
    QDeclarativeFlipable(QDeclarativeItem *parent=0);
    ~QDeclarativeFlipable();

    QGraphicsObject *front();
    void setFront(QGraphicsObject *);

    QGraphicsObject *back();
    void setBack(QGraphicsObject *);

    enum Side { Front, Back };
    Side side() const;

Q_SIGNALS:
    void frontChanged();
    void backChanged();
    void sideChanged();

private Q_SLOTS:
    void retransformBack();

private:
    Q_DISABLE_COPY(QDeclarativeFlipable)
    Q_DECLARE_PRIVATE_D(QGraphicsItem::d_ptr.data(), QDeclarativeFlipable)
};

QT_END_NAMESPACE

QML_DECLARE_TYPE(QDeclarativeFlipable)


#endif // QDECLARATIVEFLIPABLE_H
