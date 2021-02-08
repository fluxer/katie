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

#ifndef QANIMATIONGROUP_H
#define QANIMATIONGROUP_H

#include <QtGui/qabstractanimation.h>


QT_BEGIN_NAMESPACE

#ifndef QT_NO_ANIMATION

class QAnimationGroupPrivate;
class Q_GUI_EXPORT QAnimationGroup : public QAbstractAnimation
{
    Q_OBJECT

public:
    QAnimationGroup(QObject *parent = Q_NULLPTR);
    ~QAnimationGroup();

    QAbstractAnimation *animationAt(int index) const;
    int animationCount() const;
    int indexOfAnimation(QAbstractAnimation *animation) const;
    void addAnimation(QAbstractAnimation *animation);
    void insertAnimation(int index, QAbstractAnimation *animation);
    void removeAnimation(QAbstractAnimation *animation);
    QAbstractAnimation *takeAnimation(int index);
    void clear();

protected:
    QAnimationGroup(QAnimationGroupPrivate &dd, QObject *parent);
    bool event(QEvent *event);

private:
    Q_DISABLE_COPY(QAnimationGroup)
    Q_DECLARE_PRIVATE(QAnimationGroup)
};

#endif //QT_NO_ANIMATION

QT_END_NAMESPACE


#endif //QANIMATIONGROUP_H
