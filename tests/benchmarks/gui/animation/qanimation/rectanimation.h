/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
**
** This file is part of the test suite of the Katie Toolkit.
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

#include <QtGui>

#ifndef RECTANIMATION_H
#define RECTANIMATION_H

class DummyObject;

//this class is even simpler than the dummy
//and uses no QVariant at all
class RectAnimation : public QAbstractAnimation
{
public:
    RectAnimation(DummyObject *obj);

    void setEndValue(const QRect &rect);
    void setStartValue(const QRect &rect);

    void setDuration(int d);
    int duration() const;

    virtual void updateCurrentTime(int currentTime);

private:
    DummyObject *m_object;
    QEasingCurve m_easing;
    QRect m_start, m_end, m_current;
    int m_dura;
};

#endif
