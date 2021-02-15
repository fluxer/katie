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

/*!
    \class QPauseAnimation
    \brief The QPauseAnimation class provides a pause for QSequentialAnimationGroup.
    \since 4.6
    \ingroup animation

    If you wish to introduce a delay between animations in a
    QSequentialAnimationGroup, you can insert a QPauseAnimation. This
    class does not animate anything, but does not
    \l{QAbstractAnimation::finished()}{finish} before a specified
    number of milliseconds have elapsed from when it was started. You
    specify the duration of the pause in the constructor. It can also
    be set directly with setDuration().

    It is not necessary to construct a QPauseAnimation yourself.
    QSequentialAnimationGroup provides the convenience functions
    \l{QSequentialAnimationGroup::}{addPause()} and
    \l{QSequentialAnimationGroup::}{insertPause()}. These functions
    simply take the number of milliseconds the pause should last.

    \sa QSequentialAnimationGroup
*/

#include "qpauseanimation.h"
#include "qabstractanimation_p.h"


#ifndef QT_NO_ANIMATION

QT_BEGIN_NAMESPACE

class QPauseAnimationPrivate : public QAbstractAnimationPrivate
{
public:
    QPauseAnimationPrivate() : QAbstractAnimationPrivate(), duration(250)
    {
        isPause = true;
    }

    int duration;
};

/*!
    Constructs a QPauseAnimation.
    \a parent is passed to QObject's constructor.
    The default duration is 0.
*/

QPauseAnimation::QPauseAnimation(QObject *parent) : QAbstractAnimation(*new QPauseAnimationPrivate, parent)
{
}

/*!
    Constructs a QPauseAnimation.
    \a msecs is the duration of the pause.
    \a parent is passed to QObject's constructor.
*/

QPauseAnimation::QPauseAnimation(int msecs, QObject *parent) : QAbstractAnimation(*new QPauseAnimationPrivate, parent)
{
    setDuration(msecs);
}

/*!
    Destroys the pause animation.
*/
QPauseAnimation::~QPauseAnimation()
{
}

/*!
    \property QPauseAnimation::duration
    \brief the duration of the pause.

    The duration of the pause. The duration should not be negative.
    The default duration is 250 milliseconds.
*/
int QPauseAnimation::duration() const
{
    Q_D(const QPauseAnimation);
    return d->duration;
}

void QPauseAnimation::setDuration(int msecs)
{
    if (msecs < 0) {
        qWarning("QPauseAnimation::setDuration: cannot set a negative duration");
        return;
    }
    Q_D(QPauseAnimation);
    d->duration = msecs;
}

/*!
    \reimp
 */
void QPauseAnimation::updateCurrentTime(int)
{
}


#include "moc_qpauseanimation.h"

QT_END_NAMESPACE

#endif //QT_NO_ANIMATION
