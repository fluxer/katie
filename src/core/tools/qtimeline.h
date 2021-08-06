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

#ifndef QTIMELINE_H
#define QTIMELINE_H

#include <QtCore/qeasingcurve.h>
#include <QtCore/qobject.h>


QT_BEGIN_NAMESPACE


class QTimeLinePrivate;
class Q_CORE_EXPORT QTimeLine : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int duration READ duration WRITE setDuration)
    Q_PROPERTY(int updateInterval READ updateInterval WRITE setUpdateInterval)
    Q_PROPERTY(int currentTime READ currentTime WRITE setCurrentTime)
    Q_PROPERTY(Direction direction READ direction WRITE setDirection)
    Q_PROPERTY(int loopCount READ loopCount WRITE setLoopCount)
    Q_PROPERTY(CurveShape curveShape READ curveShape WRITE setCurveShape)
    Q_PROPERTY(QEasingCurve easingCurve READ easingCurve WRITE setEasingCurve)
public:
    enum State {
        NotRunning,
        Paused,
        Running
    };
    enum Direction {
        Forward,
        Backward
    };
    enum CurveShape {
        EaseInCurve,
        EaseOutCurve,
        EaseInOutCurve,
        LinearCurve,
        SineCurve,
        CosineCurve
    };

    explicit QTimeLine(int duration = 1000, QObject *parent = nullptr);
    virtual ~QTimeLine();

    State state() const;

    int loopCount() const;
    void setLoopCount(int count);

    Direction direction() const;
    void setDirection(Direction direction);

    int duration() const;
    void setDuration(int duration);

    int startFrame() const;
    void setStartFrame(int frame);
    int endFrame() const;
    void setEndFrame(int frame);
    void setFrameRange(int startFrame, int endFrame);

    int updateInterval() const;
    void setUpdateInterval(int interval);

    CurveShape curveShape() const;
    void setCurveShape(CurveShape shape);

    QEasingCurve easingCurve() const;
    void setEasingCurve(const QEasingCurve &curve);

    int currentTime() const;
    int currentFrame() const;
    qreal currentValue() const;

    int frameForTime(int msec) const;
    virtual qreal valueForTime(int msec) const;

public Q_SLOTS:
    void start();
    void resume();
    void stop();
    void setPaused(bool paused);
    void setCurrentTime(int msec);
    void toggleDirection();

Q_SIGNALS:
    void valueChanged(qreal x);
    void frameChanged(int);
    void stateChanged(QTimeLine::State newState);
    void finished();

protected:
    void timerEvent(QTimerEvent *event);

private:
    Q_DISABLE_COPY(QTimeLine)
    Q_DECLARE_PRIVATE(QTimeLine)
};

QT_END_NAMESPACE


#endif

