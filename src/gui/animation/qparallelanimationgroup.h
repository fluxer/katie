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

#ifndef QPARALLELANIMATIONGROUP_H
#define QPARALLELANIMATIONGROUP_H

#include <QtGui/qanimationgroup.h>


QT_BEGIN_NAMESPACE

#ifndef QT_NO_ANIMATION

class QParallelAnimationGroupPrivate;
class Q_GUI_EXPORT QParallelAnimationGroup : public QAnimationGroup
{
    Q_OBJECT

public:
    QParallelAnimationGroup(QObject *parent = nullptr);
    ~QParallelAnimationGroup();

    int duration() const;

protected:
    QParallelAnimationGroup(QParallelAnimationGroupPrivate &dd, QObject *parent);

    void updateCurrentTime(int currentTime);
    void updateState(QAbstractAnimation::State newState, QAbstractAnimation::State oldState);
    void updateDirection(QAbstractAnimation::Direction direction);

private:
    Q_DISABLE_COPY(QParallelAnimationGroup)
    Q_DECLARE_PRIVATE(QParallelAnimationGroup)
    Q_PRIVATE_SLOT(d_func(), void _q_uncontrolledAnimationFinished())
};

#endif //QT_NO_ANIMATION

QT_END_NAMESPACE


#endif // QPARALLELANIMATIONGROUP
