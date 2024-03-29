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

#ifndef QDECLARATIVESMOOTHEDANIMATION_H
#define QDECLARATIVESMOOTHEDANIMATION_H

#include "qdeclarative.h"
#include "qdeclarativeanimation_p.h"

#include <QtCore/qobject.h>


QT_BEGIN_NAMESPACE


class QDeclarativeProperty;
class QDeclarativeSmoothedAnimationPrivate;
class Q_AUTOTEST_EXPORT QDeclarativeSmoothedAnimation : public QDeclarativeNumberAnimation
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(QDeclarativeSmoothedAnimation)
    Q_ENUMS(ReversingMode)

    Q_PROPERTY(qreal velocity READ velocity WRITE setVelocity NOTIFY velocityChanged)
    Q_PROPERTY(ReversingMode reversingMode READ reversingMode WRITE setReversingMode NOTIFY reversingModeChanged)
    Q_PROPERTY(qreal maximumEasingTime READ maximumEasingTime WRITE setMaximumEasingTime NOTIFY maximumEasingTimeChanged)

public:
    enum ReversingMode { Eased, Immediate, Sync };

    QDeclarativeSmoothedAnimation(QObject *parent = nullptr);
    ~QDeclarativeSmoothedAnimation();

    ReversingMode reversingMode() const;
    void setReversingMode(ReversingMode);

    virtual int duration() const;
    virtual void setDuration(int);

    qreal velocity() const;
    void setVelocity(qreal);

    int maximumEasingTime() const;
    void setMaximumEasingTime(int);

    virtual void transition(QDeclarativeStateActions &actions,
                            QDeclarativeProperties &modified,
                            TransitionDirection direction);
    QAbstractAnimation* qtAnimation();

Q_SIGNALS:
    void velocityChanged();
    void reversingModeChanged();
    void maximumEasingTimeChanged();
};

QT_END_NAMESPACE

QML_DECLARE_TYPE(QDeclarativeSmoothedAnimation)


#endif // QDECLARATIVESMOOTHEDANIMATION_H
