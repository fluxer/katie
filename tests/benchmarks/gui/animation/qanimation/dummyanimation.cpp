/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
**
** This file is part of the test suite module of the Katie Toolkit.
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

#include "dummyanimation.h"
#include "dummyobject.h"


DummyAnimation::DummyAnimation(DummyObject *d) : m_dummy(d)
{
}

void DummyAnimation::updateCurrentValue(const QVariant &value)
{
    if (state() == Stopped)
        return;
    if (m_dummy)
        m_dummy->setRect(value.toRect());
}

void DummyAnimation::updateState(State state)
{
    Q_UNUSED(state);
}
