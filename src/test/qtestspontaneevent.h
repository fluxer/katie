/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016-2020 Ivailo Monev
**
** This file is part of the QtTest module of the Katie Toolkit.
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
** As a special exception, The Qt Company gives you certain additional
** rights. These rights are described in The Qt Company LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QTESTSPONTANEEVENT_H
#define QTESTSPONTANEEVENT_H

#include <QtCore/qcoreevent.h>

QT_BEGIN_HEADER

QT_BEGIN_NAMESPACE

class QSpontaneKeyEvent
{
public:
    void setSpontaneous() { spont = true; }
    bool spontaneous() { return spont; }
    virtual ~QSpontaneKeyEvent() {}

    static inline void setSpontaneous(QEvent *ev)
    {
        Q_ASSERT(sizeof(QSpontaneKeyEvent) == sizeof(QEvent));

        // use a union instead of a reinterpret_cast to prevent alignment warnings
        union
        {
            QSpontaneKeyEvent *skePtr;
            QEvent *evPtr;
        } helper;

        helper.evPtr = ev;
        helper.skePtr->setSpontaneous();
    }

protected:
    QEvent::Type t;

private:
    bool posted;
    bool spont;
    bool m_accept;
    int looplevel;
};

QT_END_NAMESPACE

QT_END_HEADER

#endif
