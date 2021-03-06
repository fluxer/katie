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

#ifndef QDECLARATIVESMOOTHEDANIMATION_P_H
#define QDECLARATIVESMOOTHEDANIMATION_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Katie API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include "qdeclarativesmoothedanimation_p.h"
#include "qdeclarativeanimation_p.h"

#include "qdeclarativeanimation_p_p.h"

#include "qparallelanimationgroup.h"

#include "qobject_p.h"
#include <QTimer>

QT_BEGIN_NAMESPACE

class Q_AUTOTEST_EXPORT QSmoothedAnimation : public QAbstractAnimation
{
public:
    QSmoothedAnimation(QObject *parent=0);

    qreal to;
    qreal velocity;
    int userDuration;

    int maximumEasingTime;
    QDeclarativeSmoothedAnimation::ReversingMode reversingMode;

    qreal initialVelocity;
    qreal trackVelocity;

    QDeclarativeProperty target;

    int duration() const;
    void restart();
    void init();

protected:
    virtual void updateCurrentTime(int);
    virtual void updateState(QAbstractAnimation::State, QAbstractAnimation::State);

private:
    qreal easeFollow(qreal);
    qreal initialValue;

    bool invert;

    int finalDuration;

    // Parameters for use in updateCurrentTime()
    qreal a;  // Acceleration
    qreal d;  // Deceleration
    qreal tf; // Total time
    qreal tp; // Time at which peak velocity occurs
    qreal td; // Time at which decelleration begins
    qreal vp; // Velocity at tp
    qreal sp; // Displacement at tp
    qreal sd; // Displacement at td
    qreal vi; // "Normalized" initialvelocity
    qreal s;  // Total s

    int lastTime;

    bool recalc();
    void delayedStop();

    QTimer delayedStopTimer;
};

class QDeclarativeSmoothedAnimationPrivate : public QDeclarativePropertyAnimationPrivate
{
    Q_DECLARE_PUBLIC(QDeclarativeSmoothedAnimation)
public:
    QDeclarativeSmoothedAnimationPrivate();
    void updateRunningAnimations();

    QParallelAnimationGroup *wrapperGroup;
    QSmoothedAnimation *anim;
    QHash<QDeclarativeProperty, QSmoothedAnimation*> activeAnimations;
};

QT_END_NAMESPACE

#endif // QDECLARATIVESMOOTHEDANIMATION_P_H
