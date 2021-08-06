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

#ifndef QPAUSEANIMATION_P_H
#define QPAUSEANIMATION_P_H

#include <QtGui/qanimationgroup.h>


QT_BEGIN_NAMESPACE


#ifndef QT_NO_ANIMATION

class QPauseAnimationPrivate;

class Q_GUI_EXPORT QPauseAnimation : public QAbstractAnimation
{
    Q_OBJECT
    Q_PROPERTY(int duration READ duration WRITE setDuration)
public:
    QPauseAnimation(QObject *parent = nullptr);
    QPauseAnimation(int msecs, QObject *parent = nullptr);
    ~QPauseAnimation();

    int duration() const;
    void setDuration(int msecs);

protected:
    void updateCurrentTime(int);

private:
    Q_DISABLE_COPY(QPauseAnimation)
    Q_DECLARE_PRIVATE(QPauseAnimation)
};

#endif //QT_NO_ANIMATION

QT_END_NAMESPACE


#endif // QPAUSEANIMATION_P_H
