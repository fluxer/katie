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

#ifndef QSEQUENTIALANIMATIONGROUP_H
#define QSEQUENTIALANIMATIONGROUP_H

#include <QtGui/qanimationgroup.h>


QT_BEGIN_NAMESPACE

#ifndef QT_NO_ANIMATION

class QPauseAnimation;
class QSequentialAnimationGroupPrivate;

class Q_GUI_EXPORT QSequentialAnimationGroup : public QAnimationGroup
{
    Q_OBJECT
    Q_PROPERTY(QAbstractAnimation* currentAnimation READ currentAnimation NOTIFY currentAnimationChanged)

public:
    QSequentialAnimationGroup(QObject *parent = nullptr);
    ~QSequentialAnimationGroup();

    QPauseAnimation *addPause(int msecs);
    QPauseAnimation *insertPause(int index, int msecs);

    QAbstractAnimation *currentAnimation() const;
    int duration() const;

Q_SIGNALS:
    void currentAnimationChanged(QAbstractAnimation *current);

protected:
    QSequentialAnimationGroup(QSequentialAnimationGroupPrivate &dd, QObject *parent);

    void updateCurrentTime(int);
    void updateState(QAbstractAnimation::State newState, QAbstractAnimation::State oldState);
    void updateDirection(QAbstractAnimation::Direction direction);

private:
    Q_DISABLE_COPY(QSequentialAnimationGroup)
    Q_DECLARE_PRIVATE(QSequentialAnimationGroup)
    Q_PRIVATE_SLOT(d_func(), void _q_uncontrolledAnimationFinished())
};

#endif //QT_NO_ANIMATION

QT_END_NAMESPACE


#endif //QSEQUENTIALANIMATIONGROUP_H
