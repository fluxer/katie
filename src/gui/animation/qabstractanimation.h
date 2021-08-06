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

#ifndef QABSTRACTANIMATION_H
#define QABSTRACTANIMATION_H

#include <QtCore/qobject.h>


QT_BEGIN_NAMESPACE

#ifndef QT_NO_ANIMATION

class QAnimationGroup;
class QSequentialAnimationGroup;
class QAnimationDriver;

class QAbstractAnimationPrivate;
class Q_GUI_EXPORT QAbstractAnimation : public QObject
{
    Q_OBJECT
    Q_ENUMS(State)
    Q_ENUMS(Direction)
    Q_PROPERTY(State state READ state NOTIFY stateChanged)
    Q_PROPERTY(int loopCount READ loopCount WRITE setLoopCount)
    Q_PROPERTY(int currentTime READ currentTime WRITE setCurrentTime)
    Q_PROPERTY(int currentLoop READ currentLoop NOTIFY currentLoopChanged)
    Q_PROPERTY(Direction direction READ direction WRITE setDirection NOTIFY directionChanged)
    Q_PROPERTY(int duration READ duration)

public:
    enum Direction {
        Forward,
        Backward
    };

    enum State {
        Stopped,
        Paused,
        Running
    };

    enum DeletionPolicy {
        KeepWhenStopped = 0,
        DeleteWhenStopped = 1
    };

    QAbstractAnimation(QObject *parent = nullptr);
    virtual ~QAbstractAnimation();

    State state() const;

    QAnimationGroup *group() const;

    Direction direction() const;
    void setDirection(Direction direction);

    int currentTime() const;
    int currentLoopTime() const;

    int loopCount() const;
    void setLoopCount(int loopCount);
    int currentLoop() const;

    virtual int duration() const = 0;
    int totalDuration() const;

Q_SIGNALS:
    void finished();
    void stateChanged(QAbstractAnimation::State newState, QAbstractAnimation::State oldState);
    void currentLoopChanged(int currentLoop);
    void directionChanged(QAbstractAnimation::Direction);

public Q_SLOTS:
    void start(QAbstractAnimation::DeletionPolicy policy = KeepWhenStopped);
    void pause();
    void resume();
    void setPaused(bool);
    void stop();
    void setCurrentTime(int msecs);

protected:
    QAbstractAnimation(QAbstractAnimationPrivate &dd, QObject *parent = nullptr);
    bool event(QEvent *event);

    virtual void updateCurrentTime(int currentTime) = 0;
    virtual void updateState(QAbstractAnimation::State newState, QAbstractAnimation::State oldState);
    virtual void updateDirection(QAbstractAnimation::Direction direction);

private:
    Q_DISABLE_COPY(QAbstractAnimation)
    Q_DECLARE_PRIVATE(QAbstractAnimation)
};

class QAnimationDriverPrivate;
class Q_GUI_EXPORT QAnimationDriver : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(QAnimationDriver)

public:
    QAnimationDriver(QObject *parent = nullptr);

    void advance();
    void install();

    bool isRunning() const;

protected:
    virtual void started() {};
    virtual void stopped() {};

    QAnimationDriver(QAnimationDriverPrivate &dd, QObject *parent = nullptr);

private:
    friend class QUnifiedTimer;

    void start();
    void stop();
};

#endif //QT_NO_ANIMATION

QT_END_NAMESPACE


#endif // QABSTRACTANIMATION_H
