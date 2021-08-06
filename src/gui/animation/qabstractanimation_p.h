/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
**
** This file is part of the QtCore module of the Katie Toolkit.
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

#ifndef QABSTRACTANIMATION_P_H
#define QABSTRACTANIMATION_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Katie API.  It exists for the convenience
// of QIODevice. This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <QtCore/qbasictimer.h>
#include <QtCore/qdatetime.h>
#include <QtCore/qtimer.h>
#include <QtCore/qelapsedtimer.h>
#include "qobject_p.h"
#include "qabstractanimation.h"

#ifndef QT_NO_ANIMATION

QT_BEGIN_NAMESPACE

class QAnimationGroup;
class QAbstractAnimation;
class QAbstractAnimationPrivate : public QObjectPrivate
{
public:
    QAbstractAnimationPrivate()
        : state(QAbstractAnimation::Stopped),
          direction(QAbstractAnimation::Forward),
          totalCurrentTime(0),
          currentTime(0),
          loopCount(1),
          currentLoop(0),
          deleteWhenStopped(false),
          hasRegisteredTimer(false),
          isPause(false),
          isGroup(false),
          group(nullptr)
    {
    }

    virtual ~QAbstractAnimationPrivate() {}

    static QAbstractAnimationPrivate *get(QAbstractAnimation *q)
    {
        return q->d_func();
    }

    QAbstractAnimation::State state;
    QAbstractAnimation::Direction direction;
    void setState(QAbstractAnimation::State state);

    int totalCurrentTime;
    int currentTime;
    int loopCount;
    int currentLoop;

    bool deleteWhenStopped;
    bool hasRegisteredTimer;
    bool isPause;
    bool isGroup;

    QAnimationGroup *group;

private:
    Q_DECLARE_PUBLIC(QAbstractAnimation)
};


class QDefaultAnimationDriver : public QAnimationDriver
{
    Q_OBJECT
public:
    QDefaultAnimationDriver();
    void timerEvent(QTimerEvent *e);

    void started();
    void stopped();

private:
    QBasicTimer m_timer;
};

class Q_GUI_EXPORT QAnimationDriverPrivate : public QObjectPrivate
{
public:
    QAnimationDriverPrivate() : running(false) {}
    bool running;
};

class Q_GUI_EXPORT QUnifiedTimer : public QObject
{
private:
    QUnifiedTimer();

public:
    //XXX this is needed by dui
    static QUnifiedTimer *instance();

    static void registerAnimation(QAbstractAnimation *animation, bool isTopLevel);
    static void unregisterAnimation(QAbstractAnimation *animation);

    /*
        this is used for updating the currentTime of all animations in case the pause
        timer is active or, otherwise, only of the animation passed as parameter.
    */
    static void ensureTimerUpdate();

    /*
        this will evaluate the need of restarting the pause timer in case there is still
        some pause animations running.
    */
    static void updateAnimationTimer();

    void installAnimationDriver(QAnimationDriver *driver);

    void restartAnimationTimer();
    void updateAnimationsTime();

protected:
    void timerEvent(QTimerEvent *);

private:
    friend class QDefaultAnimationDriver;

    QAnimationDriver *driver;
    QDefaultAnimationDriver defaultDriver;

    QBasicTimer animationTimer;
    // timer used to delay the check if we should start/stop the animation timer
    QBasicTimer startStopAnimationTimer;

    QElapsedTimer time;

    qint64 lastTick;
    int currentAnimationIdx;
    bool insideTick;

    // bool to indicate that only pause animations are active
    bool isPauseTimerActive;

    QList<QAbstractAnimation*> animations, animationsToStart;

    // this is the count of running animations that are not a group neither a pause animation
    int runningLeafAnimations;
    QList<QAbstractAnimation*> runningPauseAnimations;

    void registerRunningAnimation(QAbstractAnimation *animation);
    void unregisterRunningAnimation(QAbstractAnimation *animation);

    int closestPauseAnimationTimeToFinish();
};

QT_END_NAMESPACE

#endif //QT_NO_ANIMATION

#endif //QABSTRACTANIMATION_P_H
