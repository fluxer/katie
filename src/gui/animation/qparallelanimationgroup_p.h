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

#ifndef QPARALLELANIMATIONGROUP_P_H
#define QPARALLELANIMATIONGROUP_P_H

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

#include "qparallelanimationgroup.h"
#include "qanimationgroup_p.h"
#include "qhash.h"

#ifndef QT_NO_ANIMATION

QT_BEGIN_NAMESPACE

class QParallelAnimationGroupPrivate : public QAnimationGroupPrivate
{
    Q_DECLARE_PUBLIC(QParallelAnimationGroup)
public:
    QParallelAnimationGroupPrivate()
        : lastLoop(0), lastCurrentTime(0)
    {
    }

    QHash<QAbstractAnimation*, int> uncontrolledFinishTime;
    int lastLoop;
    int lastCurrentTime;

    bool shouldAnimationStart(QAbstractAnimation *animation, bool startIfAtEnd) const;
    void applyGroupState(QAbstractAnimation *animation);
    bool isUncontrolledAnimationFinished(QAbstractAnimation *anim) const;
    void connectUncontrolledAnimations();
    void disconnectUncontrolledAnimations();

    void animationRemoved(int index, QAbstractAnimation *);

    // private slot
    void _q_uncontrolledAnimationFinished();
};

QT_END_NAMESPACE

#endif //QT_NO_ANIMATION

#endif //QPARALLELANIMATIONGROUP_P_H
