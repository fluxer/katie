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

#ifndef QDECLARATIVESPRINGANIMATION_H
#define QDECLARATIVESPRINGANIMATION_H

#include "qdeclarative.h"
#include "qdeclarativeanimation_p.h"

#include <QtCore/qobject.h>


QT_BEGIN_NAMESPACE


class QDeclarativeSpringAnimationPrivate;
class Q_AUTOTEST_EXPORT QDeclarativeSpringAnimation : public QDeclarativeNumberAnimation
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(QDeclarativeSpringAnimation)
    Q_INTERFACES(QDeclarativePropertyValueSource)

    Q_PROPERTY(qreal velocity READ velocity WRITE setVelocity)
    Q_PROPERTY(qreal spring READ spring WRITE setSpring)
    Q_PROPERTY(qreal damping READ damping WRITE setDamping)
    Q_PROPERTY(qreal epsilon READ epsilon WRITE setEpsilon)
    Q_PROPERTY(qreal modulus READ modulus WRITE setModulus NOTIFY modulusChanged)
    Q_PROPERTY(qreal mass READ mass WRITE setMass NOTIFY massChanged)

public:
    QDeclarativeSpringAnimation(QObject *parent=0);
    ~QDeclarativeSpringAnimation();

    qreal velocity() const;
    void setVelocity(qreal velocity);

    qreal spring() const;
    void setSpring(qreal spring);

    qreal damping() const;
    void setDamping(qreal damping);

    qreal epsilon() const;
    void setEpsilon(qreal epsilon);

    qreal mass() const;
    void setMass(qreal modulus);

    qreal modulus() const;
    void setModulus(qreal modulus);

    virtual void transition(QDeclarativeStateActions &actions,
                            QDeclarativeProperties &modified,
                            TransitionDirection direction);

protected:
    virtual QAbstractAnimation *qtAnimation();

Q_SIGNALS:
    void modulusChanged();
    void massChanged();
    void syncChanged();
};

QT_END_NAMESPACE

QML_DECLARE_TYPE(QDeclarativeSpringAnimation)


#endif // QDECLARATIVESPRINGANIMATION_H
